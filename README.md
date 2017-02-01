# openAAS_workshop
Development Repository for Workshop (2/21/2017) Demonstrator

# Introduction
This demo requires the ACPLT/RTE to be running. The ACPLT/RTE works as a host for the Asset Administration Shells. Besides hosting AAS, the ACPLT/RTE provides functionality to access and manipulate data of the AAS. The structure is discoverable with standard OPC UA services. However, the message-based It also serves as a local evironment for message-based communication with the industrie 4.0 component.
With this repository you can create the needed structures for an Asset Administration Shell, i.e., overall Container, Property Value Statement lists with their statements, Life Cycle Archive and its entries.

<img src="/pics/structure.png" alt="general structure" width="500">

This repository provides a project that creates a library which implements basic functionalty to create and delete asset administration shells and their inner objects (propert value statements, lifecycle entries etc.). The library expects an OPC UA server on the other side that implements the OPC UA models that are provided in the *models* folder. 
All interface functions depend only on basic c-datatypes. That allows other applications like libreoffice or excel to call this functions directly.
An example application openaas_eng shows how to use the library. Furthermore, within the libreoffice folder an example python script is delivered that uses the libopcua_interface library to create AAS objects. In that case, libreoffice can be used as a client to create asset administrations shells and their describing property value statements.




# Requirements
The server: Linux (tested on Ubuntu 14.4) / Windows 7 

The Engineering Client: (tested on Ubuntu 14.4) 

# Getting Started
## Setting up the Server
The AAS are hosted by the [ACPLT/RTE](https://github.com/acplt/rte). Compile it as described and add in the ov_server.conf following line at the end:
```sh
LIBRARY openaas
```
Start the Linux Version:
(Linux)
```sh
acplt\servers\MANAGER\build_database.bat
acplt\servers\MANAGER\start_server.bat
```
The ACPLT/RTE should now be available to be explored with ACPLT/KS (iFBsPro) or OPC UA (e.g., uaExpert)
## Setting up the Engineering Client
**Download the client software**

To download the full repository you have to clone it:
```sh
git clone --recursive https://github.com/acplt/openAAS_workshop
```
**Building the client software**

Configure the project with CMake
```sh
cmake .
```
Build it:
```sh
make
```
Afterwards you will get the library *libopcua_interface.so* that provides several OPC UA functions. A basic example application *openaas_eng.c* shows how to use these.

** **









