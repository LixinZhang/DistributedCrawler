#!/usr/bin/python
# -*- coding:utf-8 -*-

def cal(filename) :	
	dict_before = {}
	dict_after = {}
	flag = True
	f = open(filename)
	for line in f :
		arr = line.split()
		if arr[0] == '向机器' :
			if flag == True:
				if dict_before.has_key(arr[1]) == False :
					dict_before[arr[1]] = 0
				dict_before[arr[1]] += int(arr[3])
			else :
				if dict_after.has_key(arr[1]) == False :
					dict_after[arr[1]] = 0
				dict_after[arr[1]] += int(arr[3])
				#print arr[3]
		elif arr[0] == '连接' :
			flag = False
		

	for key in dict_before.keys() :
		print key , dict_before[key]

	for key in dict_after.keys() :
		print key , dict_after[key] ,dict_before[key] + dict_after[key]


print 'A'*30
cal('testCrawlerA/a.log')
print 'B'*30
cal('testCrawlerB/b.log')
print 'C'*30
cal('testCrawlerC/c.log')
print 'D'*30
cal('testCrawlerD/d.log')
