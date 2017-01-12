/*
*Copyright [2017] [Chair of Process Control Engineering (ACPLT) RWTH Aachen University]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
*/
#include <stdio.h>
#include <inttypes.h>
#include "ua_openaas_generated.h"
#ifdef UA_NO_AMALGAMATION
#include "ua_client.h"
#include "ua_config_standard.h"
#else
#include "open62541.h"
#include "ua_client_highlevel.h"
#endif
/* create an Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType, char* name, char* AssetIdSpec, int AssetIdType);

/* create a Property value statement list */
UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType, char* name, char* CarrierIdSpec, int CarrierIdType);

/* create a Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress,char* AASIdSpec, int AASIdType, char* PVSLName, char* Name, int RelationalExpression, int ExpressionSemantic, char* Value, int ValueType, char* Unit);
/* create a LifeCycle Entry */
UA_StatusCode call_CreateLCE(char* ipAddress,char* AASIdSpec, int AASIdType, char* creatingInstanceIdSpec, int creatingInstanceIdType, char* writingInstanceIdSpec, int writingInstanceIdType, char* EventClass, char* Subject, UA_DateTime TimeStamp);
