//
//  Commands.cpp
//  PA3_code
//
//  Created by Mikey Roush on 3/5/19.
//  Copyright © 2019 Michael Roush. All rights reserved.
//

#include "Commands.hpp"
using namespace std;

Commands::Commands(string command){
    //if the vector is empty, give it an index
    if (args.size() == 0) {
        args.push_back("");
    }
    //erase leading whitespaces
    while (command[0] == ' ') {
        command.erase(0,1);
    }
    //erase ending whitespaces
    while (command[command.length()-1] == ' ') {
        command.erase(command.length()-1,1);
    }
    //loop throught the input, keeping track of the quotes, breaking it up by spaces
    int quoteNum = 0;
    int sQuoteNum = 0;
    for (int i = 0; i < command.length(); i++) {
        if (command[i] == '"') {
            quoteNum++;
        }
        if (command[i] == '\'') {
            sQuoteNum++;
        }
        if (command[i] == ' ' && quoteNum % 2 == 0 && sQuoteNum % 2 == 0) {
            args.push_back("");
        }
        else if (command[i] != '"' && command[i] != '\''){
            args[args.size()-1].push_back(command[i]);
        }
    }
    
}

int Commands::test(){
    char* cmds[args.size()+1];
    for (int i = 0; i < args.size(); i++) {
        cmds[i] = (char*)args[i].c_str();
    }
    cmds[args.size()] = nullptr;
    if (fork() == 0) {
        execvp(cmds[0], cmds);
    }
    else{
        wait(0);
    }
    return 0;
}

vector<string> Commands::getArgs(){
    return args;
}

string Commands::getArg(int index){
    return args[index];
}

int Commands::getArgsSize(){
    return (int)args.size();
}

void Commands::print(){
    cout << "command: " << args[0] << endl;
    cout << "args:" << endl;
    for (int i = 0; i < args.size(); i++) {
        cout << args[i] << endl;
    }
    cout << endl;
}
