# openAAS_workshop
Development Repository for Workshop (2/21/2017) Demonstrator

# What's in?
This repository provides a project that creates a library which implements basic functionalty to create and delete asset administration shells and their inner objects (propert value statements, lifecycle entries etc.). The library expects an OPC UA server on the other side that implements the models that are provided in the models folder. 
All interface functions depend only on basic c-datatypes. That allows other applications like libreoffice or excel to call this functions directly.
An example application openaas_eng shows how to use the library. Furthermore, within the libreoffice folder an example python script is delivered that uses the libopcua_interface library to create AAS objects. In that case, libreoffice can be used as a client to create asset administrations shells and their describing property value statements.


#How to set up the demonstrator
This demo requires the ACPLT/RTE to be running. The ACPLT/RTE works as a host for the asset administration shells. Besides hosting AAS, the ACPLT/RTE provides functionality to access and manipulate data of the AAS. It also serves as a local evironment for message-based communication with the industrie 4.0 component.
#Use Cases


