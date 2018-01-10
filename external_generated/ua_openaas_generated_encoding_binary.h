/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/opcua/Downloads/openAASworkshop/external/open62541/tools/generate_datatypes.py
 * on host opcua-VirtualBox by user opcua at 2018-01-10 09:00:49 */

#include "ua_types_encoding_binary.h"
#include "ua_openaas_generated.h"

/* ExpressionSemanticEnum */
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_encodeBinary(const UA_ExpressionSemanticEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionSemanticEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
}

/* ExpressionLogicEnum */
static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_encodeBinary(const UA_ExpressionLogicEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionLogicEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM]);
}

/* ViewEnum */
static UA_INLINE UA_StatusCode
UA_ViewEnum_encodeBinary(const UA_ViewEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_VIEWENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ViewEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
}

/* VisibilityEnum */
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_encodeBinary(const UA_VisibilityEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_VisibilityEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);
}

/* IdEnum */
static UA_INLINE UA_StatusCode
UA_IdEnum_encodeBinary(const UA_IdEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_IDENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_IdEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_IdEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_IDENUM]);
}

/* Identification */
static UA_INLINE UA_StatusCode
UA_Identification_encodeBinary(const UA_Identification *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_Identification_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Identification *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
}

/* LifeCycleEntry */
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_encodeBinary(const UA_LifeCycleEntry *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_decodeBinary(const UA_ByteString *src, size_t *offset, UA_LifeCycleEntry *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);
}
