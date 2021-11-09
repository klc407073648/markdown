# -*- coding: utf-8 -*-
"""
Created on Sat Aug  8 19:46:14 2020

@author: admin
"""
import json
# 1.把PYTHON转化为JSON字符串
# 1.1PYTHON类型数据
json_str = '''[ {"name": "鹿晗","age": "26"},{"name": "李易峰","age": "29"},{"name": "陈赫","age": "31"}]'''
rs = json.loads(json_str)
# 1.2把PYTHON转化为JSON字符串
json_str = json.dumps(rs,  ensure_ascii=False)
print(json_str)

# 2.把PYTHON以JSON格式存储到文件中去
# 2.1构建要写入文件对象
with open('data/test1.json', 'w') as fp:
    # 2.2把Python以json格式存储到文件中
    #ensure_ascii=False用来转化汉字
    json.dump(rs, fp, ensure_ascii=False)
    
