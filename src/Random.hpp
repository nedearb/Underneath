//
//  Random.h
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Random__
#define __Underneath__Random__

struct Point2;

namespace Random {

    double randDouble(double min, double max);

    int randomFromPoint(Point2 seed);

    template <typename T> const T choose(int count, ...) {
        vector<T> options;
        va_list args;
        va_start(args, count);
        for(int i=0;i<count;i++){
            T a = va_arg(args, T);
            options.push_back(a);
        }
        va_end(args);
        int index = rand() % count;
        return options[index];
    }


}

#endif /* defined(__Underneath__Random__) */
