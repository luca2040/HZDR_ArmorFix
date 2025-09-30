#pragma once
#include <cstdint>

struct EquipementSlotComponent
{
  char _pad0[0x88];              // 0x00
  void *transmogInventoryEntity; // 0x88
};

struct EquipementViewController
{
  void *_unk0;                                // 0x00
  void *soldier;                              // 0x08
  void *humanoidInventory;                    // 0x10
  EquipementSlotComponent *selectedArmorSlot; // 0x18
  void *inventoryController;                  // 0x20
};

struct ViewControllers
{
  char _pad0[0xD0];                               // 0x000
  void *careerUnlockSystem;                       // 0x0D0
  void *ammoCraftingViewController;               // 0x0D8
  void *toolCraftingViewController;               // 0x0E0
  void *weaponCraftingViewController;             // 0x0E8
  void *outfitCraftingViewController;             // 0x0F0
  void *upgradeCraftingViewController;            // 0x0F8
  EquipementViewController *playerViewController; // 0x100
  void *merchantViewController;                   // 0x108
  void *questsViewController;                     // 0x110
  void *entityInventoryViewController;            // 0x118
  void *robotNotebookViewController;              // 0x120
  void *inventoryLootPackageViewController;       // 0x128
  void *modificationViewController;               // 0x130
  void *collectiblesViewController;               // 0x138
  void *saveSystemViewController;                 // 0x140
  void *mapViewController;                        // 0x148
  void *inputRemappingMenuController;             // 0x150
};

struct Menu
{
  void *_unk0;                          // 0x00
  char _pad0[0x18];                     // 0x08
  void *inGameMenuResource;             // 0x20
  char _pad1[0x08];                     // 0x28
  void *_unk1;                          // 0x30
  void *_unk2;                          // 0x38
  ViewControllers *menuViewControllers; // 0x40
};

// All the offsets in this test refer to version 1.5.89.0 from the Epic Games store

typedef bool (*updateTransmog_t)(EquipementViewController *playerViewController, char *newTransmogGuid);
// This will be replaced by pattern scanning
uintptr_t updateTransmogOffset = 0x0d62a20;
// And also for stability better getting this by detouring the menu init function (exe + 0x2be8d2) and the menu pointer is parameter 1 (RCX)
uintptr_t menuPointerOffset = 0x99a2170;