#include "../include/conio.h"
#include <iostream>
using namespace std;

const int KB_UP = 72;
const int KB_DOWN = 80;
const int KB_RIGHT = 77;
const int KB_LEFT = 75;
const int ESC = 27;

int main() {
    
    while (true) {
        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            switch (ch) {
            case KB_UP: 
                std::cout << "up\n";   
                break;
            case KB_DOWN:
                std::cout << "down\n";
                break;
            case KB_RIGHT:
                std::cout << "right\n";
                break;
            case KB_LEFT:
                std::cout << "left\n";
                break;
            default: std::cout << "unknown\n";
            }
        }
        else if (ch == ESC)
        {
            std::cout << "Escape pressed, going out!\n";
            break;
        }
    }
}