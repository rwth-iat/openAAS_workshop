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

    creatingInstanceIdSpec = oSheet.getCellRangeByName("B8").String
    creatingInstanceIdType = TypeToInt(oSheet.getCellRangeByName("B9"))
    writingInstanceIdSpec = oSheet.getCellRangeByName("B10").String
    writingInstanceIdType = TypeToInt(oSheet.getCellRangeByName("B11"))
    
    eventClass = oSheet.getCellRangeByName("B12").String
    subject = oSheet.getCellRangeByName("B13").String
    #time string conversion?
    timeStamp = TypeToInt(oSheet.getCellRangeByName("B14").String)
    value = oSheet.getCellRangeByName("B15").String
    valueType = TypeToInt(oSheet.getCellRangeByName("B16").String)

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

