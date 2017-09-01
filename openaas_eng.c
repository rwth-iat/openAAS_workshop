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
#define PVS_NAME "Diameter"
#define PVS_MASK 3
#define PVS_CARRIER_ID_STRING "http://acplt.org/Sensor4711"
#define PVS_CARRIER_ID_TYPE 0
#define PVS_EXPRESSIONLOGIC 1
#define PVS_EXPRESSIONSEMANTIC 2
#define PVS_PROPERTY_ID_STRING "https://openaas.org/properties/Diameter"
#define PVS_PROPERTY_ID_TYPE 0
#define PVS_VIEW 1
#define PVS_VISIBILITY 2
#define PVS_VALUE	"10"
#define PVS_VALUETYPE 3

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
#define PVS_SET_CARRIER_ID_STRING "http://acplt.org/Sensor4711_New"
#define PVS_SET_CARRIER_ID_TYPE 0
#define PVS_SET_EXPRESSIONLOGIC 3
#define PVS_SET_EXPRESSIONSEMANTIC 4
#define PVS_SET_PROPERTY_ID_STRING "https://openaas.org/properties/Diameter_New"
#define PVS_SET_PROPERTY_ID_TYPE 1
#define PVS_SET_VIEW 3
#define PVS_SET_VISIBILITY 2
#define PVS_SET_VALUE	"Hallo"
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

#define LCE_TIMESTAMP 116444736000000000 + 6444736000000000
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
	printf("%s\t%s\t%i\t%i\t%i\t%s\t%i\t%i\t%i\t%s\t%i", PVS_GET_PVSName, PVS_GET_CarrierIdSpec, PVS_GET_CarrierIdType,
				PVS_GET_ExpressionLogic, PVS_GET_ExpressionSemantic, PVS_GET_propertyIdSpec, PVS_GET_propertyIdType,
				PVS_GET_view, PVS_GET_visibility, PVS_GET_Value, PVS_GET_ValueType);

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
	printf("%s\t%s\t%i\t%i\t%i\t%s\t%i\t%i\t%i\t%s\t%i", PVS_GET_PVSName, PVS_GET_CarrierIdSpec, PVS_GET_CarrierIdType,
				PVS_GET_ExpressionLogic, PVS_GET_ExpressionSemantic, PVS_GET_propertyIdSpec, PVS_GET_propertyIdType,
				PVS_GET_view, PVS_GET_visibility, PVS_GET_Value, PVS_GET_ValueType);
/*
    retval = call_CreateLCE(serverIP, AAS_ID_STRING, AAS_ID_TYPE,
            CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE,
            WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, EVENTCLASS,
            SUBJECT, TIMESTAMP1, VALUE2, VALUETYPE2);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("LCE creation failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("LCE created \n");

    retval = call_CreateLCE(serverIP, AAS_ID_STRING, AAS_ID_TYPE,
            CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE,
            WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "memory",
            "memoSubject", TIMESTAMP2, "85", VALUETYPE2);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("LCE creation failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("LCE created \n");

    retval = call_CreateLCE(serverIP, AAS_ID_STRING, AAS_ID_TYPE,
            CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE,
            WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "spot", "run",
            TIMESTAMP1, "2213", VALUETYPE2);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("LCE creation failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("LCE created \n");

    retval = call_CreateLCE(serverIP, AAS_ID_STRING, AAS_ID_TYPE,
            CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE,
            WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "DOUBLE_VALUE", "DOUBLE_VALUE",
            TIMESTAMP1, "221123423423433", DT_DOUBLE);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("LCE creation failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("LCE created \n");

    retval = call_SetLCE(serverIP, AAS_ID_STRING, AAS_ID_TYPE, 2,
            CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE,
            WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "movie",
            "actress", TIMESTAMP3, "644", VALUETYPE2);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("Setting LCE failed with: %i \n", (int) retval);
        goto clean;
    }
    printf("LCE successfully set \n");

    if (deleteElements) {
        retval = call_DeletePVS(serverIP, AAS_ID_STRING, AAS_ID_TYPE, PVSL_NAME,
                "Assembly");
        if (retval != UA_STATUSCODE_GOOD) {
            printf("PVS deletion failed with: %i \n", (int) retval);
            goto clean;
        }
        printf("PVS deleted \n");

        retval = call_SetPVS(serverIP, AAS_ID_STRING, AAS_ID_TYPE, PVSL_NAME,
                "Property", 0, 3, "54", 0, "mm",
                "https://openaas.org/properties/Property", 1, 1, 1);
        if (retval != UA_STATUSCODE_GOOD) {
            printf("Setting PVS failed with: %i \n", (int) retval);
            goto clean;
        }
        printf("PVS successfully set \n");

        retval = call_DeleteLCE(serverIP, AAS_ID_STRING, AAS_ID_TYPE, 1);
        if (retval != UA_STATUSCODE_GOOD) {
            printf("LCE deletion failed with: %i \n", (int) retval);
            goto clean;
        }
        printf("LCE deleted \n");
        call_DeleteAAS(serverIP, AAS_ID_STRING, AAS_ID_TYPE);
        if (retval != UA_STATUSCODE_GOOD) {
            printf("AAS deletion failed with: %i \n", (int) retval);
            goto clean;
        }
        printf("AAS deleted \n");
    }

    pvsType *pvs_c = NULL;
    //testing with wrong list name
    int pvscount = getPVSFromListByName(serverIP, "http://acplt.org/Sensor4711_AAS1", 0,
            "thing4711_properti", &pvs_c);
    //now with correct list name
    pvscount = getPVSFromListByName(serverIP, "http://acplt.org/Sensor4711_AAS1", 0,
            "thing4711_properties", &pvs_c);
    printf("------------------------------\n");
    printf("property value statements\n");
    for (int i = 0; i < pvscount; i++) {
        printf("#%i ----\n", i);
        printf("pvs name: %s \n", pvs_c[i].name);
        printf("pvs view: %i \n", pvs_c[i].view);
        printf("pvs unit: %s \n", pvs_c[i].unit);
        printf("pvs value: %s \n", pvs_c[i].value);
        printf("pvs IDSpec: %s \n", pvs_c[i].IDIdSpec);
        printf("pvs IDType: %i \n", pvs_c[i].IDIdType);
        printf("pvs Expression semantic: %i \n", pvs_c[i].expressionSemantic);
        printf("pvs Expression logic: %i \n", pvs_c[i].expressionLogic);

    }
    free(pvs_c);

    //create a dst-AAS to test the getCoreData function
    call_CreateAAS(serverIP, DST_AAS_ID_STRING, DST_AAS_ID_TYPE, DST_AAS_NAME,
            ASSET_ID_STRING, ASSET_ID_TYPE);

    lifeCycleEntryType *lce = NULL;
    int lcecount = call_GetLastLCEs(serverIP, AAS_ID_STRING, AAS_ID_TYPE, 10,
            &lce);
    printf("------------------------------\n");
    printf("lifecycle\n");

    for (int i = 0; i < lcecount; i++) {
        printf("#%i ----\n", i);
        printf("lce writing instance spec: %s \n", lce[i].writingInstanceSpec);
        printf("lce writing instance type: %i \n", lce[i].writingInstanceType);
        printf("lce creating instance spec: %s \n",
                lce[i].creatingInstanceSpec);
        printf("lce creating instance type: %i \n",
                lce[i].creatingInstanceType);
        printf("lce subject: %s \n", lce[i].subject);
        printf("lce eventClass: %s \n", lce[i].eventClass);
        printf("lce dataType: %i \n", lce[i].dataType);
        printf("lce id:" "%" PRId64 "\n", lce[i].id);
        printf("lce data: %s \n", lce[i].data);
        printf("lce timestamp" "%" PRId64 "\n", lce[i].timestamp);
    }

    free(lce);

    */
/*

    call_startGetAssetLCEData(serverIP,AAS_ID_STRING,AAS_ID_TYPE,"127.0.0.1",ASSET_ID_STRING,ASSET_ID_TYPE);
    int w = 7;
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<1000000;j++)
        {
            w=w+1;
            w=w-1;
        }
        if(i%2==0)
            printf("waiting..\n");
        else
            printf("waiting...\n");

    }
    call_stopGetAssetLCEData(serverIP,AAS_ID_STRING,AAS_ID_TYPE);
*/
    clean:

    return 0;
}

