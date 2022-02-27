import os,shutil

'''
目的:获取目标文件夹下所有类型的指定文件

findAllFilesWithSpecifiedSuffix:寻找特定后缀的所有文件
copyFilesToSpecifiedPath:拷贝文件到特定路径
replaceFilesContent:替换文件列表里old_content为new_content
'''

def findAllFilesWithSpecifiedSuffix(target_dir, target_suffix="txt"):
    find_res = []
    target_suffix_dot = "." + target_suffix
    walk_generator = os.walk(target_dir)
    for root_path, dirs, files in walk_generator:
        if len(files) < 1:
            continue
        for file in files:
            file_name, suffix_name = os.path.splitext(file)
            if suffix_name == target_suffix_dot:
                find_res.append(os.path.join(root_path, file))
    return find_res

def copyFilesToSpecifiedPath(file_name_list,targetPath):
    for file_name in file_name_list:
        shutil.copy(file_name, targetPath)


def findAndCopyPngFileToSpecifiedPath(target_dir,suffix,target_path):
    find_res = findAllFilesWithSpecifiedSuffix(target_dir, suffix)
    print(find_res)
    copyFilesToSpecifiedPath(find_res,target_path)

#findAndCopyPngFileToSpecifiedPath("D:\CODE\klc\markdown", "png","D:\\CODE\klc\\all_images")


def replaceFilesContent(file_name_list, old_content, new_content):
    for file_name in file_name_list:
        with open(file_name, mode="r", encoding="UTF-8") as f1, \
                open(file_name + "_new", mode="w", encoding="UTF-8") as f2:
            for line in f1:
                line = line.replace(old_content, new_content)
                f2.write(line)
        os.remove(file_name)
        os.rename(file_name + "_new", file_name)

def findAndReplaceFilesToSpecifiedContent(target_dir,suffix,old_content,new_content):
    find_res = findAllFilesWithSpecifiedSuffix(target_dir, suffix)
    print(find_res)
    replaceFilesContent(find_res,old_content,new_content)

##r"./images/微服务框架学习路线|./images/elasticsearch
#findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\微服务架构","md","\.\/images.*\/","http://www.stibel.icu/wp-content/uploads/2022/01/micro_services/")


root_path_list = ["elasticsearch","ES集群","ES集群部署","ES与数据库数据同步","安装elasticsearch","多级缓存","分布式事务","分布式搜索","服务网关","服务注册与发现","微服务保护","微服务架构","微服务框架学习路线","消息队列"]

root_path_list = ["elasticsearch","ES集群","ES集群部署","ES与数据库数据同步","安装elasticsearch","多级缓存","分布式事务","分布式搜索","服务网关","服务注册与发现","微服务保护","微服务架构","微服务框架学习路线","消息队列"]

for root_path in root_path_list:
   findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\微服务架构", "md", "./images",
                                         "http://www.stibel.icu/wp-content/uploads/2022/01/micro_services")

def replaceNetWorkContent():
    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem","md","./images/concurrentProgram","http://www.stibel.icu/wp-content/uploads/2022/01/csapp/concurrentProgram")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/exception",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/exception")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/infoExpressAndDeal",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/infoExpressAndDeal")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/link",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/link")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/network",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/network")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/optimizeProgram",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/optimizeProgram")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/programRepresentation",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/programRepresentation")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/storage",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/storage")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/systemIO",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/systemIO")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/systemRoam",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/systemRoam")

    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\computerSystem", "md", "./images/virtualMemory",
                                          "http://www.stibel.icu/wp-content/uploads/2022/01/csapp/virtualMemory")


replaceNetWorkContent()

def replaceRedisContent():
    findAndReplaceFilesToSpecifiedContent("D:\\CODE\\klc\\blog\\redis","md","./images","http://www.stibel.icu/wp-content/uploads/2021/12/redis/images")

#replaceRedisContent()