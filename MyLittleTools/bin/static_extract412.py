# -*- coding: cp936 -*-

#fpy为int型的频度，fpyL为int型的频度的二进制位数，转换为字符返回
def get_fpyL(fpy):

    fpyL = ord("A") - 1
    i = fpy
    
    while i:
        fpyL += 1
        i = i / 2

    return chr(fpyL)



#f1是原始汇编指令数据记录文档，f2是频度整理记录文档
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



        
#获取程序汇编指令，将有立即数的指令写入fp中
def get_orgin_ord(fp):

    ea = MinEA() #exe起始地址
    ea_end = MaxEA() #终止地址

    print_flag = 0; #指令是否有立即数，有为1，无为0

    while ea <= ea_end: #从起始地址遍历到终止地址
        #获得反汇编的字符串 eg:push 0
        disassembled_str = idc.GetDisasm(ea)
        disassembled_name = idc.GetMnem(ea) #指令名称（如果指令是db、dd之类的非汇编指令，返回""）
            
        if ("" == disassembled_name):
            ea = idc.NextHead(ea)
            continue

        '''这个方法不靠谱,放弃
        #获得操作数个数（通过‘, ’个数:有n个逗号就有n+1个操作数）
        operands_sum = disassembled_str.count(', ', 0, len(disassembled_str)) + 1
        '''

        #print disassembled_str + "   ====   " + str(operands_sum)
        
        print_str = disassembled_name #初始化输出字符串
        
        #获取操作数中的立即数
        i =0
        while True:
            opnd = idc.GetOpnd(ea, i)
            if (idc.GetOpType(ea, i) == o_imm):#如果有立即数，就构造输出字符串
                print_flag = 1
                print_str = print_str + '_' + (opnd)
            #终于找到了：GetOpType会在最后一个操作数之后的index返回0（也就是为什么对于没有操作数的指令返回0的原因）
            elif (idc.GetOpType(ea, i) == 0):#如果操作数type为0，就不再进行下一个搜索
                break

            i += 1
        
        if print_flag == 1:#如果有立即数就输出
            fp.write(print_str + '\n')
            #print print_str    
            
        print_flag = 0
        
        # 取下一条指令地址
        ea = idc.NextHead(ea)




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
        if (flags & FUNC_LIB) and (idc.GetFunctionName(func)):#表示函数属性为FUNC_LIB
            fp.write( idc.GetFunctionName(func) + '\n')
        
#=====================================================================
import os
gpus = GetInputFilePath()
gpus = gpus[0: len(gpus) - 4]
print gpus

if not os.path.exists(gpus):
    os.makedirs(gpus)

Wait()

#获取汇编指令消息
fp = open(gpus + "\\1.txt", "w")
get_orgin_ord(fp)
fp.close()

#构建频度记录文件            
f1 = open(gpus + "\\1.txt", "r")
f2 = open(gpus + "\\ord_fpy.txt", "w")
analys_fpy(f1, f2);
f2.close()
f1.close()
os.remove(gpus + "\\1.txt")


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

Exit(0)
