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
 * @file asn1json.h
 * JSON low-level C encode/decode functions for ASN.1.
 */
#ifndef _ASN1JSON_H_
#define _ASN1JSON_H_

#include "rtsrc/asn1type.h"

#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxError.h"

#ifndef EXTERNJSON
#ifdef BUILDJSONDLL
#define EXTERNJSON __declspec(dllexport)
#elif defined (USEJSONLDLL)
#define EXTERNJSON __declspec(dllimport)
#else
#define EXTERNJSON
#endif /* BUILDJSONDLL */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "osrtjson.h"

/**
 * @defgroup rtJsonAsn1Enc JSON runtime encoding functions for ASN.1 data types.
 * @{
 */

/**
 * This function encodes the JSON representation of an OID value.
 *
 * @param   pctxt    A pointer to a context data structure.
 * @param   pOID     A pointer to an ASN1OBJID data structure.
 *
 * @return  0 on success, less than zero otherwise.
 */
EXTERNJSON int rtJsonAsn1EncOID (OSCTXT *pctxt, ASN1OBJID *pOID);

/**
 * This function encodes an ASN.1 open type to JSON.
 *
 * If the input data are valid JSON data, then the output is encoded as a
 * JSON string object.  If the input data are not valid (e.g., binary
 * data encoded with different rules, they are dropped.
 */
EXTERNJSON int rtJsonAsn1EncOpenType (OSCTXT *pctxt, OSOCTET *pdata,
                                       OSUINT32 numocts);


/**
 * This function encodes an unknown extension element.
 *
 * @param pctxt   A pointer to a context data structure.
 * @param pElem   The unknown extension element value. If the encoding is
 *                not set or is set to JSON, the data is expected to be a
 *                JSONNamedValue, which will be written to the encoding as-is.
 *                Othewise, the data will be written in hexadecimal form,
 *                wrapped inside an element.
 */
EXTERNJSON int rtJsonAsn1EncExtElem (OSCTXT* pctxt, ASN1OpenType* pElem);

/**
 * This function encodes a list of unknown extension elements.
 *
 * @param pctxt   A pointer to a context data structure.
 * @param pvalue  A list of ASN1OpenType objects, each representing an
 *                unknown extension element.  These will be encoded as by
 *                rtJsonEncExtElem.
 */
EXTERNJSON int rtJsonAsn1EncOpenTypeExt (OSCTXT* pctxt,
                                          const OSRTDList* pvalue);

/**
 * This function decodes the JSON representation of an OID value.
 *
 * @param   pctxt    A pointer to a context data structure.
 * @param   pOID     A pointer to an ASN1OBJID data structure.
 *
 * @return  0 on success, less than zero otherwise.
 */

/** @} */ /* rtJsonAsn1Enc */

/**
 * @defgroup rtJsonAsn1Dec JSON runtime decoding functions for ASN.1 data types.
 * @{
 */

EXTERNJSON int rtJsonAsn1DecOIDValue (OSCTXT *pctxt, ASN1OBJID *pOID);

/**
 * This function decodes the JSON representation of a NULL type.
 *
 * @param   pctxt    A pointer to a context data structure.
 *
 * @return  0 on success, less than zero otherwise.
 */
EXTERNJSON int rtJsonAsn1DecNull (OSCTXT *pctxt);

/**
 * This function decodes the ASN.1 BMPString type.  We assume that the input
 * data are encoded in UTF-8.  The data are internally represented as two-byte
 * characters.
 *
 * @param   pctxt    A pointer to a context data structure.
 * @param   pBMPStr  A pointer to an ASN.1 BMP string.
 *
 * @return  0 on success, less than zero otherwise.
 */
EXTERNJSON int rtJsonAsn1DecBMPString (OSCTXT *pctxt, ASN1BMPString *pBMPStr);

/** @} */ /* rtJsonAsn1Dec */

#ifdef __cplusplus
}
#endif

#endif

