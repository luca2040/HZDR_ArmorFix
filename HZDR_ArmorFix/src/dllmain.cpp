#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <iostream>

#include "external/minhook/include/MinHook.h"
#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

#include "external/scanlib/include/scanlib.h"
#ifdef _DEBUG
#pragma comment(lib, "scanlib_Debug.lib")
#elif NDEBUG
#pragma comment(lib, "scanlib_Release.lib")
#endif

#define modName "HZDR_ArmorFix"
#include "winmmProxy.h"
#include "gameStructures.h"

updateTransmog_t updateTransmog = NULL;
updateTransmog_t updateTransmog_original = NULL;
Menu **menuPointerAddress;

bool updateTransmog_detour(EquipementViewController *playerViewController, char *newTransmogGuid)
{
    std::cout << newTransmogGuid << std::endl;
    return updateTransmog_original(playerViewController, newTransmogGuid);
}

// Not used right now and probably never will - just for the idea
void exampleSetArmor()
{
    std::string newGuidStr;
    std::cout << "Input armor GUID" << std::endl;
    std::cin >> newGuidStr;

    // Examples obtained with the detoured updateTransmog function:
    // Each one of these is a specific armor

    // 9be73d2f-100f-44ee-8886-7c1c0f5d13dd
    // 8cf2e62b-24f5-4c73-b750-f88ce3d0444b
    // a23db10b-1ccd-4c22-b665-2785b119922b
    // 443f0566-f95e-4d58-83e7-5506cf6d37e1
    // e1323e9d-a284-4e38-a903-e70e0c1f7acd
    // fd563e7e-f039-41d8-8deb-4ddec7909198
    // b4f6775e-a5d4-41d5-b99b-1b64e358d83e
    // 0c8ad44e-bd88-4f6c-b8ac-cc2e77cced4e
    // 351a7583-f531-4f57-a596-ca5dc94a552e
    // 1a2165db-2500-4050-a08b-ac9f7e9ae414
    // bf92abc8-0e1d-430c-8a88-a94db9f11743
    // 1db319ea-603b-4a23-8456-bef310058388

    char newTransmogGuid[37];
    strcpy_s(newTransmogGuid, sizeof newTransmogGuid, newGuidStr.c_str());

    Menu *menuPointer = *menuPointerAddress;
    if (menuPointer && menuPointer->menuViewControllers && menuPointer->menuViewControllers->playerViewController)
    {
        EquipementViewController *playerViewController = menuPointer->menuViewControllers->playerViewController;

        updateTransmog_original(playerViewController, newTransmogGuid);
        std::cout << "Change applied" << std::endl;
    }
    else
    {
        std::cout << "Invalid pointer" << std::endl;
    }
}

void modMain()
{
#ifdef _DEBUG
    // Pop out a console for debugging
    AllocConsole();
    freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE **)stdin, "CONIN$", "r", stdin);
#endif

    std::cout << "Starting " << modName << std::endl;

    HMODULE h = GetModuleHandleA(NULL);
    uintptr_t baseExeAddr = (uintptr_t)h;

    MemoryPattern updateTransmogScan{updateTransmogSignatureStr};
    MemoryPattern resetMenuScan{resetMenuSignatureStr};
    MemoryPattern applyMenuDataScan{applyMenuDataSignatureStr};

    Scanlib_AddPattern(&updateTransmogScan);
    Scanlib_AddPattern(&resetMenuScan);
    Scanlib_AddPattern(&applyMenuDataScan);

    std::cout << "Starting scan" << std::endl;

    ScanlibResult res = Scanlib_Search();
    if (res != SCANLIB_OK)
    {
        MessageBoxA(nullptr, "Cannot scan memory", modName, MB_ICONERROR);
        return;
    }

    if (updateTransmogScan.foundAddr)
        std::cout << "updateTransmog function SCANNED at: " << (uintptr_t)updateTransmogScan.foundAddr << std::endl;

    updateTransmog = (updateTransmog_t)(baseExeAddr + updateTransmogOffset);
    menuPointerAddress = (Menu **)(baseExeAddr + menuPointerOffset);

    std::cout << "updateTransmog function at: " << (uintptr_t)updateTransmog << std::endl;
    std::cout << "Menu pointer at: " << (uintptr_t)menuPointerAddress << std::endl;

    if (MH_Initialize() != MH_OK)
    {
        MessageBoxA(nullptr, "Cannot initialize hooking library", modName, MB_ICONERROR);
        return;
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(updateTransmog),
                      reinterpret_cast<LPVOID>(&updateTransmog_detour),
                      reinterpret_cast<LPVOID *>(&updateTransmog_original)) != MH_OK)
    {
        MessageBoxA(nullptr, "Cannot initialize hooks", modName, MB_ICONERROR);
        return;
    }
    if (MH_EnableHook(reinterpret_cast<LPVOID>(updateTransmog)) != MH_OK)
    {
        MessageBoxA(nullptr, "Cannot enable hooks", modName, MB_ICONERROR);
        return;
    }

#ifdef _DEBUG
    // Wait to launch game in debug mode
    char confirm;
    std::cout << "Launch game? (y/n)" << std::endl;
    std::cin >> confirm;
    if (confirm != 'y')
        ExitProcess(0);
#endif
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        modMain();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
