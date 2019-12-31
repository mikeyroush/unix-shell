//
//  Parser.cpp
//  PA3_code
//
//  Created by Mikey Roush on 3/5/19.
//  Copyright © 2019 Michael Roush. All rights reserved.
//

#include "Parser.hpp"
using namespace std;

Parser::Parser(){
    char buf[PATH_MAX];
    getcwd(buf, sizeof(buf));
    string dir(buf);
    currentDirectoryPath = dir;
    updateCD();
}

void Parser::fill(string input){
    //reset properties
    commands.erase(commands.begin(),commands.end());
    commands.push_back("");
    execs.erase(execs.begin(),execs.end());
    runInBackground = false;
    int quoteNum = 0;
    int fileInLength = 0;
    int fileOutLength = 0;
    
    //find and extract input/output filenames from input
    //and determine whether or not to run in the background
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '"') {
            quoteNum++;
        }
        if (input[i] == '&' && quoteNum % 2 == 0) {
            runInBackground = true;
            input.erase(i,1);
        }
        if (input[i] == '<' && quoteNum % 2 == 0) {
            for (int j = i+1; j < input.length(); j++) {
                if (input[j] == '|' || input[j] == '>') {
                    break;
                }
                fileInLength++;
            }
            fileIn = input.substr(i+1,fileInLength);
            int index = 0;
            //remove whitespace from input filename
            for (int j = 0; j < fileIn.length(); j++) {
                if (fileIn[j] != ' ') {
                    fileIn[index++] = fileIn[j];
                }
            }
            fileIn[index] = '\0';
            input.erase(i,fileInLength+1);
        }
        if (input[i] == '>' && quoteNum % 2 == 0) {
            for (int j = i+1; j < input.length(); j++) {
                fileOutLength++;
            }
            fileOut = input.substr(i+1,fileOutLength);
            int index = 0;
            //remove whitespace from ouptut filename
            for (int j = 0; j < fileOut.length(); j++) {
                if (fileOut[j] != ' ') {
                    fileOut[index++] = fileOut[j];
                }
            }
            fileOut[index] = '\0';
            input.erase(i,fileOutLength+1);
        }
    }
    
    //loop through the input, keeping track of quotes, breaking it up by the |'s
    quoteNum = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '"') {
            quoteNum++;
        }
        if (input[i] == '|' && quoteNum % 2 == 0) {
            commands.push_back("");
        } else{
            commands[commands.size() - 1].push_back(input[i]);
        }
    }
    
    //loop through vector, creating a Command for each index
    for (int i = 0; i < commands.size(); i++) {
        Commands c(commands[i]);
        //c.print();
        execs.push_back(c);
    }
}

int Parser::runCmds(){
    //change directory if need be
    vector<string> args = execs[0].getArgs();
    if (strcmp(args[0].c_str(), "cd") == 0) {
        if (strcmp(args[1].c_str(), "-") == 0) {
            string dir = currentDirectoryPath;
            size_t found = dir.find_last_of("/\\");
            string dirNew = dir.substr(0,found);
            //            cout << "new dir: " << dirNew << endl;
            chdir(dirNew.c_str());
            currentDirectoryPath = dirNew;
        }
        else{
            if (strcmp(args[1].substr(0,1).c_str(), ".") == 0) {
                string dir = currentDirectoryPath;
                string app = args[1].substr(1,args[1].length()-1);
                dir.append(app);
                //                cout << "new dir: " << dir << endl;
                chdir(dir.c_str());
                currentDirectoryPath = dir;
            }
            else{
                chdir(args[1].c_str());
                //                cout << "new dir: " << args[1] << endl;
                currentDirectoryPath = args[1].c_str();
            }
        }
        updateCD();
    }
    //else execute pipelines
    else{
        int savestdin = 0;
        for (int i = 0; i < execs.size(); i++) {
            int fd[2];
            pipe(fd);
            checkAndRemoveChildren();
            pid_t pid = fork();
            if (pid == 0) {
                //if we are on the first iteration of the loop,
                if (i == 0) {
                    //if there was an input file detected, redirect the input
                    if (!fileIn.empty()) {
                        int fdIn = open(fileIn.c_str(), O_RDONLY);
                        dup2(fdIn, 0);
                    }
                }
                //if we are on the last iteration of the loop,
                if (i == (execs.size() - 1)) {
                    //if there was an output file detected, redirect the output
                    if (!fileOut.empty()) {
                        int fdOut = open (fileOut.c_str(), O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                        dup2(fdOut, 1);
                    }
                }
                else{
                    //else, change stdout to write to the pipe
                    dup2(fd[1], 1);
                }
                close(fd[0]); //close the read end of the pipe
                
                //put the arguments from the current command in the execs vector into a char*[]
                args = execs[i].getArgs();
                char* cmds[args.size()+1];
                for (int j = 0; j < args.size(); j++) {
                    cmds[j] = (char*)args[j].c_str();
                }
                cmds[args.size()] = nullptr; //add a nullptr to the end of the array
                execvp(cmds[0], cmds); //execute the command
            }
            else{
                int* status = 0;
                //if not run in background, then wait
                if (!runInBackground) {
                    waitpid(pid, status, 0);
                }
                //else add pid to list of children
                else{
                    children.push_back(pid);
                    cout << children.size()-1 << ": " << pid << endl;
                }
                //if we are on the first iteration, make a copy of stdin
                if (i == 0) {
                    savestdin = dup(0);
                }
                //if not last execution, change stdin to read from pipe
                if (i != (execs.size() -1)) {
                    dup2(fd[0], 0);
                }
                //else change it back to stdin
                else{
                    dup2(savestdin, 0);
                }
                close(fd[1]); //close the write end of the pipe
                checkAndRemoveChildren();
            }
        }
    }
    return 0;
}

vector<Commands> Parser::getExecs(){
    return execs;
}

void Parser::updateCD(){
    string path = currentDirectoryPath;
    size_t found = path.find_last_of("/\\");
    currentDirectory = path.substr(found+1,path.length()-found-1);
}

void Parser::checkAndRemoveChildren(){
    int* status = 0;
    int j = 0;
    while (j < children.size()) {
        if ((waitpid(children[j], status, WNOHANG)) != 0) {
            cout << "child " << children[j] << " removed" << endl;
            children.erase(children.begin()+j);
            j--;
        }
        j++;
    }
}

void Parser::prompt(){
    time_t t = time(0);   // get time now
    tm* now = localtime(&t);
    cout << "\n" << getenv("USER") << " | "
    << (now->tm_year + 1900) << '-'
    << (now->tm_mon + 1) << '-'
    << now->tm_mday << ' '
    << now->tm_hour << ':'
    << now->tm_min << ':'
    << now->tm_sec << " | "
    << currentDirectory << ": ";
}

void Parser::print(){
    cout << "first parse: " << endl;
    if (!fileIn.empty()) {
        cout << "fileIn: " << fileIn << endl;
    }
    if (!fileOut.empty()) {
        cout << "fileOut: " << fileOut << endl;
    }
    cout << "run in background: " << runInBackground << endl;
    for (int i = 0; i < commands.size(); i++) {
        cout << commands[i] << endl;
    }
    cout << endl << "second parse: " << endl;
    for (int i = 0; i < execs.size(); i++) {
        execs[i].print();
        cout << endl;
    }
    cout << endl << "children processes:" << endl;
    for (int i = 0; i < children.size(); i++) {
        cout << children[i] << endl;
    }
}
