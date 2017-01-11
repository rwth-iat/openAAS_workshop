#include "opcua_interface.h"

//#define AAS_FACTORY_NODEID UA_NODEID_STRING(4, "/TechUnits/AASFolder/ModelmanagerOpenAAS")
#define AAS_FACTORY_NODEID UA_NODEID_NUMERIC(3, 1005)

#define AAS_FACTORY_CREATEAAS_NODEID UA_NODEID_NUMERIC(3, 7001)
#define AAS_FACTORY_CREATELCE_NODEID UA_NODEID_NUMERIC(3, 7001)
#define AAS_FACTORY_CREATEPVS_NODEID UA_NODEID_NUMERIC(3, 7001)
#define AAS_FACTORY_CREATEPVSLIST_NODEID UA_NODEID_NUMERIC(3, 7001)

#define AAS_FACTORY_DELETEAAS_NODEID UA_NODEID_NUMERIC(3, 7001)
#define AAS_FACTORY_DELETELCE_NODEID UA_NODEID_NUMERIC(3, 7001)
#define AAS_FACTORY_DELETEPVS_NODEID UA_NODEID_NUMERIC(3, 7001)
#define AAS_FACTORY_DELETEPVSLIST_NODEID UA_NODEID_NUMERIC(3, 7001)


///* create an Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType, char* name, char* AssetIdSpec, int AssetIdType){
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
     }
     size_t argInSize = 3;
     size_t argOutSize;
     UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
     for(size_t i=0; i<argInSize; i++){
         UA_Variant_init(&inputArgs[i]);
     }
     /* convert input to UA types */
     UA_Identification AASId;
     AASId.idType = AASIdType;
     AASId.idSpec = UA_String_fromChars(AASIdSpec);
     UA_Identification assetId;
     assetId.idType = AssetIdType;
     assetId.idSpec = UA_String_fromChars(AssetIdSpec);
     UA_String AASName = UA_String_fromChars(name);
     UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
     UA_Variant_setScalarCopy(&inputArgs[1], &AASName, &UA_TYPES[UA_TYPES_STRING]);
     UA_Variant_setScalarCopy(&inputArgs[2], &assetId, &UA_TYPES[UA_OPENAAS_IDENTIFICATION]);

     UA_Variant *output;
     UA_NodeId methNodeId = UA_NODEID_STRING(4,"/TechUnits/AASFolder/ModelmanagerOpenAAS||createAAS");
     UA_NodeId objectId = UA_NODEID_STRING(4,"/TechUnits/AASFolder/ModelmanagerOpenAAS");
     retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,&argOutSize, &output);

     if(retval == UA_STATUSCODE_GOOD) {
         printf("Method call was successful, and %lu returned values available.\n",
                (unsigned long)argOutSize);
         UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
     } else {
         printf("Method call was unsuccessful, and %x returned values available.\n", retval);
     }
     /*TODO evaluate Output (Status) */
     UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
     return retval;
}
//
///* create an Asset administration shell */
//static UA_StatusCode call_CreateAAS(UA_Client *client, UA_Identification AASId, UA_String name, UA_Identification assetId){
//     size_t argInSize = 3;
//     size_t argOutSize;
//     UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//     for(size_t i=0; i<argInSize; i++){
//         UA_Variant_init(&inputArgs[i]);
//     }
//
//     UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//     UA_Variant_setScalarCopy(&inputArgs[1], &name, &UA_TYPES[UA_TYPES_STRING]);
//     UA_Variant_setScalarCopy(&inputArgs[2], &assetId, &UA_TYPES[UA_OPENAAS_IDENTIFICATION]);
//
//     UA_Variant *output;
//     UA_StatusCode retval = UA_Client_call(client, AAS_FACTORY_NODEID, AAS_FACTORY_CREATEAAS_NODEID, argInSize, inputArgs,&argOutSize, &output);
//
//
//     if(retval == UA_STATUSCODE_GOOD) {
//         printf("Method call was successful, and %lu returned values available.\n",
//                (unsigned long)argOutSize);
//         UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//     } else {
//         printf("Method call was unsuccessful, and %x returned values available.\n", retval);
//     }
//     /*TODO evaluate Output (Status) */
//     UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
//     return retval;
//}
//
///* delete an Asset administration shell */
//static UA_StatusCode call_DeleteAAS(UA_Client *client, UA_Identification AASId){
//     size_t argInSize = 1;
//     size_t argOutSize;
//     UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//     for(size_t i=0; i<argInSize; i++){
//         UA_Variant_init(&inputArgs[i]);
//     }
//
//     UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//
//     UA_Variant *output;
//     UA_StatusCode retval = UA_Client_call(client, AAS_FACTORY_NODEID, AAS_FACTORY_CREATEAAS_NODEID, argInSize, inputArgs, &argOutSize, &output);
//
//
//     if(retval == UA_STATUSCODE_GOOD) {
//         printf("Method call was successful, and %lu returned values available.\n",
//                (unsigned long)argOutSize);
//         UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//     } else {
//         printf("Method call was unsuccessful, and %x returned values available.\n", retval);
//     }
//     /*TODO evaluate Output (Status) */
//     UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
//     return retval;
//}
///* create a LifeCycle Entry */
//static UA_StatusCode call_CreateLCE(UA_Client *client, UA_Identification AASId, UA_Identification creatingInstance, UA_Identification writingInstance, UA_String EventClass, UA_String Subject, UA_DateTime TimeStamp){
//    size_t argInSize = 5;
//    size_t argOutSize;
//    UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    for(size_t i=0; i<argInSize; i++){
//        UA_Variant_init(&inputArgs[i]);
//    }
//
//    UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[1], &creatingInstance, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[2], &writingInstance, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[3], &EventClass, &UA_TYPES[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&inputArgs[4], &Subject, &UA_TYPES[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&inputArgs[5], &TimeStamp, &UA_TYPES[UA_OPENAAS_IDENTIFICATION]);
//
//    UA_Variant *output;
//    UA_StatusCode retval = UA_Client_call(client, AAS_FACTORY_NODEID, AAS_FACTORY_CREATELCE_NODEID, argInSize, inputArgs, &argOutSize, &output);
//
//
//    if(retval == UA_STATUSCODE_GOOD) {
//        printf("Method call was successful, and %lu returned values available.\n",
//               (unsigned long)argOutSize);
//        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    } else {
//        printf("Method call was unsuccessful, and %x returned values available.\n", retval);
//    }
//    UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
//    return retval;
//}
///* delete a LifeCycle Entry */
//static UA_StatusCode call_DeleteLCE(UA_Client *client, UA_Identification AASId, UA_UInt64 LCEId){
//    size_t argInSize = 5;
//    size_t argOutSize;
//    UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    for(size_t i=0; i<argInSize; i++){
//        UA_Variant_init(&inputArgs[i]);
//    }
//
//    UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[1], &LCEId, &UA_TYPES[UA_TYPES_UINT64]);
//    UA_Variant *output;
//    UA_StatusCode retval = UA_Client_call(client, AAS_FACTORY_NODEID, AAS_FACTORY_DELETELCE_NODEID, argInSize, inputArgs, &argOutSize, &output);
//
//
//    if(retval == UA_STATUSCODE_GOOD) {
//        printf("Method call was successful, and %lu returned values available.\n",
//               (unsigned long)argOutSize);
//        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    } else {
//        printf("Method call was unsuccessful, and %x returned values available.\n", retval);
//    }
//    UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
//    return retval;
//}
//
///* create a Property value statement list */
//static UA_StatusCode call_CreatePVSList(UA_Client *client, UA_Identification AASId, UA_String Name, UA_Identification Carrier){
//    size_t argInSize = 3;
//    size_t argOutSize;
//    UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    for(size_t i=0; i<argInSize; i++){
//        UA_Variant_init(&inputArgs[i]);
//    }
//
//    UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[1], &Name, &UA_TYPES[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&inputArgs[2], &Carrier, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//
//    UA_Variant *output;
//    UA_StatusCode retval = UA_Client_call(client, AAS_FACTORY_NODEID, AAS_FACTORY_CREATEPVSLIST_NODEID, argInSize, inputArgs, &argOutSize, &output);
//
//
//    if(retval == UA_STATUSCODE_GOOD) {
//        printf("Method call was successful, and %lu returned values available.\n",
//               (unsigned long)argOutSize);
//        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    } else {
//        printf("Method call was unsuccessful, and %x returned values available.\n", retval);
//    }
//    UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
//    return retval;
//}
//
///* create a Property value statement */
//static UA_StatusCode call_CreatePVS(UA_Client *client, UA_Identification AASId, UA_String PVSLName, UA_String Name, UA_RelationalExpressionEnum RelationalExpression, UA_ExpressionSemanticEnum ExpressionSemantic, UA_DataValue Value, UA_String Unit){
//    size_t argInSize = 7;
//    size_t argOutSize;
//    UA_Variant *inputArgs = UA_Array_new(argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    for(size_t i=0; i<argInSize; i++){
//        UA_Variant_init(&inputArgs[i]);
//    }
//
//    UA_Variant_setScalarCopy(&inputArgs[0], &AASId, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[1], &PVSLName, &UA_OPENAAS[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&inputArgs[2], &Name, &UA_TYPES[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&inputArgs[3], &RelationalExpression, &UA_OPENAAS[UA_OPENAAS_RELATIONALEXPRESSIONENUM]);
//    UA_Variant_setScalarCopy(&inputArgs[4], &ExpressionSemantic, &UA_TYPES[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
//    UA_Variant_setScalarCopy(&inputArgs[5], &Value, &UA_TYPES[UA_TYPES_DATAVALUE]);
//    UA_Variant_setScalarCopy(&inputArgs[6], &Unit,  &UA_TYPES[UA_TYPES_STRING]);
//
//    UA_Variant *output;
//    UA_StatusCode retval = UA_Client_call(client, AAS_FACTORY_NODEID, AAS_FACTORY_CREATEPVS_NODEID, argInSize, inputArgs, &argOutSize, &output);
//
//    if(retval == UA_STATUSCODE_GOOD) {
//        printf("Method call was successful, and %lu returned values available.\n",
//               (unsigned long)argOutSize);
//        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    } else {
//        printf("Method call was unsuccessful, and %x returned values available.\n", retval);
//    }
//    UA_Array_delete(inputArgs,argInSize,&UA_TYPES[UA_TYPES_VARIANT]);
//    return retval;
//}
//
//

