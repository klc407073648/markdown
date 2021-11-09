import requests
from bs4 import BeautifulSoup
import re
import json
from tqdm import tqdm

#思路
# 1. 发送请求,获取Gitstar Ranking首页的数据（包括Users，Organizations和Repositories的Ranking top 10）
# 2. 从h2标签中，提取出Users，Organizations和Repositories块的数据
# 3. 各个块数据的组织结构为table,即可从thead、tbody中得到题头和正文内容
# 4. 把标签里的内容，转换为dict数据
# 5. 把Python类型的dict数据,以json格式存入文件中

class CrawlerSearch(object):

    def __init__(self, url, path):
        self.url = url
        self.path = path

    def get_content_from_url(self):
        response = requests.get(self.url)
        home_page = response.content.decode()
        return home_page

    def parse_home_page(self, home_page):
        soup = BeautifulSoup(home_page, 'lxml')
        theme_all = soup.find_all('div', class_='col-md-4')  # 主题包含在h2级标签里
        #print('theme_all: ', theme_all)
        results = [] #最终结果集

        # table标签中thead、tbody、tfoot的作用:题头、正文和脚注
        for item in theme_all:
            column_names = []
            column_values_list = []
            cur_theme = item.find('a').text  # Users
            theme_thead = item.find('thead').find_all('tr') #[<th>Rank</th>, <th>User</th>, <th>Star</th>]
            theme_tbody = item.find('tbody').find_all('tr')#[<tr><td>1</td><td>...indresorhus...</td><td>571,529</td></tr>,...]
            print('cur_theme:',cur_theme,' is dealing.')

            # 找到所有列名
            for ths in theme_thead:
                ths_content = ths.find_all('th')
                for content in ths_content:
                    cur_content=content.text.strip()
                    column_names.append(cur_content)

            print('column_names:', column_names)

            # 找到所有列名对应的内容
            for tds in theme_tbody:
                column_values = []
                tds_content = tds.find_all('td')
                for content in tds_content:
                    cur_content = content.text.strip()
                    column_values.append(cur_content)
                column_values_list.append(column_values)

            # 组织当前主题的数据内容
            cur_data_str=self.splice_data_result(cur_theme, column_names, column_values_list)
            results.append(cur_data_str)

        # 保存数据到json文件
        self.save_json_file(results, self.path)

    def run(self):
        home_page = self.get_content_from_url()
        self.parse_home_page(home_page)

    def splice_data_result(self, cur_theme, column_names, column_values_list):
        cur_data_str = {}
        result = []
        for values in column_values_list:
            data_str = {}
            for index, value in enumerate(column_names):
                data_str[column_names[index]] = values[index]
            result.append(data_str)
        cur_data_str[cur_theme] = result # 组装好一个主题的内容
        #print(result)
        return cur_data_str

    def save_json_file(self, data, path):
        with open(path, 'w', encoding="utf-8") as fp:
            json.dump(data, fp, ensure_ascii=False)
        print('final data save in ',path)

if __name__ == '__main__':
    # spider = CrawlerSearch(['https://gitstar-ranking.com/users','https://gitstar-ranking.com/organizations','https://gitstar-ranking.com/repositories'])
    spider = CrawlerSearch('https://gitstar-ranking.com','data/Gitstar_Ranking.json')
    spider.run()
