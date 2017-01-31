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


#define MAX_STRING_SIZE 256
/* Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* AssetIdSpec, int AssetIdType);

UA_StatusCode call_DeleteAAS(char* ipAddress, char* AASIdSpec, int AASIdType);

/* Property value statement list */
UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* CarrierIdSpec, int CarrierIdType);

UA_StatusCode call_DeletePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name);

/* Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int RelationalExpression,
        int ExpressionSemantic, char* Value, int ValueType, char* Unit,
        char* propertyReferenceIdSpec, int propertyReferenceIdType, int view);

UA_StatusCode call_DeletePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name);

UA_StatusCode call_SetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int RelationalExpression,
        int ExpressionSemantic, char* Value, int ValueType, char* Unit,
        char* propertyReferenceIdSpec, int propertyReferenceIdType, int view);
typedef struct pvsType{
    char name[256];
    char value[256];
    int valueType;
    char unit[256];
    int relationalExpression;
    int expressionSemantic;
    int view;
    int accessControl;
    char propertyReferenceIdSpec[256];
    int propertyReferenceIdType;
}pvsType;
typedef struct lifeCycleEntry{
    char timestamp[256];
    char subject[256];
    char eventClass[256];
    char creatingInstanceSpec[256];
    int creatingInstanceType;
    char writingInstanceSpec[256];
    int writingInstanceType;
    char data[256];
}lifeCycleEntry;
typedef struct opcua_pvs{
    UA_PropertyValueStatement statement;
    UA_NodeId id;
}opcua_pvs;
typedef struct PVSL{
    UA_Identification Carrier;
    opcua_pvs* statements;
    size_t statementsSize;
}PVSL;
typedef struct pvslist{
    char name[256];
}pvslist;


int getAllPVS(char* ipAddress, pvsType **pvs);
int getPVSFromListByName(char* ipAddress,char*AASIdSpec,int AASIdType,char* listname, pvsType **pvs_c);
UA_StatusCode call_GetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int* RelationalExpression,
        int* ExpressionSemantic, char** Value, int* ValueType, char** Unit,
        char** propertyReferenceIdSpec, int* propertyReferenceIdType, int* view);

/* LifeCycle Entry */
UA_StatusCode call_CreateLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime timeStamp, char* value,
        int valueType);

UA_StatusCode call_DeleteLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long LCEId);

UA_StatusCode call_SetLCE(char* ipAddress, char* AASIdSpec, int AASIdType, long long lceId,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime timeStamp, char* value,
        int valueType);

UA_StatusCode call_GetLCE(char* ipAddress, char* AASIdSpec, int AASIdType, long long lceId,
        char** creatingInstanceIdSpec, int* creatingInstanceIdType,
        char** writingInstanceIdSpec, int* writingInstanceIdType,
        char** EventClass, char** Subject, UA_DateTime* timeStamp, char** value,
        int* valueType);
