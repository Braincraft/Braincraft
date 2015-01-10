#ifndef __Assume__
#define __Assume__

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include "sstring.h"

/** Checks a condition at run-time a throw a fatal exception if not verified.
 * @param condition The condition that must be true.
 * @param thrown The name of the thrown exception if the assume fails. Typical exception are:
 * - illegal-argument : when a method or function receives an argument with a spurious value.
 * - illegal-state    : when detecting an unexpected condition (a bug) at some point in the code.
 * - IO-exception     : when an input/output operation fails.
 * @param message The error message, a string format à-la printf with more parameters if used.
 * @throws The exception to be thrown if the assume fails.
 */
static void assume(bool condition, String thrown, String message, ...)
{
  const unsigned int nchars = 10000;
  char chars[nchars];
  if(!(condition)) {
    if(message.length() < nchars) {
      va_list a;
      va_start(a, message);
      vsnprintf(chars, nchars, message.c_str(), a);
      va_end(a);
    } else {
      sprintf(chars, "(spurious error message)");
    }
    fprintf(stderr, "Runtime fatal error %s : %s\n", thrown.c_str(), chars);
#ifdef WIN32
    // Quit program
    exit(3);
#else
    // send signal (3) to stop the program (keep the stack)
    raise(SIGQUIT);
#endif
  }
}
#endif
