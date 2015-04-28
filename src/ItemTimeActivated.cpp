//
//  ItemTimeActivated.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemTimeActivated.h"
#include "Utility.h"

ItemTimeActivated::ItemTimeActivated(TimeActivatedType timeActivatedType, double time, double power, double radius, double weight) : Item("", weight){
    this->timeActivatedType = timeActivatedType;
    this->time = time;
    this->power = power;
    this->radius = radius;

    switch (timeActivatedType) {
        case timeActivatedBomb:
            name = "Harmful Bomb";
            artIndex = Arts::artBomb;
            break;

        case timeActivatedWallBomb:
            name = "Destructive Bomb";
            artIndex = Arts::artBomb;
            break;

        default:
            name = "UNDEFINED TIME ACTIVATED";
            break;
    }
}

void ItemTimeActivated::save(vector<unsigned char>* data){
    Item::save(data);
    Utility::saveInt(data, timeActivatedType);
    Utility::saveDouble(data, time);
    Utility::saveDouble(data, power);
    Utility::saveDouble(data, radius);
}

void ItemTimeActivated::load(unsigned char* data, int* position){
    Item::load(data, position);
    timeActivatedType = Utility::loadInt(data, position);
    time = Utility::loadDouble(data, position);
    power = Utility::loadDouble(data, position);
    radius = Utility::loadDouble(data, position);
}

ItemTimeActivated* ItemTimeActivated::cloneUnsafe(ItemTimeActivated* oldE, ItemTimeActivated* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->timeActivatedType = oldE->timeActivatedType;
    newE->time = oldE->time;
    newE->power = oldE->power;
    newE->radius = oldE->radius;

    return newE;

}

bool ItemTimeActivated::equalsExceptQty(Item* other){
    ItemTimeActivated* otherW = dynamic_cast<ItemTimeActivated*>(other);
    return Item::equalsExceptQty(other)
    &&(otherW)
    &&(otherW->timeActivatedType == timeActivatedType)
    &&(otherW->time == time)
    &&(otherW->power == power)
    &&(otherW->radius == radius);
}