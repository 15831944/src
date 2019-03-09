#-*- coding: UTF-8 -*-
import sys,os

'''
描述：bilibili本地弹幕手动删减脚本
操作：
    1 输入本地弹幕路径
    2 根据提示选择保留该弹幕还是删除该条弹幕，如果没有保留上一条弹幕但现在后悔了，
    则可以输入2表明自己很2，然后就重新保留上一条弹幕了

结果：将删减后的结果保存在<name>.xml文件同目录的<name_reduced>.xml文件中
'''

#1. 获得xml标签首尾的位置和内容
#eg:GetTab('<d 2332323>sk</d>', '<d', '</d>')
def GetTab(text, tabBegin, tabEnd):
    
    posBegin = text.find(tabBegin) #标签起始
    posEnd = text.find(tabEnd) + len(tabEnd) #标签结束

    senBegin = 0
    senEnd = 0
    
    if posBegin >= 0 and posEnd >= 0:   
       senBegin = text[posBegin:].find('>') + 1 + posBegin#标签内容开始
       senEnd = text[senBegin:].find('<') + senBegin 
    
    return posBegin, posEnd, senBegin, senEnd


# 2. 将text文本追加写到outputFile
def AddWrite(outputFile, text):
    fw = open(outputFile, 'a')
    fw.write(text)
    fw.close()
    

# 3. 根据规则过滤符合要求的标签
def FilterBullet(inputFile, outputFile, filterPattern):
    return


# 4. 提示用户手动选择是否保留某些内容
def ManualChangeBullet(inputFile, outputFile):
    
    # 1. 读原数据
    fr = open(inputFile, 'r')
    readText = fr.read();
    fr.close();    
    
    tmp = readText
    writeText = ''

    posEnd = 0

    # 2. 将之前的数据写入
    [posBegin, posEnd, senBegin, senEnd] = GetTab(tmp, '<d', '</d>')

    lastChoice = -1;
    lastBegin = 0;
    lastEnd = 0;

    AddWrite(outputFile, tmp[:posBegin]) #先把posBegin之前的数据写到outputFile中，然后在选择是否保留该标签
    posEnd = 0

    # 3. 从第一个标签开始处理并输出

    while True:

        [posBegin, posEnd, senBegin, senEnd] = GetTab(tmp[lastEnd:], '<d', '</d>')
        posBegin = posBegin + lastEnd
        posEnd = posEnd + lastEnd
        senBegin = senBegin + lastEnd
        senEnd = senEnd + lastEnd
        #print posBegin, posEnd, senBegin, senEnd
        
        if posBegin < lastEnd: #找不到标签,把剩余的tmp写进去
            AddWrite(outputFile, tmp[lastEnd:])
            break

        #输出标签内容给用户看看
        #print sen.decode('UTF-8').encode(conCodeType)
        print '[content] ' + tmp[senBegin:senEnd].decode('UTF-8')

        print "your choice（save-1,not save-any key, save last-2）:",

        try:
            bIsSave = input() # 2-不要，1-要
        except SyntaxError:
            bIsSave = 0        
        
        if bIsSave == 1:
            AddWrite(outputFile, tmp[posBegin:posEnd])
        elif bIsSave == 2 and lastEnd != 0 and lastChoice == 0:
            AddWrite(outputFile, tmp[lastBegin:lastEnd])
            print 'some 2 person regerted for del : ' + tmp[lastBegin:lastEnd]
            continue #后悔了上一次的，这次就重来吧

        lastBegin = posBegin
        lastEnd = posEnd
        lastChoice = bIsSave;


#=============================================================================================


if __name__ == "__main__":
    inputFile = raw_input('Input the xml file path:\n')
    outputFile = inputFile[:inputFile.find('.xml')] + '_reduced.xml'
    
    fw = open(outputFile, 'w')
    fw.close()
    
    filterPattern = ''#raw_input('Input the filter pattern:\n')

    FilterBullet(inputFile, outputFile, filterPattern)

    ManualChangeBullet(inputFile, outputFile)
















