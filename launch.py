#!/usr/bin/python
# -*- coding:utf-8 -*-
import os
import time
import datetime
import threading

os.system('clear')
print "-"*30
print "Welcome to MiniCrawler!"
time_now = datetime.datetime.now()
print "启动时间:\t",time_now
#os.system('./Debug/WebCrawler')
print "-"*30

def displayConfiguration () :
	configFile = open('config.h')
	lines = configFile.readlines()
	index = 0
	for line in lines :
		index+=1
		lst = line.split()
		if lst.count('CRAWLER_NAME') > 0 : 
			print '爬虫名称:\t',lst[2]
			continue
		if lst.count('CRAWLER_DEAPTH') > 0 :
			print '爬虫深度：\t',lst[2]
			continue
		if lst.count('THREAD_NUMBER') > 0 :
			print '开启采集线程数量:\t',lst[2]
			continue
		if lst.count('BASE_NAME') > 0 :
			path = lst[2]
			print '数据存储路径:\t',lst[2]
			continue
		if lst.count('SEEDURL') > 0 :
			print '启始种子URL：\t',lst[2]
			continue
		if lst.count('SEEDURL_PRIORITY') > 0 :
			print '启始种子URL优先级权值:\t',lst[2]
			continue
		if lst.count('HOSTFILTER') > 0 :
			print '过滤URL主机名列表:',lines[index],
			continue
		if lst.count('SUFFIXFILTER') > 0 :
			print '过滤URL后缀名列表:',lines[index],
			continue
		if lst.count('MACHINE_NAME') > 0 :
			print '本爬虫机器节点名称:\t' , lst[2]
			continue
		if lst.count('PORT') > 0 :
			print '本爬虫机器节点通信端口:\t',lst[2]
			continue;
	return path
	print '-'*30

path_name = displayConfiguration();
path_name = path_name[1:-6]
def Crawler_State(path):
	while True:	
		time.sleep(5)
		time_another = datetime.datetime.now()
		print "爬虫已经运行:\t",time_another - time_now
		print "已经采集的URL数量: "
		os.system('wc -l '+path+'/*.index')
		print "已下载文件大小 : "
		os.system('du '+path+'/')
		#os.system('clear')

def GetCrawler_Time(path):
	while True:
		time.sleep(5)
		os.system('tail -n 1 '+path+'/*.index >> '+path+'/tmp.txt')
		os.system('head -n 1 '+path+'/*.index >> '+path+'/tmp2.txt')
		max_time = '00:00:00'
		url = ''
		f = open(path+'/tmp.txt')
		for line in f :
			arr = line.split()
			if len(arr) < 6 :
				continue
			if max_time < arr[-2] :
				max_time = arr[-2]
				url = arr[4]
		print "Start at :\t",url,max_time
		min_time = '23:59:59'
		f = open(path+'/tmp2.txt')
		for line in f :
			arr = line.split()
			if len(arr) < 6 :
				continue
			if min_time > arr[-2] :
				min_time = arr[-2]
				url = arr[4]
		print "End at ：\t",url,min_time

def SortFetchedUrls(path):
	def parse(url):
		'''Parse a url to hostname+filename'''
		try:
		    u = url
		    if u.startswith('http://') :
		        u = u[7:]
		    elif u.startswith('https://'):
		        u = u[8:]
		    if u.find(':80')>0 :
		        p = u.index(':80')
		        p2 = p + 3
		    else:
		        if u.find('/')>0:
		            p = u.index('/') 
		            p2 = p
		        else:
		            p = len(u)
		            p2 = -1
		    hostname = u[:p]
		    if p2>0 :
		        filename = u[p2:]
		    else : filename = '/'
		    return hostname, filename
		except Exception ,e:
		    print "Parse wrong : " , url
		    print e

	def display():
		host_dict = {}
		host_dict.setdefault(0)
		fmtname = path+"/file%03d_.index"

		for index in range(20) :
			filename = fmtname % (index+1)
			f = open(filename)
			for line in f:
				try:
					url = line.split()[4]
				except Exception , e:
					#print e
					continue
				host_name , file_name = parse(url)
				if host_dict.get(host_name) == None :
					host_dict[host_name] = 0
				host_dict[host_name] +=1 

		arr = sorted(host_dict.items(), key=lambda d: d[1] , reverse=True) 
		index  = 1
		for hostname , count in arr :
			print index,'\t',hostname,'\t',count
			index+=1

	while(True):
		time.sleep(5)
		display()


thread1 = threading.Thread(name='daemon1',target = Crawler_State ,args = (path_name,))
thread1.start()
thread2 = threading.Thread(name='daemon2',target = GetCrawler_Time,args = (path_name,))
thread2.start()
thread3 = threading.Thread(name='daemon3',target = SortFetchedUrls,args = (path_name,))
thread3.start()

thread1.join()
thread2.join()
thread3.join()

