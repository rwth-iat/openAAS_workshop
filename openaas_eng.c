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
#include <string.h>
#include <inttypes.h>
#include "ua_openaas_generated_encoding_binary.h"
#include "ua_openaas_generated_handling.h"
#include "ua_openaas_generated.h"
#ifdef UA_NO_AMALGAMATION
#include "ua_client.h"
#include "ua_config_standard.h"
#else
#include "open62541.h"
#include "ua_client_highlevel.h"
#endif
#include "opcua_interface.h"

// createAAS
#define AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define AAS_ID_TYPE 0
#define AAS_NAME "Sensor4711"
#define ASSET_ID_STRING "http://acplt.org/Sensor4711"
#define ASSET_ID_TYPE 0

// deleteAAS
#define AAS_2_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define AAS_2_ID_TYPE 0

//createSubModel
#define SM_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define SM_AAS_ID_TYPE 0
#define SM_PARENT_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body"
#define SM_PARENT_ID_TYPE 0
#define SM_MODEL_ID_STRING "http://acplt.org/model1"
#define SM_MODEL_ID_TYPE 0
#define SM_NAME "SubModel1"
#define SM_REVISION 2
#define SM_VERSION 3

// deleteSubModel
#define SM_DEL_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define SM_DEL_AAS_ID_TYPE 0
#define SM_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1"
#define SM_ID_TYPE 0

// create PVSL
#define PVSL_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVSL_AAS_ID_TYPE 0
#define PVSL_PARENT_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1"
#define PVSL_PARENT_ID_TYPE 0
#define PVSL_NAME "AssetProperties"
#define PVSL_MASK 3
#define PVSL_CARRIER_ID_STRING "http://acplt.org/Sensor4711"
#define PVSL_CARRIER_ID_TYPE 0
#define PVSL_EXPRESSIONLOGIC 1
#define PVSL_EXPRESSIONSEMANTIC 2
#define PVSL_PROPERTY_ID_STRING "https://openaas.org/properties/Diameter"
#define PVSL_PROPERTY_ID_TYPE 0
#define PVSL_VIEW 1
#define PVSL_VISIBILITY 2

// deletePVSL
#define PVSL_DEL_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVSL_DEL_AAS_ID_TYPE 0
#define PVSL_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1/AssetProperties"
#define PVSL_ID_TYPE 0

// createPVS
#define PVS_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVS_AAS_ID_TYPE 0
#define PVS_LIST_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1/AssetProperties"
#define PVS_LIST_ID_TYPE 0
#define PVS_NAME "Diameter2"
#define PVS_MASK 255
#define PVS_CARRIER_ID_STRING "http://acplt.org/Sensor4711_2"
#define PVS_CARRIER_ID_TYPE 0
#define PVS_EXPRESSIONLOGIC 3
#define PVS_EXPRESSIONSEMANTIC 3
#define PVS_PROPERTY_ID_STRING "https://openaas.org/properties/Diameter2"
#define PVS_PROPERTY_ID_TYPE 0
#define PVS_VIEW 2
#define PVS_VISIBILITY 1
#define PVS_VALUE	"Hallo"
#define PVS_VALUETYPE 8

// deletePVS
#define PVS_DEL_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVS_DEL_AAS_ID_TYPE 0
#define PVS_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1/AssetProperties/Diameter"
#define PVS_ID_TYPE 0

// setPVS
#define PVS_SET_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVS_SET_AAS_ID_TYPE 0
#define PVS_SET_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1/AssetProperties/Diameter"
#define PVS_SET_ID_TYPE 0
#define PVS_SET_MASK 254
#define PVS_SET_NAME "Diameter_New"
#define PVS_SET_CARRIER_ID_STRING "http://acplt.org/Sensor4711_Old"
#define PVS_SET_CARRIER_ID_TYPE 0
#define PVS_SET_EXPRESSIONLOGIC 2
#define PVS_SET_EXPRESSIONSEMANTIC 2
#define PVS_SET_PROPERTY_ID_STRING "https://openaas.org/properties/Diameter_Old"
#define PVS_SET_PROPERTY_ID_TYPE 1
#define PVS_SET_VIEW 2
#define PVS_SET_VISIBILITY 1
#define PVS_SET_VALUE	"asdd"
#define PVS_SET_VALUETYPE 8

// getPVS
#define PVS_GET_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVS_GET_AAS_ID_TYPE 0
#define PVS_GET_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1/AssetProperties/Diameter"
#define PVS_GET_ID_TYPE 0
char* PVS_GET_PVSName = NULL;
char* PVS_GET_CarrierIdSpec = NULL;
int PVS_GET_CarrierIdType = 0;
int PVS_GET_ExpressionLogic = 0;
int PVS_GET_ExpressionSemantic = 0;
char* PVS_GET_propertyIdSpec = NULL;
int PVS_GET_propertyIdType = 0;
int PVS_GET_view = 0;
int PVS_GET_visibility = 0;
char* PVS_GET_Value = NULL;
int PVS_GET_ValueType = 0;

// getPVSFromList
#define PVS_GETFROMLIST_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define PVS_GETFROMLIST_AAS_ID_TYPE 0
#define PVS_GETFROMLIST_ID_STRING "/TechUnits/openAAS/AASFolder/Sensor4711.Body/SubModel1/AssetProperties"
#define PVS_GETFROMLIST_ID_TYPE 0

// createLCE
#define LCE_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define LCE_AAS_ID_TYPE 0
#define LCE_CREATINGINSTANCE_ID_STRING "Creating1"
#define LCE_CREATINGINSTANCE_ID_TYPE 0
#define LCE_WRITINGINSTANCE_ID_STRING "Writing1"
#define LCE_WRITINGINSTANCE_ID_TYPE 0
#define LCE_EVENTCLASS "Event1"
#define LCE_SUBJECT "Sub1"
#define LCE_TIMESTAMP 116444736000000000 + 6444736000000000
#define LCE_VALUE	"10"
#define LCE_VALUETYPE 3

// deleteLCE
#define LCE_DEL_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define LCE_DEL_AAS_ID_TYPE 0
#define LCE_ID 1

// setLCE
#define LCE_SET_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define LCE_SET_AAS_ID_TYPE 0
#define LCE_SET_ID 2
#define LCE_SET_CREATINGINSTANCE_ID_STRING "Creating7"
#define LCE_SET_CREATINGINSTANCE_ID_TYPE 0
#define LCE_SET_WRITINGINSTANCE_ID_STRING "Writing7"
#define LCE_SET_WRITINGINSTANCE_ID_TYPE 0
#define LCE_SET_EVENTCLASS "Event7"
#define LCE_SET_SUBJECT "Sub7"
#define LCE_SET_TIMESTAMP 226444736000000000 + 6444736000000000
#define LCE_SET_VALUE	"sd"
#define LCE_SET_VALUETYPE 8
lifeCycleEntryType LCE_SET_lceData;

// getLCE
#define LCE_GET_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define LCE_GET_AAS_ID_TYPE 0
#define LCE_GET_ID 2
char* LCE_GET_CreatingInstanceIdSpec = NULL;
int LCE_GET_CreatingInstanceIdType = 0;
char* LCE_GET_WritingInstanceIdSpec = NULL;
int LCE_GET_WritingInstanceIdType = 0;
char* LCE_GET_EventClass = NULL;
char* LCE_GET_Subject = NULL;
UA_DateTime LCE_GET_TimeStamp = NULL;
char* LCE_GET_Value = NULL;
int LCE_GET_ValueType = 0;
lifeCycleEntryType LCE_GET_lceData;

// getLastLCE
#define LCE_GET_LAST_AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define LCE_GET_LAST_AAS_ID_TYPE 0
lifeCycleEntryType *LCE_GET_LAST_lceData = NULL;
#define LCE_GET_LAST_COUNT 2
unsigned int LCE_GET_LAST_lastLCEsCount = 0;


// Data types
#define DT_BOOL 1
#define DT_FLOAT 2
#define DT_INT32 3
#define DT_INT64 4
#define DT_UINT32 5
#define DT_UINT64 6
#define DT_DOUBLE 7
#define DT_STRING 8
#define DT_DATETIME 9
#define DT_IDENTIFICATION 10

int main(int argc, char *argv[]) {

    char* serverIP;
    if (argc < 2) {
        printf("using standard serverIP opc.tcp://134.130.125.81:16664 \n");
        //serverIP = malloc(26);
        serverIP = "opc.tcp://127.0.0.1:16664";
    } else
        serverIP = argv[1];

    UA_Boolean deleteElements = false;
    if (argc == 3) {
        UA_String arg3 = UA_String_fromChars(argv[2]);
        UA_String argDelete = UA_String_fromChars("delete");
        deleteElements = UA_String_equal(&arg3, &argDelete);
    }
    UA_StatusCode retval = UA_STATUSCODE_GOOD;

    retval = call_CreateAAS(serverIP, AAS_ID_STRING, AAS_ID_TYPE, AAS_NAME,
            ASSET_ID_STRING, AAS_ID_TYPE);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("AAS creation failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("AAS created \n");
/*
    retval = call_DeleteAAS(serverIP, AAS_2_ID_STRING, AAS_2_ID_TYPE);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("AAS deletion failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("AAS deleted \n");
*/

    retval = call_CreateSubModel(serverIP, SM_AAS_ID_STRING, SM_AAS_ID_TYPE,
    		SM_PARENT_ID_STRING, SM_PARENT_ID_TYPE, SM_MODEL_ID_STRING, SM_MODEL_ID_TYPE,
    		SM_NAME, SM_REVISION, SM_VERSION);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("SM creation failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("SM created \n");

/*
	retval = call_DeleteSubModel(serverIP, SM_AAS_ID_STRING, SM_AAS_ID_TYPE, SM_ID_STRING, SM_ID_TYPE);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("SM deletion failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("SM deleted \n");
*/

    retval = call_CreatePVSL(serverIP, PVSL_AAS_ID_STRING, PVSL_AAS_ID_TYPE,
    		PVSL_PARENT_ID_STRING, PVSL_PARENT_ID_TYPE, PVSL_NAME, PVSL_MASK,
    		PVSL_CARRIER_ID_STRING, PVSL_CARRIER_ID_TYPE, PVSL_EXPRESSIONLOGIC, PVSL_EXPRESSIONSEMANTIC,
    		PVSL_PROPERTY_ID_STRING, PVSL_PROPERTY_ID_TYPE, PVSL_VIEW, PVSL_VISIBILITY);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("PVSL creation failed: %i \n", (int) retval);
        goto clean;;
    }
    printf("PVSL created \n");
/*
    retval = call_DeletePVSL(serverIP, PVSL_DEL_AAS_ID_STRING, PVSL_DEL_AAS_ID_TYPE,
       		PVSL_PVSL_ID_STRING, PVSL_PVSL_ID_TYPE);
   if (retval != UA_STATUSCODE_GOOD) {
	   printf("PVSL deletion failed: %i \n", (int) retval);
	   goto clean;;
   }
   printf("PVSL deleted \n");
*/

    retval = call_CreatePVS(serverIP, PVS_AAS_ID_STRING, PVS_AAS_ID_TYPE, PVS_LIST_ID_STRING,
            PVS_LIST_ID_TYPE, PVS_NAME, PVS_VALUE, PVS_VALUETYPE, PVS_MASK,
    		PVS_CARRIER_ID_STRING, PVS_CARRIER_ID_TYPE, PVS_EXPRESSIONLOGIC, PVS_EXPRESSIONSEMANTIC,
    		PVS_PROPERTY_ID_STRING, PVS_PROPERTY_ID_TYPE, PVS_VIEW, PVS_VISIBILITY);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("PVS creation failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("PVS created \n");

/*
    retval = call_DeletePVS(serverIP, PVS_DEL_AAS_ID_STRING, PVS_DEL_AAS_ID_TYPE, PVS_ID_STRING,
            PVS_ID_TYPE);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("PVS deletion failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("PVS deleted \n");
*/

    retval = call_GetPVS(serverIP, PVS_GET_AAS_ID_STRING, PVS_GET_AAS_ID_TYPE, PVS_GET_ID_STRING,
			PVS_GET_ID_TYPE, &PVS_GET_PVSName, &PVS_GET_CarrierIdSpec, &PVS_GET_CarrierIdType,
			&PVS_GET_ExpressionLogic, &PVS_GET_ExpressionSemantic, &PVS_GET_propertyIdSpec, &PVS_GET_propertyIdType,
			&PVS_GET_view, &PVS_GET_visibility, &PVS_GET_Value, &PVS_GET_ValueType);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("PVS getting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("PVS getted \n");
	printf("pvs name: %s \n", PVS_GET_PVSName);
	printf("pvs carrierId: %s, %i \n", PVS_GET_CarrierIdSpec, PVS_GET_CarrierIdType);
	printf("pvs Expression logic: %i \n", PVS_GET_ExpressionLogic);
	printf("pvs Expression semantic: %i \n", PVS_GET_ExpressionSemantic);
	printf("pvs propertyId: %s, %i \n", PVS_GET_propertyIdSpec, PVS_GET_propertyIdType);
	printf("pvs view: %i \n", PVS_GET_view);
	printf("pvs visibility: %i \n", PVS_GET_visibility);
	printf("pvs value: %s, %i \n", PVS_GET_Value, PVS_GET_ValueType);

    retval = call_SetPVS(serverIP, PVS_SET_AAS_ID_STRING, PVS_SET_AAS_ID_TYPE, PVS_SET_ID_STRING,
			PVS_SET_ID_TYPE, PVS_SET_MASK, PVS_SET_NAME, PVS_SET_CARRIER_ID_STRING,
			PVS_SET_CARRIER_ID_TYPE, PVS_SET_EXPRESSIONLOGIC, PVS_SET_EXPRESSIONSEMANTIC,
			PVS_SET_PROPERTY_ID_STRING, PVS_SET_PROPERTY_ID_TYPE, PVS_SET_VIEW, PVS_SET_VISIBILITY,
			PVS_SET_VALUE, PVS_SET_VALUETYPE);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("PVS setting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("PVS setted \n");

	retval = call_GetPVS(serverIP, PVS_GET_AAS_ID_STRING, PVS_GET_AAS_ID_TYPE, PVS_GET_ID_STRING,
			PVS_GET_ID_TYPE, &PVS_GET_PVSName, &PVS_GET_CarrierIdSpec, &PVS_GET_CarrierIdType,
			&PVS_GET_ExpressionLogic, &PVS_GET_ExpressionSemantic, &PVS_GET_propertyIdSpec, &PVS_GET_propertyIdType,
			&PVS_GET_view, &PVS_GET_visibility, &PVS_GET_Value, &PVS_GET_ValueType);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("PVS getting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("PVS getted \n");
	printf("pvs name: %s \n", PVS_GET_PVSName);
	printf("pvs carrierId: %s, %i \n", PVS_GET_CarrierIdSpec, PVS_GET_CarrierIdType);
	printf("pvs Expression logic: %i \n", PVS_GET_ExpressionLogic);
	printf("pvs Expression semantic: %i \n", PVS_GET_ExpressionSemantic);
	printf("pvs propertyId: %s, %i \n", PVS_GET_propertyIdSpec, PVS_GET_propertyIdType);
	printf("pvs view: %i \n", PVS_GET_view);
	printf("pvs visibility: %i \n", PVS_GET_visibility);
	printf("pvs value: %s, %i \n", PVS_GET_Value, PVS_GET_ValueType);


	retval = call_CreateLCE(serverIP, LCE_AAS_ID_STRING, LCE_AAS_ID_TYPE, LCE_CREATINGINSTANCE_ID_STRING,
			LCE_CREATINGINSTANCE_ID_TYPE, LCE_WRITINGINSTANCE_ID_STRING, LCE_WRITINGINSTANCE_ID_TYPE,
			LCE_EVENTCLASS, LCE_SUBJECT, LCE_TIMESTAMP, LCE_VALUE, LCE_VALUETYPE);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCE creation failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCE created \n");


	retval = call_DeleteLCE(serverIP, LCE_DEL_AAS_ID_STRING, LCE_DEL_AAS_ID_TYPE, LCE_ID);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCE deletion failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCE deleted \n");


	retval = call_GetLCESimple(serverIP, LCE_GET_AAS_ID_STRING, LCE_GET_AAS_ID_TYPE, LCE_GET_ID,
			&LCE_GET_CreatingInstanceIdSpec, &LCE_GET_CreatingInstanceIdType, &LCE_GET_WritingInstanceIdSpec,
			&LCE_GET_WritingInstanceIdType, &LCE_GET_EventClass, &LCE_GET_Subject, &LCE_GET_TimeStamp,
			&LCE_GET_Value, &LCE_GET_ValueType);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCESimple getting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCESimple getted \n");
	printf("#%i ----\n", LCE_GET_ID);
	printf("lce creating instance: %s, %i \n",LCE_GET_CreatingInstanceIdSpec, LCE_GET_CreatingInstanceIdType);
	printf("lce writing instance: %s, %i \n", LCE_GET_WritingInstanceIdSpec, LCE_GET_WritingInstanceIdType);
	printf("lce eventClass: %s \n", LCE_GET_EventClass);
	printf("lce subject: %s \n", LCE_GET_Subject);
	printf("lce timestamp" "%" PRId64 "\n", LCE_GET_TimeStamp);
	printf("lce data: %s, %i \n", LCE_GET_Value, LCE_GET_ValueType);

	LCE_GET_lceData.creatingInstanceSpec = NULL;
	LCE_GET_lceData.data = NULL;
	LCE_GET_lceData.eventClass = NULL;
	LCE_GET_lceData.subject = NULL;
	LCE_GET_lceData.writingInstanceSpec = NULL;
	retval = call_GetLCE(serverIP, LCE_GET_AAS_ID_STRING, LCE_GET_AAS_ID_TYPE, LCE_GET_ID,
			&LCE_GET_lceData);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCE getting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCE getted \n");
    printf("#%i ----\n", LCE_GET_ID);
	printf("lce creating instance: %s, %i \n",LCE_GET_lceData.creatingInstanceSpec, LCE_GET_lceData.creatingInstanceType);
	printf("lce writing instance: %s, %i \n", LCE_GET_lceData.writingInstanceSpec, LCE_GET_lceData.writingInstanceType);
	printf("lce eventClass: %s \n", LCE_GET_lceData.eventClass);
	printf("lce subject: %s \n", LCE_GET_lceData.subject);
	printf("lce timestamp" "%" PRId64 "\n", LCE_GET_lceData.timestamp);
	printf("lce data: %s, %i \n", LCE_GET_lceData.data, LCE_GET_lceData.dataType);

/*
	retval = call_SetLCESimple(serverIP, LCE_SET_AAS_ID_STRING, LCE_SET_AAS_ID_TYPE, LCE_SET_ID, LCE_SET_CREATINGINSTANCE_ID_STRING,
			LCE_SET_CREATINGINSTANCE_ID_TYPE, LCE_SET_WRITINGINSTANCE_ID_STRING, LCE_SET_WRITINGINSTANCE_ID_TYPE,
			LCE_SET_EVENTCLASS, LCE_SET_SUBJECT, LCE_SET_TIMESTAMP, LCE_SET_VALUE, LCE_SET_VALUETYPE);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCESimple setting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCESimple setted \n");
*/
	lifeCycleEntryType LCE_SET_lceData;
	LCE_SET_lceData.creatingInstanceSpec = malloc(sizeof(LCE_SET_CREATINGINSTANCE_ID_STRING));
	LCE_SET_lceData.data = malloc(sizeof(LCE_SET_VALUE));
	LCE_SET_lceData.eventClass = malloc(sizeof(LCE_SET_EVENTCLASS));
	LCE_SET_lceData.subject = malloc(sizeof(LCE_SET_SUBJECT));
	LCE_SET_lceData.writingInstanceSpec = malloc(sizeof(LCE_SET_WRITINGINSTANCE_ID_STRING));

	strcpy(LCE_SET_lceData.creatingInstanceSpec, LCE_SET_CREATINGINSTANCE_ID_STRING);
	LCE_SET_lceData.creatingInstanceType = LCE_SET_CREATINGINSTANCE_ID_TYPE;
	strcpy(LCE_SET_lceData.data, LCE_SET_VALUE);
	LCE_SET_lceData.dataType = LCE_SET_VALUETYPE;
	strcpy(LCE_SET_lceData.eventClass, LCE_SET_EVENTCLASS);
	LCE_SET_lceData.id = LCE_SET_ID;
	strcpy(LCE_SET_lceData.subject, LCE_SET_SUBJECT);
	LCE_SET_lceData.timestamp = LCE_SET_TIMESTAMP;
	strcpy(LCE_SET_lceData.writingInstanceSpec, LCE_SET_WRITINGINSTANCE_ID_STRING);
	LCE_SET_lceData.writingInstanceType = LCE_SET_WRITINGINSTANCE_ID_TYPE;

	retval = call_SetLCE(serverIP, LCE_SET_AAS_ID_STRING, LCE_SET_AAS_ID_TYPE, LCE_SET_ID, LCE_SET_lceData);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCE setting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCE setted \n");

	retval = call_GetLCE(serverIP, LCE_GET_AAS_ID_STRING, LCE_GET_AAS_ID_TYPE, LCE_GET_ID,
			&LCE_GET_lceData);
	if (retval != UA_STATUSCODE_GOOD) {
		printf("LCE getting failed with: %i \n", (int) retval);
		goto clean;
	}
	printf("LCE getted \n");
    printf("#%i ----\n", LCE_GET_ID);
    printf("lce creating instance: %s, %i \n",LCE_GET_lceData.creatingInstanceSpec, LCE_GET_lceData.creatingInstanceType);
    printf("lce writing instance: %s, %i \n", LCE_GET_lceData.writingInstanceSpec, LCE_GET_lceData.writingInstanceType);
    printf("lce eventClass: %s \n", LCE_GET_lceData.eventClass);
    printf("lce subject: %s \n", LCE_GET_lceData.subject);
    printf("lce timestamp" "%" PRId64 "\n", LCE_GET_lceData.timestamp);
    printf("lce data: %s, %i \n", LCE_GET_lceData.data, LCE_GET_lceData.dataType);

    pvsType *pvs_c = NULL;
    //testing with wrong list name
    int pvscount = getPVSFromListByName(serverIP, PVS_GETFROMLIST_AAS_ID_STRING, PVS_GETFROMLIST_AAS_ID_TYPE,
    		PVS_GETFROMLIST_ID_STRING, PVS_GETFROMLIST_ID_TYPE, &pvs_c);

    printf("------------------------------\n");
    printf("property value statements\n");
    for (int i = 0; i < pvscount; i++) {
        printf("#%i ----\n", i);
        printf("pvs name: %s \n", pvs_c[i].name);
        printf("pvs carrierId: %s, %i \n", pvs_c[i].carrierIdIdSpec, pvs_c[i].carrierIdIdType);
        printf("pvs Expression logic: %i \n", pvs_c[i].expressionLogic);
        printf("pvs Expression semantic: %i \n", pvs_c[i].expressionSemantic);
        printf("pvs propertyId: %s, %i \n", pvs_c[i].propertyIdIdSpec, pvs_c[i].propertyIdIdType);
        printf("pvs view: %i \n", pvs_c[i].view);
        printf("pvs visibility: %i \n", pvs_c[i].visibility);
        printf("pvs value: %s, %i \n", pvs_c[i].value, pvs_c[i].valueType);
    }
    free(pvs_c);


    int lcecount = call_GetLastLCEs(serverIP, LCE_GET_LAST_AAS_ID_STRING, LCE_GET_LAST_AAS_ID_TYPE,
    		LCE_GET_LAST_COUNT, &LCE_GET_LAST_lastLCEsCount, &LCE_GET_LAST_lceData);
    printf("------------------------------\n");
    printf("lifecycle\n");

    for (int i = 0; i < LCE_GET_LAST_lastLCEsCount; i++) {
        printf("#%i ----\n", i);
        printf("lce id:" "%" PRId64 "\n", LCE_GET_LAST_lceData[i].id);
        printf("lce writing instance: %s, %i \n", LCE_GET_LAST_lceData[i].writingInstanceSpec, LCE_GET_LAST_lceData[i].writingInstanceType);
        printf("lce creating instance: %s, %i \n", LCE_GET_LAST_lceData[i].creatingInstanceSpec, LCE_GET_LAST_lceData[i].creatingInstanceType);
        printf("lce subject: %s \n", LCE_GET_LAST_lceData[i].subject);
        printf("lce eventClass: %s \n", LCE_GET_LAST_lceData[i].eventClass);
        printf("lce dataType: %i \n", LCE_GET_LAST_lceData[i].dataType);
        printf("lce data: %s, %i \n", LCE_GET_LAST_lceData[i].data, LCE_GET_LAST_lceData[i].dataType);
        printf("lce timestamp" "%" PRId64 "\n", LCE_GET_LAST_lceData[i].timestamp);
    }



    clean:

    return 0;
}

