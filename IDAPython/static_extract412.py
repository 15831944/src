# -*- coding: cp936 -*-
'''
IDApython�ű�
���ܣ���ȡ����̬��Ϣ������PEͷ��Ϣ��dll���ơ�api���ơ��ַ������⺯����
      ��ȡ��������ڳ����ͬĿ¼ͬ���ļ����£��ֱ�����ΪPEstr.txt��dll.txt��api.txt��str.txt
��������
ʹ�ã�IDAPython���нű��ļ�����      
'''

#PEͷ��Ϣ
import idautils

def get_PE_Header(fp):

    #��������
    file_type = idaapi.get_file_type_name()
    fp.write(file_type + '\n')
    
    #����
    for seg in idautils.Segments(): #idautils.Segments��Get list of segments (sections) in the binary image
        fp.write(idc.SegName(seg) + '\n')




#��ȡdll����f1�У���ȡapi����fg��
import idaapi

def imp_cb(ea, name, ord):
    if name:
        write_str = "%s\n" % name
        fg.write(write_str)
    # True -> Continue enumeration
    # False -> Stop enumeration
    return True #��return TRUE�Ļ�����Ĭ����False��ֹͣ����


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
        idaapi.enum_import_names(i, imp_cb) #imp_cb����enum_import_names�ص��Ĺ涨�ĺ������й涨�Ĳ����ͷ���ֵ




#��ȡ�ַ���
def get_str(fp):
    for item in Strings():
        fp.write("%s\n"%item)



#��ȡ�⺯��
import idc, idautils
def get_lib_func(fp):    
    for func in idautils.Functions(MinEA(), MaxEA()):
        flags = idc.GetFunctionFlags(func)
        # Ignore library functons
        if (idc.GetFunctionName(func)):#��ʾ��������ΪFUNC_LIB(flags & FUNC_LIB) and ;�����ǻ�����к�����
            fp.write( idc.GetFunctionName(func) + '\n')
        
#=====================================================================
import os
gpus = GetInputFilePath()
gpus = gpus[0: len(gpus) - 4]
print gpus

if not os.path.exists(gpus):
    os.makedirs(gpus)

Wait()

#��ȡ����PE�ṹ�е��ַ�����Ϣ
f3 = open(gpus + "\\PEStr.txt", "w")
get_PE_Header(f3)
f3.close()

#��ȡdll��4����api��5��
f4 = open(gpus + "\\dll.txt", "w")
get_dll_api(f4)
f4.close()
fg.close()

#��ȡstr
f6 = open(gpus + "\\str.txt", "w")
get_str(f6)
f6.close()

#��ȡ�⺯��
f7 = open(gpus + "\\libfunc.txt", "w")
get_lib_func(f7)
f7.close()


