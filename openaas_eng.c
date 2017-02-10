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



#define AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define AAS_ID_TYPE 0
#define AAS_NAME "Sensor4711"
#define ASSET_ID_STRING "http://acplt.org/Sensor4711"
#define ASSET_ID_TYPE 0

#define PVSL_NAME "AssetProperties"
#define CARRIER_ID_STRING "http://acplt.org/Sensor4711"
#define CARRIER_ID_TYPE 0

#define PVS_NAME "Diameter"
#define RELATIONALEXPRESSION 1
#define EXPRESSIONSEMANTIC 1
#define VALUE	"10"
#define VALUETYPE 1
#define UNIT "cm"
#define PROPERTYREFERENCE_ID_STRING "https://openaas.org/properties/Diameter"
#define PROPERTYREFERENCE_ID_TYPE 0
#define VIEW 3

#define CREATINGINSTANCE_ID_STRING "http://acplt.org/Sensor4711_creating"
#define CREATINGINSTANCE_ID_TYPE 0
#define WRITINGINSTANCE_ID_STRING "http://acplt.org/Sensor4711_writing"
#define WRITINGINSTANCE_ID_TYPE 0
#define EVENTCLASS "eventclass"
#define SUBJECT "subject"
#define VALUE2 "15"
#define VALUETYPE2 3
#define TIMESTAMP 123456


int main ( int argc, char *argv[] ){

    UA_DateTime dt = UA_DateTime_now();//(UA_DateTime_now());

    printf("%" PRIu64 "\n", dt);
    return -1;
    char* serverIP;
    if(argc < 2){
        printf("using standard serverIP opc.tcp://134.130.125.81:16664 \n");
        //serverIP = malloc(26);
        serverIP = "opc.tcp://127.0.0.1:16664";
    }else
        serverIP = argv[1];

    UA_Boolean deleteElements =false;
    if(argc == 3){
        UA_String arg3 = UA_String_fromChars(argv[2]);
        UA_String argDelete = UA_String_fromChars("delete");
        deleteElements = UA_String_equal(&arg3,&argDelete);
    }
	UA_StatusCode retval = UA_STATUSCODE_GOOD;

	retval = call_CreateAAS(serverIP,AAS_ID_STRING,AAS_ID_TYPE, AAS_NAME, ASSET_ID_STRING,AAS_ID_TYPE);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("AAS creation failed with: %i \n",(int)retval);
	    goto clean;
	}
	printf("AAS created \n");



    retval = call_CreatePVSL(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME,CARRIER_ID_STRING,CARRIER_ID_TYPE,CREATINGINSTANCE_ID_STRING,CREATINGINSTANCE_ID_TYPE);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("A creation failed: %i \n",(int)retval);
	    goto clean;;
	}
	printf("PVSL created \n");


	retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME, RELATIONALEXPRESSION, EXPRESSIONSEMANTIC, VALUE, VALUETYPE, UNIT, PROPERTYREFERENCE_ID_STRING, PROPERTYREFERENCE_ID_TYPE, VIEW, 1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("PVS creation failed with: %i \n",(int)retval);
	    goto clean;
	}
	printf("PVS created \n");

	retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Property", 2, 2, "33", 1, "dm", "https://openaas.org/properties/Property", 1, 2, 1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("PVS creation failed with: %i \n",(int)retval);
	    goto clean;
	}
	printf("PVS created \n");

	retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Assembly", 1, 0, "900", 0, "m", "https://openaas.org/properties/Assembly", 2, 0, 0);
		if(retval!=UA_STATUSCODE_GOOD){
		    printf("PVS creation failed with: %i \n",(int)retval);
		    goto clean;
		}
		printf("PVS created \n");

	retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, EVENTCLASS, SUBJECT, TIMESTAMP, VALUE2, VALUETYPE2);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("LCE creation failed with: %i \n",(int)retval);
	    goto clean;
	}
	printf("LCE created \n");

	retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "memory", "memoSubject", 33344, "85", VALUETYPE2);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("LCE creation failed with: %i \n",(int)retval);
	    goto clean;
	}
	printf("LCE created \n");

	retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "spot", "run", 77455774, "2213", VALUETYPE2);
    if(retval!=UA_STATUSCODE_GOOD){
        printf("LCE creation failed with: %i \n",(int)retval);
        goto clean;
    }
    printf("LCE created \n");

    retval = call_SetLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,2, CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "movie", "actress", 464646, "644", VALUETYPE2);
    if(retval!=UA_STATUSCODE_GOOD){
        printf("Setting LCE failed with: %i \n",(int)retval);
        goto clean;
    }
    printf("LCE successfully set \n");

    if(deleteElements){
        retval = call_DeletePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Assembly");
        if(retval!=UA_STATUSCODE_GOOD){
            printf("PVS deletion failed with: %i \n",(int)retval);
            goto clean;
        }
        printf("PVS deleted \n");


        retval = call_SetPVS(serverIP, AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Property", 0, 3, "54", 0, "mm", "https://openaas.org/properties/Property", 1, 1, 1);
        if(retval!=UA_STATUSCODE_GOOD){
            printf("Setting PVS failed with: %i \n",(int)retval);
            goto clean;
        }
        printf("PVS successfully set \n");


        retval = call_DeleteLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,1);
        if(retval!=UA_STATUSCODE_GOOD){
            printf("LCE deletion failed with: %i \n",(int)retval);
            goto clean;
        }
        printf("LCE deleted \n");
        call_DeleteAAS(serverIP,AAS_ID_STRING,AAS_ID_TYPE);
        if(retval!=UA_STATUSCODE_GOOD){
            printf("AAS deletion failed with: %i \n",(int)retval);
            goto clean;
        }
        printf("AAS deleted \n");
    }


	pvsType *pvs_c = NULL;
	int pvscount = getPVSFromListByName(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME,&pvs_c);
    printf("------------------------------\n");
    printf("property value statements\n");
	for(int i=0;i<pvscount; i++){
        printf("#%i ----\n",i);
	    printf("pvs view: %i \n", pvs_c->view);
	    printf("pvs unit: %s \n", pvs_c->unit);
	    printf("pvs value: %s \n", pvs_c->value);
	    printf("pvs IDSpec: %s \n", pvs_c->IDIdSpec);
        printf("pvs IDType: %i \n", pvs_c->IDIdType);
        printf("pvs Expression semantic: %i \n", pvs_c->expressionSemantic);
        printf("pvs Expression logic: %i \n", pvs_c->expressionLogic);
	 }
	free(pvs_c);
    lifeCycleEntryType *lce = NULL;

//    int lcecount = call_GetLastLCEs(serverIP,AAS_ID_STRING,AAS_ID_TYPE,10,&lce);

//    printf("------------------------------\n");
//    printf("lifecycle\n");


//    for(int i=0;i<lcecount; i++){
//        printf("#%i ----\n",i);
//        printf("lce writing instance spec: %s \n", lce->writingInstanceSpec);
//        printf("lce writing instance type: %i \n", lce->writingInstanceType);
//        printf("lce creating instance spec: %s \n", lce->creatingInstanceSpec);
//        printf("lce creating instance type: %i \n", lce->creatingInstanceType);
//        printf("lce subject: %s \n", lce->subject);
//        printf("lce eventClass: %s \n", lce->eventClass);
//        printf("lce dataType: %i \n", lce->dataType);
//        printf("lce id:" "%" PRId64 "\n", lce->id);
//        printf("lce data: %s \n", lce->data);
//        printf("lce timestamp: %s \n", lce->timestamp);
//    }

//    free(lce);


    clean:

    return 0;
}

