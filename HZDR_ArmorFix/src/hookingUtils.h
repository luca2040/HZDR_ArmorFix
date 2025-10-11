#pragma once
#include "external/minhook/include/MinHook.h"

template <typename func_t>
bool createAndEnableHook(func_t funcAddr, func_t funcDetour, func_t &funcOriginal)
{
  if (MH_CreateHook(reinterpret_cast<LPVOID>(funcAddr),
                    reinterpret_cast<LPVOID>(funcDetour),
                    reinterpret_cast<LPVOID *>(&funcOriginal)) != MH_OK)
    return false;
  if (MH_EnableHook(reinterpret_cast<LPVOID>(funcAddr)) != MH_OK)
    return false;

  return true;
}