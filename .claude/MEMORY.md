# katri_dsrc Project Memory

## Project Overview
- DSRC/V2X RSU ↔ ROS bridge. 단일 파일 C 앱: `c_ubuntu_18.04_64bit/app/decodeSample.c`.
- 동작: DSRC RSU 로부터 UDP(:60000) 수신 → 16바이트 벤더 헤더 뒤 SAE J2735 PER 디코드 → SPaT 평탄화(`parseSpat`) → `SIG_SPAT[5]` 를 1024바이트 UDP 로 `127.0.0.1:50000` 송신 → 하류 ROS 노드(mcar `katri_obu_interface`) 소비.
- Objective Systems ASN1C 런타임 사용. 생성 헤더(`DSRC.h` 등)는 손으로 편집 금지(ASN.1 모듈에서 재생성).

## Build & Run
```sh
cd c_ubuntu_18.04_64bit/app
make                                     # → ./decodeSample
LD_LIBRARY_PATH=../lib ./decodeSample    # 공유 lib 가 시스템 경로에 없음
```
테스트/린터/CI 없음. `c_ubuntu_18.04_64bit/` 디렉토리명은 load-bearing(Ubuntu 18.04 x86_64 prebuilt .so).

## Wire 포맷 (하류 katri_obu_interface 와 공유 — 변경 시 양쪽 동기 필수)
- `SIG_SPAT { int Intersection_id; unsigned char movementName[5]; int signalGroup; int eventState; unsigned int minEndTime; }` — x86-64 기본 정렬 24바이트, `sig_SPaT[5]`=120바이트(LE, host order, htonl 없음).
- 송신은 1024바이트 datagram(앞 120B=데이터, 뒤 0 패딩). 수신측은 앞 120B만 사용.
- 하드코딩 라우팅 5개: sig_SPaT[0]=교차로1300/sg3, [1]=300/9, [2]=400/2, [3]=610/2, [4]=700/4. 그 외 교차로/signalGroup 은 버려짐.
- 하류 mcar `katri_obu_interface` → `/katri_v2x_node/katri_spat`(v2x_msgs/intersection_array_msg). 참고: `MovementStateName` 은 하류에서 읽는 소비자 없음.

## 변경 이력
- **2026-06-04 메모리 안전 버그 3종 수정** (wire 포맷 보존, `make` 빌드 확인):
  1. OOB read: `memcpy(p, sig_SPaT, BUFF_SIZE=1024)` → `sizeof(sig_SPaT)`(120B). 120B 배열을 1024 읽던 것.
  2. OOB write: 종료 클리어 루프 `for(i<10) memset(&sig_SPaT[i],0,sizeof(sig_SPaT)=120)` → `for(i<5) ... sizeof(SIG_SPAT)=24`. 배열 끝 너머 쓰기 + 매 반복 전체 클리어.
  3. strcpy 오버플로(parseSpat 5곳): `strcpy(movementName(5B), src)` → `movementNamePresent` 가드 + `strncpy(..., sizeof-1)` + NUL 종단.

## 건드리지 않은 "의도적일 수 있는" 항목 (firmware-adjacent, RSU 거동 의존 가능)
- `void main(void)`, `if(ret = UDP_Init() < 0)` 연산자 우선순위, `UDP_Init` 의 return 누락, broadcast 주소 bind. → drive-by 재작성 금지. 자세한 배경은 `CLAUDE.md`.
