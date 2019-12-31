//
//  Commands.hpp
//  PA3_code
//
//  Created by Mikey Roush on 3/5/19.
//  Copyright © 2019 Michael Roush. All rights reserved.
//

#ifndef Commands_hpp
#define Commands_hpp

#include <string>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h> 

class Commands{
public:
    Commands(std::string);
    int test();
    void print();
    std::vector<std::string> getArgs();
    std::string getArg(int);
    int getArgsSize();
private:
    std::vector<std::string> args;
};

#endif /* Commands_hpp */
