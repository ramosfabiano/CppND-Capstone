# CppND Capstone Project - Minimal HTTP Server

This is my capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

In order to exercise the skills learned during the course, I have decided to implement a minimalistic HTTP server.  Such application provides a good framework for showcasing the OOP, Memory Management and Concurrency topics explored throughout the course.

## Overview

An HTTP server is a well-known application. 

For this project, we do not aim to implement a full-featured server, but a basic implemention capable of handling a simple static website using HTTP/1.1.

The HTTP/1.1 protocol is defined in RFCs [7230](http://www.rfc-editor.org/info/rfc7230), [7231](http://www.rfc-editor.org/info/rfc7231), 
[7232](http://www.rfc-editor.org/info/rfc7232), [7233](http://www.rfc-editor.org/info/rfc7233), 
[7234](http://www.rfc-editor.org/info/rfc7234) and [7235](http://www.rfc-editor.org/info/rfc7235).


## System Requirements

The project was developed/tested on Debian Bullseye and Arch Linux, but it should compile and run fine on any Linux distribution.

## Build Dependencies 

* cmake >= 3.18
  * Debian: `sudo apt install cmake`
  * Arch: `sudo pacman -S cmake`
* make >= 4.3 
  * Debian: `sudo apt install make`
  * Arch: `sudo pacman -S make`
* gcc/g++ >= 10.2
  * Debian: `sudo apt install g++`
  * Arch: `sudo pacman -S gcc`
* Optional:
  * clang-format >= 11.0.1
    * Debian: `sudo apt install clang-format`
  * Arch: `sudo pacman -S clang`
  * clang-tidy >= 11.0.1
    * Debian: `sudo apt install clang-tidy`
    * Arch: `sudo pacman -S clang`
  
## Basic Build Instructions

The project can be build using `make <target>`.  The Makefile has five targets:

* `build`: compiles the source code and generates an executable.
* `debug`: compiles the source code and generates an executable, including debugging information.
* `clean`: deletes the `build/` and `install/`  directories, i.e., cleans the build artifacts.
* `format`: applies [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to style the source code.
* `all`: cleans the build artifacts, formats and builds the code.

## Running the Server

In order to run the resulting executable manually: `./install/http_server --port=<port> --folder=<folder>` where `port` should be the port to listen at and
`folder` should be the path to a directory containing the files to be served. The options are optional and defaults to `12345` and `./site` respectivelly.
The `./site` folder contains a simple web site for demonstration purposes.

Then with the server running, open a web browser and point to `http://localhost:<port>`, where  `port` should be the port the server is listening at.


** TODO: add a folder with a simple web site as an example. **

** TODO: add gif dual window browser and server on a terminal **


## Implementation Details

** TODO: complete as the implemenation moves on... **

After being launched, the server binds to the indicated port and listen for connections.

Upon receiving a connection, the server handles the request in separate thread.

bla bla bla....

## Project Rubric

These are the rubric items addressed by this project:

* README
- [x] [A README with instructions is included with the project.](https://github.com/pragmaerror/CppND-Capstone/blob/master/README.md#cppnd-capstone-hello-world-repo)
- [x] [The README indicates which project is chosen.](https://github.com/pragmaerror/CppND-Capstone/blob/master/README.md#L05)
- [x] [The README includes information about each rubric point addressed.](https://github.com/pragmaerror/CppND-Capstone/blob/master/README.md#project-rubric)

* Compiling and Testing
- [x] The submission must compile and run.

* Loops, Functions, I/O
- [ ] The project demonstrates an understanding of C++ functions and control structures.
- [ ] The project reads data from a file and process the data, or the program writes data to a file.
- [ ] The project accepts user input and processes the input.

* Object Oriented Programming
- [ ] The project uses Object Oriented Programming techniques.
- [ ] Classes use appropriate access specifiers for class members.
- [ ] Class constructors utilize member initialization lists.
- [ ] Classes abstract implementation details from their interfaces.
- [ ] Classes encapsulate behavior.
- [ ] Classes follow an appropriate inheritance hierarchy.
- [ ] Overloaded functions allow the same function to operate on different parameters.
- [ ] Derived class functions override virtual base class functions.
- [ ] Templates generalize functions in the project.

* Memory Management
- [ ] The project makes use of references in function declarations.
- [ ] The project uses destructors appropriately.
- [ ] The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
- [ ] The project follows the Rule of 5.
- [ ] The project uses move semantics to move data, instead of copying it, where possible.
- [ ] The project uses smart pointers instead of raw pointers.

* Concurrency
- [ ] The project uses multithreading.
- [ ] A promise and future is used in the project.
- [ ] A mutex or lock is used in the project.
- [ ] A condition variable is used in the project.








  






