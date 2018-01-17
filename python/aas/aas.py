from ctypes import *
import datetime
from datetime import *
import time
 #   typedef struct pvsType{
 #   char name[256];
 #   char value[256];
 #   int valueType;
 #   char unit[256];
 #   int expressionLogic;
 #   int expressionSemantic;
 #   int view;
 #   char IDIdSpec[256];
 #   int IDIdType;
 #   int visibility;
#}pvsType;
class PVS(Structure):
    _fields_ = [("name", c_char*256),
    ("value",c_char*256),
    ("valueType",c_int),
    ("unit",c_char*256),
    ("expressionLogic", c_int),
    ("expressionSemantic", c_int),
    ("view",c_int),
    ("IDIdSpec",c_char*256),
    ("IDIdType",c_int),
    ("visibility",c_int)]

#typedef struct lifeCycleEntryType{
#   char timestamp[MAX_STRING_SIZE];
#   char subject[MAX_STRING_SIZE];
#   char eventClass[MAX_STRING_SIZE];
#    char creatingInstanceSpec[MAX_STRING_SIZE];
#    int creatingInstanceType;
#    char writingInstanceSpec[MAX_STRING_SIZE];
#    int writingInstanceType;
#    char data[256];
#    int dataType;
#    int64_t id;
#}lifeCycleEntryType;
class LCE(Structure):
    _fields_ = [("timestamp", c_int64),
    ("subject",c_char*256),
    ("eventClass",c_char*256),
    ("creatingInstanceSpec",c_char*256),
    ("creatingInstanceType",c_int),
    ("writingInstanceSpec",c_char*256),
    ("writingInstanceType",c_int),
    ("data",c_char*256),
    ("dataType", c_int),
    ("id", c_int64)]

#ID#################
def TypeToInt_Id(typ):
    #URI=0;ISO=1
    typ = typ.upper()
    if typ == "URI":
        return 0
    return 1

def IntToType_Id(Int):
    if Int == 0:
        return "URI"
    return "ISO"
#######################
#expression logic
def TypeToInt_EL(typ):
    typ = typ.upper()
    return {
        "GREATER_THAN": 0,
        "GREATER_EQUAL": 1,
        "EQUAL":2,
        "NOT_EQUAL":3,
        "LESS_EQUAL":4,
        "LESS_THAN":5,
    }.get(typ, 99) #return 99 if not found

def IntToType_EL(Int):
    return {
        0: "GREATER_THAN",
        1: "GREATER_EQUAL",
        2: "EQUAL",
        3: "NOT_EQUAL",
        4: "LESS_EQUAL",
        5: "LESS_THAN",
    }.get(Int, "not defined")
#####################
#expression semantic
def TypeToInt_ES(typ):
    typ = typ.upper()
    return {
        "ASSURANCE": 0,
        "SETTING": 1,
        "MEASUREMENT":2,
        "REQUIREMENT":3,
    }.get(typ, 99)

def IntToType_ES(Int):
    return {
        0:"ASSURANCE",
        1:"SETTING",
        2:"MEASUREMENT",
        3:"REQUIREMENT",
    }.get(Int, "not defined")
########################
#data type(value type)

def TypeToInt_valueType(typ):
    typ = typ.upper()
    return {
        "BOOL" : 1,
        "FLOAT": 2,
        "INT32": 3,
        "INT64": 4,
        "UINT32": 5,
        "UINT64": 6,
        "DOUBLE": 7,
        "STRING": 8,
        "DATETIME":9,
        "IDENTIFICATION":10,
    }.get(typ, 99)

def IntToType_valueType(Int):
    return {
        1 :"BOOL",
        2 :"FLOAT",
        3 :"INT32",
        4 :"INT64",
        5:"UINT32",
        6:"UINT64",
        7:"DOUBLE",
        8:"STRING",
        9:"DATETIME",
        10:"IDENTIFICATION",
    }.get(Int, "not defined")
########################
#view####################
def TypeToInt_view(typ):
    typ = typ.upper()
    return {
        "BUSINESS": 0,
        "CONSTRUCTION": 1,
        "POWER":2,
        "FUNCTION":3,
        "LOCATION":4,
        "SECURITY":5,
        "NETWORK": 6,
        "LIFECYCLE": 7,
        "HUMAN":8,
    }.get(typ, 99)

def IntToType_view(Int):
    return {
        0:"BUSINESS",
        1:"CONSTRUCTION",
        2:"POWER",
        3:"FUNCTION",
        4:"LOCATION",
        5:"SECURITY",
        6:"NETWORK",
        7:"LIFECYCLE",
        8:"HUMAN",
    }.get(Int, "not defined")
def IntToType_VIS(Int):
    return {
      0 : "PRIVATE",
      1 : "CONTRACT",
      2 : "PUBLIC"}.get(Int,"not defined")

def TypeToInt_VIS(typ):
    typ = typ.upper()
    return {
        "PRIVATE":0,
        "CONTRACT":1,
        "PUBLIC":2
    }.get(typ, "not defined")


def createAAS(pathToLibrary, endpointStr, aasIDSpec, aasIDType, aasName, assetIdSpec , assetIdType):
  """
  Creates an Asset Administraiton Shell

  This function creates an asset administration shell on the given endpoint.

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : string
    asset administration shell id type
  aasName : string 
    a name for the aas object
  assetIdSpec : string 
    asset id specification
  assetIdType : int
    asset id type (URI=0, ISO=1)
      
  Returns:
  string: Status Code
  """
  lib = CDLL(pathToLibrary)
  ip_c = endpointStr.encode('utf-8')
  AASIdSpec_c = aasIDSpec.encode('utf-8')
  AASIdType_c = c_int(aasIDType)
  AASName_c = aasName.encode('utf-8')
  AssetIdSpec_c = assetIdSpec.encode('utf-8')
  AssetIdType_c = c_int(assetIdType)
  return lib.call_CreateAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(AASName_c), c_char_p(AssetIdSpec_c), AssetIdType_c)



def deleteAAS(pathToLibrary, endpointStr, aasIDSpec, aasIDType):
  """
  Deletes an Asset Administraiton Shell

  This function deletes an asset administration shell on the given endpoint.

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  
  Returns:
  -------
  string
    Status Code
  """
  lib = CDLL(pathToLibrary)
  ip_c = endpointStr.encode('utf-8')
  AASIdSpec_c = aasIDSpec.encode('utf-8')
  AASIdType_c = c_int(aasIDType)
  StatusCall = lib.call_DeleteAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c)
  if(StatusCall!=0):
    status_str = "failed"
  else:
    status_str = "good"
  del lib
  return status_str  


  
def createSubModel(pathToLibrary, endpointStr, aasIDSpec, aasIDType,  parentIdSpec, parentIdType, modelIdSpec, modelIdType, modelName, revision, version):
  """
  Creates a Sub Model within an Asset Administraiton Shell

  This function creates an asset administration shell within a given Asset administration Shell.

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  parentIdSpec : string
    parent object id
  parentIdType : int
    parent object id type (URI=0, ISO=1)
  modelIdSpec : string
    reference to the definition of the sub modelIdSpec
  modelIdType : int
    model id type (URI=0, ISO=1)
  modelName : string
    local name of the sub model 
  revision : int
    revision of the sub model
  version : int 
    version of the sub model    
  Returns:
  -------
  string
    Status Code
  """
  lib = CDLL(pathToLibrary)
  ip_c = endpointStr.encode('utf-8')
  AASIdSpec_c = aasIDSpec.encode('utf-8')
  AASIdType_c = c_int(aasIDType)

  ParentIdSpec_c = parentIdSpec.encode('utf-8')
  ParentIdType_c = c_int(parentIdType)

  ModelIdSpec_c = modelIdSpec.encode('utf-8')
  ModelIdType_c = c_int(modelIdType)

  ModelName_c = modelName.encode('utf-8')
  Revision_c = c_int(revision)
  Version_c = c_int(version)

  StatusCall = lib.call_CreateSubModel(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,  c_char_p(ParentIdSpec_c), ParentIdType_c,c_char_p(ModelIdSpec_c), ModelIdType_c, c_char_p(ModelName_c), Revision_c, Version_c)
  if(StatusCall!=0):
    status_str = "failed"
  else:
    status_str = "good"
  del lib
  return status_str  



def deleteSubModel(pathToLibrary, endpointStr, aasIDSpec, aasIDType, modelIdSpec, modelIdType):
  """
  This function deletes a Sub Model which has been created within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  modelIdSpec : string
    reference to the definition of the sub modelIdSpec
  modelIdType : int
    model id type (URI=0, ISO=1)
  Returns:
  -------
  string
    Status Code
  """
  lib = CDLL(pathToLibrary)
  ip_c = endpointStr.encode('utf-8')
  AASIdSpec_c = aasIDSpec.encode('utf-8')
  AASIdType_c = c_int(aasIDType)
  SubModelIdSpec_c = modelIdSpec.encode('utf-8')
  SubModelIdType_c = c_int(modelIdType)


  StatusCall = lib.call_DeleteSubModel(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(SubModelIdSpec_c), SubModelIdType_c)

  if(StatusCall!=0):
    status_str = "failed"
  else:
    status_str = "good2"
  del lib
  return status_str



def createLCE (pathToLibrary, endpointStr, aasIDSpec, aasIDType, creatingInstanceIdSpec, creatingInstanceIdType, writingInstanceIdSpec, writingInstanceIdType, eventClass, subject, value, valueType): 
  """
  This function creates a LifeCycleEntry which has been created within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  creatingInstanceIdSpec : string
    creating instance id specification
  creatingInstanceIdType : int
    creating instance id type
  writingInstanceIdSpec : string
    writing instance id specification
  writingInstanceIdType : int
    writing instance id type
  eventClass : string
    event Class description
  subject : string
    subject description
  value : string
    value description
  valueType : int
    value type
  Returns:
  -------
  string
    Status Code
  """
   
  lib = CDLL(pathToLibrary)
  ip_c = endpointStr.encode('utf-8')
  AASIdSpec_c = aasIDSpec.encode('utf-8')
  AASIdTypeInt_c = c_int(aasIDType)

  creatingInstanceIdSpec_c = creatingInstanceIdSpec.encode('utf-8')
  creatingInstanceIdType_c = c_int(creatingInstanceIdType)
  writingInstanceIdSpec_c = writingInstanceIdSpec.encode('utf-8')
  writingInstanceIdType_c = c_int(writingInstanceIdType)

  eventClass_c = eventClass.encode('utf-8')
  subject_c = subject.encode('utf-8')

  value_c = value.encode('utf-8')
  valueType_c = c_int(valueType)
  dtTicks = c_int64((time.time()*10000000+116444736000000000))
  print(dtTicks)

  StatusCall = lib.call_CreateLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdTypeInt_c, c_char_p(creatingInstanceIdSpec_c), creatingInstanceIdType_c, c_char_p(writingInstanceIdSpec_c), writingInstanceIdType_c, c_char_p(eventClass_c), c_char_p(subject_c), dtTicks, c_char_p(value_c), valueType_c)

  if(StatusCall!=0):
    status_str = "failed"
  else:
    status_str = "good"
  del lib
  return status_str

def deleteLCE(pathToLibrary, endpointStr, aasIDSpec, aasIDType, lceID):
    """
  This function deletes a LifeCycleEntry which has been created within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  lceID : int
    life cycle entry identification
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)
    LCEId_c = c_longlong(int(lceID))

    StatusCall = lib.call_DeleteLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,LCEId_c)
    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str



def createLCE1(pathToLibrary, endpointStr, aasIDSpec, aasIDType, creatingInstanceIdSpec, creatingInstanceIdType, writingInstanceIdSpec, writingInstanceIdType, eventClass, subject, value, valueType):
    """
  This function creates a LifeCycleEntry which has been created within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  creatingInstanceIdSpec : string
    creating instance id specification
  creatingInstanceIdType : int
    creating instance id type
  writingInstanceIdSpec : string
    writing instance id specification
  writingInstanceIdType : int
    writing instance id type
  eventClass : string
    event Class description
  subject : string
    subject description
  value : string
    value description
  valueType : int
    value type
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)

    creatingInstanceIdSpec_c = creatingInstanceIdSpec.encode('utf-8')
    creatingInstanceIdType_c = c_int(creatingInstanceIdType)
    writingInstanceIdSpec_c = writingInstanceIdSpec.encode('utf-8')
    writingInstanceIdType_c = c_int(writingInstanceIdType)

    eventClass_c = eventClass.encode('utf-8')
    subject_c = subject.encode('utf-8')
    timeStamp_c = c_int64(int((time.time())*10000000+116444736000000000))

    value_c = value.encode('utf-8')
    valueType_c = c_int(valueType)
    print(valueType_c)

    StatusCall = lib.call_CreateLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(creatingInstanceIdSpec_c), creatingInstanceIdType_c, c_char_p(writingInstanceIdSpec_c), writingInstanceIdType_c, c_char_p(eventClass_c), c_char_p(subject_c), timeStamp_c, c_char_p(value_c), valueType_c)
    StatusCall = 1
    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str



def createPVSL(pathToLibrary, endpointStr, aasIDSpec, aasIDType, listName, parentIdType, parentIdSpec, carrierIdType, carrierIdSpec): 
# check if subModelIdType and subModelIdSpec have to be added
    """
  This function creates a PropertyValueStatementList which has been created within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  listName : string
    name of property value statement list
  parentIdSpec : string
    parent object id
  parentIdType : int
    parent object id type (URI=0, ISO=1)
  subModelIdType : int
    submodel id type
  subModelIdSpec : string
    submodel id specification
  carrierIdType : int
    carrier id type
  carrierIdSpec : string
    carrier id specification
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)

    listName_c = listName.encode('utf-8')

    parentIdType_c = c_int(parentIdType)
    parentIdSpec_c = parentIdSpec.encode('utf-8')

    #subModelIdType_c = c_int(subModelIdType)
    #subModelIdSpec_c = subModelIdSpec.encode('utf-8')

    carrierIdType_c = c_int(carrierIdType)
    carrierIdSpec_c = carrierIdSpec.encode('utf-8')


    propertyIdSpec = "dummy property id"
    propertyIdType_c = c_int(0)
    propertyIdSpec_c = propertyIdSpec.encode('utf-8')

    mask = c_int(1);
    expressionLogic_c = c_int(0);
    expressionSemantic_c = c_int(0);
    view_c = c_int(0);
    visibility_c = c_int(0);

    StatusCall = 0
    print(carrierIdSpec)
    StatusCall = lib.call_CreatePVSL(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(parentIdSpec_c), parentIdType_c, c_char_p(listName_c), mask, c_char_p(carrierIdSpec_c), carrierIdType_c, expressionLogic_c, expressionSemantic_c, c_char_p(propertyIdSpec_c), propertyIdType_c, view_c, visibility_c)
    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str



def deletePVSL(pathToLibrary, endpointStr, aasIDSpec, aasIDType, listIdSpec, listIdType):
    """
  This function deletes a PropertyValueStatementList which has been created within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  listIdSpec : string
    list id specification
  listIdType : int
    list id type
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)

    listIdSpec_c = listIdSpec.encode('utf-8')
    listIdType_c = c_int(listIdType)

    StatusCall = lib.call_DeletePVSL(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(listIdSpec_c),listIdType_c)

    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str

"""
check this one again
"""



def createPVS(pathToLibrary, endpointStr, AASIdSpec, AASIdType, ListIdSpec, ListIdType, PVSName, PRIdType, PRIdSpec, unit, valueType, Value, expressionSemantic, expressionLogic, view, visibility, mask):

#ipAddress,
#char* AASIdSpec,
#int AASIdType,
#char* ListIdSpec,
#int ListIdType,
#char* PVSName,
#char* Value,
#int ValueType,
#int mask,
#char* CarrierIdSpec,
#int CarrierIdType,
#int ExpressionLogic,
#int ExpressionSemantic,
#char* propertyIdSpec,
#int propertyIdType,
#int view,
#int visibility

    lib = CDLL(pathToLibrary)

    #PVSLName_c = PVSLName.encode('utf-8')
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)

    #SubModelIdSpec_c = SubModelIdSpec.encode('utf-8')
    #SubModelIdType_c = c_int(SubModelType)

    ListIdSpec_c = ListIdSpec.encode('utf-8')
    ListIdType_c = c_int(ListIdType)

  
    PVSName_c = PVSName.encode('utf-8')
    expressionLogic_c = c_int(TypeToInt_EL(expressionLogic))
    expressionSemantic_c = c_int(TypeToInt_ES(expressionSemantic))
    Value_c = Value.encode('utf-8')
    valueType_c = c_int(TypeToInt_valueType(valueType))
    unit_c = unit.encode('utf-8')
    PRIdSpec_c = PRIdSpec.encode('utf-8')
    PRIdType_c = c_int(TypeToInt_Id(PRIdType))
    view_c = c_int(TypeToInt_view(view))
    visibility_c = c_int(TypeToInt_VIS(visibility))
    mask_c = c_int(mask)
    print(mask_c)

    StatusCall = lib.call_CreatePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c),AASIdType_c,c_char_p(ListIdSpec_c),ListIdType_c, c_char_p(PVSName_c), c_char_p(Value_c),valueType_c,mask_c,c_char_p(CarrierIdSpec_c),CarrierIdType_c,expressionLogic_c,expressionSemantic_c,c_char_p(PRIdSpec_c),PRIdType_c,view_c,visibility_c)
    if(StatusCall!=0):
        status_str = "failed"
    else:
        status_str = "good"
    del lib
    return status_str



def deletePVS(pathToLibrary, endpointStr, aasIDSpec, aasIDType, subModelSpec, subModelType, pvslName, pvsName):
    """
This function deletes a PropertyValueStatement which was added to a PropertyValueStatementList

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  subModelIdSpec : string
    subModel id specification
  subModelIdType : int
    subModel id type
  pvslName : string
    name of property value statement list
  pvsName : string
    name of property value statement
  Returns:
  -------
  string
    Status Code
    """
 
    lib = CDLL(pathToLibrary)

    PVSLName_c = pvslName.encode('utf-8')
    PVSName_c = pvsName.encode('utf-8')

    subModelType_c = subModelSpec.encode('utf-8') #hier ist subModelType_c richtig?

    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)

    StatusCall = lib.call_DeletePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(subModelType_c),subModelType,c_char_p(PVSLName_c),c_char_p(PVSName_c))
    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str



def getPVSFromServer(pathToLibrary, endpointStr, aasIDSpec, aasIDType, subModelSpec, subModelType, pvslName):
    """
This function gets a PropertyValueStatement which was added to a PropertyValueStatementList

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  subModelIdSpec : string
    subModel id specification
  subModelIdType : int
    subModel id type
  pvslName : string
    name of property value statement list
  
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)

    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdTypeInt_c = c_int(aasIDType)
    PVSLName_c = pvslName.encode('utf8')
    propertyValueStatements = c_void_p()

    subModelType_c = subModelSpec.encode('utf-8')

    count = lib.getPVSFromListByName(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdTypeInt_c,c_char_p(subModelType_c),subModelType, c_char_p(PVSLName_c), byref(propertyValueStatements))
    
    del lib
    return count



def getLastLCEs(pathToLibrary, endpointStr, aasIDSpec, aasIDType, lceCountToReturn):
    """
This function gets the last created LifeCycleEntries

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
 lceCountToReturn : int
		# ?  
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdTypeInt_c = c_int(aasIDType)
    lceCountToReturn_c = c_int(lceCountToReturn)
    lifeCycleEntries = c_void_p()

    count = lib.call_GetLastLCEs(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdTypeInt_c,lceCountToReturn_c, byref(lifeCycleEntries))

    del lib
    return count



def triggerGetCoreData(pathToLibrary, endpointStr, srcaasIDSpec, srcaasIDType, dstaasIDSpec, dstaasIDType):
    """
This function triggers an output of core data

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
			weitere ????
  Returns:
  -------
  string
    Status Code
    """
    lib = CDLL(pathToLibrary)

    ip_c = endpointStr.encode('utf-8')
    srcAASIdSpec_c = srcaasIDSpec.encode('utf-8')
    srcAASIdTypeInt_c = c_int(srcaasIDType)

    dstAASIdSpec_c = dstaasIDSpec.encode('utf-8')
    dstAASIdTypeInt_c = c_int(dstaasIDType)

    count = lib.call_triggerGetCoreData(c_char_p(ip_c), c_char_p(srcAASIdSpec_c), srcAASIdTypeInt_c,c_char_p(dstAASIdSpec_c), dstAASIdTypeInt_c)
    del lib
    return count



def startGetAssetLCEData(pathToLibrary, endpointStr, aasIDSpec, aasIDType, assetIdSpec, assetIdType, assetIp):
    """
This function gets LCE data of asset

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  assetIdSpec : string
    asset id specification
  assetIdType : int	
    asset id type
  assetIp : 		??
    asset ip
  Returns:
  -------
  string
    Status Code
    """

    lib = CDLL(pathToLibrary)

    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)

    AssetIdSpec_c = assetIdSpec.encode('utf-8')
    AssetIdType_c = c_int(aasIdType)

    AssetIp_c = assetIp.encode('utf-8')

    StatusCall = lib.call_startGetAssetLCEData(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(AssetIp_c), c_char_p(AssetIdSpec_c), AssetIdType_c)
    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str



def call_stopGetAssetLCEData(pathToLibrary, endpointStr, aasIDSpec, aasIDType):
    """
This function terminates access to LCE data of asset

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  aasIDSpec : string
    asset administration shell id specification
  aasIDType : int 
    asset administration shell id type (URI=0, ISO=1)
  Returns:
  -------
  string
    Status Code
    """
    lib = CDLL(pathToLibrary)
    ip_c = endpointStr.encode('utf-8')
    AASIdSpec_c = aasIDSpec.encode('utf-8')
    AASIdType_c = c_int(aasIDType)

    StatusCall = lib.call_stopGetAssetLCEData(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c)
    if(StatusCall!=0):
      status_str = "failed"
    else:
      status_str = "good"
    del lib
    return status_str

