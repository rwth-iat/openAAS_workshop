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
#define VALUETYPE2 1
#define TIMESTAMP 123456


int main ( int argc, char *argv[] ){
    char* serverIP = NULL;
    if(argc < 2){
        printf("using standard serverIP opc.tcp://134.130.125.81:16664 \n");
        serverIP = malloc(26);
        serverIP = "opc.tcp://134.130.125.81:16664";
    }else
        serverIP = argv[1];

	UA_StatusCode retval = UA_STATUSCODE_GOOD;

    pvsType *pvs = NULL;
    int count;

   // count = getPVSFromListByName(serverIP,"http://acplt.org/Sensor4711",0,"AssetProperties",&pvs);

//    for(int i=0;i<count;i++){
//        printf("pvs.value: %.*s \n",8,pvs->value);
//    }

	retval = call_CreateAAS(serverIP,AAS_ID_STRING,AAS_ID_TYPE, AAS_NAME, ASSET_ID_STRING,AAS_ID_TYPE);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("AAS creation failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("AAS created \n");


//    retval = call_DeleteAAS(serverIP,AAS_ID_STRING,AAS_ID_TYPE);
//    return -1;
	retval = call_CreatePVSL(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME,CARRIER_ID_STRING,CARRIER_ID_TYPE);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("PVSL creation failed: %i \n",(int)retval);
	    return -1;
	}
	printf("PVSL created \n");


	retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME, RELATIONALEXPRESSION, EXPRESSIONSEMANTIC, VALUE, VALUETYPE, UNIT, PROPERTYREFERENCE_ID_STRING, PROPERTYREFERENCE_ID_TYPE, VIEW, 1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("PVS creation failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("PVS created \n");

	retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Property", 2, 2, "33", 1, "dm", "https://openaas.org/properties/Property", 1, 2, 1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("PVS creation failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("PVS created \n");

	retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Assembly", 1, 0, "900", 0, "m", "https://openaas.org/properties/Assembly", 2, 0, 0);
		if(retval!=UA_STATUSCODE_GOOD){
		    printf("PVS creation failed with: %i \n",(int)retval);
		    return -1;
		}
		printf("PVS created \n");

	retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, EVENTCLASS, SUBJECT, TIMESTAMP, VALUE2, VALUETYPE2);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("LCE creation failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("LCE created \n");

	retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "memory", "memoSubject", 33344, "85", 0);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("LCE creation failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("LCE created \n");

	retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "spot", "run", 77455774, "2213", 2);
		if(retval!=UA_STATUSCODE_GOOD){
		    printf("LCE creation failed with: %i \n",(int)retval);
		    return -1;
		}
		printf("LCE created \n");



	retval = call_DeletePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Assembly");
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("PVS deletion failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("PVS deleted \n");


	retval = call_SetPVS(serverIP, AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, "Property", 0, 3, "54", 0, "mm", "https://openaas.org/properties/Property", 1, 1, 1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("Setting PVS failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("PVS successfully set \n");


	retval = call_DeleteLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("LCE deletion failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("LCE deleted \n");


	retval = call_SetLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,2, CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, "movie", "actress", 464646, "644", 1);
	if(retval!=UA_STATUSCODE_GOOD){
	    printf("Setting LCE failed with: %i \n",(int)retval);
	    return -1;
	}
	printf("LCE successfully set \n");




	pvsType *pvs_c;
	int pvscount = getPVSFromListByName(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME,&pvs_c);
	for(int i=0;i<pvscount; i++){
	    printf("pvs view %i \n", pvs->view);
	 }
//    //Get-Methoden
//    int relExpress = 0;
//    int expressSem = 0;
//    char *getValue = "";
//    int getValueType = 0;
//    char *getUnit = "";
//    char *getPropRefSpec = "";
//    int getPropRefType = 0;
//    int getView = 0;
//	retval = call_GetPVS(serverIP, AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME, relExpress, expressSem, &getValue, getValueType, &getUnit, &getPropRefSpec, getPropRefType, getView);
//	if(retval!=UA_STATUSCODE_GOOD){
//	    printf("Getting PVS failed with: %i \n",(int)retval);
//	    return -1;
//	}
//	printf("Got PVS \n");
//
//
//	char *changeCreatingString = "";
//	int changeCreatingType = 0;
//	char *changeWritingString = "";
//	int changeWritingType = 0;
//	char *changeEventclass = "";
//	char *changeSubject = "";
//	UA_DateTime changeTimestamp = 0;
//	char *changeValue2 = "0";
//	int changeValuetype2 = 0;
//	retval = call_GetLCE (serverIP,AAS_ID_STRING,AAS_ID_TYPE,2, &changeCreatingString, &changeCreatingType, &changeWritingString, &changeWritingType, &changeEventclass, &changeSubject, &changeTimestamp, &changeValue2, &changeValuetype2);
//	if(retval!=UA_STATUSCODE_GOOD){
//	    printf("Getting LCE failed with: %i \n",(int)retval);
//	    return -1;
//	}
//	printf("Got LCE \n");


/*	AAS Create, Delete
retval = call_CreateAAS(serverIP,AAS_ID_STRING,AAS_ID_TYPE, AAS_NAME, ASSET_ID_STRING,AAS_ID_TYPE);
if(retval!=UA_STATUSCODE_GOOD){
    printf("AAS creation failed with: %i \n",(int)retval);
    return -1;
}
printf("AAS created \n");

retval = call_DeleteAAS(serverIP, AAS_ID_STRING, AAS_ID_TYPE);
if(retval!=UA_STATUSCODE_GOOD){
    printf("AAS deletion failed with: %i \n",(int)retval);
    return -1;
}
printf("AAS deleted \n");
*/

/*	PVSL Create, Delete
retval = call_CreatePVSL(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME,CARRIER_ID_STRING,CARRIER_ID_TYPE);
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVSL creation failed with: %i \n",(int)retval);
    return -1;
}
printf("PVSL created \n");

retval = call_DeletePVSL(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME);
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVSL deletion failed with: %i \n",(int)retval);
    return -1;
}
printf("PVSL deleted \n");
*/

//	PVS

/*	PVS Create, Delete
retval = call_CreatePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME, RELATIONALEXPRESSION, EXPRESSIONSEMANTIC, VALUE, VALUETYPE, UNIT, PROPERTYREFERENCE_ID_STRING, PROPERTYREFERENCE_ID_TYPE, VIEW);
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVS creation failed with: %i \n",(int)retval);
    return -1;
}
printf("PVS created \n");

retval = call_DeletePVS(serverIP,AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME);
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVS deletion failed with: %i \n",(int)retval);
    return -1;
}
printf("PVS deleted \n");
*/

/*	PVS Set, Get
retval = call_SetPVS(serverIP, AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME, RELATIONALEXPRESSION, EXPRESSIONSEMANTIC, VALUE, VALUETYPE, UNIT, PROPERTYREFERENCE_ID_STRING, PROPERTYREFERENCE_ID_TYPE, VIEW);
if(retval!=UA_STATUSCODE_GOOD){
    printf("Setting PVS failed with: %i \n",(int)retval);
    return -1;
}
printf("PVS successfully set \n");

retval = call_GetPVS(serverIP, AAS_ID_STRING,AAS_ID_TYPE,PVSL_NAME, PVS_NAME, RELATIONALEXPRESSION, EXPRESSIONSEMANTIC, &VALUE, VALUETYPE, &UNIT, &PROPERTYREFERENCE_ID_STRING, PROPERTYREFERENCE_ID_TYPE, VIEW);
if(retval!=UA_STATUSCODE_GOOD){
    printf("Getting PVS failed with: %i \n",(int)retval);
    return -1;
}
printf("Got PVS \n");
*/

//	LCE

/*	LCE Create, Delete
retval = call_CreateLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, EVENTCLASS, SUBJECT, TIMESTAMP, VALUE2, VALUETYPE2);
if(retval!=UA_STATUSCODE_GOOD){
    printf("LCE creation failed with: %i \n",(int)retval);
    return -1;
}
printf("LCE created \n");

retval = call_DeleteLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,LifeCycleEntrySize);
if(retval!=UA_STATUSCODE_GOOD){
    printf("LCE deletion failed with: %i \n",(int)retval);
    return -1;
}
printf("LCE deleted \n");
*/

/*	LCE Set, Get
retval = call_SetLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE,long long lceID, CREATINGINSTANCE_ID_STRING, CREATINGINSTANCE_ID_TYPE, WRITINGINSTANCE_ID_STRING, WRITINGINSTANCE_ID_TYPE, EVENTCLASS, SUBJECT, TIMESTAMP, VALUE2, VALUETYPE2);
if(retval!=UA_STATUSCODE_GOOD){
    printf("Setting LCE failed with: %i \n",(int)retval);
    return -1;
}
printf("LCE successfully set \n");

retval = call_GetLCE (serverIP,AAS_ID_STRING,AAS_ID_TYPE,long long lceID, &CREATINGINSTANCE_ID_STRING, &CREATINGINSTANCE_ID_TYPE, &WRITINGINSTANCE_ID_STRING, &WRITINGINSTANCE_ID_TYPE, &EVENTCLASS, &SUBJECT, &TIMESTAMP, &VALUE2, &VALUETYPE2);
if(retval!=UA_STATUSCODE_GOOD){
    printf("Getting LCE failed with: %i \n",(int)retval);
    return -1;
}
printf("Got LCE \n");




int a;
int b;
char *s = NULL;
retval = call_GetLCE(serverIP,AAS_ID_STRING,AAS_ID_TYPE, &a, &b, &s,);
if(retval!=UA_STATUSCODE_GOOD){
    printf("Getting LCE failed with: %i \n",(int)retval);
    return -1;
}
printf("Got LCE \n");
*/


return 0;
}

