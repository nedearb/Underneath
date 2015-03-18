//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.h"
#include "MenuMain.h"
#include "MenuInv.h"
#include "MenuStats.h"
#include "MenuYesNo.h"
#include "MenuDebug.h"
#include "AiEntity.h"
#include "Math.h"
#include "Utility.h"
#include "Controls.h"
#include "Ranged.h"

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
        if(initSuccess){
            currentLevel->menuGame = this;
        }

        viewUpdate();


    }

    MenuGame::~MenuGame() {
        delete currentWorld;
        delete saveAnswer;
        delete useItem;
    }

    bool MenuGame::init(string worldName, Abilities<int> playerAbilities){

        if(worldName.size() == 0){
            return false;
        }

        if(WorldLoader::exists(worldName)){
            currentWorld = WorldLoader::load(worldName);
            if(currentWorld == nullptr){
                currentWorld = WorldLoader::create(worldName, playerAbilities);
            }
        }else{
            currentWorld = WorldLoader::create(worldName, playerAbilities);
        }
        if(currentWorld == nullptr){
            return false;
        }

        return true;
    }

    bool MenuGame::openUi() {
        debug("Open MenuGame Ui");

        if(*saveAnswer == aYes){

            WorldLoader::save(currentWorld);
            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        }else if(*saveAnswer == aNo){

            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        }

        if(*useItem != -1){
            if(currentPlayer->inventory[*useItem]->instantUse()){
                timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, currentPlayer->inventory[*useItem]);
            }else{
            	mode = modeInterectChoose;
            }
        }

        if(!initSuccess){
            closeThisMenu();
			return false;
        }

        setGameAreaSize();

        move(0, 0);
        clrtobot();
        refresh();
        return true;
    }

    void MenuGame::closeUi(){

    }

    void MenuGame::setGameAreaSize() {
        gameArea.x = terminalSize.x - borderSize.x;
        gameArea.y = terminalSize.y - borderSize.y;
    }

    void MenuGame::viewUpdate() {
        if(mode == modePlayerControl) {
            
            if(currentPlayer != nullptr){
                viewPos = currentPlayer->pos - (gameArea/2);
            }
        }
    }

    void MenuGame::drawTileAt(Point2 p) {
        Ui::Color fg = C_LIGHT_RED;
        Ui::Color bg = C_BLACK;
        char symbol = ' ';
        int attr = 0;


        bool inView = false;
        if(currentLevel != nullptr){
            if(currentPlayer != nullptr) {
                if(currentLevel->canSee(currentPlayer->pos, p, currentPlayer->viewDistance, true)) {
                    currentLevel->setExplored(p, true);
                    inView = true;
                }if(!currentLevel->getExplored(p)) {
                    symbol = ' ';
                }else{
                    Tile* tempTile = currentLevel->tileAt(p);

                    fg = tempTile->getFgColor(inView);
                    bg = tempTile->getBgColor(inView);
                    symbol = tempTile->getIcon();

                    if(currentLevel->inRange(p)) {
                        Entity* e = nullptr;
                        int d = INT16_MIN;
                        for(Entity* ei : currentLevel->entityList){
                            if(ei->pos == p){
                                int dd = ei->getRenderDepth();
                                if(d < dd){
                                    e = ei;
                                    d = dd;
                                }
                            }
                        }
                        if(e != nullptr) {

                            if(inView) {
                                if(currentPlayer == e && mode == modeInterectChoose){
                                    fg = e->getBgColorCode();
                                    bg = e->getFgColorCode();
                                }else{
                                    fg = e->getFgColorCode();
                                    bg = e->getBgColorCode();
                                }
                                symbol = e->getIcon(p, tick, currentLevel);
                            }

                        }
                    }
                }
                Ranged* ranged = dynamic_cast<Ranged*>(currentPlayer->getActiveWeapon());
                if(ranged){
                    if(mode == modeSelectTarget){
                        if(p == targetPosition){
                            bg = C_LIGHT_GREEN;
                            attr = A_BLINK;
                        }
                        if(!currentLevel->canSee(currentPlayer->pos, p, ranged->range, false) && inView) {
                            if(p == targetPosition){
                                bg = C_LIGHT_RED;
                            }
                            if(bg == C_BLACK){
                                bg = C_DARK_RED;
                            }
                        }

                    }else if(mode == modeJumpSelectTarget){
                        if(p == targetPosition){
                            bg = C_LIGHT_BLUE;
                            attr = A_BLINK;
                        }
                        if(!currentLevel->canSee(currentPlayer->pos, p, ranged->range, false) && inView) {
                            if(p == targetPosition){
                                bg = C_LIGHT_RED;
                            }
                            if(bg == C_BLACK){
                                bg = C_DARK_RED;
                            }
                        }
                    }
                }

            }
        }else{
            fg = C_LIGHT_MAGENTA;
            symbol = '?';
        }

        setColor(fg, bg, attr);
        addch(symbol);
    }

    void MenuGame::arrowMove(Point2 p){
        if(mode == modeSelectTarget){
            targetPosition += p;
        }if(mode == modeInterectChoose){

            Item* i = *useItem!=-1?(currentPlayer->inventory[*useItem]):currentPlayer->getActiveWeapon();
            
            timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos+p, false, i);

            *useItem = -1;

            mode = modePlayerControl;

        }else if(mode == modeAdjustBorder){
            borderSize -= p;
            setGameAreaSize();
        } else if(mode == modePlayerControl && currentPlayer != nullptr && currentLevel != nullptr){

            timePassed += currentPlayer->moveRelative(p, currentLevel);
            
        } else if(mode == modeJumpSelectTarget){
            Point2 temp = Point2Zero;
            if(targetEntity){
            	temp = targetEntity->pos;
            }else{
                temp = currentPlayer->pos;
            }
            temp += p;
            vector<Entity*> list = currentLevel->getAllVisableEntitiesSortedByNearest(currentPlayer->pos, currentPlayer->viewDistance, {currentPlayer, targetEntity}, temp, p);
            if(list.size() > 0){
                targetEntity = list[0];
                targetPosition = targetEntity->pos;
            }else if(!targetEntity){
                mode = modePlayerControl;
                targetEntity = nullptr;
                targetPosition = Point2Zero;
            }else{
                targetPosition = targetEntity->pos;
            }
        }
    }

    void MenuGame::handleInput(int in) {
        if(in == KEY_RESIZE){
            setGameAreaSize();

        }else if(in == ERR){

        }else if(in == KEY_ESCAPE){
            if(currentPlayer == nullptr){
                WorldLoader::deleteWorld(currentWorld->name);
                
                delete currentWorld;
                currentWorld = nullptr;

                closeAllMenus();
                return;
            }else{
                openMenu(new MenuYesNo("Do you want to save '"+currentWorld->name+"' ?", saveAnswer, true));
            }

        }else if(in == Key::moveUp){
            arrowMove(Point2Up);

        }else if(in == Key::moveDown){
            arrowMove(Point2Down);

        }else if(in == Key::moveLeft){
            arrowMove(Point2Left);

        }else if(in == Key::moveRight){
            arrowMove(Point2Right);

        }else if(in == Key::inventory){
            if(currentPlayer != nullptr){
                openMenu(new MenuInv(currentPlayer, currentWorld, useItem));
            }

        }else if(in == Key::statsMenu){
            if(currentPlayer != nullptr){
                openMenu(new MenuStats(currentPlayer, currentWorld));
            }

        }else if(in == Key::debugMenu){
            if(currentPlayer != nullptr){
                openMenu(new MenuDebug(currentWorld));
            }

        }else if(in == Key::adjustConsole){
            if(mode == modeAdjustBorder){
                mode = modePlayerControl;
            }else{
                mode = modeAdjustBorder;
            }

        }else if(in == '\n'){
            if(!consoleInputMode){
                consoleInputMode = true;
            }

        }
        if(currentWorld != nullptr && currentPlayer != nullptr && currentLevel != nullptr){
            if(mode == modeJumpSelectTarget && (in == 'f' || in == Key::interact)){

                if(targetEntity){
                    if(currentLevel->canSee(currentPlayer->pos, targetEntity->pos, currentPlayer->viewDistance, false)){
                        timePassed += currentPlayer->interactWithEntity(currentLevel, targetEntity, targetEntity->pos, currentPlayer->getActiveWeapon());
                    }
                }

                mode = modePlayerControl;
                
            }else if(in == 'f'){
                Ranged* ranged = dynamic_cast<Ranged*>(currentPlayer->getActiveWeapon());
                if(ranged && mode != modeJumpSelectTarget){
                    mode = modeJumpSelectTarget;
                    if(!targetEntity){
                        arrowMove(Point2Zero);
                    }else{
                        if(currentLevel->canSee(currentPlayer->pos, targetEntity->pos, currentPlayer->viewDistance, false)){
                            targetPosition = targetEntity->pos;
                        }else{
                            targetPosition = Point2Zero;
                        }
                    }
                }

            }else if(in == Key::interact){
                    if(mode == modeInterectChoose){

                        timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, currentPlayer->getActiveWeapon());

                        mode = modePlayerControl;
                    }else if(mode == modeSelectTarget){

                        timePassed += currentPlayer->interact(currentLevel, targetPosition, false, currentPlayer->getActiveWeapon());

                        mode = modePlayerControl;
                    }else{
                        Ranged* ranged = dynamic_cast<Ranged*>(currentPlayer->getActiveWeapon());
                        if(ranged){
                            mode = modeSelectTarget;
                            if(!currentLevel->canSee(currentPlayer->pos, targetPosition, ranged->range, false)){
                            	targetPosition = currentPlayer->pos;
                            }
                        }else{
                        	mode = modeInterectChoose;
                        }
                    }
            }else if(in == Key::wait5){
                    timePassed += 5;

            }else if(in == 'r'){
                Point2 p = currentLevel->findRandomWithoutFlag(tileFlagSolid);
                timePassed += currentPlayer->moveAbsalute(p, currentLevel);

            }else if(in == '['){
                currentPlayer->hurt(damDebug, 1);

            }else if(in == ']'){
                currentPlayer->heal(1);

            }
        }
    }

    void MenuGame::update() {

        if(currentWorld==nullptr){
            return;
        }

        render(currentWorld->worldTime);

        if(timePassed > 0) {
            currentWorld->worldTime += timePassed;
            timePassed = 0;
            currentLevel->update(currentWorld->worldTime - currentWorld->worldLastTime, currentWorld->worldTime, viewPos);
            render(currentWorld->worldTime);

            currentWorld->worldLastTime = currentWorld->worldTime;
        }
        
    }

    void MenuGame::render(double displayTime) {
        viewUpdate();
        move(0, 0);
        clrtobot();
        for(int j=0; j<gameArea.y; j++) {
            move(j, 0);
            for(int i=0; i<gameArea.x; i++) {
                drawTileAt(Point2(viewPos.x+i, viewPos.y+j));
            }
        }
        setColor((mode == modeAdjustBorder)?C_BLACK:C_WHITE,
                 (mode == modeAdjustBorder)?C_LIGHT_YELLOW:C_BLACK,
                 (mode == modeAdjustBorder)?A_BLINK:0);

        for(int j=0; j<gameArea.y; j++) {
            mvaddch(j, gameArea.x, '|');
        }
        for(int i=0; i<terminalSize.x; i++) {
            mvaddch(gameArea.y, i, '-');
        }
        mvaddch(gameArea.y, gameArea.x, '+');

        Ui::setColor(C_WHITE);
        Point2 p = viewPos;

        int a = 0;

        if(currentPlayer != nullptr){

            p = currentPlayer->pos;
            mvprintw(a, gameArea.x+1, "%s", currentPlayer->getName().c_str());

            const int hp = Math::roundToInt(currentPlayer->getHp());
            const int maxHp = Math::roundToInt(currentPlayer->getMaxHp());
            const int mp = Math::roundToInt(currentPlayer->getMp());
            const int maxMp = Math::roundToInt(currentPlayer->getMaxMp());

            a++;

            move(a, gameArea.x+1);
            clrtoeol();

            mvprintw(a, gameArea.x+1, "HP: %3d/%3d", hp, maxHp);
            Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
            printw(" %s", Utility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
            Ui::setColor(C_WHITE);

            a++;

            move(a, gameArea.x+1);
            clrtoeol();

            mvprintw(a, gameArea.x+1, "MP: %3d/%3d", mp, maxMp);
            Ui::setColor(C_LIGHT_BLUE);
            printw(" %s", Utility::makeBar(mp, maxMp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
            Ui::setColor(C_WHITE);

            a++;

            for(size_t i=0;i<currentPlayer->effects.size();i++){
                Effect eff = currentPlayer->effects[i];
                string name = "EFFECT";
                Ui::Color color = C_LIGHT_GREEN;
                switch (eff.eId) {
                    case effDamage:{
                        name = Weapon::damageTypeName((int)eff.meta);
                        color = Weapon::damageTypeColor((int)eff.meta);
                        break;
                    }

                    case effHeal:
                        name = "Heal";
                        color = C_LIGHT_GREEN;
                        break;

                    case effBuff:
                        name = "Buff TODO";
                        color = C_LIGHT_BLUE;
                        break;
                }
                setColor(color);
                mvprintw(a, gameArea.x+1, (name+" %s: %.2f").c_str(), Utility::intToRomanNumerals((int)eff.power).c_str(), eff.timeLeft);
                a++;
            }

            a++;

            Ui::setColor(C_WHITE);

            mvprintw(a, gameArea.x+1, "Time: %.2f", displayTime);
            a++;
            mvprintw(a, gameArea.x+1, "Time: %s", Utility::intToRomanNumerals((int)displayTime).c_str());
            //mvprintw(11, gameArea.x+1, "Tick: %d", tick);

            a++;
            a++;

            move(a, gameArea.x+1);
            clrtoeol();

            vector<Entity*> nearestEntities = currentLevel->getAllVisableEntitiesSortedByNearest(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer);

            for(size_t i=0; i<nearestEntities.size() && a<terminalSize.y; i++){
                Entity* nearestEntity = nearestEntities[i];
                if(nearestEntity != nullptr){
                    Ui::setColor(C_WHITE);
                    mvprintw(a, gameArea.x+1, "%s [", nearestEntity->getName().c_str());
                    Ui::setColor(nearestEntity->getFgColorCode(), nearestEntity->getBgColorCode());
                    printw("%c", nearestEntity->getIcon(nearestEntity->pos, currentWorld->worldTime, currentLevel));
                    Ui::setColor(C_WHITE);
                    printw("]");
                    a++;
                    AiEntity* aiEntity = dynamic_cast<AiEntity*>(nearestEntity);
                    if(aiEntity){
                        const int hp = Math::roundToInt(aiEntity->getHp());
                        const int maxHp = Math::roundToInt(aiEntity->getMaxHp());
                        mvprintw(a, gameArea.x+1, " HP: %d/%d", hp, maxHp);
                        Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
                        printw(" %s", Utility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
                        Ui::setColor(C_WHITE);
                        a++;
                    }
                    a++;
                }

            }


        }else{
            printCenterOffset(gameArea.y/2, -(borderSize.x/2), "Game Over");
        }


        if(currentLevel != nullptr){
            Ui::setColor(C_DARK_GRAY);
            mvprintw(gameArea.y, 0, "%d, %d  e: %d  te: %d  p: %s   %s", p.x, p.y, currentLevel->entityCount(), currentLevel->tileEntityList.size(), (currentPlayer == nullptr)?"null":"not null", currentLevel->getName().c_str());
        }


        
        printConsole(gameArea.y+1);
        
        refresh();
    }

}
