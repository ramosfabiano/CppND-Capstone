# CppND Capstone Project - Simple HTTP Server

This is my capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

For the capstone, I have decided to implement a (simple) HTTP server.  An HTTP server is a well-known application, and offers excelent grounds for showcasing the OOP, Memory Management and Concurrency topics explored throughout the course.

## Overview

For this project, our objetive is to implement a basic HTTP server capable of handling a static website using HTTP/1.1.

The HTTP/1.1 protocol is defined in RFCs [7230](http://www.rfc-editor.org/info/rfc7230), [7231](http://www.rfc-editor.org/info/rfc7231), 
[7232](http://www.rfc-editor.org/info/rfc7232), [7233](http://www.rfc-editor.org/info/rfc7233), 
[7234](http://www.rfc-editor.org/info/rfc7234) and [7235](http://www.rfc-editor.org/info/rfc7235).


## System Requirements

The project was developed/tested on Debian 11 (Bullseye) and Arch Linux, but it should compile and run fine on any Linux distribution.

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
* boost >= 1.74
  * Debian: `sudo apt install libboost-all-dev`
  * Arch: `sudo pacman -S boost`
* clang-tidy >= 11.0.1
  * Debian: `sudo apt install clang-tidy`
  * Arch: `sudo pacman -S clang`
* astyle >= 3.1
  * Debian: `sudo apt install astyle`
  * Arch: `sudo pacman -S astyle`  

## Basic Build Instructions

The project can be build using `make <target>`.  The Makefile has five targets:

* `build`: compiles the source code and generates an executable.
* `debug`: compiles the source code and generates an executable, including debugging information.
* `clean`: deletes the `build/` and `install/`  directories, i.e., cleans the build artifacts.
* `format`: formats the source code using the `astyle` tool.


## Running the Server

In order to run the resulting executable manually: `./install/http_server --port=<port> --folder=<folder>` where `port` should be the port to listen on and
`folder` should be the path to a directory containing the files to be served. The options are optional and defaults to `8080` and `./site` respectivelly.
The `./site` folder contains a simple web site for demonstration purposes.

Then with the server running, open a web browser and point to `http://localhost:<port>`, where  `port` should be the port the server is listening on.


**TODO: add a folder with a simple web site as an example.**

**TODO: add gif dual window browser and server on a terminal**


## Implementation Details

**TODO: complete as the implemenation moves on...**

After being launched, the server binds to the indicated port and listen for connections.

Upon receiving a connection, the server handles the request in separate thread.

## Project Rubric

These are the rubric items addressed by this project:

**README**

- [x] A README with instructions is included with the project.
- [x] The README indicates which project is chosen.
- [x] The README includes information about each rubric point addressed. 

**Compiling and Testing**
- [x] The submission must compile and run.

**Loops, Functions, I/O**
- [x] The project demonstrates an understanding of C++ functions and control structures.
- [ ] The project reads data from a file and process the data, or the program writes data to a file.
- [x] The project accepts user input and processes the input. [See here.](https://github.com/pragmaerror/CppND-Capstone/blob/cb81708dc54448534d03638e35b77e54242a2aca/src/app/main.cpp#L20)

**Object Oriented Programming**
- [ ] The project uses Object Oriented Programming techniques.
- [ ] Classes use appropriate access specifiers for class members.
- [ ] Class constructors utilize member initialization lists.
- [ ] Classes abstract implementation details from their interfaces.
- [ ] Classes encapsulate behavior.
- [ ] Classes follow an appropriate inheritance hierarchy.
- [ ] Overloaded functions allow the same function to operate on different parameters.
- [ ] Derived class functions override virtual base class functions.
- [x] Templates generalize functions in the project. [See here.](https://github.com/pragmaerror/CppND-Capstone/blob/cb81708dc54448534d03638e35b77e54242a2aca/src/lib/logger/logger.hpp#L35)

**Memory Management**
- [x] The project makes use of references in function declarations.
- [ ] The project uses destructors appropriately.
- [ ] The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
- [ ] The project follows the Rule of 5.
- [ ] The project uses move semantics to move data, instead of copying it, where possible.
- [ ] The project uses smart pointers instead of raw pointers.

**Concurrency**
- [ ] The project uses multithreading.
- [ ] A promise and future is used in the project.
- [x] A mutex or lock is used in the project. [See here.](https://github.com/pragmaerror/CppND-Capstone/blob/cb81708dc54448534d03638e35b77e54242a2aca/src/lib/logger/logger.hpp#L38)
- [ ] A condition variable is used in the project.








  






