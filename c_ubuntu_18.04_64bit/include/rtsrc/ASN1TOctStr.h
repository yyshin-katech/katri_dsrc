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
//
// CHANGE LOG
// Date         Init    Description
//
////////////////////////////////////////////////////////////////////////////
/**
 * @file rtsrc/ASN1TOctStr.h
 * ASN.1 OCTET string class definition.
 */
#ifndef _ASN1TOCTSTR_H_
#define _ASN1TOCTSTR_H_

#include "rtsrc/asn1type.h"
/**
 * @ingroup cppruntime
 */
/**
 * @addtogroup asn1data
 * @{
 */
/**
 * Dynamic octet string. This is the base class for generated C++ data type
 * classes for unsized OCTET string's.
 */
struct EXTRTCLASS ASN1TDynOctStr : public ASN1DynOctStr {
   /**
    * The default constructor creates an empty octet string.
    */
   ASN1TDynOctStr () { numocts = 0; data = 0; }

   /**
    * This constructor initializes the octet string to contain the given data
    * values.
    *
    * @param _numocts     - Number of octet in the octet string.
    * @param _data        - The binary octet data values.
    */
   ASN1TDynOctStr (OSUINT32 _numocts, const OSOCTET* _data) {
      numocts = _numocts;
      data = _data;
   }

   /**
    * This constructor initializes the octet string to contain the given data
    * values.
    *
    * @param _os          - C octet string structure.
    */
   ASN1TDynOctStr (ASN1DynOctStr& _os) {
      numocts = _os.numocts;
      data = _os.data;
   }

   /**
    * This constructor initializes the octet string to contain the given data
    * values. In this case, it is initializes the string to contain the
    * characters in a null-terminated C character string.
    *
    * @param cstring      - C null-terminated string.
    */
   ASN1TDynOctStr (const char* cstring) {
      if (strlen(cstring) > 0) {
         numocts = (OSUINT32)strlen(cstring) + 1;
         data = (const OSOCTET*) cstring;
      }
      else numocts = 0;
   }

   /**
    * This assignment operator sets the octet string to contain the characters
    * in a null-terminated C character string. For example, <code>myOctStr = "a
    * char string";</code>
    *
    * @param cstring      - C null-terminated string.
    */
   ASN1TDynOctStr& operator= (const char* cstring) {
      if (strlen(cstring) > 0) {
         numocts = (OSUINT32)strlen(cstring) + 1;
         data = (const OSOCTET*) cstring;
      }
      else numocts = 0;
      return *this;
   }

   /**
    * This assignment operator sets the octet string to contain the characters
    * from the given C++ octet string object.
    *
    * @param octet      - Octet string object reference
    */
   EXTRTMETHOD ASN1TDynOctStr& operator=(const ASN1TDynOctStr& octet);

   /**
    * This method converts the binary octet string to a human-readable
    * representation.  The string is first checked to see if it contains
    * all printable characters.  If this is the case, the characters in
    * the string are returned; otherwise, the string contents are converted
    * into a hexadecimal character string.
    *
    * @param pctxt      - Pointer to a context structure.
    */
   EXTRTMETHOD const char* toString (OSCTXT* pctxt) const;

   /**
    * This method converts the binary octet string to a hexadecimal
    * string representation.
    *
    * @param pctxt      - Pointer to a context structure.
    */
   EXTRTMETHOD const char* toHexString (OSCTXT* pctxt) const;

   /**
    * This method compares the first n octets of this octet string with
    * the given octet string.
    *
    * @param n          - Number of octets to compare
    * @param o          - Octet string for comparison
    * @return           - 0 if strings are equal, -1 if this octet string is
    *                     less than the given string, +1 if this string >
    *                     given string.
    */
   EXTRTMETHOD int nCompare (OSUINT32 n, const ASN1TDynOctStr &o) const;

} ;

//operator==

/**
 * The equality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1TDynOctStr class.
 * @param   rhs   The right-hand ASN1TDynOctStr class.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator==(const ASN1TDynOctStr &lhs,const ASN1TDynOctStr &rhs);

/**
 * The equality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1TDynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator==(const ASN1TDynOctStr &lhs,const char *string);

/**
 * The equality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1DynOctStr class.
 * @param   rhs   The right-hand ASN1DynOctStr structure.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator==(const ASN1DynOctStr &lhs,const ASN1DynOctStr &rhs);

/**
 * The equality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1DynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator==(const ASN1DynOctStr &lhs,const char *string);

//operator !=

/**
 * The inequality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1TDynOctStr class.
 * @param   rhs   The right-hand ASN1TDynOctStr class.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator!=(const ASN1TDynOctStr &lhs,const ASN1TDynOctStr &rhs);

/**
 * The inequality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1TDynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator!=(const ASN1TDynOctStr &lhs,const char *string);

/**
 * The inequality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1DynOctStr class.
 * @param   rhs   The right-hand ASN1DynOctStr structure.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator!=(const ASN1DynOctStr &lhs,const ASN1DynOctStr &rhs);

/**
 * The inequality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1DynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator!=(const ASN1DynOctStr &lhs,const char *string);

//operator <

/**
 * The less-than test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1TDynOctStr class.
 * @param   rhs   The right-hand ASN1TDynOctStr class.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator<(const ASN1TDynOctStr &lhs,const ASN1TDynOctStr &rhs);

/**
 * The less-than test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1TDynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator<(const ASN1TDynOctStr &lhs,const char *string);

/**
 * The inequality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1DynOctStr class.
 * @param   rhs   The right-hand ASN1DynOctStr structure.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator<(const ASN1DynOctStr &lhs,const ASN1DynOctStr &rhs);

/**
 * The inequality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1DynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator<(const ASN1DynOctStr &lhs,const char *string);

//operator <=

/**
 * The less-equal test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1TDynOctStr class.
 * @param   rhs   The right-hand ASN1TDynOctStr class.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator<=(const ASN1TDynOctStr &lhs,const ASN1TDynOctStr &rhs);

/**
 * The less-than test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1TDynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator<=(const ASN1TDynOctStr &lhs,const char *string);

/**
 * The inequality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1DynOctStr class.
 * @param   rhs   The right-hand ASN1DynOctStr structure.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator<=(const ASN1DynOctStr &lhs,const ASN1DynOctStr &rhs);

/**
 * The inequality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1DynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator<=(const ASN1DynOctStr &lhs,const char *string);

 //operator >

/**
 * The greater-than test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1TDynOctStr class.
 * @param   rhs   The right-hand ASN1TDynOctStr class.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator>(const ASN1TDynOctStr &lhs,const ASN1TDynOctStr &rhs);

/**
 * The greater-than test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1TDynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator>(const ASN1TDynOctStr &lhs,const char *string);

/**
 * The inequality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1DynOctStr class.
 * @param   rhs   The right-hand ASN1DynOctStr structure.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator>(const ASN1DynOctStr &lhs,const ASN1DynOctStr &rhs);

/**
 * The inequality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1DynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator>(const ASN1DynOctStr &lhs,const char *string);

//operator >=

/**
 * The greater-equal test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1TDynOctStr class.
 * @param   rhs   The right-hand ASN1TDynOctStr class.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator>=(const ASN1TDynOctStr &lhs,const ASN1TDynOctStr &rhs);

/**
 * The less-than test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1TDynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator>=(const ASN1TDynOctStr &lhs,const char *string);

/**
 * The inequality test operator:  compares two dynamic octet strings to each
 * other.
 *
 * @param   lhs   The left-hand ASN1DynOctStr class.
 * @param   rhs   The right-hand ASN1DynOctStr structure.
 *
 * @return  1 if the two octet strings are equal; 0 otherwise.
 */
EXTERNRT int operator>=(const ASN1DynOctStr &lhs,const ASN1DynOctStr &rhs);

/**
 * The inequality test operator:  compares a dynamic octet string against a
 * character string.
 *
 * @param   lhs      The left-hand ASN1DynOctStr class.
 * @param   string   A character string to be compared against.
 *
 * @return  1 if the two strings are equal; 0 otherwise.
 */
EXTERNRT int operator>=(const ASN1DynOctStr &lhs,const char *string);

#endif

/**
 * @}
 */
