#include <stdio.h>

#include "Parser.hpp"
using namespace std;

int main(int argc, char ** argv){
	bool cont = true;
	string input = "";
	string quitMsg = "quit";
    Parser p;
    
    while(cont){
        p.prompt();
		getline(cin, input);
        if (strcmp(input.c_str(), quitMsg.c_str()) == 0){
			cont = false;
            break;
        }
        p.fill(input);
//        p.print();
        p.runCmds();
        
//        cont = false; //change later*******
     }
	return 0;
}
