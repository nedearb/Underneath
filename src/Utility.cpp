//
//  Utility.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Utility.h"

namespace FileUtility {




    void saveDouble(vector<unsigned char>* data, double n){

        union{
            double d;
            unsigned char b[sizeof(double)];
        } uDoubleBytes;

        uDoubleBytes.d = n;
        for(int i=0;i<sizeof(double);i++){
            data->push_back((unsigned char)(((uDoubleBytes.b[i]) >> (i*8)) & 0xFF));
        }
    }

    void saveInt(vector<unsigned char>* data, int n){
        data->push_back((unsigned char)((n >> 24) & 0xFF));
        data->push_back((unsigned char)((n >> 16) & 0xFF));
        data->push_back((unsigned char)((n >> 8) & 0xFF));
        data->push_back((unsigned char)(n & 0xFF));
    }

    void saveUnsignedLong(vector<unsigned char>* data, unsigned long n){
        data->push_back((unsigned char)((n >> 48) & 0xFF));
        data->push_back((unsigned char)((n >> 40) & 0xFF));
        data->push_back((unsigned char)((n >> 32) & 0xFF));
        data->push_back((unsigned char)((n >> 24) & 0xFF));
        data->push_back((unsigned char)((n >> 16) & 0xFF));
        data->push_back((unsigned char)((n >> 8) & 0xFF));
        data->push_back((unsigned char)(n & 0xFF));
    }

    void saveUnsignedChar(vector<unsigned char>* data, unsigned char n){
        data->push_back(n);
    }

    void saveInt8Bit(vector<unsigned char>* data, int8_t n){
        data->push_back((unsigned char)n);
    }

    void saveBool(vector<unsigned char>* data, bool n){
        data->push_back((unsigned char)(n?'T':'F'));
    }

    void saveString(vector<unsigned char>* data, string n){
        int count = (int)n.size();
        saveInt(data, count);
        for(int i=0;i<count;i++){
            saveUnsignedChar(data, (unsigned char)n[i]);
        }
    }


    double loadDouble(unsigned char* data, int* position){
        union{
            double d;
            unsigned char b[sizeof(double)];
        } uDoubleBytes;

        for(int i=0;i<sizeof(double);i++){
            uDoubleBytes.b[i] = ((unsigned char)loadUnsignedChar(data, position));
        }
        return uDoubleBytes.d;
    }

    int loadInt(unsigned char* data, int* position){
        return ((loadUnsignedChar(data, position)<<24) |
                (loadUnsignedChar(data, position)<<16) |
                (loadUnsignedChar(data, position)<< 8) |
                (loadUnsignedChar(data, position)<< 0));
    }

    unsigned long loadUnsignedLong(unsigned char* data, int* position){
        return ((((unsigned long)loadUnsignedChar(data, position))<<48) |
				(((unsigned long)loadUnsignedChar(data, position))<<40) |
				(((unsigned long)loadUnsignedChar(data, position))<<32) |
                (((unsigned long)loadUnsignedChar(data, position))<<24) |
                (((unsigned long)loadUnsignedChar(data, position))<<16) |
                (((unsigned long)loadUnsignedChar(data, position))<< 8) |
                (((unsigned long)loadUnsignedChar(data, position))<< 0));
    }

    unsigned char loadUnsignedChar(unsigned char* data, int* position){
        unsigned char l = data[*position];
        *position+=1;
        return l;
    }

    int8_t loadInt8Bit(unsigned char* data, int* position){
        int8_t l = (int8_t)(data[*position]);
        *position+=1;
        return l;
    }

    bool loadBool(unsigned char* data, int* position){
        bool l = ((char)(data[*position]))=='T';
        *position+=1;
        return l;
    }

    string loadString(unsigned char* data, int* position){
        int count = loadInt(data, position);
        string l;
        for(int i=0;i<count;i++){
            l+=(char)loadUnsignedChar(data, position);
        }
        return l;
    }

}

namespace DrawingUtility {

    string makeBar(int progress, int maxProgress, unsigned int size, char filled, char empty){
        string s = "";
        int l = (int)(((double)size/maxProgress)*progress);
        for(int i=0;i<size;i++){
            s+=i>=l?empty:filled;
        }
        return s;
    }

}

namespace ParsingUtility {

    int getNumberFromSymbol(char symbol){
        if(symbol >= 48 && symbol <= 57){
            return symbol - 48;
        }else if(symbol >= 65 && symbol <= 90){
            return symbol - 65 + 10;
        }else if(symbol >= 97 && symbol <= 122){
            return symbol - 97 + 10;
        }else{
            throw ParseException("Invalid symbol: '"+to_string(symbol)+"'");
        }
    }

    double parseDouble(string s, int base, double defaultIfException){
        try{
            return parseDoubleUnstable(s, base);
        }catch(ParseException){
            return defaultIfException;
        }
    }

    double parseDoubleUnstable(string s, int base){
        if(base < 1 || base > 36){
            throw ParseException("Base out of range: "+to_string(base));
        }else{
            bool addToLeft = true;
            vector<int> left;
            vector<int> right;
            for(int i=0;i<s.length();i++){
                if(s[i] == '.'){
                    if(addToLeft){
                        addToLeft = false;
                    }else{
                        throw ParseException("Too many decimal points.");
                    }
                }else{
                    int n = getNumberFromSymbol(s[i]);
                    if(n >= base){
                        throw ParseException("Symbol too large for specified base.");
                    }else{
                        if(addToLeft){
                            left.push_back(n);
                        }else{
                            right.push_back(n);
                        }
                    }
                }
            }
            double final = 0;
            int d = 0;
            for(int i=(int)left.size()-1;i>=0;i--){
                final += pow(base, d)*left[i];
                d++;
            }

            d = -1;

            for(int i=0;i<right.size();i++){
                final += pow(base, d)*right[i];
                d--;
            }

            return final;
        }
    }

    int parseInt(string s, int base, int defaultIfException){
        try{
            return parseIntUnstable(s, base);
        }catch(ParseException){
            return defaultIfException;
        }
    }

    int parseIntUnstable(string s, int base){
        if(base < 1 || base > 36){
            throw ParseException("Base out of range: "+to_string(base));
        }else{
            vector<int> left;
            for(int i=0;i<s.length();i++){
                int n = getNumberFromSymbol(s[i]);
                if(n >= base){
                    throw ParseException("Symbol too large for specified base.");
                }else{
                    left.push_back(n);
                }
            }
            double final = 0;
            int d = 0;
            for(int i=(int)left.size()-1;i>=0;i--){
                final += pow(base, d)*left[i];
                d++;
            }

            return final;
        }
    }

}


