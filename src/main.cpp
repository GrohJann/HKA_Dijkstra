//
// Created by ruvaak on 12.11.25.
//

#include <iostream>

#include "Model/OSMHandler.cpp"



int main() {
    //std::string fileName;
    //std::cout << "please enter the file name: ";
    //std::cin >> fileName;
    OSMHandler handler;
    //handler.parseFile(fileName);
    handler.parseFile("OSMmaps/maposm.sec");
    return 0;
}

