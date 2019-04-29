# -*- coding: cp936 -*-

#fpyΪint�͵�Ƶ�ȣ�fpyLΪint�͵�Ƶ�ȵĶ�����λ����ת��Ϊ�ַ�����
def get_fpyL(fpy):

    fpyL = ord("A") - 1
    i = fpy
    
    while i:
        fpyL += 1
        i = i / 2

    return chr(fpyL)



#f1��ԭʼ���ָ�����ݼ�¼�ĵ���f2��Ƶ�������¼�ĵ�
def analys_fpy(f1, f2):

    ord_fqy_dict = {'none':0}

    for line in f1.readlines():
        line_str = line[0: len(line) - 1]
        if ord_fqy_dict.has_key(line_str) == True:        
            ord_fqy_dict[line_str] += 1;

        else:
            ord_fqy_dict[line_str] = 1


    for key in ord_fqy_dict:
        if key == 'none':
            continue
        write_str = key + ("_") + get_fpyL(ord_fqy_dict[key]) + '\n'
        f2.write(write_str)



        
#��ȡ������ָ�������������ָ��д��fp��
def get_orgin_ord(fp):

    ea = MinEA() #exe��ʼ��ַ
    ea_end = MaxEA() #��ֹ��ַ

    print_flag = 0; #ָ���Ƿ�������������Ϊ1����Ϊ0

    while ea <= ea_end: #����ʼ��ַ��������ֹ��ַ
        #��÷������ַ��� eg:push 0
        disassembled_str = idc.GetDisasm(ea)
        disassembled_name = idc.GetMnem(ea) #ָ�����ƣ����ָ����db��dd֮��ķǻ��ָ�����""��
            
        if ("" == disassembled_name):
            ea = idc.NextHead(ea)
            continue

        '''�������������,����
        #��ò�����������ͨ����, ������:��n�����ž���n+1����������
        operands_sum = disassembled_str.count(', ', 0, len(disassembled_str)) + 1
        '''

        #print disassembled_str + "   ====   " + str(operands_sum)
        
        print_str = disassembled_name #��ʼ������ַ���
        
        #��ȡ�������е�������
        i =0
        while True:
            opnd = idc.GetOpnd(ea, i)
            if (idc.GetOpType(ea, i) == o_imm):#��������������͹�������ַ���
                print_flag = 1
                print_str = print_str + '_' + (opnd)
            #�����ҵ��ˣ�GetOpType�������һ��������֮���index����0��Ҳ����Ϊʲô����û�в�������ָ���0��ԭ��
            elif (idc.GetOpType(ea, i) == 0):#���������typeΪ0���Ͳ��ٽ�����һ������
                break

            i += 1
        
        if print_flag == 1:#����������������
            fp.write(print_str + '\n')
            #print print_str    
            
        print_flag = 0
        
        # ȡ��һ��ָ���ַ
        ea = idc.NextHead(ea)




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
        if (flags & FUNC_LIB) and (idc.GetFunctionName(func)):#��ʾ��������ΪFUNC_LIB
            fp.write( idc.GetFunctionName(func) + '\n')
        
#=====================================================================
import os
gpus = GetInputFilePath()
gpus = gpus[0: len(gpus) - 4]
print gpus

if not os.path.exists(gpus):
    os.makedirs(gpus)

Wait()

#��ȡ���ָ����Ϣ
fp = open(gpus + "\\1.txt", "w")
get_orgin_ord(fp)
fp.close()

#����Ƶ�ȼ�¼�ļ�            
f1 = open(gpus + "\\1.txt", "r")
f2 = open(gpus + "\\ord_fpy.txt", "w")
analys_fpy(f1, f2);
f2.close()
f1.close()
os.remove(gpus + "\\1.txt")


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

Exit(0)
