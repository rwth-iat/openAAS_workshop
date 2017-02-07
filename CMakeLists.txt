cmake_minimum_required(VERSION 2.8.11)
project(openaas_engineering C)
include(ExternalProject)
find_package(PythonInterp REQUIRED)
set (CMAKE_C_FLAGS "--std=c99 -Werror ${CMAKE_C_FLAGS}")

# Set default build type.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not given; setting to 'Debug'.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()
set(CMAKE_VERBOSE_MAKEFILE ON) 


#open62541 business
set(OPEN62541_DIR "${CMAKE_SOURCE_DIR}/external/open62541")
ExternalProject_Add(open62541
DOWNLOAD_COMMAND ""
CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
           -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
           -DUA_ENABLE_AMALGAMATION=ON
           -DUA_ENABLE_DISCOVERY=OFF
SOURCE_DIR ${OPEN62541_DIR}
BINARY_DIR ${CMAKE_SOURCE_DIR}/external_generated
INSTALL_COMMAND ""
)
include_directories(${CMAKE_SOURCE_DIR}/external_generated)
link_directories(${PROJECT_SOURCE_DIR}/external_generated)
include_directories(${PROJECT_BINARY_DIR}) #for nodeset.h

set(MODELS  ${PROJECT_SOURCE_DIR}/models/aas.xml)



#nodeset generation
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/nodeset.h ${PROJECT_BINARY_DIR}/nodeset.c
                  PRE_BUILD
                  COMMAND ${PYTHON_EXECUTABLE} ${OPEN62541_DIR}/tools/pyUANamespace/generate_open62541CCode.py
                                                -i ${OPEN62541_DIR}/tools/pyUANamespace/NodeID_Blacklist_FullNS0.txt
                                                ${OPEN62541_DIR}/tools/schema/namespace0/Opc.Ua.NodeSet2.xml
                                                ${MODELS}
                                                ${PROJECT_BINARY_DIR}/nodeset
                  DEPENDS ${OPEN62541_DIR}/tools/pyUANamespace/generate_open62541CCode.py
                          ${OPEN62541_DIR}/tools/pyUANamespace/open62541_MacroHelper.py
                          ${OPEN62541_DIR}/tools/pyUANamespace/ua_builtin_types.py
                          ${OPEN62541_DIR}/tools/pyUANamespace/ua_constants.py
                          ${OPEN62541_DIR}/tools/pyUANamespace/ua_namespace.py
                          ${OPEN62541_DIR}/tools/pyUANamespace/ua_node_types.py
                          ${OPEN62541_DIR}/tools/pyUANamespace/NodeID_Blacklist_FullNS0.txt
                          ${MODELS}
                  )
# openaas data types
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas_generated.c
                          ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas_generated.h
                          ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas_generated_handling.h
                          ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas_generated_encoding_binary.h
                   PRE_BUILD
                   COMMAND ${PYTHON_EXECUTABLE} ${OPEN62541_DIR}/tools/generate_datatypes.py
                           --typedescriptions ${PROJECT_SOURCE_DIR}/models/aas.csv
                           --namespace=3 --selected_types=${PROJECT_SOURCE_DIR}/models/aas_selected_types.txt
                           ${OPEN62541_DIR}/tools/schema/Opc.Ua.Types.bsd
                           ${PROJECT_SOURCE_DIR}/models/Custom.Opc.Ua.AssetAdministrationShell.bsd
                           ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas
                   DEPENDS ${OPEN62541_DIR}/tools/generate_datatypes.py
                           ${PROJECT_SOURCE_DIR}/models/aas_selected_types.txt
                           ${PROJECT_SOURCE_DIR}/models/Custom.Opc.Ua.AssetAdministrationShell.bsd
                           ${PROJECT_SOURCE_DIR}/models/aas.csv
                           ${OPEN62541_DIR}/tools/schema/Opc.Ua.Types.bsd)

add_definitions(-DUA_NO_AMALGAMATION)
#shared object 
add_library(opcua_interface SHARED opcua_interface.c ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas_generated.c)
target_include_directories(opcua_interface PRIVATE ${CMAKE_SOURCE_DIR}/external/open62541/plugins ${CMAKE_SOURCE_DIR}/external/open62541/include ${CMAKE_SOURCE_DIR}/external_generated/src_generated ${CMAKE_SOURCE_DIR}/external/open62541/src ${CMAKE_SOURCE_DIR}/external/open62541/src/server ${CMAKE_SOURCE_DIR}/external/open62541/deps) # needs an internal header
add_dependencies(opcua_interface open62541)
target_link_libraries(opcua_interface libopen62541.a)
#test client for shared object

add_executable(openaas_eng    openaas_eng.c 
                              ${PROJECT_BINARY_DIR}/nodeset.c
                              ${CMAKE_SOURCE_DIR}/external_generated/ua_openaas_generated.c)
target_include_directories(openaas_eng PRIVATE ${CMAKE_SOURCE_DIR}/external/open62541/plugins ${CMAKE_SOURCE_DIR}/external/open62541/include ${CMAKE_SOURCE_DIR}/external_generated/src_generated ${CMAKE_SOURCE_DIR}/external/open62541/src ${CMAKE_SOURCE_DIR}/external/open62541/src/server ${CMAKE_SOURCE_DIR}/external/open62541/deps) # needs an internal header
#find_library(${PROJECT_BINARY_DIR} opcua_interface)
target_link_libraries(openaas_eng LINK_PUBLIC opcua_interface)          
add_dependencies(openaas_eng opcua_interface)





