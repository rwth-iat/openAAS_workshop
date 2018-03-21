import sys
import os
from ctypes import *
import datetime
from datetime import *
from struct import *
import time
from opcua import ua
from opcua import Client
import json
from os.path import expanduser


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
        "FUNCTIONAL":3,
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
        3:"FUNCTIONAL",
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

class Domain:
    def __init__(self, name):
        self.Name = name
        self.Domains = []
        self.PropertyValueStatementContainers = []
        self.Services = []
    def Name(self):
        return self.Name
    def addSubDomain(self,Domain):
       self.Domains.append(Domain)
    #def addSubModel(self, subModel):
    #  self.SubModels.append(subModel)
    def addPropertyValueStatementContainer(self,pvsContainer):
      self.PropertyValueStatementContainers.append(pvsContainer)
    @classmethod
    def fromOPCUANodes(cls,node):
        domain = cls(name = node.get_browse_name().Name)
        for c in node.get_children():
            for r in c.get_references(refs=40):
              if r.BrowseName.Name == "PropertyValueStatementListType":
                domain.addPropertyValueStatementContainer(propertyValueStatementContainer.fromOPCUANodes(node = c))
              if r.BrowseName.Name == "domain" or "Folder":
                domain.addSubDomain(cls.fromOPCUANodes(c))
            if c.get_node_class().name == "Method":
              service = Service(Name = p.get_browse_name().Name)
              domain.addService(service)
        return domain
class Service:
    def __init__(self,name):
      self.Name = name


class Identification:
    def __init__(self, IDType, IDSpec):
        self.IDType = IDType
        self.IDSpec = IDSpec
    @classmethod
    def fromBinary(cls,body):
      #print(body[0:4])
      IDLen = unpack('i',body[0:4])[0]
      #print(IDLen)
      if IDLen<0:
        IDSpec = ""
        IDType = 0
      else:
        #print(len(body)-(4+IDLen))
        IDSpec = body[4:4+IDLen].decode("utf-8")
        #print(self.IDSpec)
        IDType = parseID(unpack('I',body[4+IDLen:len(body)])[0])
      return cls(IDType,IDSpec)
    @classmethod
    def fromString(cls,str):
      IDType = str[0:3]
      IDSpec = str[4:len(str)]
      return cls(IDType,IDSpec)

class subModel:
    def __init__(self,Name):
      #Domain.__init__(self,name=Name)
      self.Name = Name
      self.Version = ""
      self.Revision = ""
      self.PropertyValueStatementContainers  = []
      self.Services = []
      self.Domains = []
    def addPropertyValueStatementContainer(self,Container):
        self.PropertyValueStatementContainers.append(Container)
    def addService(self,service):
        self.Services.append(service)
    def addDomain(self, domain):
        self.Domains.append(domain)
    @classmethod
    def fromOPCUANodes(cls,node):
         sm = subModel(Name=node.get_browse_name().Name)
         print("parsing Submodel %s" % (sm.Name))
         for p in node.get_children():
           browseName = p.get_browse_name().Name
           if browseName.upper() == "MODELID":
             sm.ModelID = Identification.fromBinary(body=p.get_value().Body)
           if browseName.upper() == "REVISION" :
              sm.Revision = p.get_value()
           if browseName.upper() == "VERSION":
             sm.Version = p.get_value()
           if p.get_node_class().name == "Method":
               service = Service(name = p.get_browse_name().Name)
               sm.addService(service)

           for r in p.get_references(refs=40):
               if r.BrowseName.Name == "PropertyValueStatementListType":
                 pvsl = propertyValueStatementContainer.fromOPCUANodes(node = p)
                 sm.addPropertyValueStatementContainer(pvsl)
               if r.BrowseName.Name == "domain":
                 #print(r.BrowseName.Name)
                 sm.addDomain(Domain.fromOPCUANodes(p))
                 #for statement in pvsl.statements:
                #     print("name is %s and value is " % statement.Name )
                #     print("value is %s" % statement.Value)
         return sm

class propertyValueStatementContainer:
   def __init__(self,Name):
      #Domain.__init__(self,name = Name)
      self.Name = Name
      self.statements = []
   def addStatement(self,statement):
     self.statements.append(statement)
   @classmethod
   def fromOPCUANodes(cls,node):
      pvsl = cls(Name=node.get_browse_name().Name)
      print("parsing Property Value Statement Container  %s" % (pvsl.Name))
      for c in  node.get_children(35):
        browseName = c.get_browse_name().Name
        #print(browseName)
        if browseName.upper() == "CARRIERID":
          #print("found Carrier Id")
          pvsl.CarrierID = Identification.fromBinary(body = c.get_value().Body)
        elif browseName.upper() == "EXPRESSIONLOGIC":
          #print("found Expression Logic")
          pvsl.ExpressionLogic = parseExpressionLogic(c.get_value())
        elif browseName.upper() =="EXPRESSIONSEMANTIC":
          #print("found Expression Semantic")
          pvsl.ExpressionSemantic = parseExpressionSemantic(c.get_value())
        elif browseName.upper() == "PROPERTYID":
          #print("found Property ID")
          pvsl.PropertyID = Identification.fromBinary(body = c.get_value().Body)
        elif browseName.upper() == "VIEW":
          #print("found View")
          pvsl.View = parseView(c.get_value())
        elif browseName.upper() == "VISIBILITY":
          #print("found Visibility")
          pvsl.Visibility = parseVisibility(c.get_value())
        else:
          for r in c.get_references(refs=40):
            if r.BrowseName.Name == "PropertyValueStatementType":
                pvs = propertyValueStatement.fromOPCUANodes(c)
                pvsl.addStatement(pvs)

      return pvsl
   #def Name(self):
     #return Domain.Name
class propertyValueStatement:
  def __init__(self, Name):
      self.Name = Name
      #self.CarrierID =""
      #self.ExpressionLogic = ""
      #self.ExpressionSemantic =""
      #self.PropertyID =""
      #self.View = ""
      #self.Visibility =""
      self.Value =""
  @classmethod
  def fromOPCUANodes(cls, node):
    pvs = cls(node.get_browse_name().Name)
    for o in node.get_children():
      browseName = o.get_browse_name().Name
      #print("%s" % browseName)
      if browseName.upper() == "CARRIERID":
        #print("found Carrier Id")
        pvs.CarrierID = Identification.fromBinary(body = o.get_value().Body)
      elif browseName.upper() == "EXPRESSIONLOGIC":
        #print("found Expression Logic")
        pvs.ExpressionLogic = parseExpressionLogic(o.get_value())
      elif browseName.upper() =="EXPRESSIONSEMANTIC":
        #print("found Expression Semantic")
        pvs.ExpressionSemantic = parseExpressionSemantic(o.get_value())
      elif browseName.upper() == "PROPERTYID":
        #print("found Property ID")
        pvs.PropertyID = Identification.fromBinary(body = o.get_value().Body)
      elif browseName.upper() == "VIEW":
        #print("found View")
        pvs.View = parseView(o.get_value())
      elif browseName.upper() == "VISIBILITY":
        #print("found Visibility")
        pvs.Visibility = parseVisibility(o.get_value())
    pvs.Value = node.get_value()
    return pvs
    #def Name(self):
    #    return Domain.Name
class Header:
  def __init__(self):
    self.SubModels =[]
    self.PropertyValueStatementContainers = []
  def addSubModel(self, subModel):
    self.SubModels.append(subModel)
  def addPropertyValueStatementContainer(self,pvsContainer):
     self.PropertyValueStatementContainers.append(pvsContainer)
class Body:
  def __init__(self):
    self.SubModels =[]
    self.PropertyValueStatementContainers = []
  def addSubModel(self, subModel):
    self.SubModels.append(subModel)
  def addPropertyValueStatementContainer(self,pvsContainer):
     self.PropertyValueStatementContainers.append(pvsContainer)
class AAS :
  def __init__(self,Name):
      self.Name = Name
      self.AASID = ""
      self.AssetID =""
      self.Header = Header();
      self.Body = Body();
  def addSubModel(self,location, subModel):
      if location=="Header":
        self.Header.addSubModel(subModel=subModel)
      elif location=="Body":
        self.Body.addSubModel(subModel=subModel)
  def addPropertyValueStatementContainer(self, location,pvsContainer):
      if location=="Header":
        self.Header.addPropertyValueStatementContainer(pvsContainer=pvsContainer)
      elif location=="Body":
        self.Body.addPropertyValueStatementContainer(pvsContainer=pvsContainer)
  def getName(self):
      return self.Name

  @classmethod
  def fromOPCUANodes(cls,node):
    aas=cls(Name=node.get_browse_name().Name)
    print("parsing Asset Administration Shell %s" % (aas.Name))
    for c2 in node.get_children(refs=35):
      if c2.get_browse_name().Name == "Header":
        HeaderFolderNode = c2#client.get_node(c2.nodeid)
        #print(HeaderFolderNode)
        for c3 in HeaderFolderNode.get_children(refs=35):
          #print(c3.get_browse_name().Name)
          if c3.get_browse_name().Name == "Config":
            #print(c3.get_browse_name().Name)
            for c4 in c3.get_children():
              #print(c4.get_browse_name().Name)
              if c4.get_browse_name().Name == "CarrierID":
                print("#####################")
                print("AAS ID = %s" % c4.get_value().Body)
                print("#####################")
                aas.AASID = Identification.fromBinary(body=c4.get_value().Body)
              if c4.get_browse_name().Name == "Asset":
                aas.AssetID = Identification.fromString(c4.get_value())
                        #print(AssetID.IDSpec)
          for r in c3.get_references(refs=40):
            if r.BrowseName.Name == "PropertyValueStatementListType":
              pvsl = propertyValueStatementContainer.fromOPCUANodes(node = c3)
              aas.addPropertyValueStatementContainer(location="Header",pvsContainer = pvsl)
          for r in c3.get_references(refs=40):
          #print(dir(r))
            if r.BrowseName.Name == "SubModelType":
              sm = subModel.fromOPCUANodes(node=c3)
              aas.addSubModel(location="Header",subModel=sm)
      if c2.get_browse_name().Name == "Body":
         for o in c2.get_children():
           for r in o.get_references(refs=40):
             #print(dir(r))
             if r.BrowseName.Name == "SubModelType":
                 #print("found SubModel")
                 sm = subModel.fromOPCUANodes(node=o)
                 aas.addSubModel(location="Body",subModel=sm)
    return aas
def del_none(d):
    for key, value in list(list(d.items())):
        if value is None or value ==[]:
            del d[key]
        elif isinstance(value, dict):
            del_none(value)
    return d

def jdefault(o):
    o1 = del_none(o.__dict__)
    if isinstance(o1, set):
        return list(o1)
    return o1

def parseExpressionLogic(input):
    return {
        0: "GREATER_THAN",
        1: "GREATER_EQUAL",
        2: "EQUAL",
        3: "NOT_EQUAL",
        4: "LESS_EQUAL",
        5: "LESS_THAN",
}.get(input, "not defined")

def parseExpressionSemantic(Int):
    return {
        0:"ASSURANCE",
        1:"SETTING",
        2:"MEASUREMENT",
        3:"REQUIREMENT",
}.get(Int, "not defined")

def parseView(Int):
    return {
        0:"BUSINESS",
        1:"CONSTRUCTION",
        2:"POWER",
        3:"FUNCTIONAL",
        4:"LOCATION",
        5:"SECURITY",
        6:"NETWORK",
        7:"LIFECYCLE",
        8:"HUMAN",
}.get(Int, "not defined")

def parseVisibility(Int):
    return {
      0 : "PRIVATE",
      1 : "CONTRACT",
      2 : "PUBLIC"}.get(Int,"not defined")

def parseID(Int):
    return {
      0 : "URI",
      1 : "ISO",
      2 : "Proprietary"}.get(Int,"not defined")






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
  StatusCall = lib.call_CreateAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(AASName_c), c_char_p(AssetIdSpec_c), AssetIdType_c)
  del lib
  return StatusCall

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

def getPVSL(endpointStr,listId):

  client = Client(endpointStr)
  pvsl = None
  try:
    client.connect()
    nsarray = client.get_node(ua.NodeId(2255, 0))
    nsList = nsarray.get_value()
    i=-1
    for entry in nsList:
      i = i + 1
      if entry == "http://acplt.org/propertyValueStatement/Ov":
        #print(entry)
        nsopenaas_propertyValueStatement = i
        break
    if i!= -1:     
      entryPoint = listId
      print("Looking for AAS at entry point %s,%s" % (nsopenaas_propertyValueStatement,listId))
      path = client.get_node(ua.NodeId(entryPoint,nsopenaas_propertyValueStatement))
      pvsl = propertyValueStatementContainer.fromOPCUANodes(path)
      for statement in pvsl.statements:
        print(statement.Name)
  finally:   
    client.disconnect()
    return pvsl  

    
def getSubModel(endpointStr,subModel_NodeId):

  client = Client(endpointStr)
  subModel = None
  try:
    client.connect()
#    nsarray = client.get_node(ua.NodeId(2255, 0))
#    nsList = nsarray.get_value()
#    i=-1
#    for entry in nsList:
#     i = i + 1

#      if entry == "http://acplt.org/openaas/":
#        nsopenaas_subModelType = i
#        
#        break
#    if i!= -1:


#      print("Looking for AAS at entry point %s,%s" % (subModel_NodeId))
 
    path = client.get_node(subModel_NodeId)
    print("path is %s"  % path)
    print ("1")
    subModelInst = subModel.fromOPCUANodes(path) #in line 259, classmethod of 'subModel' exists
    print ("2")                                  #no print out of "2" during tests
    for statement in subModelInst.statements:
      print(statement.Name)
  finally:   
    client.disconnect()
    return subModel  
      
    
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
           print(entry)
           nsopenaas_discovery = i
           break
       if i == -1:
         return res
       getAASEntryPointByAASID_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/NetworkAAS.Body/NameResolution",nsopenaas_discovery)
       getAASEntryPointByAASIDMethod_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/NetworkAAS.Body/NameResolution.GetAASEntryPointByAASIDService",nsopenaas_discovery)
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
           print(entry)
           nsopenaas_discovery = i
           break
       if i == -1:
         return res
       getAASIDFromAssetId_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/NetworkAAS.Body/NameResolution",nsopenaas_discovery)
       getAASIDFromASsetIdMethod_nodeId = ua.NodeId("/TechUnits/openAAS/AASFolder/NetworkAAS.Body/NameResolution.GetAASIDByAssetIDService",nsopenaas_discovery)
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

  del lib
  return StatusCall  
  
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

  ParentIdSpec_c = parent.encode('utf-8')
  ParentIdType_c = c_int(0)

  ModelIdSpec_c = modelIdSpec.encode('utf-8')
  ModelIdType_c = c_int(modelIdType)

  ModelName_c = modelName.encode('utf-8')
  Revision_c = c_int(revision)
  Version_c = c_int(version)

  StatusCall = lib.call_CreateSubModel(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,  c_char_p(ParentIdSpec_c), ParentIdType_c,c_char_p(ModelIdSpec_c), ModelIdType_c, c_char_p(ModelName_c), Revision_c, Version_c)

  del lib
  return StatusCall    


def createSubModel_raw(pathToLibrary, endpointStr, aasIDSpec, aasIDType,  parentIdSpec, parentIdType, modelIdSpec, modelIdType, modelName, revision, version):
  """
  Creates a Sub Model within an Asset Administraiton Shell

  This function creates a sub model within a given Asset administration Shell.

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

  del lib
  return StatusCall  
  
  
def deleteSubModel(pathToLibrary, endpointStr, aasIDSpec, aasIDType, localID):
  """
  Deletes a Sub Model within an Asset Administraiton Shell

  This function deletes a sub model shell within a given Asset administration Shell.

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
  localID : string
    localID of the sub model within the AAS
  Returns:
  -------
  string
    Status Code
  """

  #Parameter parsing
  lib = CDLL(pathToLibrary)

  ip = endpointStr
  AASIdSpec = aasIDSpec
  AASIdType = aasIDType

  SubModelIdSpec = localID
  SubModelIdType = 0

  ip_c = ip.encode('utf-8')
  AASIdSpec_c = AASIdSpec.encode('utf-8')
  AASIdType_c = c_int(AASIdType)

  SubModelIdSpec_c = SubModelIdSpec.encode('utf-8')
  SubModelIdType_c = c_int(SubModelIdType)

  print(ip)
  StatusCall = lib.call_DeleteSubModel(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(SubModelIdSpec_c), SubModelIdType_c)

  del lib
  return StatusCall


def createLCE (pathToLibrary, ip, AASIdSpec, AASIdType, creatingInstanceIdSpec, creatingInstanceIdType, writingInstanceIdSpec, writingInstanceIdType, eventClass, subject, value, valueType):
    """
    Creates a LifeCycleEntry which is stored like Sub Models in the .Body folder within an Asset Administraiton Shell

    This function deletes a sub model shell within a given Asset administration Shell.

    Args:
    ----------
    pathToLibrary : string
      path to the shared object that provides the opc ua functionality
    ip : string
      opc ua endpoint to the aas repository server
    AASIDSpec : string
      asset administration shell id specification
    AASIDType : int 
      asset administration shell id type (URI=0, ISO=1)
    creatingInstanceIdSpec : string
      adress of the instance which creates the LifeCycleEntry
    creatingInstanceIdType : int
      type of creating instance (URI=0, ISO=1)
    writingInstanceIdSpec : string
      adress of the instance which fills the LifeCycleEntry with information
    writingInstanceIdType : int
      type of the writing instance (URI=0, ISO=1)
    eventClass : string
      description of the event which forms the basis of the LifeCycleEntry
    subject : string
      further description of the occured event
    value : string
      stored values in the LCE
    valueType : int
      definition of the Valuetype (1=BOOL; 2=FLOAT; 3=INT32; 4=INT64; 5=UINT32; 6=UINT64; 7=DOUBLE; 8=STRING; 9=DATETIME; 10=IDENTIFICATION
    timeStamp : ???
      source timestamp of the event
    Returns:
    -------
    string
      Status Code
    """
    lib = CDLL(pathToLibrary)
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)

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

    del lib
    return StatusCall


def deleteLCE (pathToLibrary, ip, AASIdSpec, AASIdType, LCEId):
    """
    Creates a LifeCycleEntry which is stored like Sub Models in the .Body folder within an Asset Administraiton Shell

    This function deletes a sub model shell within a given Asset administration Shell.

    Args:
    ----------
    pathToLibrary : string
      path to the shared object that provides the opc ua functionality
    ip : string
      opc ua endpoint to the aas repository server
    AASIDSpec : string
      asset administration shell id specification
    AASIDType : int 
      asset administration shell id type (URI=0, ISO=1)
    LCEId : int
      number of the specific LifeCycleEntry
    Returns:
    -------
    string
      Status Code
    """
    lib = CDLL(pathToLibrary)
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    LCEId_c = c_longlong(int(LCEId))

    StatusCall = lib.call_DeleteLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,LCEId_c)

    del lib
    return StatusCall


def createPVSL(pathToLibrary,endpointStr,AASIdSpec,AASIdType,listName,carrierIdSpec,carrierIdType,parentID):
  """
  Creates a Property Value Statement Container (list) within an Asset Administraiton Shell
  This function creates a property value statement list within a given Asset administration Shell.

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
  carrierIdSpec : string
    specification of the carrier that is related to the statement
  carrierIdType : string
    carrier id type (URI=0, ISO=1)
  parentID : string
    local id of the parent object
  Returns:
  -------
  string
    Status Code
  """

  #Parameter parsing
  pathToLibrary = pathToLibrary
  lib = CDLL(pathToLibrary)
  ip = endpointStr

  parentIdSpec = parentID
  parentIdType = 0

  ip_c = ip.encode('utf-8')
  AASIdSpec_c = AASIdSpec.encode('utf-8')
  AASIdType_c = c_int(AASIdType)

  listName_c = listName.encode('utf-8')
  parentIdType_c = c_int(parentIdType)
  parentIdSpec_c = parentIdSpec.encode('utf-8')
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

  del lib
  return StatusCall

  
  
def deletePVSL(pathToLibrary,endpointStr,AASIdSpec,AASIdType,listIdSpec):
  """
  Deletes a Property Value Statement Container (list) within an Asset Administraiton Shell
  This function deletes a property value statement list within a given Asset administration Shell.

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  endpointStr : string
    opc ua endpoint to the aas repository server
  AASIdSpec : string
    asset administration shell id specification
  AASIdType : int 
    asset administration shell id type (URI=0, ISO=1)
  listIdSpec : string
    specification of the local list id 
  -------
  string
    Status Code
  """


  #Parameter parsing
  lib = CDLL(pathToLibrary)
  ip = endpointStr
  listIdType  = 0

  ip_c = ip.encode('utf-8')
  AASIdSpec_c = AASIdSpec.encode('utf-8')
  AASIdType_c = c_int(AASIdType)

  listIdSpec_c = listIdSpec.encode('utf-8')
  listIdType_c = c_int(listIdType)

  StatusCall = lib.call_DeletePVSL(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(listIdSpec_c),listIdType_c)

  del lib
  return StatusCall



def createPVS (pathToLibrary, ip, AASIdSpec, AASIdType, ListIdType, ListIdSpec, mask, PVSName, expressionLogic, expressionSemantic, Value, valueType, PRIdSpec, PRIdType, view, visibility):
  """
  This function creates a Property Value Statement in a specific PVSL within an Asset Administraiton Shell

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  ip : string
    opc ua endpoint to the aas repository server
  AASIdSpec : string
    asset administration shell id specification
  AASIdType : int 
    asset administration shell id type (URI=0, ISO=1)
  ListIdSpec : string
    specification of the local list id (path on the server)
  ListIdType : int
    type of PVSL (0=URI; 1=ISO)
  expressionLogic : int
    logical expression of PVS; different stadiums possible (compare with function 'TypeToInt_EL' line 76)
  expressionSemantic : int
    semantic expression of PVS; different stadiums possible (compare with function 'TypeToInt_ES' line 87)
  Value : string
    value of the property value statement
  valueType : int
    type of value
  PRIdSpec : string (?)
    link to definition of a property dictionary
  PRIdType : int (?)
    type of property ID (0=URI;
  view : string
    distinction of different groups of value statements (e.g. "BUSINESS", "HUMAN", "FUNCTIONAL" compare with function 'TypeToInt_view'
  visibility : int
    stage of visibility of the PVS ("PRIVATE", "CONTRACT", "PUBLIC")
  mask : int
    choose which attributes are set (1=carrierId ; 2=expressionLogic ; 4=expressionSemantic ; 8=propertyId ; 16=view ; 32=visibility)
  -------
  string
    Status Code
  """
  
  pathToLibrary = pathToLibrary
  lib = CDLL(pathToLibrary)
  ip_c = ip.encode('utf-8')

  AASIdSpec_c = AASIdSpec.encode('utf-8')
  AASIdType_c = c_int(AASIdType)
  ListIdSpec_c = ListIdSpec.encode('utf-8')
  ListIdType_c = c_int(ListIdType)
  #PVSLName_c = PVSLName.encode('utf-8')

  #SubModelIdSpec_c = SubModelIdSpec.encode('utf-8')
  #SubModelIdType_c = c_int(SubModelType)

  #dont use the carrier Id in this case
  CarrierIdSpec_c = ListIdSpec_c  # dummy
  CarrierIdType_c = ListIdType_c  # dummy

  PVSName_c = PVSName.encode('utf-8')
  expressionLogic_c = c_int(TypeToInt_EL(expressionLogic))
  expressionSemantic_c = c_int(TypeToInt_ES(expressionSemantic))
  print("expression Semantic %s" % expressionSemantic_c)
  Value_c = Value.encode('utf-8')
  valueType_c = c_int(TypeToInt_valueType(valueType))
  PRIdSpec_c = PRIdSpec.encode('utf-8')
  PRIdType_c = c_int(TypeToInt_Id(PRIdType))
  view_c = c_int(TypeToInt_view(view))
  visibility_c = c_int(TypeToInt_VIS(visibility))
  mask_c = c_int(mask)
  print(mask_c)

  StatusCall = lib.call_CreatePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(ListIdSpec_c),ListIdType_c, c_char_p(PVSName_c), c_char_p(Value_c),valueType_c,mask_c,c_char_p(CarrierIdSpec_c),CarrierIdType_c,expressionLogic_c,expressionSemantic_c,c_char_p(PRIdSpec_c),PRIdType_c,view_c,visibility_c)

  del lib
  return StatusCall




def deletePVS (pathToLibrary, ip, AASIdSpec, AASIdType, PVSName):
  """
  With this function a specific Property Value Statement can be deleted

  Args:
  ----------
  pathToLibrary : string
    path to the shared object that provides the opc ua functionality
  ip : string
    opc ua endpoint to the aas repository server
  AASIdSpec : string
    asset administration shell id specification
  AASIdType : int 
    asset administration shell id type (URI=0, ISO=1)
  PVSName = string
    name of the property value statement (contains the path on the server)
  -------
  string
    Status Code
  """

  lib = CDLL(pathToLibrary)
  ip_c = ip.encode('utf-8')
  AASIdSpec_c = AASIdSpec.encode('utf-8')
  AASIdType_c = c_int(AASIdType)
  PVSName_c = PVSName.encode('utf-8')

  print(AASIdType)
  print (TypeToInt_Id(AASIdType))
  print (AASIdType_c)
  StatusCall = lib.call_DeletePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(PVSName_c),c_int(0))
  
  del lib
  return StatusCall

  

  
def serialize_AAS(endpointStr, identifierType,identifer,namespaceIndex,filename):
  """
  Serializes an Asset Administraiton Shell to a given file

  Args:
  ----------
  endpoint : string
    path to the shared object that provides the opc ua functionality
  identifierType : string
    opc ua endpoint to the aas repository server
  identifer:
    identifier for a node Id
  namespaceIndex
    opc ua namespace index 
  Returns:
  -------
  string
    Status Code
  """  
  
  client = Client(endpointStr)
  try:
    client.connect()
    nsarray = client.get_node(ua.NodeId(2255, 0))
    nsList = nsarray.get_value()
    i=-1
    for entry in nsList:
      i = i + 1
      if entry == "http://acplt.org/openaas/Ov":
        #print(entry)
        namespaceIndex = i
        break
    if i!= -1:     
      print("Looking for AAS at entry point %s,%s" % (namespaceIndex,identifer))
      path = client.get_node(ua.NodeId(identifer,namespaceIndex))
      aas = AAS.fromOPCUANodes(node=path)
      #currentDir = cwd = os.getcwd()
      #filename = "JSON_%s" % (filename)
      #print("Writing AAS data of %s in %s" % (aas.Name, os.path.join(currentDir,filename)))
      file = open(filename,"w")
      
      file.write(json.dumps(aas, default=jdefault,indent=4))

      file.close()
  finally:   
    client.disconnect() 
