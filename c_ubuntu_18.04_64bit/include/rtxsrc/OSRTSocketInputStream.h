/*
 * Copyright (c) 2003-2018 Objective Systems, Inc.
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
 * @file OSRTSocketInputStream.h
 * C++ base class definitions for operations with input socket streams.
 */
#ifndef _OSRTSOCKETINPUTSTREAM_H_
#define _OSRTSOCKETINPUTSTREAM_H_

#ifndef _OS_NOSOCKET

#include "rtxsrc/OSRTSocket.h"
#include "rtxsrc/OSRTInputStream.h"

/**
 * Generic socket input stream. This class opens an existing socket for input in
 * binary mode and reads data from it.
 */
class EXTRTCLASS OSRTSocketInputStream : public OSRTInputStream {
 protected:
   OSRTSocket mSocket; ///< a socket
 public:
   /**
    * Creates and initializes a socket input stream using the OSRTSocket
    * instance of socket.
    *
    * @param socket                    Reference to OSRTSocket instance.
    * @see                           ::rtxStreamSocketAttach
    */
   EXTRTMETHOD OSRTSocketInputStream (OSRTSocket& socket);

   /**
    * Creates and initializes a socket input stream using the OSRTSocket
    * instance of socket.
    *
    * @param pContext                 Pointer to a context to use.
    * @param socket                   Reference to OSRTSocket instance.
    * @see                           ::rtxStreamSocketAttach
    */
   EXTRTMETHOD OSRTSocketInputStream
      (OSRTContext* pContext, OSRTSocket& socket);

   /**
    * Creates and initializes the socket input stream using the socket
    * handle.
    *
    * @param socket                    Handle of the socket.
    * @param ownership                 Indicates ownership of the socket.
    *                                  Set to TRUE to pass ownership to
    *                                  this object instance.  The socket
    *                                  will be closed when this object
    *                                  instance is deleted or goes out
    *                                  of scope.
    * @see                           ::rtxStreamSocketAttach
    */
   EXTRTMETHOD OSRTSocketInputStream
      (OSRTSOCKET socket, OSBOOL ownership = FALSE);

   /**
    * Creates and initializes the socket input stream using the socket
    * handle.
    *
    * @param pContext                  Pointer to a context to use.
    * @param socket                    Handle of the socket.
    * @param ownership                 Indicates ownership of the socket.
    *                                  Set to TRUE to pass ownership to
    *                                  this object instance.  The socket
    *                                  will be closed when this object
    *                                  instance is deleted or goes out
    *                                  of scope.
    * @see                           ::rtxStreamSocketAttach
    */
   OSRTSocketInputStream (OSRTContext* pContext,
                          OSRTSOCKET socket,
                          OSBOOL ownership = FALSE);

   /**
    * This method is used to query a stream object in order to determine
    * its actual type.
    *
    * @param id  Enumerated stream identifier
    * @return    True if the stream matches the identifier
    */
   virtual OSBOOL isA (StreamID id) const {
      return id == Socket;
   }

} ;

#endif /* _OS_NOSOCKET */

#endif /* _OSRTSOCKETINPUTSTREAM_H_ */
