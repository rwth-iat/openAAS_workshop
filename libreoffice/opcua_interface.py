from ctypes import *
import datetime
from datetime import *
import time
import uno

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
    }.get(Int, "Not Defined")
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
    }.get(Int, "Not Defined")
########################
#data type(value type)
def TypeToInt_valueType(typ):
    typ = typ.upper()
    return {
        "INTEGER": 0,
        "DOUBLE": 1,
        "STRING":2,
        "DATETIME":3,
    }.get(typ, 99) 

def IntToType_valueType(Int):
    return {
        0:"INTEGER",
        1:"DOUBLE",
        2:"STRING",
        3:"DATETIME",
    }.get(Int, "Not Defined")
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
    }.get(Int, "Not Defined")
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
    }.get(typ, "Not Defined") 
def call_createAAS(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B4").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B5").String
    AASIdSpec = oSheet.getCellRangeByName("B6").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B7").String)

    AASName = oSheet.getCellRangeByName("B8").String

    AssetIdSpec = oSheet.getCellRangeByName("B9").String
    AssetIdType = TypeToInt_Id(oSheet.getCellRangeByName("B10").String)

    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    AASName_c = AASName.encode('utf-8')
    AssetIdSpec_c = AssetIdSpec.encode('utf-8')
    AssetIdType_c = c_int(AssetIdType)
    StatusCall = lib.call_CreateAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(AASName_c), c_char_p(AssetIdSpec_c), AssetIdType_c)
    oSheet.getCellRangeByName("B11").String = StatusCall
    del lib
    return None

def call_deleteAAS(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B4").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B16").String
    AASIdSpec = oSheet.getCellRangeByName("B17").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B18").String)

 #   AASName = oSheet.getCellRangeByName("B8").String

 #   AssetIdSpec = oSheet.getCellRangeByName("B9").String
 #   AssetIdType = TypeToInt_Id(oSheet.getCellRangeByName("B10").String)

    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
#    AASName_c = AASName.encode('utf-8')
#    AssetIdSpec_c = AssetIdSpec.encode('utf-8')
#    AssetIdType_c = c_int(AssetIdType)
    StatusCall = lib.call_DeleteAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c)
    oSheet.getCellRangeByName("B19").String = StatusCall
    del lib
    return None


def call_createLCE(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    #row counter
    count = oSheet.getCellRangeByName("B20").Value
    if count < 0:
        oSheet.getCellRangeByName("B20").Value = "#entries can't be negative!"
    i = count%11

    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B3").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B4").String
    AASIdSpec = oSheet.getCellRangeByName("B5").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B6").String)
    #oSheet.getCellRangeByName("B6").String = AASIdType
	
    creatingInstanceIdSpec = oSheet.getCellByPosition(0,i+7).String #i starts with 0, i+7 is the first entry
    creatingInstanceIdType = TypeToInt_Id(oSheet.getCellByPosition(1,i+7).String)
    writingInstanceIdSpec = oSheet.getCellByPosition(2,i+7).String
    writingInstanceIdType = TypeToInt_Id(oSheet.getCellByPosition(3,i+7).String)
    
    eventClass = oSheet.getCellByPosition(4,i+7).String
    subject = oSheet.getCellByPosition(5,i+7).String

    value = oSheet.getCellByPosition(7,i+7).String
    valueType = TypeToInt_Id(oSheet.getCellByPosition(8,i+7).String)

    #type conversion
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdTypeInt_c = c_int(AASIdType)

    creatingInstanceIdSpec_c = creatingInstanceIdSpec.encode('utf-8')
    creatingInstanceIdType_c = c_int(creatingInstanceIdType)
    writingInstanceIdSpec_c = writingInstanceIdSpec.encode('utf-8')
    writingInstanceIdType_c = c_int(writingInstanceIdType)

    eventClass_c = eventClass.encode('utf-8')
    subject_c = subject.encode('utf-8')

    value_c = value.encode('utf-8')
    valueType_c = c_int(valueType)  

    StatusCall = lib.call_CreateLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdTypeInt_c, c_char_p(creatingInstanceIdSpec_c), creatingInstanceIdType_c, c_char_p(writingInstanceIdSpec_c), writingInstanceIdType_c, c_char_p(eventClass_c), c_char_p(subject_c), c_int(int(time.time()*10000000+116444736000000000)), c_char_p(value_c), valueType_c)
    #time string
    oSheet.getCellByPosition(6,i+7).String = str(datetime.utcnow())
    oSheet.getCellByPosition(9,i+7).String = StatusCall #this is col_J
    oSheet.getCellRangeByName("B20").Value = count+1 #increment number of entries
    del lib
    return None

def call_deleteLCE(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B3").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B4").String
    AASIdSpec = oSheet.getCellRangeByName("B5").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B6").String)

    LCEId = oSheet.getCellRangeByName("B24").String
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    LCEId_c = c_longlong(int(LCEId))
 
    StatusCall = lib.call_DeleteLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,LCEId_c)
    oSheet.getCellRangeByName("B25").String = StatusCall
    del lib
    return None

def call_createLCE1(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B4").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B5").String
    AASIdSpec = oSheet.getCellRangeByName("B6").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B7"))

    creatingInstanceIdSpec = oSheet.getCellRangeByName("B8").String
    creatingInstanceIdType = TypeToInt_Id(oSheet.getCellRangeByName("B9").String)
    writingInstanceIdSpec = oSheet.getCellRangeByName("B10").String
    writingInstanceIdType = TypeToInt_Id(oSheet.getCellRangeByName("B11").String)
    
    eventClass = oSheet.getCellRangeByName("B12").String
    subject = oSheet.getCellRangeByName("B13").String
    #time string conversion?
    timeStamp = TypeToInt_Id(oSheet.getCellRangeByName("B14").String)
    value = oSheet.getCellRangeByName("B15").String
    valueType = TypeToInt_Id(oSheet.getCellRangeByName("B16").String)

    #type conversion
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)

    creatingInstanceIdSpec_c = creatingInstanceIdSpec.encode('utf-8')
    creatingInstanceIdType_c = c_int(creatingInstanceIdType)
    writingInstanceIdSpec_c = writingInstanceIdSpec.encode('utf-8')
    writingInstanceIdType_c = c_int(writingInstanceIdType)

    eventClass_c = eventClass.encode('utf-8')
    subject_c = subject.encode('utf-8')
    timeStamp_c = c_int(timeStamp)
    value_c = value.encode('utf-8')
    valueType_c = c_int(valueType)  

    StatusCall = lib.call_CreateLCE(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(creatingInstanceIdSpec_c), creatingInstanceIdType_c, c_char_p(writingInstanceIdSpec_c), writingInstanceIdType_c, c_char_p(eventClass_c), c_char_p(subject_c), timeStamp_c, c_char_p(value_c), valueType_c)

    oSheet.getCellRangeByName("B17").String = StatusCall
    del lib
    return None


def call_createPVSL(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B4").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B5").String
    AASIdSpec = oSheet.getCellRangeByName("B6").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B7").String)
    listName = oSheet.getCellRangeByName("B8").String
    carrierIdSpec = oSheet.getCellRangeByName("B9").String
    carrierIdType = TypeToInt_Id(oSheet.getCellRangeByName("B10").String)
    creatingInstanceSpec = oSheet.getCellRangeByName("B11").String
    creatingInstanceType = TypeToInt_Id(oSheet.getCellRangeByName("B12").String)
    
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    listName_c = listName.encode('utf-8')
    carrierIdSpec_c = carrierIdSpec.encode('utf-8')
    carrierIdType_c = c_int(carrierIdType)
    
    creatingInstanceIdType_c = c_int(creatingInstanceType)
    creatingInstanceSpec_c = creatingInstanceSpec.encode('utf-8')
    
    StatusCall = 0
    print(carrierIdSpec)
    StatusCall = lib.call_CreatePVSL(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(listName_c),c_char_p(carrierIdSpec_c),carrierIdType_c,c_char_p(creatingInstanceSpec_c),creatingInstanceIdType_c)
    oSheet.getCellRangeByName("B13").String = StatusCall
    del lib
    return None



def call_deletePVSL(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B4").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B16").String
    AASIdSpec = oSheet.getCellRangeByName("B17").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B18").String)
    name = oSheet.getCellRangeByName("B17").String

    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    name_c = name.encode('utf-8')
 
    StatusCall = lib.call_DeletePVSL(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(name_c))
    oSheet.getCellRangeByName("B20").String = StatusCall
    del lib
    return None



def call_createPVS(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    

    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B2").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B3").String
    AASIdSpec = oSheet.getCellRangeByName("B4").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B5").String)

    PVSLName = oSheet.getCellRangeByName("B6").String
    PVSLName_c = PVSLName.encode('utf-8')
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    
    offsetY = 8

    for i in range(5):

        #hierarchy
        PVSName = oSheet.getCellByPosition(1,i+offsetY).String 
        if len(PVSName)==0:
            break
        PRIdType = oSheet.getCellByPosition(2,i+offsetY).String
        if len(PRIdType)==0:
            break
        PRIdSpec = oSheet.getCellByPosition(3,i+offsetY).String
        if len(PRIdSpec)==0:
            break
        #definition: get from website?
        unit = oSheet.getCellByPosition(5,i+offsetY).String
        valueType = oSheet.getCellByPosition(6,i+offsetY).String 
        if len(valueType)==0:
            break
        #valuelist
        Value = oSheet.getCellByPosition(8,i+offsetY).String 
        if len(Value)==0:
            break
        expressionSemantic = oSheet.getCellByPosition(9,i+offsetY).String
        if len(expressionSemantic)==0:
            break
        expressionLogic = oSheet.getCellByPosition(10,i+offsetY).String 
        if len(expressionLogic)==0:
            break
        view = oSheet.getCellByPosition(11,i+offsetY).String    
        if len(view)==0:
            break
        visibility = oSheet.getCellByPosition(12,i+offsetY).String   
        if len(visibility)==0:
            break

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
        
        StatusCall = lib.call_CreatePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c),AASIdType_c,c_char_p(PVSLName_c), c_char_p(PVSName_c),expressionLogic_c,expressionSemantic_c, c_char_p(Value_c),valueType_c,c_char_p(unit_c),c_char_p(PRIdSpec_c),PRIdType_c,view_c,visibility_c)
        if(StatusCall!=0):
          break
        oSheet.getCellRangeByName("B16").Value = i+1
    del lib
    return None
  
def call_deletePVS(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B2").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B3").String
    PVSLName = oSheet.getCellRangeByName("B6").String
    PVSLName_c = PVSLName.encode('utf-8')
    AASIdSpec = oSheet.getCellRangeByName("B4").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B5").String)
    PVSName = oSheet.getCellRangeByName("Q8").String
    PVSName_c = PVSName.encode('utf-8') 
    
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)

    StatusCall = lib.call_DeletePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c,c_char_p(PVSLName_c),c_char_p(PVSName_c))
    print(StatusCall)
    del lib
    return None
  
  
  
  
def getPVS(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    ip = oSheet.getCellRangeByName("B22").String
    AASIdSpec = oSheet.getCellRangeByName("B23").String
    AASIdType = TypeToInt_Id(oSheet.getCellRangeByName("B24").String)
    PVSLName = oSheet.getCellRangeByName("B25").String
    pathToLibrary = oSheet.getCellRangeByName("B2").String
    lib = CDLL(pathToLibrary)
    
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdTypeInt_c = c_int(AASIdType)
    PVSLName_c = PVSLName.encode('utf8')
    propertyValueStatements = c_void_p()
    
    count = lib.getPVSFromListByName(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdTypeInt_c, c_char_p(PVSLName_c), byref(propertyValueStatements))
    #oSheet.getCellByPosition(0,42).String = count

        
    if count > 0:   
        PVSArray = POINTER(PVS * count)
        pvs_array = PVSArray.from_address(addressof(propertyValueStatements))
        print_start_x = 0
        print_start_y = 27
        
        
        n = 0
        while (len(oSheet.getCellByPosition(print_start_x+0,print_start_y+n).String) > 0):
          n = n + 1
        for i in range(n):
          for j in range(15):
            oSheet.getCellByPosition(print_start_x+j,print_start_y+i).String=""
        for i in range(count):
            oSheet.getCellByPosition(print_start_x+0,print_start_y+i).String = "-"
            oSheet.getCellByPosition(print_start_x+1,print_start_y+i).String = pvs_array.contents[i].name
            oSheet.getCellByPosition(print_start_x+2,print_start_y+i).String = IntToType_Id(pvs_array.contents[i].IDIdType)
            oSheet.getCellByPosition(print_start_x+3,print_start_y+i).String = pvs_array.contents[i].IDIdSpec
            oSheet.getCellByPosition(print_start_x+4,print_start_y+i).String = "-"

            oSheet.getCellByPosition(print_start_x+5,print_start_y+i).String = pvs_array.contents[i].unit
            oSheet.getCellByPosition(print_start_x+6,print_start_y+i).String = IntToType_valueType(pvs_array.contents[i].valueType)
            oSheet.getCellByPosition(print_start_x+7,print_start_y+i).String = "-"
            oSheet.getCellByPosition(print_start_x+8,print_start_y+i).String = pvs_array.contents[i].value
            oSheet.getCellByPosition(print_start_x+9,print_start_y+i).String = IntToType_ES(pvs_array.contents[i].expressionSemantic)
            oSheet.getCellByPosition(print_start_x+10,print_start_y+i).String = IntToType_EL(pvs_array.contents[i].expressionLogic)
            oSheet.getCellByPosition(print_start_x+11,print_start_y+i).String = IntToType_view(pvs_array.contents[i].view)
            oSheet.getCellByPosition(print_start_x+12,print_start_y+i).String = IntToType_VIS(pvs_array.contents[i].visibility)
            oSheet.getCellByPosition(print_start_x+13,print_start_y+i).String = "-"            
            oSheet.getCellByPosition(print_start_x+14,print_start_y+i).String = "-"           
    del lib
    return None
