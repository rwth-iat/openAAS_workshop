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
#include "ua_openaas_generated.h"
#include "ua_openaas_generated_handling.h"
#include "ua_openaas_generated_encoding_binary.h"

#define  SET_CHAR_STR(SRC,DST) \
	*DST = (char*)malloc((sizeof(char)*(SRC.length+1)));\
    if(SRC.length>=MAX_STRING_SIZE-1 || DST == NULL) \
    	return -1; \
	memcpy(*DST,SRC.data,SRC.length); \
	(*DST)[SRC.length] = '\0';

static UA_StatusCode parseIdentification(UA_Identification *ident, char* str) {

    switch (ident->idType) {
    case UA_IDENUM_ISO:
        str[0] = 'I';
        str[1] = 'S';
        str[2] = 'O';
        str[3] = ':';
        break;
    case UA_IDENUM_URI:
        str[0] = 'U';
        str[1] = 'R';
        str[2] = 'I';
        str[3] = ':';
        break;
    }
    SET_CHAR_STR(ident->idSpec, &str)
    return 0;
}
static UA_StatusCode parseToVariant(char* value, int valueType,
        UA_Variant *variant) {
    char* tail = NULL;

    UA_Variant_init(variant);
    switch (valueType) {

    case 1: {
        UA_Boolean val;

        val = strtol(value, &tail, 10) != false ? true : false;
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_BOOLEAN]);

        break;
    }
    case 2: {
        UA_Float val;
        val = (UA_Float) strtold(value, &tail);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_FLOAT]);
        break;
    }
    case 3: {
        UA_Int32 val;
        val = strtol(value, &tail, 10);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_INT32]);

        break;
    }
    case 4: {
        UA_Int64 val;
        val = strtod(value, &tail);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_INT64]);
        break;
    }
    case 5: {
        UA_UInt32 val;
        val = strtol(value, &tail, 10); //TODO not safe
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    }
    case 6: {
        UA_UInt64 val;
        val = strtoll(value, &tail, 10);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_UINT64]);
        break;
    }
    case 7: {
        UA_Double val;
        val = strtold(value, &tail);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_DOUBLE]);
        break;
    }
    case 8: {
        UA_String val = UA_String_fromChars(value);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_STRING]);
        UA_String_deleteMembers(&val);
        break;
    }
    case 9: {
        UA_DateTime val;
        val = strtod(value, &tail);
        UA_Variant_setScalarCopy(variant, &val, &UA_TYPES[UA_TYPES_DATETIME]);
        break;
    }
    case 10: {
        UA_Identification val;
        if (value[0] == 'U' && value[1] == 'R' && value[2] == 'I'
                && value[3] == ':') {
            val.idType = UA_IDENUM_URI;
        } else if (value[0] == 'I' && value[1] == 'S' && value[2] == 'O'
                && value[3] == ':') {
            val.idType = UA_IDENUM_ISO;
        }else
            return UA_STATUSCODE_BADDATAENCODINGUNSUPPORTED;
        val.idSpec = UA_String_fromChars(&value[4]);
        UA_Variant_setScalarCopy(variant, &val,
                &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
        UA_Identification_deleteMembers(&val);
        break;
    }
    default:
        return UA_STATUSCODE_BADDATAENCODINGUNSUPPORTED;

    }
    return UA_STATUSCODE_GOOD;
}
static UA_StatusCode parseFromVariant(UA_Variant variant, char **value,
        int* valueType) {

    if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_BOOLEAN])) {
    	*value = malloc(sizeof(char)*12);
        sprintf(*value, "%d", *(UA_Boolean*) variant.data);
        *valueType = 1;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_FLOAT])) {
    	*value = malloc(sizeof(char)*32);
        sprintf(*value, "%f", *(UA_Float*) variant.data);
        *valueType = 2;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_INT32])) {
    	*value = malloc(sizeof(char)*12);
        sprintf(*value, "%d", *(UA_Int32*) variant.data);
        *valueType = 3;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_INT64])) {
    	*value = malloc(sizeof(char)*21);
        sprintf(*value, "%" PRId64 "", *(UA_Int64*) variant.data);
        *valueType = 4;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_UINT32])) {
        sprintf(*value, "%d", *(UA_UInt32*) variant.data);
        *valueType = 5;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_UINT64])) {
    	*value = malloc(sizeof(char)*32);
        sprintf(*value, "%" PRId64 "", *(UA_Int64*) variant.data);
        *valueType = 6;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_DOUBLE])) {
    	*value = malloc(sizeof(char)*32);
        sprintf(*value, "%f", *(UA_Double*) variant.data);
        *valueType = 7;
    } else if (UA_Variant_hasScalarType(&variant, &UA_TYPES[UA_TYPES_STRING])) {
        UA_String *tmpString = (UA_String*) variant.data;
        SET_CHAR_STR((*tmpString), value)
        *valueType = 8;
    } else if (UA_Variant_hasScalarType(&variant,
            &UA_TYPES[UA_TYPES_DATETIME])) {
        UA_String tmpDateTimeStr = UA_DateTime_toString(
                *((UA_DateTime*) variant.data));
        SET_CHAR_STR(tmpDateTimeStr, value)
        *valueType = 9;
        UA_String_deleteMembers(&tmpDateTimeStr);
    } else if (UA_Variant_hasScalarType(&variant,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION])) {
        UA_Identification* ident = (UA_Identification*) variant.data;
        parseIdentification(ident, *value);
        *valueType = 10;
    }
    return 0;
}

static UA_StatusCode getopenAASNamespaceIndex(UA_Client *client,UA_UInt16 *index){
	UA_Variant v;
	UA_Variant_init(&v);
	UA_String openAASURI = UA_String_fromChars("http://acplt.org/openaas/Ov");
	UA_Client_readValueAttribute(client,UA_NODEID_NUMERIC(0,2255),&v);
	if(UA_Variant_hasArrayType(&v,&UA_TYPES[UA_TYPES_STRING])){
		for(size_t i; i<v.arrayLength;i++){
			UA_String *nsArray = (UA_String*)v.data;
			if(UA_String_equal(&nsArray[i],&openAASURI)){
				*index = (UA_UInt16)i;
				UA_String_deleteMembers(&openAASURI);
				return UA_STATUSCODE_GOOD;
			}
		}
	}
	UA_String_deleteMembers(&openAASURI);
	return UA_STATUSCODE_BADNOTFOUND;
}

UA_StatusCode copyOPCUAStringToChar(UA_String src, char **dst) {
    if (src.data == NULL)
        return UA_STATUSCODE_GOOD;
    *dst = UA_malloc(sizeof(char) * (src.length + 1));
    if (*dst == NULL)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    memcpy(*dst, src.data, src.length);
    *(*dst + src.length) = '\0';
    return UA_STATUSCODE_GOOD;
}

/* Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* AssetIdSpec, int AssetIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    UA_UInt16 openAASnsIndex=0;
    getopenAASNamespaceIndex(client,&openAASnsIndex);
    printf("openAAS opc ua model has %i ns-index \n",openAASnsIndex);
    size_t argInSize = 3;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    printf("call_CreateAAS \n");
    printf("ipAddress: %s\n", AASIdSpec);
    printf("AASIdType: %i \n", AASIdType);
    printf("name: %s \n", name);
    printf("AssetIdSpec: %s \n", AssetIdSpec);
    printf("AssetIdType: %i \n", AssetIdType);

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

    UA_Variant *output = NULL;
    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createAAS");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    clean: UA_Identification_deleteMembers(&AASId);
    UA_String_deleteMembers(&AASName);

    UA_Identification_deleteMembers(&assetId);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    /*TODO evaluate Output (Status) */
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeleteAAS(char* ipAddress, char* AASIdSpec, int AASIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 1;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_Variant *output = NULL;
    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deleteAAS");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    clean:
    /*TODO evaluate Output (Status) */
    UA_Identification_deleteMembers(&AASId);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}


/* SubModel */
UA_StatusCode call_CreateSubModel(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* ParentIdSpec, int ParentIdType, char* ModelIdSpec, int ModelIdType,
        char* name, int revision, int version) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 6;
    size_t argOutSize = 0;
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);
    UA_Identification parentId;
    parentId.idType = ParentIdType;
    parentId.idSpec = UA_String_fromChars(ParentIdSpec);
    UA_Identification modelId;
    modelId.idType = ModelIdType;
    modelId.idSpec = UA_String_fromChars(ModelIdSpec);
    UA_String ModelName = UA_String_fromChars(name);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &parentId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[2], &modelId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[3], &ModelName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[4], &revision,
			&UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&inputArgs[5], &version,
		   &UA_TYPES[UA_TYPES_UINT32]);

    UA_Variant *output = NULL;
    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createSubModel");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    clean: UA_Identification_deleteMembers(&AASId);
    UA_Identification_deleteMembers(&parentId);
    UA_Identification_deleteMembers(&modelId);
    UA_String_deleteMembers(&ModelName);

    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    /*TODO evaluate Output (Status) */
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeleteSubModel(char* ipAddress, char* AASIdSpec, int AASIdType, char* SMIdSpec, int SMIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);
    UA_Identification SMId;
    SMId.idType = SMIdType;
    SMId.idSpec = UA_String_fromChars(SMIdSpec);


    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &SMId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_Variant *output = NULL;
    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deleteSubModel");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    clean:
    /*TODO evaluate Output (Status) */
    UA_Identification_deleteMembers(&AASId);
    UA_Identification_deleteMembers(&SMId);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

/* Property value statement list */
UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* ParentIdSpec, int ParentIdType, char* PVSLName, int mask,
		char* CarrierIdSpec, int CarrierIdType, int ExpressionLogic, int ExpressionSemantic,
		char* propertyIdSpec, int propertyIdType, int view, int visibility) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    size_t argInSize = 10;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);
    UA_Identification ParentId;
    ParentId.idType = ParentIdType;
    ParentId.idSpec = UA_String_fromChars(ParentIdSpec);

    UA_String listName = UA_String_fromChars(PVSLName);

    UA_Identification carrierId;
    carrierId.idType = CarrierIdType;
    carrierId.idSpec = UA_String_fromChars(CarrierIdSpec);

    UA_Identification propertyId;
    propertyId.idType = propertyIdType;
    propertyId.idSpec = UA_String_fromChars(propertyIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &ParentId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[2], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[3], &mask,
			&UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&inputArgs[4], &carrierId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[5], &ExpressionLogic,
			&UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM]);
	UA_Variant_setScalarCopy(&inputArgs[6], &ExpressionSemantic,
			&UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[7], &propertyId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[8], &view,
			&UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
    UA_Variant_setScalarCopy(&inputArgs[9], &visibility,
			&UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createPVSL");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }

    clean: UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);

    UA_Identification_deleteMembers(&AASId);
    UA_Identification_deleteMembers(&ParentId);
    UA_String_deleteMembers(&listName);
    UA_Identification_deleteMembers(&carrierId);
    UA_Identification_deleteMembers(&propertyId);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeletePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSLIdSpec, int PVSLIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification PVSLId;
    PVSLId.idType = PVSLIdType;
    PVSLId.idSpec = UA_String_fromChars(PVSLIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &PVSLId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deletePVSL");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    clean: UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Identification_deleteMembers(&AASId);
	UA_Identification_deleteMembers(&PVSLId);
    UA_Client_delete(client);
    return retval;
}

/* Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* ListIdSpec, int ListIdType, char* PVSName, char* Value, int ValueType,
		int mask, char* CarrierIdSpec, int CarrierIdType, int ExpressionLogic,
		int ExpressionSemantic,	char* propertyIdSpec, int propertyIdType,
		int view, int visibility) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    size_t argInSize = 11;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification ListID;
    ListID.idType = ListIdType;
    ListID.idSpec = UA_String_fromChars(ListIdSpec);

    UA_String pvsname = UA_String_fromChars(PVSName);

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    parseToVariant(Value, ValueType, &dataValue.value);

    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = UA_DateTime_now();
    dataValue.hasValue = true;

    UA_Identification carrierId;
    carrierId.idSpec = UA_String_fromChars(CarrierIdSpec);
    carrierId.idType = CarrierIdType;

    UA_Identification propertyId;
    propertyId.idSpec = UA_String_fromChars(propertyIdSpec);
    propertyId.idType = propertyIdType;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &ListID,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[2], &pvsname,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[3], &dataValue,
			&UA_TYPES[UA_TYPES_DATAVALUE]);
    UA_Variant_setScalarCopy(&inputArgs[4], &mask,
			&UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&inputArgs[5], &carrierId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[6], &ExpressionLogic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[7], &ExpressionSemantic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[8], &propertyId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[9], &view,
            &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
    UA_Variant_setScalarCopy(&inputArgs[10], &visibility,
			&UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createPVS");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;

    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    clean: UA_DataValue_deleteMembers(&dataValue);
    UA_Identification_deleteMembers(&AASId);
    UA_Identification_deleteMembers(&ListID);
    UA_Identification_deleteMembers(&carrierId);
    UA_Identification_deleteMembers(&propertyId);
    UA_String_deleteMembers(&pvsname);

    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeletePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification PVSId;
    PVSId.idType = PVSIdType;
    PVSId.idSpec = UA_String_fromChars(PVSIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &PVSId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deletePVS");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;

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
    UA_Identification_deleteMembers(&AASId);
    UA_Identification_deleteMembers(&PVSId);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_SetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType, int mask, char* PVSName,
		char* CarrierIdSpec, int CarrierIdType, int ExpressionLogic,
		int ExpressionSemantic,	char* propertyIdSpec, int propertyIdType,
		int view, int visibility, char* Value, int ValueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 11;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification PVSId;
    PVSId.idType = PVSIdType;
    PVSId.idSpec = UA_String_fromChars(PVSIdSpec);

    UA_String pvsName = UA_String_fromChars(PVSName);

    UA_Identification carrierId;
    carrierId.idSpec = UA_String_fromChars(CarrierIdSpec);
    carrierId.idType = CarrierIdType;

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    retval = parseToVariant(Value, ValueType, &dataValue.value);
    if (retval != UA_STATUSCODE_GOOD) {
        return retval;
    }
    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = UA_DateTime_now();
    dataValue.hasValue = true;

    UA_Identification propertyId;
    propertyId.idSpec = UA_String_fromChars(propertyIdSpec);
    propertyId.idType = propertyIdType;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &PVSId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[2], &mask,
			&UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&inputArgs[3], &pvsName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[4], &carrierId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[5], &ExpressionLogic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[6], &ExpressionSemantic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[7], &propertyId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[8], &view,
			&UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
    UA_Variant_setScalarCopy(&inputArgs[9], &visibility,
			&UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);
    UA_Variant_setScalarCopy(&inputArgs[10], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||setPVS");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;

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
    clean: UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_DataValue_deleteMembers(&dataValue);
	UA_Identification_deleteMembers(&AASId);
	UA_Identification_deleteMembers(&PVSId);
	UA_Identification_deleteMembers(&carrierId);
	UA_Identification_deleteMembers(&propertyId);
	UA_String_deleteMembers(&pvsName);
    UA_Client_delete(client);
    return retval;
}
// get the node id of a specific child node by its browse name
static void getNodeIdOfChildByBrowseName(UA_Client *client, UA_NodeId startId,
        UA_String browseNameToFind, UA_NodeId *foundId) {

    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;
    UA_NodeId_copy(&startId, &bReq.nodesToBrowse[0].nodeId);

    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    for (size_t i = 0; i < bResp.resultsSize; i++) {
        for (size_t j = 0; j < bResp.results[i].referencesSize; j++) {
            if (UA_String_equal(&browseNameToFind,
                    &bResp.results[i].references[j].displayName.text)) {
                UA_NodeId_copy(&bResp.results[i].references[j].nodeId.nodeId,
                        foundId);
                goto clean;
            }
        }
    }

    UA_NodeId tmpNodeId = UA_NODEID_NUMERIC(0, 0);
    UA_NodeId_copy(&tmpNodeId, foundId);
    clean: UA_BrowseRequest_deleteMembers(&bReq);
    UA_BrowseResponse_deleteMembers(&bResp);
    return;
}

static void FindChildrenWithSpecialType(UA_Client *client, UA_NodeId startId,
        UA_String TypedefinitionBrowseName, UA_ReferenceDescription **ids,
        size_t *idsSize) {
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;

    UA_NodeId_copy(&startId, &bReq.nodesToBrowse[0].nodeId);
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
    size_t n = 0;
    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    if (bResp.resultsSize == 0) {
        *ids = NULL;
        *idsSize = 0;
        goto clean;
    }
    for (size_t i = 0; i < bResp.results[0].referencesSize; i++) {
        UA_BrowseRequest bReq1;
        UA_BrowseRequest_init(&bReq1);
        bReq1.requestedMaxReferencesPerNode = 0;
        bReq1.nodesToBrowse = UA_BrowseDescription_new();
        bReq1.nodesToBrowseSize = 1;
        UA_NodeId_copy(&bResp.results[0].references[i].nodeId.nodeId,
                &bReq1.nodesToBrowse[0].nodeId); //could be a string node id

        bReq1.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
        bReq1.nodesToBrowse[0].referenceTypeId = UA_NODEID_NUMERIC(0,
                UA_NS0ID_HASTYPEDEFINITION);
        UA_BrowseResponse bResp1 = UA_Client_Service_browse(client, bReq1);
        for (size_t j = 0; j < bResp1.results[0].referencesSize; j++) {
        	if (UA_String_equal(
					&bResp1.results[0].references[j].displayName.text,
					&TypedefinitionBrowseName)) {
				n++;
        	}
        }
        UA_BrowseRequest_deleteMembers(&bReq1);
        UA_BrowseResponse_deleteMembers(&bResp1);
    }
    *idsSize = n;
    if (!n)
        goto clean;
    size_t m = 0;
    *ids = UA_Array_new(n, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    for (size_t i = 0; i < n; i++) {
        UA_ReferenceDescription_init(&(*ids)[i]);
    }
    if (!*ids)
        goto clean;

    for (size_t i = 0; i < bResp.results[0].referencesSize; i++) {
        UA_BrowseRequest bReq1;
        UA_BrowseRequest_init(&bReq1);
        bReq1.requestedMaxReferencesPerNode = 0;
        bReq1.nodesToBrowse = UA_BrowseDescription_new();
        bReq1.nodesToBrowseSize = 1;
        UA_NodeId_copy(&bResp.results[0].references[i].nodeId.nodeId,
                &bReq1.nodesToBrowse[0].nodeId); //could be a string node id

        bReq1.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
        bReq1.nodesToBrowse[0].referenceTypeId = UA_NODEID_NUMERIC(0,
                UA_NS0ID_HASTYPEDEFINITION);
        UA_BrowseResponse bResp1 = UA_Client_Service_browse(client, bReq1);
        for (size_t j = 0; j < bResp1.results[0].referencesSize; j++) {
        	if (UA_String_equal(
					&bResp1.results[0].references[j].displayName.text,
					&TypedefinitionBrowseName)) {
				UA_ReferenceDescription_copy(&bResp.results[0].references[i],
						&((*ids)[m]));
				m++;
        	}
        }
        UA_BrowseRequest_deleteMembers(&bReq1);
        UA_BrowseResponse_deleteMembers(&bResp1);
        if (m >= n) {
            goto clean;
        }
    }
    clean: UA_BrowseRequest_deleteMembers(&bReq);
    UA_BrowseResponse_deleteMembers(&bResp);
    return;
}


static int getAllPVSFromList(UA_Client *client, UA_NodeId entryPoint,
        UA_ReferenceDescription** foundListIds, size_t *foundListIdsSize) {
	/*all property value statements are attached via HasComponent Reference */
    UA_String tmpStr = UA_String_fromChars("PropertyValueStatementType");
    UA_ReferenceDescription *subNodes = NULL;

    // get all Children that have HasProperty Reference
    size_t subNodesSize = 0;
    FindChildrenWithSpecialType(client, entryPoint, tmpStr, &subNodes,
            &subNodesSize);

    //filter out only nodes that have the datatype "PropertyValueStatement"
    size_t n = 0;
    for (size_t i = 0; i < subNodesSize; i++) {
        UA_NodeId dataTypeNodeId;
        if (UA_STATUSCODE_GOOD
                == UA_Client_readDataTypeAttribute(client,
                        subNodes[i].nodeId.nodeId, &dataTypeNodeId)) {
            if (UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT].typeId.identifier.numeric
                    == dataTypeNodeId.identifier.numeric) { //ignore namespace index
                n++;
            }
        }
    }

    *foundListIds = UA_Array_new(n, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    if (!foundListIds) {
        *foundListIdsSize = 0;
        return -1;
    }

    *foundListIdsSize = n;
    n = 0;
    for (size_t i = 0; i < subNodesSize; i++) {
        UA_NodeId dataTypeNodeId;
        if (UA_STATUSCODE_GOOD
                == UA_Client_readDataTypeAttribute(client,
                        subNodes[i].nodeId.nodeId, &dataTypeNodeId)) {
            if (UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT].typeId.identifier.numeric
                    == dataTypeNodeId.identifier.numeric) { //ignore namespace index
                if (n < *foundListIdsSize) {
                    //UA_ReferenceDescription *pref;
                    //UA_ReferenceDescription_new(pref);

                    UA_ReferenceDescription_copy(&subNodes[i],
                            &(*foundListIds)[n]);
                    n++;
                } else
                    return subNodesSize;
            }
        }
    }
    UA_Array_delete(subNodes, subNodesSize,
            &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    UA_String_deleteMembers(&tmpStr);
    return 0;
}

UA_Boolean findAASNodeId(UA_Client *client, UA_Identification AASId,
        UA_NodeId* nodeId) {
//dummy
    UA_Boolean found = false;

    size_t argInSize = 1;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getAASNodeId");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;

    UA_StatusCode retval = UA_Client_call(client, objectId, methNodeId,
            argInSize, inputArgs, &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
    } else {
        found = false;
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }
    if (!UA_Variant_hasScalarType(output, &UA_TYPES[UA_TYPES_NODEID])) {
        found = false;
        goto clean;
    }
    UA_NodeId_copy((UA_NodeId *) output->data, nodeId);

    found = true;
    clean: UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return found;
}

int getPVSFromListByName(char* ipAddress, char*AASIdSpec, int AASIdType,
		char*ListIdSpec, int ListIdType, pvsType **pvs_c) {

    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    UA_Identification AASIdentification;
    AASIdentification.idSpec = UA_String_fromChars(AASIdSpec);
    AASIdentification.idType = AASIdType;

    /* for message transmission - NOT IMPLEMENTED
    UA_NodeId AASNodeId;
    findAASNodeId(client, AASIdentification, &AASNodeId);
    UA_Identification_deleteMembers(&AASIdentification);
    */
    UA_NodeId idPVSL = UA_NODEID_STRING_ALLOC(openAASnsIndex, ListIdSpec);
	UA_ReferenceDescription *foundStatementIds = NULL;
	size_t foundStatementIdsSize = 0;

	getAllPVSFromList(client, idPVSL, &foundStatementIds,
			&foundStatementIdsSize);

	for (size_t i = 0; i < foundStatementIdsSize; i++) {
		switch (foundStatementIds[i].nodeId.nodeId.identifierType) {
		case UA_NODEIDTYPE_NUMERIC:
			printf("Found statement with Id: %i \n",
					foundStatementIds[i].nodeId.nodeId.identifier.numeric);
			break;
		case UA_NODEIDTYPE_STRING:
			printf("Found statement with Id: %.*s \n",
					foundStatementIds[i].nodeId.nodeId.identifier.string.length,
					foundStatementIds[i].nodeId.nodeId.identifier.string.data);
			break;
		}
	}

    *pvs_c = calloc(foundStatementIdsSize, sizeof(pvsType));

    for (size_t i = 0; i < foundStatementIdsSize; i++) {
    	char* PVSIdSpec = NULL;
    	copyOPCUAStringToChar(foundStatementIds[i].nodeId.nodeId.identifier.string, &PVSIdSpec);
    	(*pvs_c)[i].carrierIdIdSpec = NULL;
    	(*pvs_c)[i].name = NULL;
    	(*pvs_c)[i].propertyIdIdSpec = NULL;
    	(*pvs_c)[i].value = NULL;
    	call_GetPVS(ipAddress, AASIdSpec, AASIdType, PVSIdSpec, 0, &((*pvs_c)[i].name), &((*pvs_c)[i].carrierIdIdSpec), &((*pvs_c)[i].carrierIdIdType), &((*pvs_c)[i].expressionLogic), &((*pvs_c)[i].expressionSemantic), &((*pvs_c)[i].propertyIdIdSpec), &((*pvs_c)[i].propertyIdIdType), &((*pvs_c)[i].view), &((*pvs_c)[i].visibility), &((*pvs_c)[i].value), &((*pvs_c)[i].valueType));
    	UA_free(PVSIdSpec);
    }

    UA_Array_delete(foundStatementIds, foundStatementIdsSize,
            &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    UA_NodeId_deleteMembers(&idPVSL);
    UA_Client_delete(client);
    return foundStatementIdsSize;

}
UA_StatusCode call_GetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
		char* PVSIdSpec, int PVSIdType, char** PVSName,
		char** CarrierIdSpec, int* CarrierIdType, int* ExpressionLogic,
		int* ExpressionSemantic, char** propertyIdSpec, int* propertyIdType,
		int* view, int* visibility, char** Value, int* ValueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification PVSId;
    PVSId.idType = PVSIdType;
    PVSId.idSpec = UA_String_fromChars(PVSIdSpec);


    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &PVSId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getPVS");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output = NULL;

    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

        UA_StatusCode *status = (UA_StatusCode*) output[0].data;
		retval = *status;

		UA_String *pvsName = (UA_String*) output[1].data;
		copyOPCUAStringToChar(*pvsName, PVSName);

		UA_Identification carrierId;
		UA_Identification_init(&carrierId);
		UA_ExtensionObject *ext = (UA_ExtensionObject*) output[2].data;
		size_t offset = 0;
		UA_Identification_decodeBinary(&(ext->content.encoded.body), &offset,
				&carrierId);
		copyOPCUAStringToChar(carrierId.idSpec, CarrierIdSpec);
		*CarrierIdType = carrierId.idType;
		UA_Identification_deleteMembers(&carrierId);

        UA_ExpressionLogicEnum* UAExpressionLogic =
                (UA_ExpressionLogicEnum*) output[3].data;
        *ExpressionLogic = *UAExpressionLogic;

        UA_ExpressionSemanticEnum* UAExpressionSemantic =
                (UA_ExpressionSemanticEnum*) output[4].data;
        *ExpressionSemantic = *UAExpressionSemantic;

        UA_Identification propertyId;
		UA_Identification_init(&propertyId);
		ext = (UA_ExtensionObject*) output[5].data;
		offset = 0;
		UA_Identification_decodeBinary(&(ext->content.encoded.body), &offset,
				&propertyId);
		copyOPCUAStringToChar(propertyId.idSpec, propertyIdSpec);
		*propertyIdType = propertyId.idType;
		UA_Identification_deleteMembers(&propertyId);

		UA_ViewEnum* UAView = (UA_ViewEnum*) output[6].data;
		*view = *UAView;

		UA_VisibilityEnum* UAVisibility = (UA_VisibilityEnum*) output[7].data;
		*visibility = *UAVisibility;

        UA_DataValue *dataValue = (UA_DataValue*) output[8].data;
        parseFromVariant(dataValue->value, Value, ValueType);

        //UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

/* LifeCycle Entry */
UA_StatusCode call_CreateLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime timeStamp, char* value,
        int valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }

    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;

    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_LifeCycleEntry lce;

    lce.creatingInstance.idSpec = UA_String_fromChars(creatingInstanceIdSpec);
    lce.creatingInstance.idType = creatingInstanceIdType;

    lce.writingInstance.idSpec = UA_String_fromChars(writingInstanceIdSpec);
    lce.writingInstance.idType = writingInstanceIdType;

    lce.eventClass = UA_String_fromChars(EventClass);

    lce.subject = UA_String_fromChars(Subject);

    retval = parseToVariant(value, valueType, &lce.data.value);
    if(retval != UA_STATUSCODE_GOOD){
        return retval;
    }
    lce.data.hasSourceTimestamp = true;
    lce.data.sourceTimestamp = timeStamp;
    lce.data.hasValue = true;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lce,
            &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createLCE");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        goto clean;
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    clean:
    UA_Client_delete(client);
    UA_Identification_deleteMembers(&AASId);
    UA_LifeCycleEntry_deleteMembers(&lce);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_DeleteLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long LCEId) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_UInt64 lceId = LCEId;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deleteLCE");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        goto clean;
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    clean: UA_Identification_deleteMembers(&AASId);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_SetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, lifeCycleEntryType lceData){
	UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
	UA_StatusCode retval = UA_Client_connect(client, ipAddress);
	if (retval != UA_STATUSCODE_GOOD) {
		UA_Client_delete(client);
		return (int) retval;
	}
	size_t argInSize = 3;
	size_t argOutSize = 0;
	UA_Variant *inputArgs = UA_Array_new(argInSize,
			&UA_TYPES[UA_TYPES_VARIANT]);
	for (size_t i = 0; i < argInSize; i++) {
		UA_Variant_init(&inputArgs[i]);
	}
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
	/* convert input to UA types */
	UA_Identification AASId;
	AASId.idType = AASIdType;
	AASId.idSpec = UA_String_fromChars(AASIdSpec);

	UA_LifeCycleEntry tmplceData;
	UA_LifeCycleEntry_init(&tmplceData);

	tmplceData.creatingInstance.idSpec = UA_String_fromChars(lceData.creatingInstanceSpec);
	tmplceData.creatingInstance.idType = lceData.creatingInstanceType;

	UA_Identification writingInstance;
	tmplceData.writingInstance.idSpec = UA_String_fromChars(lceData.writingInstanceSpec);
	tmplceData.writingInstance.idType = lceData.writingInstanceType;

	tmplceData.eventClass = UA_String_fromChars(lceData.eventClass);

	tmplceData.subject = UA_String_fromChars(lceData.subject);

	parseToVariant(lceData.data, lceData.dataType, &tmplceData.data.value);
	tmplceData.data.hasSourceTimestamp = true;
	tmplceData.data.sourceTimestamp = lceData.timestamp;
	tmplceData.data.hasValue = true;

	UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
			&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
	UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);
	UA_Variant_setScalarCopy(&inputArgs[2], &tmplceData,
			&UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);

	UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
			"/TechUnits/openAAS/ModelmanagerOpenAAS||setLCE");
	UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
			"/TechUnits/openAAS/ModelmanagerOpenAAS");

	UA_Variant *output = NULL;
	retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
			&argOutSize, &output);

	if (retval == UA_STATUSCODE_GOOD) {
		printf(
				"Method call was successful, and %lu returned values available.\n",
				(unsigned long) argOutSize);
		goto clean;
	} else {
		printf(
				"Method call was unsuccessful, and %x returned values available.\n",
				retval);
	}

	clean:
	UA_Identification_deleteMembers(&AASId);
	UA_LifeCycleEntry_deleteMembers(&tmplceData);
		UA_Client_delete(client);
	return retval;
}

UA_StatusCode call_SetLCESimple(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char* creatingInstanceIdSpec,
        int creatingInstanceIdType, char* writingInstanceIdSpec,
        int writingInstanceIdType, char* EventClass, char* Subject,
        UA_DateTime timeStamp, char* value, int valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 7;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
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

    UA_String eventClassStr = UA_String_fromChars(EventClass);

    UA_String subjectStr = UA_String_fromChars(Subject);

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    parseToVariant(value, valueType, &dataValue.value);
    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = timeStamp;
    dataValue.hasValue = true;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);
    UA_Variant_setScalarCopy(&inputArgs[2], &creatingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[3], &writingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[4], &eventClassStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[5], &subjectStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[6], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||setLCESimple");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        goto clean;
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    clean: UA_DataValue_deleteMembers(&dataValue);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);

    UA_Identification_deleteMembers(&AASId);
    UA_Identification_deleteMembers(&creatingInstance);
    UA_Identification_deleteMembers(&writingInstance);

    UA_String_deleteMembers(&eventClassStr);
    UA_String_deleteMembers(&subjectStr);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_GetLCESimple(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char** creatingInstanceIdSpec,
        int* creatingInstanceIdType, char** writingInstanceIdSpec,
        int* writingInstanceIdType, char** EventClass, char** Subject,
        UA_DateTime* timeStamp, char** value, int* valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getLCESimple");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

        UA_StatusCode *status = (UA_StatusCode*) output[0].data;
        retval = *status;


        UA_Identification creatingInstance;
		UA_Identification_init(&creatingInstance);
        UA_ExtensionObject *ext = (UA_ExtensionObject*) output[1].data;
		size_t offset = 0;
		UA_Identification_decodeBinary(&(ext->content.encoded.body), &offset,
				&creatingInstance);
		copyOPCUAStringToChar(creatingInstance.idSpec, creatingInstanceIdSpec);
		*creatingInstanceIdType = creatingInstance.idType;
		UA_Identification_deleteMembers(&creatingInstance);

		UA_Identification writingInstance;
		UA_Identification_init(&writingInstance);
		ext = (UA_ExtensionObject*) output[2].data;
		offset = 0;
		UA_Identification_decodeBinary(&(ext->content.encoded.body), &offset,
				&writingInstance);
		copyOPCUAStringToChar(writingInstance.idSpec, writingInstanceIdSpec);
		*writingInstanceIdType = writingInstance.idType;
		UA_Identification_deleteMembers(&writingInstance);

        UA_String *eventClassStr = (UA_String*) output[3].data;
        copyOPCUAStringToChar(*eventClassStr, EventClass);

        UA_String *subjectStr = (UA_String*) output[4].data;
        copyOPCUAStringToChar(*subjectStr, Subject);

        UA_DataValue *dataValue = (UA_DataValue*) output[5].data;
        *timeStamp = dataValue->sourceTimestamp;
        parseFromVariant(dataValue->value, value, valueType);

        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Client_delete(client);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_GetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, lifeCycleEntryType* lceData) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getLCE");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);


        UA_StatusCode *status = (UA_StatusCode*) output[0].data;
        retval = *status;

        UA_LifeCycleEntry tmplceData;
        UA_LifeCycleEntry_init(&tmplceData);
        UA_ExtensionObject *ext = (UA_ExtensionObject*) output[1].data;
		size_t offset = 0;
		UA_LifeCycleEntry_decodeBinary(&(ext->content.encoded.body), &offset,
				&tmplceData);

		copyOPCUAStringToChar(tmplceData.creatingInstance.idSpec, &lceData->creatingInstanceSpec);
		lceData->creatingInstanceType = tmplceData.creatingInstance.idType;

		copyOPCUAStringToChar(tmplceData.writingInstance.idSpec, &lceData->writingInstanceSpec);
		lceData->writingInstanceType = tmplceData.writingInstance.idType;

		copyOPCUAStringToChar(tmplceData.eventClass, &lceData->eventClass);

		copyOPCUAStringToChar(tmplceData.subject, &lceData->subject);

		lceData->timestamp = tmplceData.data.sourceTimestamp;

		parseFromVariant(tmplceData.data.value, &lceData->data, &lceData->dataType);

		UA_LifeCycleEntry_deleteMembers(&tmplceData);

        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Client_delete(client);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_GetLastLCEs(char* ipAddress, char* AASIdSpec, int AASIdType,
        unsigned int count, unsigned int* lastLCEsCount, lifeCycleEntryType **lifeCycleEntries) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize = 0;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    UA_UInt16 openAASnsIndex=0;
    if(getopenAASNamespaceIndex(client,&openAASnsIndex)!=UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADNOTFOUND;
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &count, &UA_TYPES[UA_TYPES_UINT32]);

    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getLastLCEs");
    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output = NULL;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        goto clean;
    }

    *lastLCEsCount = output[1].arrayLength;
    *lifeCycleEntries = calloc(*lastLCEsCount, sizeof(lifeCycleEntryType));

    if (!UA_Variant_hasArrayType(&output[1],
            &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]))
        return -1;

    for (size_t i = 0; i < *lastLCEsCount; i++) {
        UA_LifeCycleEntry lce;
        UA_LifeCycleEntry_init(&lce);
        UA_ExtensionObject ext = ((UA_ExtensionObject*) output[1].data)[i];
        size_t offset = 0;
        UA_LifeCycleEntry_decodeBinary(&ext.content.encoded.body, &offset,
                &lce);

        (*lifeCycleEntries)[i].creatingInstanceSpec = NULL;
        copyOPCUAStringToChar(lce.creatingInstance.idSpec, &(*lifeCycleEntries)[i].creatingInstanceSpec);
        (*lifeCycleEntries)[i].creatingInstanceType =
                lce.creatingInstance.idType;

        (*lifeCycleEntries)[i].eventClass = NULL;
        copyOPCUAStringToChar(lce.eventClass, &(*lifeCycleEntries)[i].eventClass);
        (*lifeCycleEntries)[i].subject = NULL;
        copyOPCUAStringToChar(lce.subject, &(*lifeCycleEntries)[i].subject);

        (*lifeCycleEntries)[i].writingInstanceSpec = NULL;
        copyOPCUAStringToChar(lce.writingInstance.idSpec, &(*lifeCycleEntries)[i].writingInstanceSpec);
        (*lifeCycleEntries)[i].writingInstanceType = lce.writingInstance.idType;

        (*lifeCycleEntries)[i].data = NULL;
        parseFromVariant(lce.data.value, &(*lifeCycleEntries)[i].data,
                &(*lifeCycleEntries)[i].dataType);
        (*lifeCycleEntries)[i].id = lce.id;

        (*lifeCycleEntries)[i].timestamp = lce.data.sourceTimestamp;

        UA_LifeCycleEntry_deleteMembers(&lce);
    }

    clean: UA_Identification_deleteMembers(&AASId);
    UA_Client_delete(client);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

//
//UA_StatusCode call_triggerGetCoreData(char* ipAddress, char* srcAASIdSpec,
//        int srcAASIdType, char* dstAASIdSpec, int dstAASIdType) {
//    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
//    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
//    if (retval != UA_STATUSCODE_GOOD) {
//        UA_Client_delete(client);
//        return (int) retval;
//    }
//    size_t argInSize = 2;
//    size_t argOutSize = 0;
//    UA_Variant *inputArgs = UA_Array_new(argInSize,
//            &UA_TYPES[UA_TYPES_VARIANT]);
//    for (size_t i = 0; i < argInSize; i++) {
//        UA_Variant_init(&inputArgs[i]);
//    }
//    /* convert input to UA types */
//    UA_Identification srcAASId;
//    UA_Identification dstAASId;
//
//    srcAASId.idType = srcAASIdType;
//    srcAASId.idSpec = UA_String_fromChars(srcAASIdSpec);
//
//    dstAASId.idType = dstAASIdType;
//    dstAASId.idSpec = UA_String_fromChars(dstAASIdSpec);
//
//    UA_Variant_setScalarCopy(&inputArgs[0], &srcAASId,
//            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[1], &dstAASId,
//            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
//            "/TechUnits/openAAS/ModelmanagerOpenAAS||triggerGetCoreData");
//    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
//            "/TechUnits/openAAS/ModelmanagerOpenAAS");
//
//    UA_Variant *output = NULL;
//    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
//            &argOutSize, &output);
//
//    if (retval == UA_STATUSCODE_GOOD) {
//        printf(
//                "Method call was successful, and %lu returned values available.\n",
//                (unsigned long) argOutSize);
//        goto clean;
//    } else {
//        printf(
//                "Method call was unsuccessful, and %x returned values available.\n",
//                retval);
//    }
//
//    clean: UA_Identification_deleteMembers(&srcAASId);
//    UA_Identification_deleteMembers(&dstAASId);
//    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    return retval;
//}
//
//
//UA_StatusCode call_startGetAssetLCEData(char* ipAddress, char* AASIdSpec, int AASIdType,char* assetIp, char* AssetIdSpec, int AssetIdType){
//    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
//    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
//    if (retval != UA_STATUSCODE_GOOD) {
//        UA_Client_delete(client);
//        return (int) retval;
//    }
//    size_t argInSize = 3;
//    size_t argOutSize = 0;
//    UA_Variant *inputArgs = UA_Array_new(argInSize,
//            &UA_TYPES[UA_TYPES_VARIANT]);
//    for (size_t i = 0; i < argInSize; i++) {
//        UA_Variant_init(&inputArgs[i]);
//    }
//    /* convert input to UA types */
//    UA_Identification AASId;
//    AASId.idType = AASIdType;
//    AASId.idSpec = UA_String_fromChars(AASIdSpec);
//    UA_Identification assetId;
//    assetId.idType = AssetIdType;
//    assetId.idSpec = UA_String_fromChars(AssetIdSpec);
//    UA_String assetIpStr = UA_String_fromChars(assetIp);
//    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
//            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//    UA_Variant_setScalarCopy(&inputArgs[1], &assetIpStr,
//            &UA_TYPES[UA_TYPES_STRING]);
//    UA_Variant_setScalarCopy(&inputArgs[2], &assetId,
//            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//
//    UA_Variant *output = NULL;
//    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
//            "/TechUnits/openAAS/ModelmanagerOpenAAS||startGetAssetLCEData");
//    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
//            "/TechUnits/openAAS/ModelmanagerOpenAAS");
//    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
//            &argOutSize, &output);
//
//    if (retval == UA_STATUSCODE_GOOD) {
//        printf(
//                "Method call was successful, and %lu returned values available.\n",
//                (unsigned long) argOutSize);
//    } else {
//        printf(
//                "Method call was unsuccessful, and %x returned values available.\n",
//                retval);
//        goto clean;
//    }
//    clean:
//    UA_Identification_deleteMembers(&AASId);
//    UA_Identification_deleteMembers(&assetId);
//    UA_String_deleteMembers(&assetIpStr);
//    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    /*TODO evaluate Output (Status) */
//    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    UA_Client_delete(client);
//    return retval;
//}
//UA_StatusCode call_stopGetAssetLCEData(char* ipAddress, char* AASIdSpec, int AASIdType){
//    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
//    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
//    if (retval != UA_STATUSCODE_GOOD) {
//        UA_Client_delete(client);
//        return (int) retval;
//    }
//    size_t argInSize = 1;
//    size_t argOutSize = 0;
//    UA_Variant *inputArgs = UA_Array_new(argInSize,
//            &UA_TYPES[UA_TYPES_VARIANT]);
//    for (size_t i = 0; i < argInSize; i++) {
//        UA_Variant_init(&inputArgs[i]);
//    }
//    /* convert input to UA types */
//    UA_Identification AASId;
//    AASId.idType = AASIdType;
//    AASId.idSpec = UA_String_fromChars(AASIdSpec);
//
//    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
//            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
//
//    UA_Variant *output = NULL;
//    UA_NodeId methNodeId = UA_NODEID_STRING(openAASnsIndex,
//            "/TechUnits/openAAS/ModelmanagerOpenAAS||stopGetAssetLCEData");
//    UA_NodeId objectId = UA_NODEID_STRING(openAASnsIndex,
//            "/TechUnits/openAAS/ModelmanagerOpenAAS");
//    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
//            &argOutSize, &output);
//
//    if (retval == UA_STATUSCODE_GOOD) {
//        printf(
//                "Method call was successful, and %lu returned values available.\n",
//                (unsigned long) argOutSize);
//    } else {
//        printf(
//                "Method call was unsuccessful, and %x returned values available.\n",
//                retval);
//        goto clean;
//    }
//    clean: UA_Identification_deleteMembers(&AASId);
//
//    UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    /*TODO evaluate Output (Status) */
//    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
//    UA_Client_delete(client);
//    return retval;
//}


