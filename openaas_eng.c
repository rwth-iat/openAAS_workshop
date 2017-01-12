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

#define IP "opc.tcp://134.130.125.38:16664"

#define ASSET_ID_STRING "http://acplt.org/Sensor4711"
#define AAS_ID_TYPE 0
#define AAS_ID_STRING "http://acplt.org/Sensor4711_AAS"
#define AAS_ID_TYPE 0
#define AAS_NAME "Sensor4711"
#define PVSL_NAME "AssetProperties"

#define CARRIER_ID_STRING "http://acplt.org/Sensor4711"
#define CARRIER_ID_TYPE 0

int main(void) {
UA_StatusCode retval = UA_STATUSCODE_GOOD;
retval = call_CreateAAS(IP,AAS_ID_STRING,AAS_ID_TYPE,AAS_NAME,ASSET_ID_STRING,AAS_ID_TYPE);
if(retval!=UA_STATUSCODE_GOOD){
    printf("AAS creation failed with: %i \n",(int)retval);
    return -1;
}
printf("AAS created \n");
retval = call_CreatePVSL(IP,ASSET_ID_STRING,AAS_ID_TYPE,PVSL_NAME,CARRIER_ID_STRING,CARRIER_ID_TYPE);
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVSL creation failed with: %i \n",(int)retval);
    return -1;
}
printf("PVSL created \n");

retval = call_CreatePVS(IP,"abc",1, "abc", "durchmesser", 1, 1, "10",1, "mm");
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVS creation failed with: %i \n",(int)retval);
    return -1;
}
printf("PVS created \n");
return 0;
//    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
//    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://127.0.0.1:16664");
//    if(retval != UA_STATUSCODE_GOOD) {
//        UA_Client_delete(client);
//        return (int)retval;
//    }

//    UA_Identification id;
//    id.idType = UA_IDENUM_URI;
//    id.idSpec = UA_STRING("abc");
//    UA_String name = UA_STRING("v1");
//    call_CreateAAS(client,id,name,id);

//    size_t inArgSize = 3;
//    UA_Variant *input = UA_Array_new(inArgSize,&UA_TYPES[UA_TYPES_VARIANT]);
//    UA_Identification AASId;
//    AASId.idSpec = UA_STRING("abc");
//    AASId.idType = 1;
//    UA_String AASname  = UA_String_fromChars("openAASShell");
//    UA_Variant_setScalarCopy(&input[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&input[1], &AASname, &UA_TYPES[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&input[2], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_NodeId methNodeId = UA_NODEID_STRING(4,"/TechUnits/AASFolder/ModelmanagerOpenAAS||createAAS");
//    UA_NodeId objectId = UA_NODEID_STRING(4,"/TechUnits/AASFolder/ModelmanagerOpenAAS");

//    UA_StatusCode retvcal = UA_Client_call(client,objectId, methNodeId , 3, input, NULL,NULL);
    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     * wrapper for the raw read service available as UA_Client_Service_read. */


//    UA_Client_delete(client); /* Disconnects the client internally */
    return 0;
}
