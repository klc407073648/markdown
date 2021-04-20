# 制作groovy的docker镜像

## 参考资料

* [Groovy 教程](https://www.w3cschool.cn/groovy/)
* [CentOS 7 搭建 groovy环境](https://www.e-learn.cn/topic/3834573)


## 制作groovy的docker镜像

```
yum install tree

[root@master01 groovy]# tree
.
|-- build_groovy.sh
|-- Dockerfile
`-- package
    |-- apache-groovy-sdk-3.0.7.zip
    `-- jdk-8u221-linux-x64.tar.gz


整个构建过程，直接执行./build_groovy.sh 即可

docker run -it centos_groovy bash
```

## 实践

[root@master01 groovy]# docker run -it centos_groovy bash
[root@046d1a77f735 /]# java -version
java version "1.8.0_221"
Java(TM) SE Runtime Environment (build 1.8.0_221-b11)
Java HotSpot(TM) 64-Bit Server VM (build 25.221-b11, mixed mode)
[root@046d1a77f735 /]# groovy -version
Groovy Version: 3.0.7 JVM: 1.8.0_221 Vendor: Oracle Corporation OS: Linux
[root@046d1a77f735 /]#



## 问题记录

```
问题1：
source /etc/profile 后，docker run 进入容器，环境变量未生效
解答1：
在容器实例中 ~/.bashrc 配置文件末尾添加 source /etc/profile，保存退出即可。这样就能够实现重启后自动刷新配置文件，而无需手动操作。

```

