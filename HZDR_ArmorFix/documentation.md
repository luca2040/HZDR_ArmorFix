Structures:

```c
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
```

---

Function:

```c
bool __fastcall updateTransmog(EquipementViewController *playerViewController, char *newTransmogGuid)
```

Signature:

```
48 8B C4 41 56 48 83 EC 60 48 89 58 10 49 C7 C6 FF FF FF FF 48 89 68 18 48 8B EA 48 89 70 F0 49 8B F6 4C 89 60 E0 4C 89 78 D8 4C 8B F9 0F 1F 00 48 FF C6 80 3C 16 00 ? ? 48 89 7C 24 50 4C 8D 25 DB 63 F8 00 85 F6 ? ? 80 3A 00 ? ? 65 48 8B 04 25 58 00 00 00 BA 00 1A 00 00 48 63 FE 48 8B 08 48 8B 04 0A 48 8D 57 11 48 8B 0D F7 F5 C3 08 48 85 C0 48 0F 45 C8 48 8B 01 ? ? ? 33 D2 8B CE 49 3B C4 4C 8B C7 0F 44 CA C7 00 01 00 00 00 48 8D 58 10 89 48 0C 48 8B CB 89 73 F8 48 8B D5 C7 43 F4 FF FF FF FF ? ? ? ? ? 48 63 43 F8 C6 04 18 00 ? ? 48 8D 1D 72 63 F8 00 8B 43 F8 48 8D 54 24 20 48 8D 4C 24 30 89 44 24 28 48 89 5C 24 70 48 89 5C 24 20 C7 44 24 2C FF FF FF FF ? ? ? ? ? 48 8B 74 24 58 48 8B AC 24 80 00 00 00 84 C0 ? ? C5 F8 10 05 19 4C
```

---

Function:

```c
Menu * __fastcall resetMenu(Menu *menuPointer)
```

Signature:

```
33 D2 48 89 0D 97 38 6E 09 48 89 51 10 48 8D 05 5C EE 5E 01 48 89 51 08 48
```

---

Function:

```c
void * applyMenuData(void *param_1)
```

Signature:

```
48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 48 89 4C 24 08 57 41 54 41 55 41 56 41 57 48 83 EC 30 48 8B 35 ? ? ? ? 33 FF 4C 8B E9 48 85 F6
```

---

Function:

```c
void __fastcall syncUIElements(DataSourceOutfitInventory *dataSourceUI, void *currentUIPointer, InventoryEntity *itemEntity, int param_4, uint64_t param_5, uint32_t param_6)
```

Signature:

```
48 89 5C 24 10 44 89 4C 24 20 55 56 57 41 56 41 57 48 83 EC 30 48 8B FA 48 8B D9
```

---

Function:

```c
uint64_t __fastcall checkItemEquipped(EquipementSlotComponent *selectedArmorSlot, InventoryEntity *item, int *resultArray)
```

Signature:

```
48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 30 4D 8B F8 4C 8B F2 48 8B F9 48
```

---

Function:

```c
char FUN_141114c30(uint64_t param_1, void *param_2)
```

Yes this is not very helpful, but since i need to just call it i just copied it straight from Ghidra as it is

Signature:

```
48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B FA 48 8B F1 48 8D 15 54 3E 02 01 48 8D 8F A0 02 00 00 ? ? ? ? ? 48 85 C0
```
