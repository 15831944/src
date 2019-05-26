#!/usr/bin/python
# -*- coding: UTF-8 -*-

import requests
import json
import re
import sys
import imp

def GetResJsonDict(url):
    try:
        jsonStr = requests.get(url).content
        return json.loads(jsonStr)
    except:
        print("[-]a-o-,be detected.")
        return None


def GetVlogTotalCount(url_home):
    response_dict = GetResJsonDict(url_home)
    if None != response_dict:
        return response_dict["data"]["userInfo"]["statuses_count"]
    return 0


def ExtractGuest(full_str):
    plain_text = (re.sub("(<.*?>)", "", full_str))
    pattern = "@李晨 @angelababy @郑恺 @朱亚文 @王彦霖 @威神V_黄旭熙_LUCAS @宋雨琦_G-I-DLE "
    if pattern in plain_text and "浙江卫视" in plain_text:
        pos = plain_text.index(pattern)
        ret = plain_text[pos + len(pattern):]
        if len(ret) > 1:
            return ("[%s]"%ret)
    return None


def DealACard(card, tar_time):
    vlog_time = card["mblog"]["created_at"]
    if vlog_time not in tar_time:
        return None
        
    extra_mem = ExtractGuest(card["mblog"]["text"])
    if None == extra_mem:
        return None
    
    return "   time-%s extra members-%s"%(vlog_time, extra_mem)


def CrawlRunningExtraGuest(url_home, uid, tar_time):
    
    recent_count = 0; max_page = 100; mem_recorded = []
    
    print ("[*]start.")

                      
    total_vlog_count = GetVlogTotalCount(url_home)
    if total_vlog_count == 0:
        print ("[*]Game over!")
        return                      
    print ("[+]%d vlogs in total"%total_vlog_count)


    for page in range(1, max_page):

        url = 'https://m.weibo.cn/api/container/getIndex?uid=%d&page=%d&display=0&retcode=6102&type=uid&value=5242381821&containerid=1076035242381821'%(uid,page)
        response_dict = GetResJsonDict(url)
        if None == response_dict:
            break
            
        vlog_list = response_dict["data"]["cards"]
        for card in vlog_list:
            tmp = DealACard(card, tar_time)
            if tmp != None and tmp not in mem_recorded:
                print(tmp)
                mem_recorded.append(tmp)
                            
        recent_count += len(vlog_list)                                         
        print("[+]page-%d already read %d vblogs"%(page,recent_count))
        if recent_count > total_vlog_count:
            print("[*]Crawl over!")
            break


#-----------------------------------------------------------------------

imp.reload(sys)

url_home = "https://m.weibo.cn/api/container/getIndex?uid=5242381821&luicode=10000011&lfid=100103type%3D1%26q%3D%E5%A5%94%E8%B7%91%E5%90%A7&sudaref=m.weibo.cn&type=uid&value=5242381821&containerid=1005055242381821"
uid = 5242381821
tar_time = ["05-24","05-17", "05-10", "05-03", "04-26"]

CrawlRunningExtraGuest(url_home, uid, tar_time)
