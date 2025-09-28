// All the offsets in this test refer to version 1.5.89.0 from the Epic Games store

// Very simple test in "injectable-dll" form
// The game probably doesnt like this since it crashes on closing, but whatever, this is just a test

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

#include "structures.h"

typedef bool (*updateTransmog_t)(void *playerViewController, char *newTransmogGuid);

// This will be replaced by pattern scanning
uintptr_t updateTransmogOffset = 0x0d62a20;
// And also for stability better getting this by detouring the menu init function (exe + 0x2be8d2) and the menu pointer is parameter 1 (RCX)
uintptr_t menuPointerOffset = 0x99a2170;

DWORD WINAPI ThreadMain(LPVOID)
{
  Sleep(100);

  HMODULE h = GetModuleHandleA(NULL);
  uintptr_t baseExeAddr = (uintptr_t)h;

  updateTransmog_t updateTransmog = (updateTransmog_t)(baseExeAddr + updateTransmogOffset);
  Menu *menuPointer = *(Menu **)(baseExeAddr + menuPointerOffset);

  if (menuPointer && menuPointer->menuViewControllers && menuPointer->menuViewControllers->playerViewController)
  {
    EquipementViewController *playerViewController = menuPointer->menuViewControllers->playerViewController;
    char newTransmogGuid[] = "a23db10b-1ccd-4c22-b665-2785b119922b"; // Some random armor i have in my inventory to test this code

    updateTransmog(playerViewController, newTransmogGuid); // Change the current transmog to the one indicated by the guid
  }
  else
  {
    MessageBoxA(NULL, "Invalid Pointer", "Title", MB_OK | MB_ICONINFORMATION);
  }

  return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH)
  {
    DisableThreadLibraryCalls(hinst);
    HANDLE h = CreateThread(NULL, 0, ThreadMain, NULL, 0, NULL);
    if (h)
      CloseHandle(h);
  }
  return TRUE;
}