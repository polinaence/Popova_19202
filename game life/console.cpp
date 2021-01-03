#include "console.h"
#include <iostream>

string console::get_com_option(){
    string comand;
    cin >> comand;
    return comand;
}

string console::get_com_2arguments() {
    string com;
    char arg[2];
    getline(std::cin, com);
    arg[0] = com[com.length()-2];
    arg[1] = com[com.length()-1];

    return arg;
}

string console::get_com_1arguments() {
    string com;
    char arg[1];
    getline(std::cin, com);
    if (!com.length())
    {
        arg[0] = '1';
        return arg; }
    arg[0] =com[com.length()-1];
    return arg;
}



