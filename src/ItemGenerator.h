//
//  ItemGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemGenerator__
#define __Underneath__ItemGenerator__


#include "Spell.h"


namespace ItemGenerator {

    Weapon* setDamageType(Weapon* w, DamageType d);

    extern Item* iCoin;

    extern Weapon* wKnife;
    extern Weapon* wShortSword;
    extern Weapon* wLongSword;
    extern Weapon* wBattleAxe;
    extern Weapon* wMase;
    extern Weapon* wSpear;

    extern Ranged* wRecurveBow;
    extern Ranged* wLongbow;
    extern Ranged* wCrossbow;

	
    extern vector<Weapon*> weaponList;

    Weapon* atl(Weapon* w);


    Weapon* createWeaponBase(DamageType d);
    Weapon* createWeapon(string name, Material* material, DamageType damageType, bool enchanted);

    void initItemTemplates();
    void cleanupItemTemplates();


}


#endif /* defined(__Underneath__ItemGenerator__) */
