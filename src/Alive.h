//
//  Alive.h
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Alive__
#define __Underneath__Alive__

#include "Entity.h"
#include "Weapon.h"
#include "Spell.h"
#include "Math.h"
#include "Inventory.h"
#include "Effect.h"

class Alive : public Entity, public Inventory{

public:

    static Alive* cloneUnsafe(Alive* oldE, Alive* newE);

    Alive();

    Alive(string name, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    ~Alive();

    virtual bool update(double time, Level* level);

    virtual string getName(){
        return name;
    }

    double getHp(){
        return hp;
    }

    double getMaxHp(){
        return maxHp;
    }

    double getMp(){
        return mp;
    }

    double getMaxMp(){
        return maxMp;
    }

    virtual void die(){
        hp = 0;
        dead = true;
        //pos.set(-1);
    }

    virtual double hurt(double amount, double damageMultiplier = 1);

    virtual double hurt(Weapon* w, double time, double damageMultiplier = 1);

    void addEffect(Effect e);

    virtual double heal(double amount);

    virtual double healMana(double amount);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);

    void setActiveWeapon(Weapon* newWeapon);

    Weapon* getActiveWeapon(){
        return activeWeapon;
    }

    bool removeItem(Item* item, bool deleteItem);

    //vector<Item*> inventory;

    vector<Effect> effects;

    int viewDistance = 10;

protected:
    string name;
    double maxHp = 30;
    double hp = maxHp;
    double maxMp = 30;
    double mp = maxMp;
    bool dead = false;

    double lastHealTime = 0;
    double healDelay = 10;

    double lastManaTime = 0;
    double manaDelay = 10;

    Weapon* activeWeapon = nullptr;

};

#endif /* defined(__Underneath__Alive__) */
