# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

Build and run are done from `c_ubuntu_18.04_64bit/app/`:

```sh
cd c_ubuntu_18.04_64bit/app
make                               # produces ./decodeSample
make clean                         # removes *.o and the binary
LD_LIBRARY_PATH=../lib ./decodeSample   # shared libs are not in system path
```

There is no test suite, linter, or CI — only the Makefile target.

The prebuilt shared libraries under `c_ubuntu_18.04_64bit/lib/` (`libtrafficinfo1.so`, `libasn1*.so`, `libxml2.so`, `libexpat.so`) are **Ubuntu 18.04 x86_64 binaries**. The directory name is load-bearing; porting to another distro/arch means replacing the whole `c_ubuntu_18.04_64bit/` tree, not just rebuilding the app.

## Architecture

This is a single-file C application (`app/decodeSample.c`) that acts as a bridge between a DSRC / V2X RSU (Road-Side Unit) and a downstream consumer (a ROS node, per inline comments):

1. **UDP ingress**: `bind()` on `0.0.0.0:60000` (actually `inet_addr("192.168.1.255")` — broadcast address of the DSRC LAN segment) and `recvfrom()` raw datagrams from the RSU.
2. **Custom 16-byte framing**: each datagram has a 16-byte vendor header before the SAE J2735 payload. The code inspects `dsrc_data[17]` (the messageId byte inside the J2735 `MessageFrame`) to filter for SPaT (`0x13` / 19), then passes `&dsrc_data[16]` with length `UDPRxDSRC - 16` to the ASN.1 decoder. Any change to the header size must update both the offset and the length subtraction.
3. **ASN.1 PER decode (two-stage)**:
   - `sDecode_MessageFrame()` → `asn1PD_MessageFrame()` extracts `messageId` + opaque `value` octet string.
   - `sDecode_DSRCmsg()` switches on `messageId` (`ASN1V_signalPhaseAndTimingMessage`, `ASN1V_mapData`, `ASN1V_roadSideAlert`, `ASN1V_rtcmCorrections`, `ASN1V_travelerInformation`, `ASN1V_basicSafetyMessage`) and calls the matching `asn1PD_*` decoder. Only SPaT is currently wired through to downstream processing — the other cases decode and print but do not forward.
   - Decoders come from Objective Systems ASN1C runtime (headers under `include/rt*src/`, libs `libasn1ber/oer/per/rt.so`). The generated type headers (`DSRC.h` — ~946 KB, `ISO14827-2.h`, `AddGrpB/C/D.h`, `ITIS.h`, `NTCIP.h`, `REGION.h`) are committed; do not edit them by hand — they are regenerated from ASN.1 modules.
4. **SPaT flattening (`parseSpat`)**: walks `SPAT → intersections (OSRTDList of IntersectionState) → states (MovementState) → state_time_speed (MovementEvent)`. For 5 hard-coded `(intersection_id, signalGroup)` pairs it populates `sig_SPaT[0..4]` (see table below). Any intersection/signalGroup outside those pairs is silently dropped.
5. **UDP egress**: `sendto()` a 1024-byte buffer to `127.0.0.1:50000` (source-bound to `:55555`), intended to be consumed by a ROS node. The buffer is `memcpy`'d from the whole `sig_SPaT[]` array — the consumer must use the same `SIG_SPAT` struct layout (packing, int sizes) or it will misparse.

### Hard-coded SPaT routing table

| `sig_SPaT[i]` | `intersection_id` | `signalGroup` |
|---|---|---|
| 0 | 1300 | 3 |
| 1 | 300  | 9 |
| 2 | 400  | 2 |
| 3 | 610  | 2 |
| 4 | 700  | 4 |
| 5 | 100  | 16 |
| 6 | 1500 | 1 |

Adding a monitored intersection requires: (a) extending `sig_SPaT[]` size, (b) adding an `if/else if` branch in `parseSpat` for the id, (c) adding a `signalGroup` check inside the inner loop, (d) updating the `memset` clear loop at the bottom of `main` (currently hard-coded to `i<10` even though the array is size 5 — discrepancy worth fixing if touched).

### Things that look like bugs but may be intentional

- `main` declared `void main(void)` and returns nothing.
- `if(ret = UDP_Init() < 0)` — due to operator precedence, `ret` gets `0` or `1`, not the return value. The `exit(1)` path only fires if `UDP_Init` returns negative.
- `UDP_Init` returns `1` (not `-1`) on `bind` failure, and has no `return` on the success path.
- The clear loop `for(i=0;i<10;i++) memset(&sig_SPaT[i], 0, sizeof(sig_SPaT))` walks past the array end **and** clears the whole array on every iteration.
- `dsrc_addr` is used both as the bind address and the `recvfrom` peer-address output — binding to a broadcast address rather than `INADDR_ANY` is unusual; treat it as deliberate for this RSU deployment, not an error to "fix."

Flag these when touching surrounding code, but don't rewrite them as drive-by cleanup — this is firmware-adjacent code where the deployed RSU may depend on current behavior.
