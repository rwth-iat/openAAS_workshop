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


int main(void) {
UA_StatusCode retval = call_CreateAAS("opc.tcp://134.130.125.38:16664","abc",1,"abc","abc",1);
if(retval!=UA_STATUSCODE_GOOD){
    printf("AAS creation failed with: %i \n",(int)retval);
    return -1;
}
printf("AAS created \n");
retval = call_CreatePVSL("opc.tcp://134.130.125.38:16664","abc",1,"abc","abcddd",1);
if(retval!=UA_STATUSCODE_GOOD){
    printf("PVSL creation failed with: %i \n",(int)retval);
    return -1;
}
printf("PVSL created \n");

retval = call_CreatePVS("opc.tcp://134.130.125.38:16664","abc",1, "abc", "durchmesser", 1, 1, "10",1, "mm");
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
