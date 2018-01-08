# openAAS_workshop
Development Repository for Workshop (2/21/2017 + 1/29/2018) Demonstrator

# Introduction
This demo requires the [ACPLT/RTE](https://github.com/acplt/rte) to be running. The [ACPLT/RTE](https://github.com/acplt/rte) works as a host for the Asset Administration Shells. Besides hosting AAS, the [ACPLT/RTE](https://github.com/acplt/rte) provides functionality to access and manipulate data of the AAS. The structure is discoverable with standard OPC UA services. However, the message-based It also serves as a local evironment for message-based communication with the industrie 4.0 component.
With this repository you can create the needed structures for an Asset Administration Shell, i.e., overall Container, Property Value Statement lists with their statements, Life Cycle Archive and its entries.

<img src="/pics/structure.png" alt="general structure" width="500">

This repository provides a project that creates a library which implements basic functionalty to create and delete asset administration shells and their inner objects (property value statements, lifecycle entries etc.). The library expects an OPC UA server on the other side that implements the OPC UA models that are provided in the *models* folder. 
All interface functions depend only on basic c-datatypes. That allows to write simple wrapper for applications like libreoffice or excel to call this functions directly.
An example application openaas_eng shows how to use the library. Furthermore, within the libreoffice folder an example python script is delivered that uses the libopcua_interface library to create AAS objects. In that case, libreoffice can be used as a client to create asset administrations shells and their describing property value statements.




# Requirements
The server: Linux (tested on Ubuntu 14.4) / Windows 7 

The Engineering Client: (tested on Ubuntu 14.4) 

# Getting Started
## Setting up the Server
Within this demo, it is assumed that the AAS are stored in a organization-bound repository which implements an OPC UA interface to interact with the AAS. In this example, the repository server is implemented with [ACPLT/RTE](https://github.com/acplt/rte). Compile it as described and add in the ov_server.conf following line at the end:
```sh
LIBRARY MessageSys
LIBRARY opcua
LIBRARY lifeCycleEntry
LIBRARY propertyValueStatement
LIBRARY openaas
LIBRARY openaasOPCUAInterface
LIBRARY openAASDiscoveryServer
```
Start the Linux Version:
(Linux)
```sh
acplt\servers\MANAGER\build_database.sh
acplt\servers\MANAGER\start_server.sh
```
Create the generic OPC UA Interface via web interface (Browser)

```
http://<YOUR_OV_SERVER_IP>:7509/createObject?format=tcl&factory=/acplt/opcua/uaServer&path=/TechUnits/opcuaInterface
```
Create the AAS specific OPC UA interface
```
http://<YOUR_OV_SERVER_IP>:7509/createObject?format=tcl&factory=/acplt/openaasOPCUAInterface/interface&path=/TechUnits/opcuaAASInterface
```
Add AAS-factory methods to the OPC UA interface with the web interface
```
http://<YOUR_OV_SERVER_IP>:7509/createObject?format=tcl&factory=/acplt/openaas/modelmanager&path=/TechUnits/openAAS/ModelmanagerOpenAAS
```

The ACPLT/RTE should now be available to be explored with ACPLT/KS (iFBsPro) or OPC UA (e.g., uaExpert). With uaExpert you should see a structure like that:

<img src="/pics/opcua_view_aas.png" alt="general structure" width="200">

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

**Installing the Python OPC-UA-openAAS-Interface**

Install the python library for openAAS with pip3.4

```
cd python && sudo pip3.4 install -e .
```
Afterwards, you are able to use this library within other python scripts.

**Using the LibreOffice Engineering Interface (Ubuntu)**

If you have Ubuntu, LibreOffice is already installed and you can create the folders /Scripts/python within
```
~/.config/libreoffice/<version>/user/
```
and copy *opcua_interface.py* to:
```
~/.config/libreoffice/<version>/user/Scripts/python
```

Afterwards, start the [ACPLT/RTE](https://github.com/acplt/rte) with the openAAS Library. Now, you can open the provided LibreOffice Calc file (aas_client.ods) from /libreOffice folder and start to create AAS, Property Value Statements and Lifecycle-Entries. 

You need to activate python macro support in libreoffice:
Install python support:
```
sudo apt-get install libreoffice-script-provider-python
```

Now you should find the entry "Python..." in:

Tools->Macros->Organize Macros










