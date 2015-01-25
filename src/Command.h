//
//  Command.h
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Command__
#define __Underneath__Command__

#include "Menu.h"

namespace Ui{

    bool execute(string commandRaw, Menu* currentMenu);

    struct Command {
        Command(){}

        virtual bool acceptableName(string name){
            return name.compare(defaultName()) == 0;
        }
        virtual bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            return false;
        }

        virtual string help(){
            return "Undefined Command";
        }

        virtual string usage(){
            return "Undefined Command";
        }
        virtual string defaultName(){
            return "Undefined";
        }
    };

    extern vector<Command*> commandList;
    
    void initCommandList();
    
}

#endif /* defined(__Underneath__Command__) */