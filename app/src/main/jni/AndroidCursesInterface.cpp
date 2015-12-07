//
// Created by Braeden Atlee on 11/9/15.
//

#include "AndroidCursesInterface.hpp"

#include <stdio.h>
#include <stdarg.h>

#include "main.hpp"

namespace Android {

    JNIEnv *javaEnv;
    jobject javaObj;
    int bufferWidth = 0;
    int bufferHeight = 0;
    int bufferSize = 0;
}

extern "C"{

void Java_takp_nedearb_underneath_GameSurfaceView_sendInfoToCpp(JNIEnv *env, jobject obj, int width, int height) {
    Android::javaEnv = env;
    Android::javaObj = obj;
    Android::bufferWidth = width;
    Android::bufferHeight = height;
    Android::bufferSize = width * height;
}

void Java_takp_nedearb_underneath_GameSurfaceView_initCpp(JNIEnv *env, jobject obj) {
    Android::javaEnv = env;
    Android::javaObj = obj;
    initMain();
}

void Java_takp_nedearb_underneath_GameSurfaceView_updateCpp(JNIEnv *env, jobject obj) {
    //Android::javaEnv = env;
    //Android::javaObj = obj;
    updateMain();
}

void Java_takp_nedearb_underneath_GameSurfaceView_cleanupCpp(JNIEnv *env, jobject obj) {
    //Android::javaEnv = env;
    //Android::javaObj = obj;
    cleanupMain();
}
}


int refresh(){
    jclass clazz = Android::javaEnv->FindClass("takp/nedearb/underneath/GameSurfaceView");
    jmethodID func = Android::javaEnv->GetMethodID(clazz, "refresh", "()V");
    Android::javaEnv->CallVoidMethod(Android::javaObj, func);
    Android::javaEnv->DeleteLocalRef(clazz);
    return 0;
}

int getCode(){
    jclass clazz = Android::javaEnv->FindClass("takp/nedearb/underneath/GameSurfaceView");
    jmethodID func = Android::javaEnv->GetMethodID(clazz, "getCode", "()I");
    int r = (int)Android::javaEnv->CallIntMethod(Android::javaObj, func);
    Android::javaEnv->DeleteLocalRef(clazz);
    return r;
}

void setCharInBuffer(int pos, int c, int a){
    jclass clazz = Android::javaEnv->FindClass("takp/nedearb/underneath/GameSurfaceView");
    jmethodID func = Android::javaEnv->GetMethodID(clazz, "setCharInBuffer", "(III)V");
    Android::javaEnv->CallVoidMethod(Android::javaObj, func, pos, c, a);
    Android::javaEnv->DeleteLocalRef(clazz);
}


int cursor = 0;
int currentAttr = 0;



bool initscr(){
    //dummy
    return true;
}

void start_color(){
    //Dummy
}

void use_default_colors(){
    //Dummy
}

void keypad(int scr, bool a){
    //Dummy
}

void noecho(){
    //Dummy
}

void cbreak(){
    //Dummy
}

void curs_set(int a){
    //Dummy
}

void timeout(int timeout){
    //TODO
}

int getch(){
    //refresh();
    return getCode();
}

int	move(int y, int x){
    cursor = (y*Android::bufferWidth)+x;
    return 0;
}

int addch(const char c){
    setCharInBuffer(cursor, c, currentAttr);
    cursor++;
    return 0;
}

int mvaddch(int y, int x, const char c){
    move(y, x);
    return addch(c);
}

int addstr(const char * s){
    unsigned int i = 0;
    while(s[i] != '\0'){
        addch(s[i]);
        i++;
    }
    return 0;
}

int	printw(const char * s, ...){
    char buff[256];
    va_list args;
    va_start(args, s);
    vsprintf(buff, s, args);
    addstr(buff);
    va_end(args);
    return 0;
}

int	mvaddstr(int y, int x, const char * s){
    move(y, x);
    return addstr(s);
}


int	mvprintw(int y, int x, const char * s, ...){
    move(y, x);
    char buff[256];
    va_list args;
    va_start(args, s);
    vsprintf(buff, s, args);
    addstr(buff);
    va_end(args);
    return 0;
}

int	hline(const char c, int l){
    for(int i=0;cursor%Android::bufferWidth>=0 && i<=l;i++){
        setCharInBuffer(cursor, c, currentAttr);
        cursor++;
    }
    return 0;
}

int	mvhline(int y, int x, const char c, int l){
    move(y, x);
    return hline(c, l);
}

int	vline(const char c, int l){
    for(int i=0;cursor<Android::bufferWidth*Android::bufferHeight && i<=l;i++){
        setCharInBuffer(cursor, c, currentAttr);
        cursor+=Android::bufferWidth;
    }
    return 0;
}

int	mvvline(int y, int x, const char c, int l){
    move(y, x);
    return vline(c, l);
}

int clrtoeol(){
    for(;cursor%Android::bufferWidth>0;cursor++) {
        setCharInBuffer(cursor, 0, 0);
    }
    return 0;
}

int clrtobot(){
    for(;cursor<Android::bufferSize;cursor++){
        setCharInBuffer(cursor, 0, 0);
    }
    return 0;
}

int endwin(){
    //Dummy
    return 0;
}

int getcurx(int scr){
    return cursor % Android::bufferWidth;
}

int getcury(int scr){
    return cursor / Android::bufferWidth;
}

int bkgd(int c){
    //TODO backgroundColor = c;
    return 0;
}

int init_pair(int i, int a, int b){
    //Dummy
    return 0;
}

int getmaxx(int scr){
    return Android::bufferWidth;
}

int getmaxy(int scr){
    return Android::bufferHeight;
}

int attrset(int attr){
    currentAttr = attr;
    return 0;
}