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

#define modName "HZDR_ArmorFix"
#include "winmmProxy.h"
#include "gameStructures.h"
#include "hookingUtils.h"

// #######################################################

updateTransmog_t updateTransmog;
checkItemEquipped_t checkItemEquipped;
Menu *menuPointer;

resetMenu_t resetMenu;
resetMenu_t resetMenu_original;
applyMenuData_t applyMenuData;
applyMenuData_t applyMenuData_original;
syncUIElements_t syncUIElements;
syncUIElements_t syncUIElements_original;

Menu *__fastcall resetMenu_detour(Menu *_menuPointer)
{
    menuPointer = _menuPointer;
    return resetMenu_original(_menuPointer);
}

void *applyMenuData_detour(void *param_1)
{
    void *callResult = applyMenuData_original(param_1);

    if (menuPointer &&
        menuPointer->menuViewControllers &&
        menuPointer->menuViewControllers->playerViewController &&
        menuPointer->menuViewControllers->playerViewController->selectedArmorSlot &&
        menuPointer->menuViewControllers->playerViewController->selectedArmorSlot->transmogInventoryEntity)
    {
        EquipementViewController *equipement = menuPointer->menuViewControllers
                                                   ->playerViewController;
        char *itemName = equipement
                             ->selectedArmorSlot
                             ->transmogInventoryEntity
                             ->itemName;

        std::cout << "Equipement view controller: " << equipement
                  << " - Item name: " << itemName << std::endl;
    }

    return callResult;
}

void __fastcall syncUIElements_detour(DataSourceOutfitInventory *dataSourceUI, void *currentUIPointer,
                                      InventoryEntity *itemEntity, int param_4, uint64_t param_5, uint32_t param_6)
{
    syncUIElements_original(dataSourceUI, currentUIPointer, itemEntity, param_4, param_5, param_6);

    bool isTransmogApplied;
    int local_res20[2];
    EquipementSlotComponent *selectedArmorSlot = dataSourceUI->equipmentSlot;
    uint64_t lVar7 = checkItemEquipped(selectedArmorSlot, itemEntity, local_res20);
    if ((lVar7 == 0) || (*(char *)(lVar7 + 0x28) != '\x05'))
    {
        isTransmogApplied = false;
    }
    else
    {
        local_res20[0] = -1;
        checkItemEquipped(selectedArmorSlot, itemEntity, local_res20);
        isTransmogApplied = local_res20[0] == 1;
    }

    char *itemName = itemEntity->itemName;
    std::cout << "syncUIElements: - " << itemName << " - Applied: " << (isTransmogApplied ? "True" : "False") << std::endl;

    return;
}

// #######################################################

void modMain()
{
#ifdef _DEBUG
    // Pop out a console for debugging
    AllocConsole();
    freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE **)stdin, "CONIN$", "r", stdin);
#endif

    std::cout << "Starting " << modName << std::endl;

    MemoryPattern updateTransmogScan{updateTransmogSignatureStr};
    MemoryPattern resetMenuScan{resetMenuSignatureStr};
    MemoryPattern applyMenuDataScan{applyMenuDataSignatureStr};
    MemoryPattern syncUIElementsScan{syncUIElementsSignatureStr};
    MemoryPattern checkItemEquippedScan{checkItemEquippedSignatureStr};

    Scanlib_AddPattern(&updateTransmogScan);
    Scanlib_AddPattern(&resetMenuScan);
    Scanlib_AddPattern(&applyMenuDataScan);
    Scanlib_AddPattern(&syncUIElementsScan);
    Scanlib_AddPattern(&checkItemEquippedScan);

    std::cout << "Starting scan" << std::endl;

    ScanlibResult res = Scanlib_Search();
    if (res != SCANLIB_OK)
    {
        MessageBoxA(nullptr, "Error scanning memory", modName, MB_ICONERROR);
        return;
    }

    // Print them before checking if they are all good to know which one wasnt found in case it isnt
    if (updateTransmogScan.foundAddr)
        std::cout << "updateTransmog function found at: " << updateTransmogScan.foundAddr << std::endl;
    if (resetMenuScan.foundAddr)
        std::cout << "resetMenu function found at: " << resetMenuScan.foundAddr << std::endl;
    if (applyMenuDataScan.foundAddr)
        std::cout << "applyMenuData function found at: " << applyMenuDataScan.foundAddr << std::endl;
    if (syncUIElementsScan.foundAddr)
        std::cout << "syncUIElements function found at: " << syncUIElementsScan.foundAddr << std::endl;
    if (checkItemEquippedScan.foundAddr)
        std::cout << "checkItemEquipped function found at: " << checkItemEquippedScan.foundAddr << std::endl;

    if ((!updateTransmogScan.foundAddr) ||
        (!resetMenuScan.foundAddr) ||
        (!applyMenuDataScan.foundAddr) ||
        (!syncUIElementsScan.foundAddr) ||
        (!checkItemEquippedScan.foundAddr))
    {
        MessageBoxA(nullptr, "Error finding functions", modName, MB_ICONERROR);
        return;
    }

    updateTransmog = reinterpret_cast<updateTransmog_t>(updateTransmogScan.foundAddr);
    resetMenu = reinterpret_cast<resetMenu_t>(resetMenuScan.foundAddr);
    applyMenuData = reinterpret_cast<applyMenuData_t>(applyMenuDataScan.foundAddr);
    syncUIElements = reinterpret_cast<syncUIElements_t>(syncUIElementsScan.foundAddr);
    checkItemEquipped = reinterpret_cast<checkItemEquipped_t>(checkItemEquippedScan.foundAddr);

    if (MH_Initialize() != MH_OK)
    {
        MessageBoxA(nullptr, "Cannot initialize hooking library", modName, MB_ICONERROR);
        return;
    }

    bool hook1ok = createAndEnableHook(resetMenu, resetMenu_detour, resetMenu_original);
    bool hook2ok = createAndEnableHook(applyMenuData, applyMenuData_detour, applyMenuData_original);
    bool hook3ok = createAndEnableHook(syncUIElements, syncUIElements_detour, syncUIElements_original);

    if (!(hook1ok && hook2ok && hook3ok))
    {
        MessageBoxA(nullptr, "Cannot hook functions", modName, MB_ICONERROR);
        return;
    }

    std::cout << "Correctly loaded " << modName << std::endl;

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
