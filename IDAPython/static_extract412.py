# -*- coding: cp936 -*-
'''
IDApython脚本
功能：提取程序静态信息，包括PE头信息、dll名称、api名称、字符串、库函数，
      提取结果保存在程序的同目录同名文件夹下，分别命名为PEstr.txt，dll.txt，api.txt，str.txt
参数：无
使用：IDAPython运行脚本文件即可      
'''

#PE头信息
import idautils

def get_PE_Header(fp):

    #程序类型
    file_type = idaapi.get_file_type_name()
    fp.write(file_type + '\n')
    
    #段名
    for seg in idautils.Segments(): #idautils.Segments：Get list of segments (sections) in the binary image
        fp.write(idc.SegName(seg) + '\n')




#获取dll存在f1中，获取api存在fg中
import idaapi

def imp_cb(ea, name, ord):
    if name:
        write_str = "%s\n" % name
        fg.write(write_str)
    # True -> Continue enumeration
    # False -> Stop enumeration
    return True #不return TRUE的话，会默认是False，停止遍历


def get_dll_api(f1):
    nimps = idaapi.get_import_module_qty()

    global fg
    fg = open(gpus + "\\api.txt", "w")
    #print "Found %d import(s)..." % nimps

    for i in xrange(0, nimps):
        name = idaapi.get_import_module_name(i)
        if not name:
            #print "Failed to get import module name for #%d" % i
            continue

        f1.write(name + '\n')
        idaapi.enum_import_names(i, imp_cb) #imp_cb是由enum_import_names回调的规定的函数，有规定的参数和返回值




#获取字符串
def get_str(fp):
    for item in Strings():
        fp.write("%s\n"%item)



#获取库函数
import idc, idautils
def get_lib_func(fp):    
    for func in idautils.Functions(MinEA(), MaxEA()):
        flags = idc.GetFunctionFlags(func)
        # Ignore library functons
        if (idc.GetFunctionName(func)):#表示函数属性为FUNC_LIB(flags & FUNC_LIB) and ;现在是获得所有函数名
            fp.write( idc.GetFunctionName(func) + '\n')
        
#=====================================================================
import os
gpus = GetInputFilePath()
gpus = gpus[0: len(gpus) - 4]
print gpus

if not os.path.exists(gpus):
    os.makedirs(gpus)

Wait()

#获取程序PE结构中的字符串信息
f3 = open(gpus + "\\PEStr.txt", "w")
get_PE_Header(f3)
f3.close()

#获取dll（4）和api（5）
f4 = open(gpus + "\\dll.txt", "w")
get_dll_api(f4)
f4.close()
fg.close()

#获取str
f6 = open(gpus + "\\str.txt", "w")
get_str(f6)
f6.close()

#获取库函数
f7 = open(gpus + "\\libfunc.txt", "w")
get_lib_func(f7)
f7.close()


