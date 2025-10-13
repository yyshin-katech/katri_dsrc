/*
 * Copyright (c) 1997-2018 Objective Systems, Inc.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Objective Systems, Inc.
 *
 * PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 *****************************************************************************/
/**
 * @file rtPrint.h  Functions to print ASN.1 data in various formats.
 */
#ifndef _RTPRINT_H_
#define _RTPRINT_H_
#include <stdio.h>
#include "rtsrc/asn1type.h"
#include "rtxsrc/rtxPrint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup valsToStdout Print Values to Standard Output
 * @ingroup cruntime
 * @{
 * These functions print the output in a "name=value" format. The
 * value format is obtained by calling one of the ToString functions with the
 * given value.
 */

/** Defines the old runtime function to its new equivalent. */
#define rtPrintBoolean rtxPrintBoolean
/** Defines the old runtime function to its new equivalent. */
#define rtPrintInteger rtxPrintInteger
/** Defines the old runtime function to its new equivalent. */
#define rtPrintInt64   rtxPrintInt64
/** Defines the old runtime function to its new equivalent. */
#define rtPrintUnsigned rtxPrintUnsigned
/** Defines the old runtime function to its new equivalent. */
#define rtPrintUInt64  rtxPrintUInt64
/** Defines the old runtime function to its new equivalent. */
#define rtPrintReal    rtxPrintReal
/** Defines the old runtime function to its new equivalent. */
#define rtPrintCharStr rtxPrintCharStr
/** Defines the old runtime function to its new equivalent. */
#define rtPrintHexStr  rtxPrintHexStr

/**
 * This function prints the value of a bit string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numbits      The number of bitsto be printed.
 * @param data         A pointer to the data to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintBitStr (const char* name, OSSIZE numbits,
                              const OSOCTET* data, const char* conn);

/**
 * This function prints the value of a bit string to stdout.  It includes
 * support for BIT STRING's with extension data.
 *
 * @param name         The name of the variable to print.
 * @param numbits      The number of bitsto be printed.
 * @param data         A pointer to the data to be printed.
 * @param dataSize     Size, in bytes, of the data array.
 * @param extdata      Pointer to byte array containing extension data.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintBitStrExt
(const char* name, OSSIZE numbits, const OSOCTET* data, OSSIZE dataSize,
 const OSOCTET* extdata, const char* conn);

/**
 * This function prints the value of a bit string to stdout in brace text
 * format.
 *
 * @param name         The name of the variable to print.
 * @param numbits      The number of bits to be printed.
 * @param data         A pointer to the data to be printed.
 */
EXTERNRT void rtPrintBitStrBraceText
(const char* name, OSSIZE numbits, const OSOCTET* data);

/**
 * This function prints the value of a bit string to stdout in brace text
 * format. It includes support for BIT STRING's with extension data.
 *
 * @param name         The name of the variable to print.
 * @param numbits      The number of bits to be printed.
 * @param data         A pointer to the data to be printed.
 * @param dataSize     Size, in bytes, of the data array.
 * @param extdata      Pointer to byte array containing extension data.
 */
EXTERNRT void rtPrintBitStrBraceTextExt
(const char* name, OSSIZE numbits, const OSOCTET* data,
 OSSIZE dataSize, const OSOCTET* extdata);

/**
 * This function prints the value of an octet string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         A pointer to the data to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintOctStr (const char* name, OSSIZE numocts,
                              const OSOCTET* data, const char* conn);

/**
 * This function prints the value of a 16-bit character string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param bstring      A pointer to the bit string to be printed.
 */
EXTERNRT void rtPrint16BitCharStr (const char* name,
                                   const Asn116BitCharString* bstring);


/**
 * This function prints the value of a 32-bit character string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param bstring      A pointer to the bit string to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrint32BitCharStr (const char* name,
                                   const Asn132BitCharString* bstring,
                                   const char* conn);

/**
 * This function prints a Universal string to standard output.  Characters
 * in the string that are within the normal Ascii range are printed as
 * single characters.  Characters outside the Ascii range are printed
 * as 8-byte hex codes (0xnnnnnnnn).
 *
 * @param name         The name of the variable to print.
 * @param bstring      A pointer to the bit string to be printed.
 */
EXTERNRT void rtPrintUnivCharStr (const char* name,
                                  const Asn132BitCharString* bstring);

/**
 * This function prints the value of an object identifier to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pOID         A pointer to the OID to be printed.
 */
EXTERNRT void rtPrintOID (const char* name, const ASN1OBJID* pOID);

/**
 * This function prints the value of an object identifier
 * to stdout.  Only the value is printed, not the name.
 *
 * @param pOID         A pointer to the OID to be printed.
 */
EXTERNRT void rtPrintOIDValue (const ASN1OBJID* pOID);

/**
 * This function prints the value of an object identifier with 64-bit arc
 * values to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pOID         A pointer to the OID to be printed.
 */
EXTERNRT void rtPrintOID64 (const char* name, const ASN1OID64* pOID);

/**
 * This function prints the value of an object identifier with 64-bit arc
 * values to stdout.  Only the value is printed, not the name.
 *
 * @param pOID         A pointer to the OID to be printed.
 */
EXTERNRT void rtPrintOID64Value (const ASN1OID64* pOID);

/**
 * This function prints the value of an open type to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         The pointer to the data to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintOpenType (const char* name, OSSIZE numocts,
                               const OSOCTET* data, const char* conn);

/**
 * This function prints the value of an open type extension to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pElemList    A pointer to an element of a list.
 */
EXTERNRT void rtPrintOpenTypeExt (const char* name, const OSRTDList* pElemList);

/**
 * This function prints the value of an open type extension in brace
 * text format to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pElemList    A pointer to an element of a list.
 */
EXTERNRT void rtPrintOpenTypeExtBraceText
(const char* name, const OSRTDList* pElemList);

/** Defines the old runtime function to its new equivalent. */
#define rtPrintIndent rtxPrintIndent
/** Defines the old runtime function to its new equivalent. */
#define rtPrintIncrIndent rtxPrintIncrIndent
/** Defines the old runtime function to its new equivalent. */
#define rtPrintDecrIndent rtxPrintDecrIndent
/** Defines the old runtime function to its new equivalent. */
#define rtPrintCloseBrace rtxPrintCloseBrace
/** Defines the old runtime function to its new equivalent. */
#define rtPrintOpenBrace  rtxPrintOpenBrace

/**
 * @} valsToStdout
 */
#ifdef __cplusplus
}
#endif
#endif
