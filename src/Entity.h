//
//  Entity.h
//  Underneath
//
//  Created by Braeden Atlee on 10/1/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Entity__
#define __Underneath__Entity__


#include "Ui.h"
#include "Geometry.h"

class Level;

const int ENTITY_TYPE_ENTITY = 0;
const int ENTITY_TYPE_ALIVE = 1;
const int ENTITY_TYPE_AIENTITY = 2;
const int ENTITY_TYPE_PLAYER = 3;
const int ENTITY_TYPE_ITEMENTITY = 4;

class Entity{

public:

    static Entity* clone(Entity* oldE, Entity* newE);

    Entity();

    Entity(char icon, Point2 startPos, Ui::color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual ~Entity();

    virtual bool tryToMoveAbsalute(Point2 p, shared_ptr<Level> world);

    virtual bool tryToMoveRelative(Point2 p, shared_ptr<Level> world);
    
    virtual bool update(int tick, shared_ptr<Level> level);

    //void setAndUnsetDisplayEntity(shared_ptr<Level> level);

    virtual char getIcon(Point2 p, int tick, shared_ptr<Level> level);

    bool isSolid(){
        return solid;
    }

    virtual string getName(){
        return "NO_NAME";
    }

    virtual int getFgColorCode();
    virtual int getBgColorCode();

    virtual void save(string* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return 0;
    }

    virtual void load(char* data, int* position);

    static Entity* loadNew(char* data, int* position);

    int uniqueId = 0;

    Point2 pos;

protected:
    char defaultIcon = '?';
    Point2 lastPos;
    Ui::color fgColorCode;
    Ui::color bgColorCode;
    bool updateIcon = true;
    bool solid = false;

};






#endif /* defined(__Underneath__Entity__) */
