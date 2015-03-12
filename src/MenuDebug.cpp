//
//  MenuDebug.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuDebug.h"
#include "ItemGenerator.h"

namespace Ui {

    MenuDebug::MenuDebug(World* w) : Menu(){
        this->currentWorld = w;

    }

    void MenuDebug::handleInput(int in){
        switch (in) {
            case KEY_ESCAPE:
                closeThisMenu();
                return;
        }
    }


    void MenuDebug::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);

        int a = 0;

        mvvline(a, column1-1, '|', terminalSize.y);
        mvvline(a, column2-1, '|', terminalSize.y);
        mvvline(a, column3-1, '|', terminalSize.y);

        mvprintw(a, column0, "Weapon");
        mvprintw(a, column1, "Damage");
        mvprintw(a, column2, "Delay");
        mvprintw(a, column3, "Dam/Time");

        a++;

        mvhline(a, 0, '-', terminalSize.x);

        a++;

        for(ItemGenerator::WeaponBase w : ItemGenerator::weaponList){
            mvprintw(a, column0, "%s", w.names[0].c_str());
            mvprintw(a, column1, "%.2f", w.damage);
            mvprintw(a, column2, "%.2f", w.useDelay);
            mvprintw(a, column3, "%.2f", w.damage / w.useDelay);
            a++;
        }

    }

}