//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.hpp"
#include "MenuMain.hpp"
#include "MenuInv.hpp"
#include "MenuStats.hpp"
#include "MenuYesNo.hpp"
#include "MenuDebug.hpp"
#include "MenuMessage.hpp"
#include "EntityAi.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Controls.hpp"
#include "ItemRanged.hpp"
#include "ItemUtilitySpell.hpp"

#define currentPlayer currentWorld->currentPlayer
#define currentLevel currentWorld->currentLevel


namespace Ui {

    MenuGame::MenuGame(string worldName, Abilities<int> playerAbilities) : Menu() {

        printConsoleByDefault = false;

        move(0, 0);
        clrtobot();
        setColor(C_WHITE, C_BLACK);
        mvaddstr(0, 0, "Building Level...");
        refresh();

        initSuccess = init(worldName, playerAbilities);

        viewPos = Point2Zero;
        viewMoveSpeed = Point2(2, 1);

        /*if(currentLevel != nullptr){
                currentLevel->setAndUnsetDisplayEntities();
        }*/
        if (initSuccess) {
            currentLevel->menuGame = this;
        }

        viewUpdate();

        spinyIcon = new AnimatedIcon({'-', '\\', '|', '/'}, 1, C_WHITE, C_BLACK);

    }

    MenuGame::~MenuGame() {
        delete currentWorld;
        delete saveAnswer;
        delete useItem;
        delete spinyIcon;
    }

    bool MenuGame::init(string worldName, Abilities<int> playerAbilities) {

        if (worldName.size() == 0) {
            return false;
        }

        if (WorldLoader::exists(worldName)) {
            currentWorld = WorldLoader::load(nullptr, worldName);
            if (currentWorld == nullptr) {
                currentWorld = WorldLoader::create(worldName, playerAbilities);
            }
        } else {
            currentWorld = WorldLoader::create(worldName, playerAbilities);
        }
        if (currentWorld == nullptr) {
            return false;
        }

        return true;
    }

    bool MenuGame::openUi() {
        debug("Open MenuGame Ui");

        if (*saveAnswer == aYes) {

            WorldLoader::save(currentWorld);
            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        } else if (*saveAnswer == aNo) {

            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        }

        if (*useItem != -1) {
            if (currentPlayer->inventory[*useItem]->instantUse()) {
                timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, currentPlayer->inventory[*useItem]);
                int selected = *useItem;
                *useItem = -1;
                MenuInv* m = new MenuInv(currentPlayer, currentWorld, useItem);
                m->selected = selected;
                openMenu(m);
            } else{
                itemToBeUsedRange = 1000;
                itemToBeUsed = currentPlayer->inventory[*useItem];
                targetPosition = currentPlayer->pos;
                changeMode(modeSelectPosition);
                *useItem = -1;
            }
        }

        if (!initSuccess) {
            closeThisMenu();
            return false;
        }

        setGameAreaSize();

        move(0, 0);
        clrtobot();
        refresh();
        return true;
    }

    void MenuGame::closeUi() {

    }

    void MenuGame::setGameAreaSize() {
        gameArea.x = terminalSize.x - borderSize.x;
        gameArea.y = terminalSize.y - borderSize.y;
    }

    void MenuGame::viewUpdate() {
        if(currentWorld != nullptr){
            if (currentPlayer != nullptr) {
                viewPos = currentPlayer->pos - (gameArea / 2);
            }
        }
    }

    void MenuGame::drawTileAt(Point2 p) {
        Ui::Color fg = C_LIGHT_RED;
        Ui::Color bg = C_BLACK;
        char symbol = ' ';
        int attr = 0;


        bool inView = false;
        if (currentLevel != nullptr) {
            if (currentPlayer != nullptr) {
                if (currentLevel->canSee(currentPlayer->pos, p, currentPlayer->viewDistance, true)) {
                    currentLevel->setExplored(p, true);
                    inView = true;
                }
                if (!currentLevel->getExplored(p)) {
                    symbol = ' ';
                } else {
                    Tile* tempTile = currentLevel->tileAt(p);

                    Icon* tempIcon = tempTile->getIcon(inView);

                    fg = tempIcon->getFgColor(tick, p, currentLevel);
                    bg = tempIcon->getBgColor(tick, p, currentLevel);
                    symbol = tempIcon->getChar(tick, p, currentLevel);

                    if (currentLevel->inRange(p)) {
                        Entity* e = nullptr;
                        int d = INT16_MIN;
                        for (Entity* ei : currentLevel->entityList) {
                            if (ei) {
                                if (!ei->removed) {
                                    if (ei->pos == p) {
                                        int dd = ei->getRenderDepth();
                                        if (d < dd) {
                                            e = ei;
                                            d = dd;
                                        }
                                    }
                                }
                            }
                        }
                        if (e) {

                            if (inView) {
                                if (currentPlayer == e && controlMode == modeSelectDirection) {
                                    fg = e->getBgColor(tick, p, currentLevel);
                                    bg = e->getFgColor(tick, p, currentLevel);
                                } else {
                                    fg = e->getFgColor(tick, p, currentLevel);
                                    bg = e->getBgColor(tick, p, currentLevel);
                                }

                                symbol = e->getChar(tick, p, currentLevel);

                            }

                        }
                    }
                }
                if (controlMode == modeSelectEntity) {
                    if (p == targetPosition) {
                        bg = C_LIGHT_BLUE;
                        attr = A_BLINK;
                    }
                    if (!currentLevel->canSee(currentPlayer->pos, p, itemToBeUsedRange, false) && inView) {
                        if (p == targetPosition) {
                            bg = C_LIGHT_RED;
                        }
                        if (bg == C_BLACK) {
                            bg = C_DARK_RED;
                        }
                    }
                } else if (controlMode == modeSelectPosition) {
                    if (p == targetPosition) {
                        bg = C_LIGHT_GREEN;
                        attr = A_BLINK;
                    }
                    if (!currentLevel->canSee(currentPlayer->pos, p, itemToBeUsedRange, false) && inView) {
                        if (p == targetPosition) {
                            bg = C_LIGHT_RED;
                        }
                        if (bg == C_BLACK) {
                            bg = C_DARK_RED;
                        }
                    }

                }

                if(currentPlayer->hasEffect(effMemory)){
                    if(!inView){
                        if(rand() % 1000 != 0){
                            bg = C_BLACK;
                            fg = C_BLACK;
                            symbol = ' ';
                        }
                    }
                }

                if(currentPlayer->hasEffect(effLSD)){
                    fg = rand() % 16;
                    if(bg != C_BLACK){
                        bg = rand() % 16;
                    }
                }
            }
        } else {
            fg = C_LIGHT_MAGENTA;
            symbol = '?';
        }

        setColor(fg, bg, attr);
        addch(symbol);
    }

    void MenuGame::arrowMove(Point2 p) {
        if (controlMode == modeSelectPosition) {
            targetPosition += p;
        } else if (controlMode == modeSelectDirection) {

            Item* i = itemToBeUsed!=nullptr? itemToBeUsed : currentPlayer->getActiveItemWeapon();

            timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos + p, false, i);

            changeMode(modeEntityPlayerControl);

        } else if (controlMode == modeAdjustBorder) {
            borderSize -= p;
            setGameAreaSize();
        } else if (controlMode == modeEntityPlayerControl && currentPlayer != nullptr && currentLevel != nullptr) {

            timePassed += currentPlayer->moveRelative(p, currentLevel);

        } else if (controlMode == modeSelectEntity) {
            Point2 temp = Point2Zero;
            if (targetEntity) {
                temp = targetEntity->pos;
            } else {
                temp = currentPlayer->pos;
            }
            temp += p;
            vector<Entity*> list = currentLevel->getAllVisableEntitiesSortedByNearest(currentPlayer->pos, currentPlayer->viewDistance, {
                currentPlayer, targetEntity
            }, temp, p);
            if (list.size() > 0) {
                targetEntity = list[0];
                targetPosition = targetEntity->pos;
            } else if (!targetEntity) {
                changeMode(modeEntityPlayerControl);
            } else {
                targetPosition = targetEntity->pos;
            }
        }
    }

    void MenuGame::handleInput(int in) {
        if (in == KEY_RESIZE) {
            setGameAreaSize();

        } else if (in == ERR) {

        } else if (in == KEY_ESCAPE) {
            if (controlMode != modeEntityPlayerControl) {
                changeMode(modeEntityPlayerControl);
            } else {
                if (currentPlayer == nullptr) {
                    WorldLoader::deleteWorld(currentWorld->name);

                    delete currentWorld;
                    currentWorld = nullptr;

                    closeAllMenus();
                    return;
                } else {
                    openMenu(new MenuYesNo("Do you want to save '" + currentWorld->name + "' ?", saveAnswer, true));
                }
            }

        } else if (in == Key::moveUp) {
            arrowMove(Point2Up);

        } else if (in == Key::moveDown) {
            arrowMove(Point2Down);

        } else if (in == Key::moveLeft) {
            arrowMove(Point2Left);

        } else if (in == Key::moveRight) {
            arrowMove(Point2Right);

        } else if (in == Key::inventory) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuInv(currentPlayer, currentWorld, useItem));
            }

        } else if (in == Key::statsMenu) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuStats(currentPlayer, currentWorld));
            }

        } else if (in == Key::debugMenu) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuDebug(currentWorld));
            }

        } else if (in == Key::adjustConsole) {
            if (controlMode == modeAdjustBorder) {
                changeMode(modeEntityPlayerControl);
            } else {
                changeMode(modeAdjustBorder);
            }

        }
        
        if (currentWorld != nullptr && currentPlayer != nullptr && currentLevel != nullptr) {
            if (controlMode == modeSelectEntity && ((/* DISABLED, deemed overpowered (part below too) */ (false) && in == 'f') || in == Key::interact)) {

                if (targetEntity) {
                    if (currentLevel->canSee(currentPlayer->pos, targetEntity->pos, currentPlayer->viewDistance, false)) {
                        timePassed += currentPlayer->interactWithEntity(currentLevel, targetEntity, targetEntity->pos, itemToBeUsed);
                    }
                }

                changeMode(modeEntityPlayerControl);

            } else if (/* DISABLED, deemed overpowered (part above too)*/ (false) && in == 'f') {
                ItemRanged* ranged = dynamic_cast<ItemRanged*> (currentPlayer->getActiveItemWeapon());
                if (ranged && controlMode != modeSelectEntity) {
                    itemToBeUsedRange = ranged->range;
                    itemToBeUsed = ranged;
                    changeMode(modeSelectEntity);
                    if (!targetEntity) {
                        arrowMove(Point2Zero);
                    } else {
                        if (currentLevel->canSee(currentPlayer->pos, targetEntity->pos, currentPlayer->viewDistance, false)) {
                            targetPosition = targetEntity->pos;
                        } else {
                            targetPosition = Point2Zero;
                        }
                    }
                }

            } else if (in == Key::interact || in == Key::secondaryAttack) {
                ItemWeapon* wep = (in == Key::interact)?currentPlayer->getActiveItemWeapon():currentPlayer->getSecondaryItemWeapon();
                if (controlMode == modeSelectDirection) {

                    timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, itemToBeUsed);

                    itemToBeUsed = nullptr;
                    changeMode(modeEntityPlayerControl);
                } else if (controlMode == modeSelectPosition) {

                    ItemUtilitySpell* us = dynamic_cast<ItemUtilitySpell*>(itemToBeUsed);

                    timePassed += currentPlayer->interact(currentLevel, targetPosition, false, itemToBeUsed);

                    if(!(us && us->continuousUse)){
                        changeMode(modeEntityPlayerControl);
                    }

                } else {
                    ItemRanged* ranged = dynamic_cast<ItemRanged*> (wep);
                    if (ranged) {
                        changeMode(modeSelectPosition);
                        if (!currentLevel->canSee(currentPlayer->pos, targetPosition, ranged->range, false)) {
                            targetPosition = currentPlayer->pos;
                        }
                        itemToBeUsedRange = ranged->range;
                        itemToBeUsed = ranged;
                    } else {
                        changeMode(modeSelectDirection);
                        itemToBeUsed = wep;
                    }
                }
            } else if(in == Key::waitUntilHealed) {
                if(currentPlayer != nullptr){
                    if(currentPlayer->getHp() < currentPlayer->getMaxHp() || currentPlayer->getMp() < currentPlayer->getMaxMp()){
                        unsigned char b = 1;
                        timeout(20);
                        while ((currentPlayer->getHp() < currentPlayer->getMaxHp() || currentPlayer->getMp() < currentPlayer->getMaxMp()) && b) {
                            vector<Entity*> nearest = currentLevel->getAllVisableEntitiesSortedByNearest(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer);
                            for (Entity* e : nearest) {
                                if (e->isHostile()) {
                                    b = 0;
                                    console("A hostile is near!");
                                    break;
                                }
                            }
                            if (b == 1) {
                                b = 2;
                                console("Waiting until fully healed...");
                            }
                            timePassed = 1;
                            MenuGame::update();
                            if (getch() != ERR) {
                                console("Wait canceled.");
                                break;
                            }
                        }
                        timeout(defaultTimeout);
                        if(currentPlayer->getHp() == currentPlayer->getMaxHp() && currentPlayer->getMp() == currentPlayer->getMaxMp()){
                            console("Fully healed.");
                        }
                    }else{
                        console("Already at full health and mana.");
                    }
                }

            } else if (in == Key::inspect) {
                if(controlMode == modeEntityPlayerControl){
                    changeMode(modeSelectPosition);
                    itemToBeUsedRange = 1000;
                    targetPosition = currentPlayer->pos;
                } else if(controlMode == modeSelectPosition){
                    changeMode(modeEntityPlayerControl);
                    if(currentLevel->getExplored(targetPosition)){
                        console("Tile: "+currentLevel->tileAt(targetPosition)->getName());
                        if(currentLevel->canSee(currentPlayer->pos, targetPosition, currentPlayer->viewDistance, true)){
                            for(Entity* e : currentLevel->entityList){
                                if(!e->removed && e->pos == targetPosition){
                                    console("Entity("+to_string(e->getEntityTypeId())+"): "+e->getName());
                                }
                            }
                            for(TileEntity* t : currentLevel->tileEntityList){
                                if(t->pos == targetPosition){
                                    console("Tile Entity("+to_string(t->getTileEntityTypeId())+"): "+t->debugString());
                                }
                            }
                        }else{
                            console("Tile not in view.");
                        }
                    }else{
                        console("Tile unexplored.");
                    }
                }
            } else if (in == 'r') {
                Point2 p = currentLevel->findRandomWithoutFlag(tileFlagSolid);
                timePassed += currentPlayer->moveAbsalute(p, currentLevel);

            } else if (in == 'R') {
                Point2 p = currentLevel->stairDownPos;
                consolef("Stair Down: %s", p.toString().c_str());
                //timePassed += currentPlayer->moveAbsalute(p, currentLevel);
                currentPlayer->pos = p;

            } else if (in == '[') {
                currentPlayer->hurt(damDebug, 1);

            } else if (in == ']') {
                currentPlayer->heal(1);

            } else if (in == '{') {
                currentPlayer->hurt(damDebug, 10);

            } else if (in == '}') {
                currentPlayer->heal(10);

            }
        }
    }

    void MenuGame::update() {

        if (currentWorld == nullptr) {
            return;
        }

        render(currentWorld->worldTime);

        if (timePassed > 0) {
            currentWorld->worldTime += timePassed;
            timePassed = 0;
            currentLevel->update(currentWorld->worldTime - currentWorld->worldLastTime, currentWorld->worldTime, viewPos);
            render(currentWorld->worldTime);

            currentWorld->worldLastTime = currentWorld->worldTime;
        }

        if(currentPlayer){
            if(currentPlayer->leveledUp){
                openMenu(new MenuMessage({"Level up!", "", "", "", string("Press [")+(char)(Key::statsMenu)+string("] to spend skill points.")}));
                currentPlayer->leveledUp = false;
            }
        }

    }

    void MenuGame::render(double displayTime) {
        viewUpdate();
        move(0, 0);
        clrtobot();
        for (int j = 0; j < gameArea.y; j++) {
            move(j, 0);
            for (int i = 0; i < gameArea.x; i++) {
                drawTileAt(Point2(viewPos.x + i, viewPos.y + j));
            }
        }
        setColor((controlMode == modeAdjustBorder) ? C_BLACK : C_WHITE,
                (controlMode == modeAdjustBorder) ? C_LIGHT_YELLOW : C_BLACK,
                (controlMode == modeAdjustBorder) ? A_BLINK : 0);

        for (int j = 0; j < gameArea.y; j++) {
            mvaddch(j, gameArea.x, '|');
        }
        for (int i = 0; i < terminalSize.x; i++) {
            mvaddch(gameArea.y, i, '-');
        }
        mvaddch(gameArea.y, gameArea.x, '+');

        Ui::setColor(C_WHITE);
        Point2 p = viewPos;

        int a = 0;

        if (currentPlayer != nullptr) {

            p = currentPlayer->pos;
            mvprintw(a, gameArea.x + 1, "%s", currentPlayer->getName().c_str());

            const int hp = Math::roundToInt(currentPlayer->getHp());
            const int maxHp = Math::roundToInt(currentPlayer->getMaxHp());
            const int mp = Math::roundToInt(currentPlayer->getMp());
            const int maxMp = Math::roundToInt(currentPlayer->getMaxMp());

            a++;

            move(a, gameArea.x + 1);
            clrtoeol();

            a += printMultiLineString(a, gameArea.x + 1, formatString("HP: %3d/%3d", hp, maxHp));
            Ui::setColor((hp < (maxHp / 3 * 2)) ? ((hp < (maxHp / 3)) ? C_LIGHT_RED : C_LIGHT_YELLOW) : C_LIGHT_GREEN);
            printw(" %s", Utility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
            Ui::setColor(C_WHITE);

            move(a, gameArea.x + 1);
            clrtoeol();

            a += printMultiLineString(a, gameArea.x + 1, formatString("MP: %3d/%3d", mp, maxMp));
            Ui::setColor(C_LIGHT_BLUE);
            printw(" %s", Utility::makeBar(mp, maxMp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
            Ui::setColor(C_WHITE);

            for (size_t i = 0; i < currentPlayer->effects.size(); i++) {
                Effect eff = currentPlayer->effects[i];
                string name = effectName(eff.eId, eff.meta);
                Ui::Color color = effectColor(eff.eId, eff.meta);
                setColor(color);
                if(eff.power > 0){
                	a += printMultiLineString(a, gameArea.x + 1, formatString("%s %s: %.2f", name.c_str(), Utility::intToRomanNumerals((int) eff.power).c_str(), eff.timeLeft));
                }else{
					a += printMultiLineString(a, gameArea.x + 1, formatString("%s: %.2f", name.c_str(), eff.timeLeft));
                }
            }

            a++;

            Ui::setColor(C_WHITE);

            mvprintw(a, gameArea.x + 1, "Tick [%c]: %s", spinyIcon->getChar(tick, Point2Neg1, currentLevel), Utility::toString((int)tick, 62).c_str());
            a++;

            mvprintw(a, gameArea.x + 1, "Time: %.2f", displayTime);
            a++;
            mvprintw(a, gameArea.x + 1, "Time: %s", Utility::intToRomanNumerals((int) displayTime).c_str());
            //mvprintw(11, gameArea.x+1, "Tick: %d", tick);

            a++;
            a++;

            move(a, gameArea.x + 1);
            clrtoeol();

            nearestEntities = currentLevel->getAllVisableEntitiesSortedByNearest(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer);

            //consolef("Nearest Entity Count: %d", nearestEntities.size());

            for (size_t i = 0; i < nearestEntities.size() && a < terminalSize.y; i++) {
                Entity* nearestEntity = nearestEntities[i];
                if (nearestEntity != nullptr) {
                    Ui::setColor(C_WHITE);


                    a += printMultiLineColoredString(a, gameArea.x + 1, formatString("%s [%s%c%s]", nearestEntity->getName().c_str(), colorCode(nearestEntity->getFgColor(tick, p, currentLevel), nearestEntity->getBgColor(tick, p, currentLevel)).c_str(), nearestEntity->getChar(tick, nearestEntity->pos, currentLevel), colorCode(C_WHITE).c_str()));
                    //printw("%c", );
                    //Ui::setColor(C_WHITE);
                    //printw("]");



                    EntityAi* aiEntity = dynamic_cast<EntityAi*> (nearestEntity);
                    if (aiEntity) {
                        const int hp = Math::roundToInt(aiEntity->getHp());
                        const int maxHp = Math::roundToInt(aiEntity->getMaxHp());
                        a += printMultiLineString(a, gameArea.x + 1, formatString(" HP: %d/%d", hp, maxHp));
                        Ui::setColor((hp < (maxHp / 3 * 2)) ? ((hp < (maxHp / 3)) ? C_LIGHT_RED : C_LIGHT_YELLOW) : C_LIGHT_GREEN);
                        printw(" %s", Utility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
                        Ui::setColor(C_WHITE);
                    }
                    a++;
                }

            }


        } else {
            //printCenterOffset(gameArea.y / 2, -(borderSize.x / 2), "G a m e   O v e r");
            //printCenterOffset(gameArea.y / 2 + 3, -(borderSize.x / 2), "Press Escape to return to the Main Menu.");

            Art* a = Arts::getArt(Arts::artGameOver);
            a->printCenter((gameArea.y / 2) - ((int)a->lines.size()/2), -(borderSize.x / 2));
            printCenterOffset(gameArea.y / 2 + 3 + ((int)a->lines.size()/2), -(borderSize.x / 2), "Press Escape to return to the Main Menu.");
        }


        if (currentLevel != nullptr) {
            Ui::setColor(C_LIGHT_GRAY);
            mvprintw(gameArea.y, 0, "%d, %d e:%d te:%d p:%s %s(%d)", p.x, p.y, currentLevel->entityCount(), currentLevel->tileEntityList.size(), (currentPlayer == nullptr) ? "null" : "not null", currentLevel->getName().c_str(), currentLevel->getDifficulty());
        }



        printConsole(gameArea.y + 1);

        refresh();
    }

    void MenuGame::changeMode(int newMode) {
        if (newMode == controlMode) {
            return;
        }
        switch (controlMode) {
            case modeSelectDirection:
            case modeSelectPosition:
            case modeSelectEntity:
                itemToBeUsedRange = -1;
                itemToBeUsed = nullptr;
                targetEntity = nullptr;
                break;

            default:
                break;
        }
        controlMode = newMode;
    }

}
