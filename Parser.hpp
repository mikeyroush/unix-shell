//
//  Parser.hpp
//  PA3_code
//
//  Created by Mikey Roush on 3/5/19.
//  Copyright © 2019 Michael Roush. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
#include <ctime>
#include <stdlib.h>

#include "Commands.hpp"

class Parser{
public:
    Parser();
    void fill(std::string);
    void print();
    void prompt();
    void updateCD();
    void checkAndRemoveChildren();
    int runCmds();
    std::vector<Commands> getExecs();
private:
    std::vector<std::string> commands;
    std::vector<Commands> execs;
    std::vector<pid_t> children;
    std::string fileIn;
    std::string fileOut;
    std::string currentDirectoryPath;
    std::string currentDirectory;
    bool runInBackground;
};

#endif /* Parser_hpp */
