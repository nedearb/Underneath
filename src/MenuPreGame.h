//
//  MenuPreGame.h
//  Underneath
//
//  Created by Braeden Atlee on 11/3/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuPreGame__
#define __Underneath__MenuPreGame__

#include "Ui.h"

namespace Ui {
    class MenuPreGame : public Ui::Menu {
    public:

        MenuPreGame() : Menu(false) {}

        ~MenuPreGame() {}

        void openUi(Menu* oldMenu);
        void closeUi(Menu* newMenu);
        void handleInput(int in);
        void update();

        int selection = 1;
        std::string name = "";
    };
}

#endif /* defined(__Underneath__MenuPreGame__) */
