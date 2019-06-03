
#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os, datetime, time
import shutil
import re
import sys

#add time for file_path, the pattern is 2018-1-1-xxx.md, and change '_drafts' into '_posts'
def GenFilePath(file_path):
    ret_path = file_path

    #1.check if there is time in path
    test_pat = re.compile("[0-9]+-[0-9]+-[0-9]+-")
    tup = re.search(test_pat, file_path)
        
    #2.add time
    if tup ==None:
        ret_name = ("%d-%02d-%02d-"%(datetime.datetime.now().year, datetime.datetime.now().month, datetime.datetime.now().day)) + os.path.basename(file_path)
    else:
        ret_name = os.path.basename(file_path)

    ret_root = os.path.join(os.path.dirname(os.path.dirname(file_path)), "_posts")
    ret_path = os.path.join(ret_root, ret_name)
    print(ret_path)
    return ret_path

def SetCateTag():
    cate = input("input the cate of the file（if exe in python2, need \"\"）:\n")
    tags = input("input the tag of the file:\n")
    return cate,tags

#按照约定构造文件头
def GenFileFront(filename, cate, tags):
    front = ""
    line = []
    line.append("---\n")
    line.append("layout: post\n")
    #这个filename需要是没有添加时间的那个原来的name，而且要去掉后缀    
    line.append("title: \"%s\"\n"%os.path.splitext(filename)[0])
    line.append("date: %d-%d-%d %d:%d:%d\n"%(datetime.datetime.now().year, datetime.datetime.now().month, datetime.datetime.now().day, datetime.datetime.now().hour, datetime.datetime.now().minute, datetime.datetime.now().second))
    line.append("categories: " + cate + "\n")
    line.append("tags: " + tags + "\n")
    line.append("---\n")

    for item in line:
        front = front + item
    
    return front

#判断是否是残缺的图片路径
def CheckImagePath(in_str):
    if "图" in in_str and "Snipaste" in in_str:
        if "https://chrishuppor.github.io/image/" in in_str:
            return False
        else:
            return True
    else:
        return False

#将文件头写入文件（没有考虑已经有文件头的情况）
def FormNewFile(file_path, front):
    fr = open(file_path, "r", encoding = "UTF-8")
    rt = fr.readlines()
    fr.close()

    fw = open(file_path, "w", encoding = "UTF-8")
    fw.write(front + "\n")

    digest_over = 0
    for item in rt:
        if item[0] == "#" and digest_over == 0:
                digest_over = 1
                fw.write("\n" + item)
                continue

        if True == CheckImagePath(item):
            pos = item.index("Snipaste")
            newline = item[:pos] + "https://chrishuppor.github.io/image/" + item[pos:]
            fw.write(newline)
            continue

        fw.write(item)
    fw.close()

#完成post格式的文件并放入_posts文件夹中
def CompletePostFile(file_path):
    cate, tags = SetCateTag()

    front = GenFileFront(os.path.basename(file_path), cate, tags)
    print("The Front is:\n" + front)
    
    FormNewFile(file_path, front)
    
    new_path = GenFilePath(file_path)
    if file_path != new_path:
        shutil.move(file_path, new_path)

#---------------------------------------------------
sysin = sys.argv
dir_path = ""
if len(sysin) == 2:
    dir_path = sysin[1]
else:
    print ("The default dir is F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io\\_drafts")
    dir_path = "F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io\\_drafts"

print("The base path is : " + dir_path)
print("********************" + "*" * len(dir_path))
for roots, dirs, files in os.walk(dir_path):
    for item in files:
        if os.path.splitext(item)[1] == ".md":
            print ("The File is : " + item)
            in_str = input("If change it(0 for NO):")
            if in_str == "0" or in_str == 0:
                print("Not to deal with it temporarily\n")
                continue
            CompletePostFile(os.path.join(roots, item))    
    
