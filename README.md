# unix-shell

This project is a simple shell which implements a subset of Bash functionality. The following represents the shell's features and requirements:
- a continual prompt for user input
- a parser based on various tokens
- execution of well formed commands using fork and exec
  - this required the implementation of child processes
- support for input and output redirection to and from files
  - this required the manipulation of file descriptors
- allowance for users to run processes in the background using a '*&*'
- a custom prompt based on current directory, user name and current date/time


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for testing purposes.

### Prerequisites

A working command line

### Installing and Running

Download the zip then link the files in the command line via the makefile and run the executable

Navigate to the correct directory and utilize the makefile

```console
foo@bar:~$ cd /path/to/directory/here
foo@bar:~$ make all
```

This will have created a few .o files and an executable. To run it, do as follows...

```console
foo@bar:~$ ./test
```

Demo

```console
mikeyroush | 2020-1-2 18:39:4 | test: echo hello world
hello world

mikeyroush | 2020-1-2 18:39:19 | test: ls
Commands.cpp	Main.cpp	Parser.hpp	test
Commands.hpp	Main.o		Parser.o	test.txt
Commands.o	Parser.cpp	makefile

mikeyroush | 2020-1-2 18:42:57 | test: mkdir helloWorld

mikeyroush | 2020-1-2 18:43:15 | test: ls
Commands.cpp	Main.cpp	Parser.hpp	makefile
Commands.hpp	Main.o		Parser.o	test
Commands.o	Parser.cpp	helloWorld	test.txt

mikeyroush | 2020-1-2 18:43:33 | test: cd helloWorld

mikeyroush | 2020-1-2 18:44:3 | helloWorld: echo hello.txt > hello.txt

mikeyroush | 2020-1-2 18:45:32 | helloWorld: cat hello.txt
hello.txt
```

## Authors

* **Michael Roush** - *Project completion*

## License

Copyright © 2019 Michael Roush. All rights reserved.

