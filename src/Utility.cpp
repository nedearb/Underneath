//
//  Utility.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Utility.h"

namespace Utility{

    void saveInt(std::string* data, int n){
        data->push_back((char)((n >> 24) & 0xFF));
        data->push_back((char)((n >> 16) & 0xFF));
        data->push_back((char)((n >> 8) & 0xFF));
        data->push_back((char)(n & 0xFF));
    }

    void saveChar(std::string* data, char n){
        data->push_back(n);
    }

    void saveInt8Bit(std::string* data, int8_t n){
        data->push_back((char)n);
    }

    void saveBool(std::string* data, bool n){
        data->push_back((char)(n?'T':'F'));
    }

    void saveString(std::string* data, std::string n){
        saveInt(data, (int)n.size());
        for(int i=0;i<n.size();i++){
            saveChar(data, n[i]);
        }
    }



    int loadInt(char* data, int* position){
        return ((loadChar(data, position)<<24) |
                (loadChar(data, position)<<16) |
                (loadChar(data, position)<< 8) |
                (loadChar(data, position)<< 0));
    }

    char loadChar(char* data, int* position){
        char l = data[*position];
        *position+=1;
        return l;
    }

    int8_t loadInt8Bit(char* data, int* position){
        int8_t l = (int8_t)(data[*position]);
        *position+=1;
        return l;
    }

    bool loadBool(char* data, int* position){
        bool l = ((char)(data[*position]))=='T';
        *position+=1;
        return l;
    }

    std::string loadString(char* data, int* position){
        int count = loadInt(data, position);
        std::string l;
        for(int i=0;i<count;i++){
            l+=loadChar(data, position);
        }
        return l;
    }

}

