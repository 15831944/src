#!/usr/bin/python
# -*- coding: UTF-8 -*-

import time
import os

#1.
def SetAMTimeToCTime(file):
    os.utime(file, (os.path.getctime(file),os.path.getctime(file)))
    
def SetCTimeToMTime(file):
    os.utime(file, (os.path.getmtime(file),os.path.getmtime(file)))
    
def ChangeFilesMTime2CTime(dir_path):
    for root, dirs, files in os.walk(dir_path):
        for item in files:
            SetAMTimeToCTime(os.path.join(root,item))

#2.
def GetMTimeStr(file):    
    return time.strftime('%Y-%m-%d',time.gmtime(os.path.getmtime(file)))

def ChangeFileTimeTitle(file_path, time_str):
    if time_str in file_path:
        print("[+]Change Front : " + os.path.basename(file_path) + "- No Update")
        return
    
    bak_path = file_path + '.bak'
    os.rename(file_path, bak_path)
    fr = open(bak_path, 'r', encoding = "UTF-8")
    r_lines = fr.readlines()
    fr.close()

    fw = open(file_path, 'w', encoding = "UTF-8")
    for line in r_lines:
        if 'date:' == line[0:5]:
            new_line = 'date: ' + time_str + ' 8:8:8\n'
            fw.write(new_line)
        else:
            fw.write(line)
    fw.close()

    os.remove(bak_path)
    print("[+]" + os.path.basename(file_path) + "  FRONT CHANGED.")


def ChangeFileNameStr(file_path, time_str):
    if time_str in file_path:
        print("[+]Change Name  : " + os.path.basename(file_path) + "- No Update")
        return

    pos = file_path.rindex("-")
    root = os.path.dirname(file_path)
    
    new_path = root + '\\' + time_str + file_path[pos:]
    if new_path != file_path:
        os.rename(file_path,new_path)
    print("[+]" + os.path.basename(file_path) + "  NAME CHANGED.")
    return new_path
        


def ChnageAllFilesTimeStr(dir_path, update_list):    
    for root, dirs, files in os.walk(dir_path):
        for item in files:
            file_path = os.path.join(root, item)
            for update_file in update_list:
                if update_file in item:
                    SetCTimeToMTime(file_path)
                    time_str = GetMTimeStr(file_path);
                    ChangeFileTimeTitle(file_path, time_str)
                    new_path = ChangeFileNameStr(file_path, time_str)
            if os.path.exists(file_path):
                SetAMTimeToCTime(file_path)


#1. 将所有文件的修改时间修改为创建时间       
dir_path = "F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io\\_posts"
update_list = ["RE工具整理", "FileInstruction", "python2.7学习笔记", "IDAPython学习笔记"]
#ChangeFilesMTime2CTime(dir_path);
#2.将文件的名称和front中的时间字符串更新为文件的更新时间（如果更新时间与时间字符串一致说明文件没有更新过，不再处理）
ChnageAllFilesTimeStr(dir_path, update_list)
print("[*]exit.")
