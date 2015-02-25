//
//  ItemGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "Random.h"
#include "ItemGenerator.h"

namespace ItemGenerator {

	
    vector<Weapon*> weaponList;

    //DamageMultiplier, Name, Weight, Use Delay
    //DamageMultiplier, Name, Weight, Use Delay, Range

    Item* iCoin;

    Weapon* wKnife;
    Weapon* wShortSword;
    Weapon* wLongSword;
    Weapon* wBattleAxe;
    Weapon* wMase;
    Weapon* wSpear;

    Ranged* wRecurveBow;
    Ranged* wLongbow;
    Ranged* wCrossbow;

    void initItemTemplates(){

        iCoin = new Item("Coin", .01);

        wKnife = atl(new Weapon(materialNone, .5, "Knife", 1, .5));
        wShortSword = atl(new Weapon(materialNone, 1, "Short Sword", 1, 1));
        wLongSword = atl(new Weapon(materialNone, 1.5, "Long Sword", 1.5, 1.5));
        wBattleAxe = atl(new Weapon(materialNone, 1.2, "Battle Axe", 1.2, 1.2));
        wMase = atl(new Weapon(materialNone, 1.4, "Mase", 1.4, 1.4));
        wSpear = setDamageType(atl(new Ranged(materialNone, 1, "Spear", 2, 1.5, 1.8)), damMelee);

        wRecurveBow = dynamic_cast<Ranged*>(atl(new Ranged(materialNone, .5, "Recurve Bow", 1.5, 1, 8)));
        wLongbow = dynamic_cast<Ranged*>(atl(new Ranged(materialNone, .4, "Longbow", 2, .8, 10)));
        wCrossbow = dynamic_cast<Ranged*>(atl(new Ranged(materialNone, .3, "Crossbow", 2, .6, 6)));
    }

    void cleanupItemTemplates(){
        for(Weapon* w : weaponList){
            delete w;
        }
    }



    Weapon* atl(Weapon* w){//add to list
        weaponList.push_back(w);
        return w;
    }

    Weapon* setDamageType(Weapon* w, DamageType d){
        w->damageType = d;
        return w;
    }

    Weapon* createWeaponBase(DamageType d){
        Weapon* w = nullptr;
        while(w == nullptr){
            size_t i = (rand())%weaponList.size();
            if(weaponList[i]->damageType == d){
                w = weaponList[i];
            }
        }

        return dynamic_cast<Weapon*>(w->clone(w));

    }

    Weapon* createWeapon(string name, Material* material, DamageType damageType, bool enchanted){

        double combatLevelMultiplier = (material->getMultipler()) * 0.5;

        Weapon* weapon = createWeaponBase(damageType);


        weapon->material = material;
        weapon->overrideName = name;
        weapon->baseDamage *= combatLevelMultiplier;
        weapon->weight *= combatLevelMultiplier;

        weapon->baseDamage *= Random::randDouble(.8, 1.2);
        weapon->weight *= Random::randDouble(.6, 1.4);
        weapon->useDelay *= Random::randDouble(.9, 1.1);

        if(weapon->damageType == damRanged){
            Ranged* ranged = dynamic_cast<Ranged*>(weapon);
            if(ranged){
            	ranged->range *= combatLevelMultiplier;
            }
        }

        if(enchanted){
            int eid = rand()%2==0?enchBleed:enchFire;
            weapon->addEnchantment(eid, 10, 1);
        }
        
        return weapon;
    }


}
