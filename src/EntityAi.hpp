//
//  EntityAi.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityAi__
#define __Underneath__EntityAi__

#include "EntityAlive.hpp"

typedef int AiType;

const AiType aiNone = 0;
const AiType aiMoveRandom = 1 << 0;
const AiType aiAttackEntityPlayer = 1 << 1;
const AiType aiFleeFromEntityPlayer = 1 << 2;

const int agroViewDistanceMultiplier = 3;

class EntityAi : public EntityAlive {
public:

    static EntityAi* cloneUnsafe(EntityAi* oldE, EntityAi* newE);

    EntityAi();

    EntityAi(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 1);

    ~EntityAi();

    void runAi(double time, Level* level);
    bool update(double deltaTime, double time, Level* level);

    virtual double hurt(DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(ItemWeapon* w, double damageMultiplier = 1);

    virtual void save(std::vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);

    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon);

    virtual ItemWeapon* getActiveItemWeapon() {
        return activeItemWeapon;
    }

    void setMoveDelay(double newDelay) {
        moveDelay = newDelay;
    }

    virtual bool removeItem(Item* item, bool deleteItem);

    bool equipItem(Equipable* newItem);

    bool equipItem(Equipable* newItem, EquipSlot slot);

    virtual void setTimes(double time) {
        lastMoveTime = time;
        lastAttackTime = time;
        EntityAlive::setTimes(time);
    }

    virtual bool isHostile() {
        return ai & aiAttackEntityPlayer;
    }
    
    double getAttackMultiplierFromEffects(DamageType damType){
        double d = 1;
        for(Effect eff : effects){
            if(eff.eId == effBuffAttack){
                if((int)eff.meta == damType){
                    d += eff.power;
                }
            }
        }
        return d;
    }


protected:
    int ai = aiNone;

    Point2 lastKnownTargetPos = Point2Neg1;
    EntityAlive* target = nullptr;

    double lastMoveTime = 0;
    double moveDelay = 1.5; //TODO should vary

    double lastAttackTime = 0;

    bool agro = false;

    ItemWeapon* activeItemWeapon = nullptr;
};

#endif /* defined(__Underneath__EntityAi__) */