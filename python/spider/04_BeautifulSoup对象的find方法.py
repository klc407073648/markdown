# -*- coding: utf-8 -*-
"""
Created on Sat Aug  8 17:56:44 2020

@author: admin
"""
#根据标签名查找
#1.导入模块 
from bs4 import BeautifulSoup

#2.文档内容
html = """
<!DOCTYPE html>
<html>
<head>
<title>My Title</title>
</head>
<body>

<a href="article.html" target="_blank">文章</a>
<a href="photo.html" target="_blank">相册</a>
<a href="video.html" target="_blank">视频</a>

</body>
</html>
"""
#3.创建BeautifulSoup对象
soup = BeautifulSoup(html, 'lxml')

#4.查找title标签
title = soup.find('title')
print(title)

#5. 查找a标签
a = soup.find('a')
print(a)

#6. 查找所有的a标签
a_s = soup.find_all('a')
print(a_s)

#二、根据属性进行查找
#查找id为link1的标签
#方式1： 通过命名参数进行制定的
a = soup.find(href="article.html")
print(a)
#方式2： 通过attrs来指定属性字典，进行查找
a = soup.find(attrs={'href': 'article.html'})
print(a)


#三、根据文本内容进行查找
text = soup.find(text = "文章")
print(text)

#Tag对象
#对应原始文档的XML和HTML标签
#属性有
# name 姓名
# attrs 属性
# text 文档内容
print(type(a))
print(a.name)
print(a.attrs)
print(a.text)
