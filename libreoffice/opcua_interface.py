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
    ip = oSheet.getCellRangeByName("A10").String
    AASIdType = TypeToInt(oSheet.getCellRangeByName("B2"))
    AASIdSpec = oSheet.getCellRangeByName("B3").String

    AssetIdType = TypeToInt(oSheet.getCellRangeByName("C2"))
    AssetIdSpec = oSheet.getCellRangeByName("C3").String

    AASName = oSheet.getCellRangeByName("D2").String
    Output = oSheet.getCellRangeByName("A5").String

    pathToLibrary = oSheet.getCellRangeByName("A8").String
    lib = CDLL(pathToLibrary)
    
 

    AASIdTypeInt = TypeToInt(AASIdType)
    AssetIdTypeInt = TypeToInt(AssetIdType)

    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdTypeInt_c = c_int(AASIdTypeInt)
    AASName_c = AASName.encode('utf-8')
    AssetIdSpec_c = AssetIdSpec.encode('utf-8')
    AssetIdTypeInt_c = c_int(AssetIdTypeInt)
    StatusCall = lib.call_CreateAAS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdTypeInt_c, AASName_c, c_char_p(AssetIdSpec_c), AssetIdTypeInt_c)
    oSheet.getCellRangeByName("A5").String = StatusCall
    del lib
    return None


