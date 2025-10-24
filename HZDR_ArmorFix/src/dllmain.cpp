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

// updateTransmog_t updateTransmog;
directUpdateTransmog_t directUpdateTransmog;
checkItemEquipped_t checkItemEquipped;
FUN_141114c30_t FUN_141114c30;
Menu *menuPointer;

resetMenu_t resetMenu;
resetMenu_t resetMenu_original;
applyMenuData_t applyMenuData;
applyMenuData_t applyMenuData_original;
syncUIElements_t syncUIElements;
syncUIElements_t syncUIElements_original;

// #######################################################

// The same logic the game uses straight from its code
bool isTransmogApplied(DataSourceOutfitInventory *dataSourceUI, InventoryEntity *itemEntity)
{
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
    return isTransmogApplied;
}

InventoryEntity *getSelectedTransmogInvEntity(DataSourceOutfitInventory *dataSourceUI)
{
    // Iterate all item entities
    // This is kind of a mess since its just copy-pasted from Ghidra and i completely decompiled just what i needed
    char cVar3;
    int *piVar15 = (int *)((unsigned char *)dataSourceUI->humanoidInventory + 0x38);
    if (dataSourceUI->humanoidInventory == 0)
    {
        piVar15 = (int *)0x58;
    }
    long long *plVar20 = *(long long **)(piVar15 + 2);
    long long *plVar12 = plVar20 + *piVar15;
    long long *local_res20 = plVar12;
    InventoryEntity *currentInvEntity = nullptr;
    if (plVar20 != plVar12)
    {
        do
        {
            currentInvEntity = (InventoryEntity *)(*plVar20);
            if ((currentInvEntity) && (cVar3 = FUN_141114c30(*(uint64_t *)&dataSourceUI->inventoryCapacityComponent, currentInvEntity), plVar12 = local_res20, cVar3 == '\x04'))
            {
                // I dont know what this line does because even without it everything its fine, but it seems that for my use case it just crashes the game
                // Also it needs an unstable offset reference in game code that will almost certainly break with an update, so better avoid it
                // currentInvEntity = (InventoryEntity *)iterateGameElement(currentInvEntity + 0x2a0, (short *)(baseGameAddr + 0x213cb80));
                plVar12 = local_res20;

                if (currentInvEntity)
                {
                    bool transmogApplied = isTransmogApplied(dataSourceUI, currentInvEntity);
                    // char *itemName = currentInvEntity->itemName;
                    // std::cout << "syncUIElements: - " << itemName << " - Applied: " << (transmogApplied ? "True" : "False") << std::endl;
                    if (transmogApplied)
                        return currentInvEntity;
                }
            }
            plVar20++;
        } while (plVar20 != plVar12);
    }

    return nullptr;
}

// #######################################################

// Just for testing
InventoryEntity *oldSelectedTransmg = nullptr;

Menu *__fastcall resetMenu_detour(Menu *_menuPointer)
{
    menuPointer = _menuPointer;
    return resetMenu_original(_menuPointer);
}

FUN_140db0f00_t FUN_140db0f00;
FUN_140db0f00_t FUN_140db0f00_original;

// Temporary shitty test
void FUN_140db0f00_detour(EquipementViewController *playerViewController, uint8_t param_2, void *param_3, void *selectedTransmog)
{
    if (selectedTransmog)
    {
        typedef char *(*getGuid_t)(void *thisPtr, uint8_t param_1);

        uintptr_t vtable = *(uintptr_t *)selectedTransmog;
        getGuid_t getGuid = *(getGuid_t *)(vtable + 0x28);

        char *transmogGuid = getGuid(selectedTransmog, 0); // Call selectedTransmog.getGuid(0)
        std::cout << "transmog class instance: " << reinterpret_cast<uintptr_t>(selectedTransmog)
                  << " vtable: " << vtable
                  << " guid: " << transmogGuid << std::endl;
    }
    return FUN_140db0f00_original(playerViewController, param_2, param_3, selectedTransmog);
}

void *applyMenuData_detour(void *param_1)
{
    void *callResult = applyMenuData_original(param_1);

    if (menuPointer)
    {
        std::cout << "Menu: " << menuPointer << std::endl;
    }

    if (menuPointer &&
        menuPointer->menuViewControllers &&
        menuPointer->menuViewControllers->playerViewController)
    {
        EquipementViewController *equipement = menuPointer->menuViewControllers
                                                   ->playerViewController;

        // Apply the transmog fix here
        // This is just a test - not the actual fix for now
        // if (oldSelectedTransmg)
        //     directUpdateTransmog(equipement, oldSelectedTransmg, '\x01', '\0');
    }

    return callResult;
}

// This funciton is just used to get the dataSourceUI while testing, hopefully i can pointer scan for it and find a stable path without needing this abomination
void __fastcall syncUIElements_detour(DataSourceOutfitInventory *dataSourceUI, void *currentUIPointer,
                                      InventoryEntity *itemEntity, int param_4, uint64_t param_5, uint32_t param_6)
{
    syncUIElements_original(dataSourceUI, currentUIPointer, itemEntity, param_4, param_5, param_6);

    // This will be the final approach to get the inventory entity of the selected one
    InventoryEntity *selectedTransmog = getSelectedTransmogInvEntity(dataSourceUI);
    if (selectedTransmog)
    {
        oldSelectedTransmg = selectedTransmog;
        std::cout << "Found the applied transmog: - " << selectedTransmog->itemName << std::endl;
    }
    else
    {
        std::cout << "No transmog applied" << std::endl;
    }

    // Just print the one from the function args to test
    bool transmogApplied = isTransmogApplied(dataSourceUI, itemEntity);
    char *itemName = itemEntity->itemName;
    std::cout << "syncUIElements: - " << itemName << " - Applied: " << (transmogApplied ? "True" : "False") << std::endl
              << "dataSourceUI: " << dataSourceUI << std::endl;

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

    // MemoryPattern updateTransmogScan{updateTransmogSignatureStr};
    MemoryPattern resetMenuScan{resetMenuSignatureStr};
    MemoryPattern applyMenuDataScan{applyMenuDataSignatureStr};
    MemoryPattern syncUIElementsScan{syncUIElementsSignatureStr};
    MemoryPattern checkItemEquippedScan{checkItemEquippedSignatureStr};
    MemoryPattern FUN_141114c30Scan{FUN_141114c30SignatureStr};
    MemoryPattern directUpdateTransmogScan{directUpdateTransmogSignatureStr};

    MemoryPattern FUN_140db0f00Scan{testFuncTransmogSignature};
    Scanlib_AddPattern(&FUN_140db0f00Scan);

    // Scanlib_AddPattern(&updateTransmogScan);
    Scanlib_AddPattern(&resetMenuScan);
    Scanlib_AddPattern(&applyMenuDataScan);
    Scanlib_AddPattern(&syncUIElementsScan);
    Scanlib_AddPattern(&checkItemEquippedScan);
    Scanlib_AddPattern(&FUN_141114c30Scan);
    Scanlib_AddPattern(&directUpdateTransmogScan);

    std::cout << "Starting scan" << std::endl;

    ScanlibResult res = Scanlib_Search();
    if (res != SCANLIB_OK)
    {
        MessageBoxA(nullptr, "Error scanning memory", modName, MB_ICONERROR);
        return;
    }

    // Print them before checking if they are all good to know which one wasnt found in case it isnt
    // if (updateTransmogScan.foundAddr)
    //     std::cout << "updateTransmog function found at: " << updateTransmogScan.foundAddr << std::endl;
    if (resetMenuScan.foundAddr)
        std::cout << "resetMenu function found at: " << resetMenuScan.foundAddr << std::endl;
    if (applyMenuDataScan.foundAddr)
        std::cout << "applyMenuData function found at: " << applyMenuDataScan.foundAddr << std::endl;
    if (syncUIElementsScan.foundAddr)
        std::cout << "syncUIElements function found at: " << syncUIElementsScan.foundAddr << std::endl;
    if (checkItemEquippedScan.foundAddr)
        std::cout << "checkItemEquipped function found at: " << checkItemEquippedScan.foundAddr << std::endl;
    if (FUN_141114c30Scan.foundAddr)
        std::cout << "FUN_141114c30 function found at: " << FUN_141114c30Scan.foundAddr << std::endl;
    if (directUpdateTransmogScan.foundAddr)
        std::cout << "directUpdateTransmog function found at: " << directUpdateTransmogScan.foundAddr << std::endl;

    if (
        // (!updateTransmogScan.foundAddr) ||
        (!resetMenuScan.foundAddr) ||
        (!applyMenuDataScan.foundAddr) ||
        (!syncUIElementsScan.foundAddr) ||
        (!checkItemEquippedScan.foundAddr) ||
        (!FUN_141114c30Scan.foundAddr) ||
        (!directUpdateTransmogScan.foundAddr))
    {
        MessageBoxA(nullptr, "Error finding functions", modName, MB_ICONERROR);
        return;
    }

    // updateTransmog = reinterpret_cast<updateTransmog_t>(updateTransmogScan.foundAddr);
    resetMenu = reinterpret_cast<resetMenu_t>(resetMenuScan.foundAddr);
    applyMenuData = reinterpret_cast<applyMenuData_t>(applyMenuDataScan.foundAddr);
    syncUIElements = reinterpret_cast<syncUIElements_t>(syncUIElementsScan.foundAddr);
    checkItemEquipped = reinterpret_cast<checkItemEquipped_t>(checkItemEquippedScan.foundAddr);
    FUN_141114c30 = reinterpret_cast<FUN_141114c30_t>(FUN_141114c30Scan.foundAddr);
    directUpdateTransmog = reinterpret_cast<directUpdateTransmog_t>(directUpdateTransmogScan.foundAddr);

    FUN_140db0f00 = reinterpret_cast<FUN_140db0f00_t>(FUN_140db0f00Scan.foundAddr);

    if (MH_Initialize() != MH_OK)
    {
        MessageBoxA(nullptr, "Cannot initialize hooking library", modName, MB_ICONERROR);
        return;
    }

    bool hook1ok = createAndEnableHook(resetMenu, resetMenu_detour, resetMenu_original);
    bool hook2ok = createAndEnableHook(applyMenuData, applyMenuData_detour, applyMenuData_original);
    // bool hook3ok = createAndEnableHook(syncUIElements, syncUIElements_detour, syncUIElements_original);
    bool hook3ok = true;

    createAndEnableHook(FUN_140db0f00, FUN_140db0f00_detour, FUN_140db0f00_original);

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
