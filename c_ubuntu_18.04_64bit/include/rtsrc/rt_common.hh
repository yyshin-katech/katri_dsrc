/**
 * @file rt_common.hh
 */
#ifndef _RT_COMMON_HH_
#define _RT_COMMON_HH_

#include "rtxsrc/rtxContext.hh"
#include "rtsrc/asn1type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function returns the integer representation of the ASN.1 runtime 
 * version.
 */
EXTERNRT int rtxGetLibVersion (OSVOIDARG);

/**
 * This function returns a string that describes the features of the BER
 * runtime library.  Different release kits will return different strings.
 * An example would be &quot;ASN1RT v6.1.1, opt, compact, limited.&quot;
 */
EXTERNRT const char *rtxGetLibInfo (OSVOIDARG);

#ifdef __cplusplus
}
#endif

#endif /* _RT_COMMON_HH_ */
