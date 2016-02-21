//
//  EntityPlayer.hpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityPlayer__
#define __Underneath__EntityPlayer__

#include "EntityAlive.hpp"
#include "ItemWeapon.hpp"
#include "ItemCombatSpell.hpp"
#include "Abilities.hpp"
#include "ItemArmor.hpp"

class EntityPlayer : public EntityAlive {
public:

    static EntityPlayer* cloneUnsafe(EntityPlayer* oldE, EntityPlayer* newE);

    EntityPlayer();

    EntityPlayer(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities);

    ~EntityPlayer();

    bool update(double deltaTime, double time, Level* world);

    double moveRelative(Point2 p, Level* level);

    double moveAbsalute(Point2, Level* level, bool canInteract);

    double useItemOnOther(Item* itemToUse, Item* itemToBeUsedOn);
    
    int repairToolPower();

    double interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item, bool okayToInteractWithSelf);

    double interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item);

    double interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item);

    double calcDamageMultiplier(ItemWeapon* weapon);

    virtual int getEntityTypeId();
    
    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual double hurt(Level* level, DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(Level* level, ItemWeapon* w, double damageMultiplier = 1);

    double hurtMiddleStep(Level* level, DamageType damageType, double damageMultiplier);

    virtual double heal(double amount) {
        double a = EntityAlive::heal(amount);
        return a;
    }

    virtual double healMana(double amount) {
        double a = EntityAlive::healMana(amount);
        return a;
    }

    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon);

    virtual ItemWeapon* getActiveItemWeapon() {
        return dynamic_cast<ItemWeapon*>(equipedItems[slotWep1]);
    }

    virtual ItemWeapon* getSecondaryItemWeapon() {
        return dynamic_cast<ItemWeapon*>(equipedItems[slotWep2]);
    }

    EquipSlot getSlot(Item* e){
        for(pair<EquipSlot, Item*> p : equipedItems){
            if(p.second == e){
                return p.first;
            }
        }
        return slotNone;
    }

    virtual Item* getEquiped(EquipSlot slot) {
        if(equipedItems.count(slot) <= 0){
            return nullptr;
        }
        return equipedItems[slot];
    }

    virtual bool removeItem(Item* item, bool deleteItem);

    void updateVariablesForAbilities();

    Abilities<int> abilities;
    int abilityPoints = 0;
    int level = 0;
    double xp = 0;
    double nextLevelXp = 0;

    //                                           STR     DEX     INT     AGI     CON     WIS     LUK
    Abilities<int> maxAbilities = Abilities<int>(999999, 999999, 999999, 999999, 999999, 999999, 999999);

    void gainXp(double amount);

    bool equipItem(ItemEquipable* newItem);
    bool equipItem(Item* newItem, EquipSlot slot);


    //don't need to save
    bool leveledUp = false;


    int xpForLevel(int l);

    double getRecivedDamageMultiplierFromArmorAndEquips(DamageType damType/*, bool reduceDurability*/);

    virtual double getAttackMultiplierFromEffectsAndEquips(DamageType damType);


    double useDelay(Item* item);

    map<EquipSlot, Item*> equipedItems;


    double interactDelay = .1;
    double strMult = 1;
    double dexMult = 1;
    double intMult = 1;


protected:
    int timeSinceCombat = 0;

    void setNextLevelXp() {
        nextLevelXp = xpForLevel(level);
    }

};

#endif /* defined(__Underneath__EntityPlayer__) */
