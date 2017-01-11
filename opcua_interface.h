
#include <stdio.h>
#include <inttypes.h>
#include "ua_openaas_generated.h"
#ifdef UA_NO_AMALGAMATION
#include "ua_client.h"
#include "ua_config_standard.h"
#else
#include "open62541.h"
#include "ua_client_highlevel.h"
#endif
/* create an Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType, char* name, char* AssetIdSpec, int AssetIdType);

/* create a Property value statement list */
UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType, char* name, char* CarrierIdSpec, int CarrierIdType);

/* create a Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress,char* AASIdSpec, int AASIdType, char* PVSLName, char* Name, int RelationalExpression, int ExpressionSemantic, char* Value, int ValueType, char* Unit);
/* create a LifeCycle Entry */
UA_StatusCode call_CreateLCE(char* ipAddress,char* AASIdSpec, int AASIdType, char* creatingInstanceIdSpec, int creatingInstanceIdType, char* writingInstanceIdSpec, int writingInstanceIdType, char* EventClass, char* Subject, UA_DateTime TimeStamp);
