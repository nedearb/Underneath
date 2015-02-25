//
//  Ranged.h
//  Underneath
//
//  Created by Braeden Atlee on 1/22/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Ranged__
#define __Underneath__Ranged__

#include "Global.h"
#include "Weapon.h"

class Ranged : public Weapon{
public:

    static Ranged* cloneUnsafe(Ranged* oldE, Ranged* newE);


    Ranged() : Ranged(materialNone, 0, "UNDEFINED", 0, 0, 0){

    }

    Ranged(Material* material, double baseDamage, string name, Weight weight, double useDelay, double range) : Weapon(material, baseDamage, name, weight, useDelay){
        this->baseDamage = baseDamage;
        this->damageType = damRanged;
        this->range = range;
    }

    virtual int getItemTypeId(){
        return ITEM_TYPE_RANGED;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other){
        Ranged* otherR = dynamic_cast<Ranged*>(other);
        return Weapon::equalsExceptQty(other)
        &&(otherR)
        &&(range == otherR->range);
    }

    double range = 4;

protected:
};

#endif /* defined(__Underneath__Ranged__) */
