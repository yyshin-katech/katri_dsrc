/*
 * Copyright (c) 1997-2009 Objective Systems, Inc.
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

/* Defines a means of performing timing tests.  The macros may be used as 
 * follows:
 *
 * DECLARE_BENCHMARK
 *
 * ...
 *
 * BEGIN_LOOP
 *    // code to be timed goes here
 * END_LOOP
 *
 * PRINT_RESULTS_MS
 *
 * Convenience macros to start and stop the performance clock are also 
 * provided in case a loop is not necessary.  In this case, the ITERCNT
 * value should be redefined to the number of cases being tested. 
 *
 * These timers should be reasonable accurate within microsecond resolution,
 * but results will vary from platform to platform.  On GNU/Linux systems,
 * it is necessary to link against librt.a. 
 *
 * If a program is terminated unexpectedly, results may fail to print since
 * the output buffer may not be flushed.  If exception handling is used,
 * make sure to call fflush(stdout) or the appropriate call for your 
 * platform. */

#ifndef __RTCLOCK_H__
#define __RTCLOCK_H__

#ifdef _cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#define ITERCNT 10000L

#ifndef _MSC_VER      /* not MSVC++; does this work for Borland? */

#define __need_timespec
#include <time.h>

#define DECLARE_BENCHMARK \
   struct timespec tp0, tp1, tp2, tp3; \
   long   __clk__ = 0; \
   double __maxms__ = 0, __minms__ = 10000000000000.f;

#define CLOCK_START \
   clock_gettime(CLOCK_REALTIME, &tp0);

#define CLOCK_STOP \
   clock_gettime(CLOCK_REALTIME, &tp1);

#define BEGIN_LOOP \
   CLOCK_START \
   for (__clk__ = 0; __clk__ < ITERCNT; __clk__ ++) { \
      clock_gettime(CLOCK_REALTIME, &tp2);

#define END_LOOP \
      clock_gettime(CLOCK_REALTIME, &tp3); \
      long _dst = tp3.tv_sec - tp2.tv_sec, \
           _dnt = tp3.tv_nsec - tp2.tv_nsec; \
      double _dmst = (_dst * 1000.f) + (_dnt / 1e6); \
      \
      if (_dmst < __minms__) __minms__ = _dmst; \
      if (_dmst > __maxms__) __maxms__ = _dmst; \
   } \
   CLOCK_STOP

#define PRINT_RESULTS_MS \
   long _ds = tp1.tv_sec - tp0.tv_sec, \
        _dn = tp1.tv_nsec - tp0.tv_nsec; \
   double _dms = (_ds * 1000.f) + (_dn / 1e6); \
   printf ("\t%.6f\t%.6f\t%.6f\t%.6f\n", _dms, __minms__, __maxms__, \
           _dms/(float)ITERCNT); 

#else                /* MSVC++ */

#include <windows.h>

#define DECLARE_BENCHMARK \
   LARGE_INTEGER _start, _stop, _freq; \
   long          __clk__ = 0; \
   double        _delta, _total; \
   QueryPerformanceFrequency(&_freq);

#define CLOCK_START \
   QueryPerformanceCounter(&_start);

#define CLOCK_STOP \
   QueryPerformanceCounter(&_stop);

#define BEGIN_LOOP \
   CLOCK_START \
   for (__clk__ = 0; __clk__ < ITERCNT; __clk__ ++) {

#define END_LOOP \
   } \
   CLOCK_STOP

#define PRINT_RESULTS_MS \
   _delta = (_stop.QuadPart - _start.QuadPart); \
   _total = _delta / (double)_freq.QuadPart * 1000; \
   printf("\t%.6f\t%.6f\n", _total, _total/(float)ITERCNT);

#endif

#endif /* DEFINE __RTCLOCK_H__ */

