//
//  Tile.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.h"
#include "Tile.h"

Tile::Tile(int index, char icon, Ui::Color fgColor, Ui::Color bgColor, TileFlag flags, Ui::Color fgColorUnseen, Ui::Color bgColorUnseen) {
    this->index = index;
    this->icon = icon;
    tileList[index] = this;

    this->fgColor = fgColor;
    this->bgColor = bgColor;
    this->fgColorUnseen = fgColorUnseen;
    this->bgColorUnseen = bgColorUnseen;

    this->flags = flags;
}

char Tile::getIcon() {
    return icon;
}

int Tile::getIndex() {
    return index;
}

Ui::Color Tile::getFgColor(bool inView) {
    return inView?fgColor:fgColorUnseen;
}

Ui::Color Tile::getBgColor(bool inView) {
    return inView?bgColor:bgColorUnseen;
}



Tile* tileList[20];


Tile* tileFloor;
Tile* tilePath;
Tile* tileWall;
Tile* tileDoor;
Tile* tileSecretDoor;
Tile* tileOpenDoor;

Tile* tileStairUp;
Tile* tileStairDown;

Tile* tileEdge;
Tile* tileUnset;

Tile* tileDebug1;
Tile* tileDebug2;
Tile* tileDebug3;
Tile* tileDebug4;
Tile* tileDebug5;
Tile* tileDebug6;

void initTiles() {

    int a = 0;

    using namespace Ui;

    Color bg = C_BLACK;

    tileFloor = new Tile(a++, '.', C_WHITE, bg, tileFlagPathable);
    tilePath = new Tile(a++, '.', C_LIGHT_GRAY, bg, tileFlagPathable);

    tileWall = new Tile(a++, ' ', C_BLACK, C_LIGHT_GRAY, tileFlagSolid | tileFlagTall,
                        C_BLACK, C_DARK_GRAY);
    tileDoor = new Tile(a++, '%', C_BLACK, C_LIGHT_GRAY, tileFlagSolid | tileFlagTall | tileFlagDoor | tileFlagPathable,
                        C_BLACK, C_DARK_GRAY);
    tileSecretDoor = new Tile(a++, ' ', C_BLACK, C_LIGHT_GRAY, tileFlagSolid | tileFlagTall | tileFlagDoor | tileFlagSecretPathable,
                              C_BLACK, C_DARK_GRAY);
    tileOpenDoor = new Tile(a++, '/', C_WHITE, bg, tileFlagPathable);


    tileStairUp = new Tile(a++, '^',  C_LIGHT_GREEN, bg, tileFlagPathable);
    tileStairDown = new Tile(a++, 'v',  C_LIGHT_GREEN, bg, tileFlagPathable);


    tileEdge = new Tile(a++, 'X',  C_LIGHT_RED, bg, tileFlagSolid | tileFlagTall);
    tileUnset = new Tile(a++, 'X',  C_LIGHT_MAGENTA, bg, tileFlagSolid | tileFlagTall);

    tileDebug1 = new Tile(a++, '1',  C_LIGHT_GREEN, bg, tileFlagPathable);
    tileDebug2 = new Tile(a++, '2',  C_LIGHT_GREEN, bg, tileFlagPathable);
    tileDebug3 = new Tile(a++, '3',  C_LIGHT_GREEN, bg, tileFlagPathable);
    tileDebug4 = new Tile(a++, '4',  C_LIGHT_GREEN, bg, tileFlagPathable);
    tileDebug5 = new Tile(a++, '5',  C_LIGHT_GREEN, bg, tileFlagPathable);
    tileDebug6 = new Tile(a++, '6',  C_LIGHT_GREEN, bg, tileFlagPathable);
}

void deleteTiles(){
    for(Tile* tile : tileList){
        delete tile;
    }
}

Tile* getTile(int index) {
    return tileList[index];
}
