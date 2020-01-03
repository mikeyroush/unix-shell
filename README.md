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

```

## Authors

* **Michael Roush** - *Project completion*

## License

Copyright © 2017 Michael Roush. All rights reserved.

