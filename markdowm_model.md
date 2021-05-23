# markdown_model
markdown_model概述

本章涉及的内容有:

* part1
* part2
* part3

---

## 目录

* [算法](./algorithm/algorithm.md)
* [读书笔记](./book_note/book_note.md)
* [C++知识学习](./c++/c++.md)
* [数据库](./database/database.md)
* [docker容器学习](./docker/docker.md)
* [Kubernetes知识](./k8s/k8s.md)
* [Linux编程学习](./linux/linux.md)
* [计算机网络](./network/network.md)
* [消息队列ZeroMQ](./zeromq/zeromq.md)

## part1

**内容加粗**

`标红`

[网址链接](https://www.bilibili.com/video/BV1c4411d7jb?p=29&spm_id_from=pageDriver)

![图片链接](./network/data_link/链路.png)

分点:

1. 分点1
2. 分点2
3. 分点3
4. 分点4

分段:

*  分段
*  分段
*  分段
*  分段

函数内容:

```c++
void FD_ZERO(fd_set *set); 			 //把文件描述符集合里所有位清0
void FD_SET(int fd, fd_set *set); 	 //把文件描述符集合里fd位置1
void FD_CLR(int fd, fd_set *set); 	 //把文件描述符集合里fd清0
int  FD_ISSET(int fd, fd_set *set);  //测试文件描述符集合里fd是否置1
```

特征 | new/delete | malloc/free
---|---|---
分配内存的位置|自由存储区|堆
内存分配成功的返回值|完整类型指针|void*
内存分配失败的返回值|默认抛出异常|返回NULL
分配内存的大小|由编译器根据类型计算得出|必须显式指定字节数
已分配内存的扩充|无法直观地处理|使用realloc简单完成
分配内存时内存不足|客户能够指定处理函数或重新制定分配器|无法通过用户代码进行处理
处理数组|有处理数组的new版本new[]|需要用户计算数组的大小后进行内存分配
是否相互调用|可以,看具体的operator new/delete实现|不可调用new
函数重载|允许|不允许
构造函数与析构函数|调用|不调用
