//
//  ItemRanged.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/22/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemRanged.hpp"
#include "Utility.hpp"

void ItemRanged::save(vector<unsigned char>* data) {
    ItemWeapon::save(data);

    Utility::saveDouble(data, range);
}

void ItemRanged::load(vector<unsigned char>* data, int* position) {
    ItemWeapon::load(data, position);

    range = Utility::loadDouble(data, position);
}

ItemRanged* ItemRanged::cloneUnsafe(ItemRanged* oldE, ItemRanged* newE) {

    ItemWeapon::cloneUnsafe(oldE, newE);

    newE->range = oldE->range;


    return newE;

}
