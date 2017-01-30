# openAAS_workshop
Development Repository for Workshop (2/21/2017) Demonstrator


# How to set up the demonstrator
This demo requires the ACPLT/RTE to be running. The ACPLT/RTE works as a host for the Asset Administration Shells. Besides hosting AAS, the ACPLT/RTE provides functionality to access and manipulate data of the AAS. It also serves as a local evironment for message-based communication with the industrie 4.0 component.
With this repository you can create the needed structures for an Asset Administration Shell, i.e., overall Container, Property Value Statement lists with their statements, Life Cycle Archive and its entries.

## Functionality
This repository provides a project that creates a library which implements basic functionalty to create and delete asset administration shells and their inner objects (propert value statements, lifecycle entries etc.). The library expects an OPC UA server on the other side that implements the models that are provided in the models folder. 
All interface functions depend only on basic c-datatypes. That allows other applications like libreoffice or excel to call this functions directly.
An example application openaas_eng shows how to use the library. Furthermore, within the libreoffice folder an example python script is delivered that uses the libopcua_interface library to create AAS objects. In that case, libreoffice can be used as a client to create asset administrations shells and their describing property value statements.
## Requirements
The server: Linux (tested on Ubuntu 14.4) / Windows 7
The Engineering Client: (tested on Ubuntu 14.4) 

## Getting Started
# Cloning
To download the full repository you have to clone it:
```sh
git clone --recursive https://github.com/acplt/openAAS_workshop
```
# Building the library (Engineering Client)
Configure the project with CMake
```sh
CMake .
```
Build it:
```sh
make
```
Afterwards you will get the library libopcua_interface.so that provides functionality to access an AAS.


CMake is needed to configure
First, you need to start up the [ACPLT/RTE](http://acplt.github.io/rte-www/releases/) which is capable to host AAS:
(Windows)
```sh
acplt\servers\MANAGER\build_database.bat
acplt\servers\MANAGER\start_server.bat
```
Start the Linux Version:
(Linux)
```sh
acplt\servers\MANAGER\build_database.bat
acplt\servers\MANAGER\start_server.bat
```






