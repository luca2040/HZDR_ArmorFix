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

#include "../include/scanlib.h"

#include <windows.h>
#include <Psapi.h>
#include <algorithm>
#include <vector>
#include <string>

// ######################################################

struct PatternFindingData // Struct used internally when finding the pattern
{
  MemoryPattern *pattern;

  unsigned char *patternBytes = nullptr;
  bool *patternMask = nullptr;
  size_t patternLen = 0;

  size_t skipTable[256] = {}; // Preprocessed skip table for the BMH algorithm

  bool found = false;

  PatternFindingData(MemoryPattern *_pattern) : pattern(_pattern) {}
};

class MemoryScanner
{
private:
  std::vector<PatternFindingData> patternList;

  bool parsePatterns();
  void cleanPatternArrays();

public:
  MemoryScanner() {}

  void addPattern(MemoryPattern *pattern);
  void clear();
  ScanlibResult search();

  static MemoryScanner instance;
};
MemoryScanner MemoryScanner::instance; // Singleton instance to make the API easier to use

// ######################################################

// Mask must be inverted: false -> consider; true -> ignore
int maskedMemcmp(const void *buf1,
                 const void *buf2,
                 const bool *mask,
                 size_t count)
{
  if (!count)
    return (0);

  while (--count && (*(bool *)mask || (*(char *)buf1 == *(char *)buf2)))
  {
    buf1 = (char *)buf1 + 1;
    buf2 = (char *)buf2 + 1;
    mask = (bool *)mask + 1;
  }

  if (*(bool *)mask)
    return (0);
  return (*((unsigned char *)buf1) - *((unsigned char *)buf2));
}

// Get the index of the last occurrence of a "true" in an array of bool values
// If none is found return 0
size_t lastTrueIndex(const bool *buf1,
                     size_t lenght)
{
  if (!lenght)
    return 0;

  size_t found = 0;
  for (size_t i = 0; i < lenght; i++)
    if (*(buf1++))
      found = i;

  return found;
}

// ######################################################

// Adds a pattern to the search list
void MemoryScanner::addPattern(MemoryPattern *pattern)
{
  patternList.push_back(PatternFindingData(pattern));
}

// Clears the search list
void MemoryScanner::clear()
{
  patternList.clear();
}

// Parses all the patterns from the strings and computes the BMH skip table
bool MemoryScanner::parsePatterns()
{
  for (auto &patternData : this->patternList)
  {
    const auto &patternStr = std::string(patternData.pattern->patternStr);
    auto patternBytesList = std::vector<unsigned char>();
    auto patternMaskList = std::vector<bool>();

    std::string currentByte;
    int i = 0;
    for (char currentChar : patternStr)
    {
      if (currentChar == '?')
      {
        if (i != 0) // There still is a buffered part of byte
          return false;

        patternBytesList.push_back(0x00);
        patternMaskList.push_back(true); // Inverted mask for the optimized masked memcpm
      }
      else if (currentChar != ' ')
      {
        currentByte.push_back(currentChar);
        i++;

        if (i > 1) // Read buffered byte
        {
          unsigned char readByte;

          try
          {
            readByte = static_cast<unsigned char>(std::stoi(currentByte, nullptr, 16));
          }
          catch (const std::exception &e)
          {
            return false;
          }

          patternBytesList.push_back(readByte);
          patternMaskList.push_back(false); // Inverted mask for the optimized masked memcpm
          currentByte.clear();
          i = 0;
        }
      }
    }

    size_t patternLenght = patternBytesList.size();
    unsigned char *patternBytes = new unsigned char[patternLenght];
    bool *patternMask = new bool[patternLenght];

    std::copy(patternBytesList.begin(), patternBytesList.end(), patternBytes);
    std::copy(patternMaskList.begin(), patternMaskList.end(), patternMask);

    patternData.patternLen = patternLenght;
    patternData.patternBytes = patternBytes;
    patternData.patternMask = patternMask;

    // Compute the BMH skip table
    size_t lastPatternI = patternLenght - 1UL;
    size_t maskedPatternLen = lastPatternI - lastTrueIndex(patternMask, patternLenght);
    maskedPatternLen = maskedPatternLen ? maskedPatternLen : 1UL;

    auto &skipTable = patternData.skipTable;
    for (int i = 0; i < 256; i++)
    {
      skipTable[i] = maskedPatternLen;
    }
    for (size_t i = (lastPatternI - maskedPatternLen); i < (lastPatternI); i++)
    {
      skipTable[(size_t)patternBytes[i]] = (unsigned char)(lastPatternI - i);
    }
  }

  return true;
}

// Clean the patterns lists to avoid memory leaks
void MemoryScanner::cleanPatternArrays()
{
  for (auto &patternData : this->patternList)
  {
    delete[] patternData.patternBytes;
    patternData.patternBytes = nullptr;

    delete[] patternData.patternMask;
    patternData.patternMask = nullptr;

    patternData.patternLen = 0;
  }
}

// The actual search
ScanlibResult MemoryScanner::search()
{
  bool patternParsed = this->parsePatterns();
  if (!patternParsed)
  {
    this->cleanPatternArrays();
    return ScanlibResult::SCANLIB_ERR_PARSE_PATTERN;
  }

  HMODULE moduleHandle = GetModuleHandleA(NULL);
  if (!moduleHandle)
  {
    this->cleanPatternArrays();
    return ScanlibResult::SCANLIB_INVALID_MODULE_HANDLE;
  }

  MODULEINFO moduleInfo{};
  HANDLE process = GetCurrentProcess();
  bool gotModuleInfo = GetModuleInformation(process, moduleHandle, &moduleInfo, sizeof(moduleInfo));
  if (!gotModuleInfo)
  {
    this->cleanPatternArrays();
    return ScanlibResult::SCANLIB_ERR_GETTING_MODULE_INFO;
  }

  uintptr_t baseExeAddr = (uintptr_t)moduleHandle;
  uintptr_t absoluteEndAddr = baseExeAddr + moduleInfo.SizeOfImage;

  uintptr_t currentPageAddr = baseExeAddr;
  while (currentPageAddr < absoluteEndAddr)
  {
    MEMORY_BASIC_INFORMATION memPageInfo{};
    SIZE_T queryResult = VirtualQuery(reinterpret_cast<LPCVOID>(currentPageAddr), &memPageInfo, sizeof(memPageInfo));
    if (queryResult == 0) // Finished readable memory
      break;

    uintptr_t currentAddr = reinterpret_cast<uintptr_t>(memPageInfo.BaseAddress);
    uintptr_t regionSize = (uintptr_t)memPageInfo.RegionSize;

    currentPageAddr = currentAddr + regionSize; // Prepare for the next run

    if (memPageInfo.State != MEM_COMMIT ||
        (memPageInfo.Protect & PAGE_GUARD) ||
        (memPageInfo.Protect & PAGE_NOACCESS)) // Skip unreadable zones
      continue;

    for (auto &pattern : patternList)
    {
      if (pattern.found)
        continue;

      uintptr_t skip = 0;
      while ((regionSize - skip) >= pattern.patternLen)
      {
        if (!maskedMemcmp((void *)(currentAddr + skip), pattern.patternBytes, pattern.patternMask, pattern.patternLen))
        {
          pattern.found = true;
          pattern.pattern->foundAddr = skip + currentAddr;
          break;
        }
        skip += pattern.skipTable[((unsigned char *)(currentAddr))[skip + pattern.patternLen - 1UL]];
      }
    }

    // If all patterns have been found stop scanning
    if (std::all_of(this->patternList.begin(), this->patternList.end(), [](const auto &pattern)
                    { return pattern.found; }))
    {
      break;
    }
  }

  this->cleanPatternArrays();
  return ScanlibResult::SCANLIB_OK;
}

// ######################################################

extern "C"
{
  void Scanlib_AddPattern(MemoryPattern *pattern)
  {
    if (!pattern)
      return;
    MemoryScanner::instance.addPattern(pattern);
  }

  void Scanlib_ClearPatternList()
  {
    MemoryScanner::instance.clear();
  }

  ScanlibResult Scanlib_Search()
  {
    return MemoryScanner::instance.search();
  }
}