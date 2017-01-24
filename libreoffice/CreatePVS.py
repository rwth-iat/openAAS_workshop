from ctypes import *
import uno
import sys
sys.path.append("/usr/lib/libreoffice/share/Scripts/python")
from TypeConversion import *

def call_create(self):
    oDoc = XSCRIPTCONTEXT.getDocument()
    oSheet = oDoc.CurrentController.ActiveSheet
    
    #row counter
    i = oSheet.getCellRangeByName("B20").Value
    #Parameter parsing
    pathToLibrary = oSheet.getCellRangeByName("B2").String
    lib = CDLL(pathToLibrary)

    ip = oSheet.getCellRangeByName("B3").String
    AASIdType = TypeToInt(oSheet.getCellRangeByName("B4"))
    AASIdSpec = oSheet.getCellRangeByName("B5").String
    PVSLName = oSheet.getCellRangeByName("B6").String

    #quasi interation
    #position begins with 0. E.g.: A8=0,7
    PVSName = oSheet.getCellByPosition(0,i+7).String #i starts with 0, i+7 is the first entry
    RE = oSheet.getCellByPosition(1,i+7).String #this is col B (relational expr)
    ES = oSheet.getCellByPosition(2,i+7).String
    Value = oSheet.getCellByPosition(3,i+7).String
    VT = oSheet.getCellByPosition(4,i+7).String #type to int?
    Unit = oSheet.getCellByPosition(5,i+7).String
    PRIdSpec = oSheet.getCellByPosition(6,i+7).String
    PRIdType = TypeToInt(oSheet.getCellByPosition(7,i+7).String)
    View = oSheet.getCellByPosition(8,i+7).String    

    #type conversion
    ip_c = ip.encode('utf-8')
    AASIdSpec_c = AASIdSpec.encode('utf-8')
    AASIdType_c = c_int(AASIdType)
    PVSLName_c = PVSLName.encode('utf-8')
    PVSName_c = PVSName.encode('utf-8') #name
    RE_c = c_int(TypeToInt_RE(RE)) #re
    ES_c = c_int(TypeToInt_ES(ES)) #es
    Value_c = Value.encode('utf-8') #val
    VT_c = c_int(TypeToInt_VT(VT)) #val type
    Unit_c = Unit.encode('utf-8') #unit
    PRIdSpec_c = PRIdSpec.encode('utf-8') #pr
    PRIdType_c = c_int(TypeToInt(PRIdType))
    View_c = c_int(TypeToInt_View(View)) #view

    StatusCall = lib.call_CreatePVS(c_char_p(ip_c), c_char_p(AASIdSpec_c), AASIdType_c, c_char_p(PVSLName_c), c_char_p(PVSName_c), RE_c, ES_c, c_char_p(Value_c), VT_c, c_char_p(Unit_c), c_char_p(PRIdSpec_c), PRIdType_c, View_c)

    oSheet.getCellByPosition(9,i+7).String = StatusCall #this is col_J
    oSheet.getCellRangeByName("B20").Value = i+1 #increment number of entries
    del lib
    return None

