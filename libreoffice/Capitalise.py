#Create AAS
from ctypes import *
import uno

def TypeToInt(type):
    #URI=0;ISO=1
    if type == "URI":
        return 0
    return 1

def call_create(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B4").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B5").String
    AASIdSpec = oSheet.getCellRangeByName("B6").String
    AASIdType = TypeToInt(oSheet.getCellRangeByName("B7"))

    AASName = oSheet.getCellRangeByName("B8").String

    AssetIdSpec = oSheet.getCellRangeByName("B9").String
    AssetIdType = TypeToInt(oSheet.getCellRangeByName("B10"))

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

