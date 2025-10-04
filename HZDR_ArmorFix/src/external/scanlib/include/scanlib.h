#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct MemoryPattern
  {
    const char *patternStr; // This needs to be a string, not directly the bytes of the pattern
    uintptr_t foundAddr;
  };

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