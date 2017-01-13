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

#include "opcua_interface.h"

///* create an Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* AssetIdSpec, int AssetIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 3;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
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
    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &AASName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &assetId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_Variant *output;
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS||createAAS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    /*TODO evaluate Output (Status) */
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}
static UA_StatusCode parseToVariant( char* value,  int valueType,
        UA_Variant *variant) {
    char* tail = NULL;

    UA_Variant_init(variant);
    switch (valueType) {
    case 0: {
        UA_Int32 intVal = strtol(value, &tail, 10);
        UA_Variant_setScalar(variant, &intVal, &UA_TYPES[UA_TYPES_INT32]);

        break;
    }
    case 1: {
        UA_Double doubleVal = strtold(value, &tail);
        UA_Variant_setScalar(variant, &doubleVal, &UA_TYPES[UA_TYPES_DOUBLE]);
        break;
    }
    case 2: {
        UA_String strVal = UA_String_fromChars(value);
        UA_Variant_setScalar(variant, &strVal, &UA_TYPES[UA_TYPES_STRING]);
    }
        break;
    case 3: {
        //TODO datetime parsing
        UA_String strVal = UA_String_fromChars(value);
        UA_Variant_setScalar(variant, &strVal, &UA_TYPES[UA_TYPES_DATETIME]);
        break;
    }
    default:
        return UA_STATUSCODE_BADDATAENCODINGUNSUPPORTED;

    }
    return UA_STATUSCODE_GOOD;
}
/* create a Property value statement list */
UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* CarrierIdSpec, int CarrierIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 3;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);
    UA_Identification carrierId;
    carrierId.idType = CarrierIdType;
    carrierId.idSpec = UA_String_fromChars(CarrierIdSpec);
    UA_String listName = UA_String_fromChars(name);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &carrierId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS||createPVSL");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS");
    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

/* create a Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int RelationalExpression,
        int ExpressionSemantic, char* Value, int ValueType, char* Unit, char* propertyReferenceIdSpec, int propertyReferenceIdType, int view) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 9;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(PVSLName);
    UA_String propertyname = UA_String_fromChars(Name);
    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);

    UA_Identification propertyReference;
    propertyReference.idSpec = UA_String_fromChars(propertyReferenceIdSpec);
    propertyReference.idType = propertyReferenceIdType;
    parseToVariant(Value,ValueType,&dataValue.value);

    UA_String unitStr = UA_String_fromChars(Unit);

    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = UA_DateTime_now();
    dataValue.hasValue = true;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &propertyname,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[3], &RelationalExpression,
            &UA_OPENAAS[UA_OPENAAS_RELATIONALEXPRESSIONENUM]);
    UA_Variant_setScalarCopy(&inputArgs[4], &ExpressionSemantic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[5], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);
    UA_Variant_setScalarCopy(&inputArgs[6], &unitStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[7], &propertyReference,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[8], &view,
            &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);

    /*
    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &propertyname,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[3], &RelationalExpression,
            &UA_TYPES[UA_TYPES_INT32]);
    UA_Variant_setScalarCopy(&inputArgs[4], &ExpressionSemantic,
            &UA_TYPES[UA_TYPES_INT32]);
    UA_Variant_setScalarCopy(&inputArgs[5], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);
    UA_Variant_setScalarCopy(&inputArgs[6], &unitStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[7], &propertyReference,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[8], &view,
            &UA_TYPES[UA_TYPES_INT32]);
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS||createPVS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS");
*/

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS||createPVS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS");
    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

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

/* create a LifeCycle Entry */

UA_StatusCode call_CreateLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime TimeStamp, char* value,
        int valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 5;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification creatingInstance;
    creatingInstance.idSpec = UA_String_fromChars(creatingInstanceIdSpec);
    creatingInstance.idType = creatingInstanceIdType;

    UA_Identification writingInstance;
    writingInstance.idSpec = UA_String_fromChars(writingInstanceIdSpec);
    writingInstance.idType = writingInstanceIdType;
    UA_DataValue data;
    UA_DataValue_init(&data);
    parseToVariant(value, valueType, &data.value);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &creatingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[2], &writingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[3], &EventClass,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[4], &Subject,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[5], &data,
            &UA_TYPES[UA_TYPES_DATAVALUE]);
    UA_Variant_setScalarCopy(&inputArgs[6], &TimeStamp,
            &UA_TYPES[UA_TYPES_DATETIME]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS||createLCE");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/AASFolder/ModelmanagerOpenAAS");

    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

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

