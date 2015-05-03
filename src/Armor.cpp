//
//  Armor.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Armor.h"
#include "Utility.h"

Armor::Armor(vector<EquipSlot> viableSlots, vector<Defense> defenses, string name, double weight) : Equipable(name, weight) {
    this->viableSlots = viableSlots;
    this->defenses = defenses;
}

Armor::Armor(EquipSlot viableSlot, vector<Defense> defenses, string name, double weight) : Armor(vector<EquipSlot>{viableSlot}, defenses, name, weight) {
}

void Armor::save(vector<unsigned char>* data) {
    Equipable::save(data);

    Utility::saveInt(data, (int) enchantments.size());
    for (Enchantment ench : enchantments) {
        ench.save(data);
    }

    Utility::saveInt(data, (int) defenses.size());
    for (Defense def : defenses) {
        def.save(data);
    }

    Utility::saveInt(data, (int) viableSlots.size());
    for (EquipSlot slot : viableSlots) {
        Utility::saveInt(data, slot);
    }
}

void Armor::load(unsigned char* data, int* position) {
    Equipable::load(data, position);

    int size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        enchantments.push_back(Enchantment(data, position));
    }

    size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        defenses.push_back(Defense(data, position));
    }

    size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        viableSlots.push_back(Utility::loadInt(data, position));
    }
}

Armor* Armor::cloneUnsafe(Armor* oldE, Armor* newE) {

    Equipable::cloneUnsafe(oldE, newE);

    newE->defenses = oldE->defenses;
    newE->enchantments = oldE->enchantments;
    newE->viableSlots = oldE->viableSlots;
    
    
    return newE;
    
}

bool operator==(const Defense& a, const Defense& b){
    return a.amount == b.amount && a.damageType == b.damageType;
}

bool operator!=(const Defense& a, const Defense& b){
    return !(a==b);
}