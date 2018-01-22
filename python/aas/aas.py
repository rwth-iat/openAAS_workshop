from ctypes import *
import datetime
from datetime import *
import time
from opcua import ua
from opcua import Client
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


#def createAAS(pathToLibrary, endpointStr, aasIDSpec, aasIDType, aasName, assetIdSpec , assetIdType):
#  """
#  Creates an Asset Administraiton Shell

#  This function creates an asset administration shell on the given endpoint.

#  Args:
#  ----------
#  pathToLibrary : string
#    path to the shared object that provides the opc ua functionality
#  endpointStr : string
#    opc ua endpoint to the aas repository server
#  aasIDSpec : string
#    asset administration shell id specification
#  aasIDType : string
#    asset administration shell id type
#  aasName : string 
#    a name for the aas object
#  assetIdSpec : string 
#    asset id specification
#  assetIdType : int
#    asset id type (URI=0, ISO=1)
#      
#  Returns:
#  string: Status Code
#  """
#  lib = CDLL(pathToLibrary)
#  ip_c = endpointStr.encode('utf-8')
#  AASIdSpec_c = aasIDSpec.encode('utf-8')
#  AASIdType_c = c_int(aasIDType)
#  AASName_c = aasName.encode('utf-8')
#  AssetIdSpec_c = assetIdSpec.encode('utf-8')
#  AssetIdType_c = c_int(assetIdType)
#  return lib.call_CreateAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(AASName_c), c_char_p(AssetIdSpec_c), AssetIdType_c)

#def getAASEntryPoint_OPCUA(AASIDSpec, AASIDType):
#  """
#  Retrieves the nodeId of the AAS by the given name
#  Args:
#  ----------
#  AASIDSpec : string
#    path to the shared object that provides the opc ua functionality
#  AASIDType : string

#  Returns:
#  -------
#    NodeID : 
#  """  
#  return 
  
#def getSubModels_Hdr(AASEntrypointOPCUA(AASNodeId):

#  """
#  Retrieves the nodeId of the AAS by the given name
#  Args:
#  ----------
#  AASNodeId : Node Id of the AAS

#  Returns:
#  -------
#    Names and NodeIds of the found SubModels
#  """  
#  return 
  
#def getSubModels_Bdy(AASEntrypointOPCUA(AASNodeId):

#  """
#  Retrieves the nodeId of the AAS by the given name
#  Args:
#  ----------
#  AASNodeId : Node Id of the AAS

#  Returns:
#  -------
#    Names and NodeIds of the found SubModels
#  """  
#  return 
  
#Body/Header
def getBodyNodeId(AASNodeId):
  return  
def getHdrNodeId(AASNodeId):
  return
    
#PVSL
def getPVSLinHdr(HdrNodeId):
  return
def getPVSLinBdy(BdyNodeId):
  return 
def getPVSLName(PVSLNodeId):
  return
def getPVSLData(PVSLNodeId):
  return  

    
#Objects  
def getObjectsInSubModel(SubModelNodeId):  
  pass
  
def getObjectsInObjects(ObjectNodeId):
  pass
#PVS  
def getPVSDataInPVSL(PVSLNodeId):
  pass
  
def getPVSLinSubModel(SubModelNodeId):
  pass
def getAASEntryPointByAASID(ResolutionServerEndpoint,AASIDSpec,AASIDType):
    client = Client(ResolutionServerEndpoint)
    # client = Client("opc.tcp://localhost:16664) #connect using a user
    res = ["",""]
    try:
       client.connect()

       nsarray = client.get_node(ua.NodeId(2255, 0))
       nsList = nsarray.get_value()
       i=-1
       for entry in nsList:
         i = i + 1
         if entry == "http://acplt.org/subModelDiscovery/Ov":
           #print(entry)
           nsopenaas_discovery = i
           break
       if i == -1:
         exit()
       getAASEntryPointByAASID_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/networkAAS.Body/DiscoveryServer",nsopenaas_discovery)
       getAASEntryPointByAASIDMethod_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/networkAAS.Body/DiscoveryServer.GetAASEntryPointByAASIDService",nsopenaas_discovery)
       discoveryNode = client.get_node(getAASEntryPointByAASID_nodeId)
       discoveryMethodNode = client.get_node(getAASEntryPointByAASIDMethod_nodeId)
       print(discoveryNode.get_browse_name().Name)
       print(discoveryMethodNode.get_browse_name().Name)
       res = discoveryNode.call_method(discoveryMethodNode, ua.Variant(AASIDType, ua.VariantType.UInt32), ua.Variant(AASIDSpec,ua.VariantType.String))
       print("result of method call is %s" % res)
    finally:   
       client.disconnect()
       return res
     
  
def getAASIDByAssetID(ResolutionServerEndpoint,AssetIDSpec,AssetIDType):
    client = Client(ResolutionServerEndpoint)
    # client = Client("opc.tcp://localhost:16664) #connect using a user
    res = ["","",""]
    try:
       client.connect()

       nsarray = client.get_node(ua.NodeId(2255, 0))
       nsList = nsarray.get_value()
       i=-1
       for entry in nsList:
         i = i + 1
         if entry == "http://acplt.org/subModelDiscovery/Ov":
           #print(entry)
           nsopenaas_discovery = i
           break
       if i == -1:
         exit()
       getAASIDFromAssetId_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/networkAAS.Body/DiscoveryServer",nsopenaas_discovery)
       getAASIDFromASsetIdMethod_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/networkAAS.Body/DiscoveryServer.GetAASIDByAssetIDService",nsopenaas_discovery)
       discoveryNode = client.get_node(getAASIDFromAssetId_nodeId)
       discoveryMethodNode = client.get_node(getAASIDFromASsetIdMethod_nodeId)
       print(discoveryNode.get_browse_name().Name)
       print(discoveryMethodNode.get_browse_name().Name)
       res = discoveryNode.call_method(discoveryMethodNode, ua.Variant(AssetIDType, ua.VariantType.UInt32), ua.Variant(AssetIDSpec,ua.VariantType.String))
       print("result of method call is %s" % res)
    finally:   
       client.disconnect()
       return res
       
#def getSubModelLocalID_OPCUA(AASEntrypointOPCUA(subModelName):
  #"""
  #Retrieves the nodeId of the AAS by the given name
  #Args:
  #----------
  #AASIDSpec : string
    #path to the shared object that provides the opc ua functionality
  #AASIDType : string

  #Returns:
  #-------
    #NodeID : 
  #"""  
  #pass   
  
#def getAAS_EntryPoint_OPCUA(AASIDSpec, AASIDType):
  #"""
  #Retrieves the nodeId of the AAS by the given name
  #Args:
  #----------
  #AASIDSpec : string
    #path to the shared object that provides the opc ua functionality
  #AASIDType : string

  #Returns:
  #-------
  #string
    #Status Code
  #"""  
  #pass 
 
  
  
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
  
def createSubModel(pathToLibrary, endpointStr, aasIDSpec, aasIDType,  parent, modelIdSpec, modelIdType, modelName, revision, version):
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

def createSubModel_raw(pathToLibrary, endpointStr, aasIDSpec, aasIDType,  parentIdSpec, parentIdType, modelIdSpec, modelIdType, modelName, revision, version):
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
