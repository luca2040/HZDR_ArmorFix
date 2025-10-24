#pragma once
#include <cstdint>

struct InventoryEntity
{
  void *_unk0;      // 0x00
  char _pad0[0x18]; // 0x08
  void *_unk1;      // 0x20
  char _pad1[0x8];  // 0x28
  void *_unk2;      // 0x30
  char _pad2[0x30]; // 0x38
  char *itemName;   // 0x68
};

struct EquipementSlotComponent
{
  char _pad0[0x88];                         // 0x00
  InventoryEntity *transmogInventoryEntity; // 0x88
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

struct DataSourceOutfitInventory
{
  void *_unk0;                             // 0x000
  char _pad0[0x18];                        // 0x008
  void *_unk1;                             // 0x020
  void *_unk2;                             // 0x028
  char _pad1[0x20];                        // 0x030
  void *_unk3;                             // 0x050
  char _pad2[0x48];                        // 0x058
  DataSourceOutfitInventory *_unk4;        // 0x0A0
  char _pad3[0x8];                         // 0x0A8
  void *dataSourceOutfitInventoryResource; // 0x0B0
  char _pad4[0x30];                        // 0x0B8
  void *humanoidInventory;                 // 0x0E8
  char _pad5[0x10];                        // 0x0F0
  void *inventoryController;               // 0x100
  EquipementSlotComponent *equipmentSlot;  // 0x108
  void *inventoryCapacityComponent;        // 0x110
  char _pad6[0x8];                         // 0x118
  void *walletComponent;                   // 0x120
  void *disassembleComponent;              // 0x128
  void *craftingComponent;                 // 0x130
};

struct DataSources
{
  void *dataSourceFacts;                                // 0x000
  char _pad0[0x8];                                      // 0x008
  void *dataSourcePlayerCharacterPerks;                 // 0x010
  char _pad1[0x8];                                      // 0x018
  void *dataSourceEntitlements;                         // 0x020
  char _pad2[0x18];                                     // 0x028
  void *dataSourceResourceInventory;                    // 0x040
  char _pad3[0x8];                                      // 0x048
  void *dataSourceModificationInventory;                // 0x050
  char _pad4[0x8];                                      // 0x058
  void *dataSourceLootBoxInventory;                     // 0x060
  char _pad5[0x8];                                      // 0x068
  void *dataSourceSpecialItemInventory;                 // 0x070
  char _pad6[0x8];                                      // 0x078
  void *dataSourceToolInventory;                        // 0x080
  char _pad7[0x8];                                      // 0x088
  DataSourceOutfitInventory *dataSourceOutfitInventory; // 0x090
  char _pad8[0x8];                                      // 0x098
  void *dataSourceAmmoInventory;                        // 0x0A0
  char _pad9[0x8];                                      // 0x0A8
  void *dataSourceLootPackageInventory;                 // 0x0B0
  char _pad10[0x8];                                     // 0x0B8
  void *dataSourceInventoryCapacity;                    // 0x0C0
  char _pad11[0x8];                                     // 0x0C8
  void *dataSourceModify;                               // 0x0D0
  char _pad12[0x8];                                     // 0x0D8
  void *dataSourceAmmoRecipeGroups;                     // 0x0E0
  char _pad13[0x8];                                     // 0x0E8
  void *dataSourceAmmoCrafting;                         // 0x0F0
  char _pad14[0x8];                                     // 0x0F8
  void *dataSourceToolRecipeGroups;                     // 0x100
  char _pad15[0x8];                                     // 0x108
  void *dataSourceToolCrafting;                         // 0x110
  char _pad16[0x8];                                     // 0x118
  void *dataSourceUpgradeRecipeGroups;                  // 0x120
  char _pad17[0x8];                                     // 0x128
  void *dataSourceUpgradeCrafting;                      // 0x130
  char _pad18[0x8];                                     // 0x138
  void *dataSourceLocationMarker;                       // 0x140
  char _pad19[0x8];                                     // 0x148
  void *dataSourceMapZone;                              // 0x150
  char _pad20[0x8];                                     // 0x158
  void *dataSourceQuests;                               // 0x160
  char _pad21[0x8];                                     // 0x168
  void *dataSourceMapIconFilter;                        // 0x170
  char _pad22[0x8];                                     // 0x178
  void *dataSourceRobotCatalogue;                       // 0x180
  char _pad23[0x8];                                     // 0x188
  void *dataSourceCollectables;                         // 0x190
  char _pad24[0x8];                                     // 0x198
  void *dataSourceDataCubes;                            // 0x1A0
  char _pad25[0x8];                                     // 0x1A8
  void *dataSourceGameStatistics;                       // 0x1B0
  char _pad26[0x8];                                     // 0x1B8
  void *dataSourceWeaponSlots;                          // 0x1C0
  char _pad27[0x8];                                     // 0x1C8
  void *dataSourceOutfitSlot;                           // 0x1D0
  char _pad28[0x8];                                     // 0x1D8
  void *dataSourceMenuBadgeCount;                       // 0x1E0
};

struct MenuDataSystem
{
  void *_unk0;              // 0x00
  void *_Menu_unk1;         // 0x08
  char _pad0[0x8];          // 0x10
  DataSources *dataSources; // 0x18
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
  char _pad2[0x48];                     // 0x48
  void *menuPageSystem;                 // 0x90
  void *menuTagControlSystem;           // 0x98
  MenuDataSystem *dataSystem;           // 0xA0
  void *menuScriptSystem;               // 0xA8
  void *menuInputSystem;                // 0xB0
  void *menuRenderSystem;               // 0xB8
  void *menuImageLoaderSystem;          // 0xC0
  void *menuSoundSystem;                // 0xC8
  void *menuRumbleSystem;               // 0xD0
};

// typedef bool (*updateTransmog_t)(EquipementViewController *playerViewController, char *newTransmogGuid);
typedef Menu *(*resetMenu_t)(Menu *menuPointer);
typedef void *(*applyMenuData_t)(void *param_1);
typedef void (*syncUIElements_t)(DataSourceOutfitInventory *dataSourceUI, void *currentUIPointer,
                                 InventoryEntity *itemEntity, int param_4, uint64_t param_5, uint32_t param_6);
typedef uint64_t (*checkItemEquipped_t)(EquipementSlotComponent *selectedArmorSlot, InventoryEntity *item, int *resultArray);
typedef char (*FUN_141114c30_t)(uint64_t param_1, void *param_2);
typedef uint64_t (*directUpdateTransmog_t)(EquipementViewController *playerViewController, InventoryEntity *newTransmog, char param_3, char param_4);

// #define updateTransmogSignatureStr "48 8B C4 41 56 48 83 EC 60 48 89 58 10 49 C7 C6 FF FF FF FF 48 89 68 18 48 8B EA 48 89 70 F0 49 8B F6 4C 89 60 E0 4C 89 78 D8 4C 8B F9 0F 1F 00 48 FF C6 80 3C 16 00 ? ? 48 89 7C 24 50 4C 8D 25 DB 63 F8 00 85 F6 ? ? 80 3A 00 ? ? 65 48 8B 04 25 58 00 00 00 BA 00 1A 00 00 48 63 FE 48 8B 08 48 8B 04 0A 48 8D 57 11 48 8B 0D F7 F5 C3 08 48 85 C0 48 0F 45 C8 48 8B 01 ? ? ? 33 D2 8B CE 49 3B C4 4C 8B C7 0F 44 CA C7 00 01 00 00 00 48 8D 58 10 89 48 0C 48 8B CB 89 73 F8 48 8B D5 C7 43 F4 FF FF FF FF ? ? ? ? ? 48 63 43 F8 C6 04 18 00 ? ? 48 8D 1D 72 63 F8 00 8B 43 F8 48 8D 54 24 20 48 8D 4C 24 30 89 44 24 28 48 89 5C 24 70 48 89 5C 24 20 C7 44 24 2C FF FF FF FF ? ? ? ? ? 48 8B 74 24 58 48 8B AC 24 80 00 00 00 84 C0 ? ? C5 F8 10 05 19 4C"
#define resetMenuSignatureStr "33 D2 48 89 0D 97 38 6E 09 48 89 51 10 48 8D 05 5C EE 5E 01 48 89 51 08 48"
#define applyMenuDataSignatureStr "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 48 89 4C 24 08 57 41 54 41 55 41 56 41 57 48 83 EC 30 48 8B 35 ? ? ? ? 33 FF 4C 8B E9 48 85 F6"
#define syncUIElementsSignatureStr "48 89 5C 24 10 44 89 4C 24 20 55 56 57 41 56 41 57 48 83 EC 30 48 8B FA 48 8B D9"
#define checkItemEquippedSignatureStr "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 30 4D 8B F8 4C 8B F2 48 8B F9 48"
#define FUN_141114c30SignatureStr "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B FA 48 8B F1 48 8D 15 54 3E 02 01 48 8D 8F A0 02 00 00 ? ? ? ? ? 48 85 C0"
#define directUpdateTransmogSignatureStr "40 56 41 54 41 56 41 57 48 83 EC 48 45 0F B6 E1 45 0F B6 F8 4C 8B F2"

#define testFuncTransmogSignature "48 89 5c 24 08 57 48 83 ec 20 49 8b 01 48 8b d9 33 d2 49 8b c9 49 8b f8 ff 50 28 48 8b d0 48 8b cb e8 fa 1a fb ff 48 8b 0f 48 8b 5c 24 30 88 01 48 83 c4 20 5f c3"
typedef void (*FUN_140db0f00_t)(EquipementViewController *playerViewController, uint8_t param_2, void *param_3, void *selectedTransmog);