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
        char* name, char* CarrierIdSpec, int CarrierIdType,
        char* CreatingInstanceIdSpec, int CreatingInstanceIdType);

UA_StatusCode call_DeletePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name);

/* Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int ExpressionLogic, int ExpressionSemantic,
        char* Value, int ValueType, char* Unit, char* propertyReferenceIdSpec,
        int propertyReferenceIdType, int view, int visibility);

UA_StatusCode call_DeletePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name);

UA_StatusCode call_SetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int ExpressionLogic, int ExpressionSemantic,
        char* Value, int ValueType, char* Unit, char* propertyReferenceIdSpec,
        int propertyReferenceIdType, int view, int visibility);
/* simple interface types */
typedef struct pvsType {
    char name[MAX_STRING_SIZE];
    char value[MAX_STRING_SIZE];
    int valueType;
    char unit[MAX_STRING_SIZE];
    int expressionLogic;
    int expressionSemantic;
    int view;
    char IDIdSpec[MAX_STRING_SIZE];
    int IDIdType;
    int visibility;
} pvsType;
typedef struct lifeCycleEntryType {
    int64_t timestamp;
    char subject[MAX_STRING_SIZE];
    char eventClass[MAX_STRING_SIZE];
    char creatingInstanceSpec[MAX_STRING_SIZE];
    int creatingInstanceType;
    char writingInstanceSpec[MAX_STRING_SIZE];
    int writingInstanceType;
    char data[256];
    int dataType;
    int64_t id;
} lifeCycleEntryType;

int getPVSFromListByName(char* ipAddress, char*AASIdSpec, int AASIdType,
        char* listname, pvsType **pvs_c);
UA_StatusCode call_GetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int* ExpressionLogic,
        int* ExpressionSemantic, char** Value, int* ValueType, char** Unit,
        char** propertyReferenceIdSpec, int* propertyReferenceIdType, int* view,
        int* visibility);

/* LifeCycle Entry */
UA_StatusCode call_CreateLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime timeStamp, char* value,
        int valueType);
UA_StatusCode call_DeleteLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long LCEId);

UA_StatusCode call_SetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char* creatingInstanceIdSpec,
        int creatingInstanceIdType, char* writingInstanceIdSpec,
        int writingInstanceIdType, char* EventClass, char* Subject,
        UA_DateTime timeStamp, char* value, int valueType);

UA_StatusCode call_GetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char** creatingInstanceIdSpec,
        int* creatingInstanceIdType, char** writingInstanceIdSpec,
        int* writingInstanceIdType, char** EventClass, char** Subject,
        UA_DateTime* timeStamp, char** value, int* valueType);

int call_GetLastLCEs(char* ipAddress, char* AASIdSpec, int AASIdType,
        unsigned int count, lifeCycleEntryType **lifeCycleEntries);

UA_StatusCode call_triggerGetCoreData(char* ipAddress, char* srcAASIdSpec,
        int srcAASIdType, char* dstAASIdSpec, int dstAASIdType);
