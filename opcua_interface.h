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
//open62541 L 6850 Encoding von Extension Objects
#define MAX_STRING_SIZE 256
#ifdef WIN32 

#define EXPORT __declspec(dllexport) 
#else
#define EXPORT 
#endif

/* Asset administration shell */
EXPORT UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* AssetIdSpec, int AssetIdType);

EXPORT UA_StatusCode call_DeleteAAS(char* ipAddress, char* AASIdSpec, int AASIdType);

/* SubModel */
EXPORT UA_StatusCode call_CreateSubModel(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* ParentIdSpec, int ParentIdType, char* ModelIdSpec, int ModelIdType,
        char* name, int revision, int version);

EXPORT UA_StatusCode call_DeleteSubModel(char* ipAddress, char* AASIdSpec, int AASIdType, char* SMIdSpec, int SMIdType);

/* Property value statement list */
EXPORT UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* ParentIdSpec, int ParentIdType, char* PVSLName, int mask,
		char* CarrierIdSpec, int CarrierIdType, int ExpressionLogic, int ExpressionSemantic,
		char* propertyIdSpec, int propertyIdType, int view, int visibility);

EXPORT UA_StatusCode call_DeletePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSLIdSpec, int PVSLIdType);

/* Property value statement */
EXPORT UA_StatusCode call_CreatePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* ListIdSpec, int ListIdType, char* PVSName, char* Value, int ValueType,
		int mask, char* CarrierIdSpec, int CarrierIdType, int ExpressionLogic,
		int ExpressionSemantic,	char* propertyIdSpec, int propertyIdType,
		int view, int visibility);

EXPORT UA_StatusCode call_DeletePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType);

EXPORT UA_StatusCode call_SetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType, int mask, char* PVSName,
		char* CarrierIdSpec, int CarrierIdType, int ExpressionLogic,
		int ExpressionSemantic,	char* propertyIdSpec, int propertyIdType,
		int view, int visibility, char* Value, int ValueType);
/* simple interface types */
typedef struct pvsType {
    char* name;
    char* value;
    int valueType;
    char* carrierIdIdSpec;
	int carrierIdIdType;
    int expressionLogic;
    int expressionSemantic;
    char* propertyIdIdSpec;
	int propertyIdIdType;
    int view;
    int visibility;
} pvsType;
typedef struct lifeCycleEntryType {
    int64_t timestamp;
    char* subject;
    char* eventClass;
    char* creatingInstanceSpec;
    int creatingInstanceType;
    char* writingInstanceSpec;
    int writingInstanceType;
    char* data;
    int dataType;
    int64_t id;
} lifeCycleEntryType;

EXPORT int getPVSFromListByName(char* ipAddress, char*AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType, pvsType **pvs_c);

EXPORT UA_StatusCode call_GetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType, char** PVSName,
		char** CarrierIdSpec, int* CarrierIdType, int* ExpressionLogic,
		int* ExpressionSemantic, char** propertyIdSpec, int* propertyIdType,
		int* view, int* visibility, char** Value, int* ValueType);

/* LifeCycle Entry */
EXPORT UA_StatusCode call_CreateLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime timeStamp, char* value,
        int valueType);
EXPORT UA_StatusCode call_DeleteLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long LCEId);

EXPORT UA_StatusCode call_SetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, lifeCycleEntryType lceData);

EXPORT UA_StatusCode call_GetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, lifeCycleEntryType* lceData);

EXPORT UA_StatusCode call_SetLCESimple(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char* creatingInstanceIdSpec,
        int creatingInstanceIdType, char* writingInstanceIdSpec,
        int writingInstanceIdType, char* EventClass, char* Subject,
        UA_DateTime timeStamp, char* value, int valueType);

EXPORT UA_StatusCode call_GetLCESimple(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char** creatingInstanceIdSpec,
        int* creatingInstanceIdType, char** writingInstanceIdSpec,
        int* writingInstanceIdType, char** EventClass, char** Subject,
        UA_DateTime* timeStamp, char** value, int* valueType);

EXPORT UA_StatusCode call_GetLastLCEs(char* ipAddress, char* AASIdSpec, int AASIdType,
        unsigned int count, unsigned int* lastLCEsCount, lifeCycleEntryType **lifeCycleEntries);

/*
UA_StatusCode call_triggerGetCoreData(char* ipAddress, char* srcAASIdSpec,
        int srcAASIdType, char* dstAASIdSpec, int dstAASIdType);

// gather Lifecylce Data from Asset
UA_StatusCode call_startGetAssetLCEData(char* ipAddress, char* AASIdSpec, int AASIdType,char* assetIp, char* AssetIdSpec, int AssetIdType);
UA_StatusCode call_stopGetAssetLCEData(char* ipAddress, char* AASIdSpec, int AASIdType);
*/
