/*
Simple to use and very fast process memory pattern finding library to use in injected DLLs.

Copyright (c) 2025 Luca2040 - https://github.com/luca2040/scanlib

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    const char *patternStr; // This needs to be a string, not directly the bytes of the pattern
    uintptr_t foundAddr;
  } MemoryPattern;

  typedef enum
  {
    SCANLIB_OK = 0,
    SCANLIB_ERR_PARSE_PATTERN,
    SCANLIB_INVALID_MODULE_HANDLE,
    SCANLIB_ERR_GETTING_MODULE_INFO
  } ScanlibResult;

  void Scanlib_AddPattern(MemoryPattern *pattern);
  void Scanlib_ClearPatternList();

  ScanlibResult Scanlib_Search();

#ifdef __cplusplus
}
#endif