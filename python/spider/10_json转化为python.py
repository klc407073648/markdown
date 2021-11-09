# -*- coding: utf-8 -*-
"""
Created on Sat Aug  8 19:30:54 2020

@author: admin
"""
import json

# 1.把JSON字符串转化为PYTHON数据
# 1.1准备JSON字符串
json_str = '''[ {"name": "鹿晗","age": "26"},{"name": "李易峰","age": "29"},{"name": "陈赫","age": "31"}]'''
# 1.2把JSON字符串转化为PYTHON
rs = json.loads(json_str)
print(rs)
print(type(rs))#<class 'list'>
print(type(rs[0]))#<class 'dict'>

# 2.把JSON文件转化为PYTHON类型的数据
# 2.1构建指向该文件的文件对象
with open('data/test.json',encoding='UTF-8') as fp:
    #2.2 加载该文件对象，转化为PYTHON类型的数据
    python_list = json.load(fp)
    print(python_list)
    print(type(python_list))#<class 'list'>
    print(type(python_list[0]))#<class 'dict'>