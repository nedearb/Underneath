//
//  Verbalizer.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/27/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Verbalizer.h"
#include "Menu.h"

namespace Verbalizer {

    void attack(Alive* attacker, Alive* underAttack, Weapon* weapon, double damage){

        Player* playerAttacker = dynamic_cast<Player*>(attacker);
        Player* playerUnderAttack = dynamic_cast<Player*>(underAttack);

        string attackerString = attacker->getName();
        string underAttackString = underAttack->getName();
        string deals = "deals";
        string their = "their";
        string theSpace = "The ";

        if(playerAttacker){
            attackerString = "You";
            deals = "deal";
            their = "your";
            theSpace = "";
        }

        if(playerUnderAttack){
            underAttackString = "you";
        }
        string verbal = theSpace +
        Ui::colorCode(C_CODE_LIGHT_BLUE) + attackerString +
        Ui::colorCode(C_CODE_WHITE) + " " + deals +
        Ui::colorCode(C_CODE_LIGHT_GREEN) + " %.2f " +
        Ui::colorCode(C_CODE_WHITE) + "damage to " +
        Ui::colorCode(C_CODE_LIGHT_BLUE) + underAttackString +
        Ui::colorCode(C_CODE_WHITE) + " with " + their + " " +
        Ui::colorCode(C_CODE_LIGHT_RED) + weapon->getExtendedName();

        consolef(verbal, damage);


    }

}

