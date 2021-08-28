# TarsCPP开发环境部署

本节主要介绍Tars Cpp服务开发和编译时需要的开发环境。

## 参考资料

* [tars项目介绍](https://tarscloud.gitbook.io/tarsdocs/)

* [tars_cpp_quickstart](https://github.com/TarsCloud/TarsDocs/blob/91fd046369b5f6fa43d02288ae2bf9be670fdb30/demo/tarscpp/tars_cpp_quickstart.md)

## 编译包依赖下载

```
yum install glibc-devel gcc gcc-c++ bison flex
yum install zlib-devel
##cmake 需要单独安装3.0以上版本
```



## 下载项目源码

```
cd /home/klc/tars/
git clone https://github.com/TarsCloud/TarsCpp.git --recursive
cd ./TarsCpp/
git submodule update --init ##下载过程可能有些文件下载失败，可以进入TarsCpp文件夹后继续下载
```

# TarsCPP 快速入门

## 创建服务

```
cd /usr/local/tars/cpp/script/
./cmake_tars_server.sh TestApp HelloServer Hello
```

命令执行后，会在当前目录的/HelloServer/src 目录下，生成下面文件：

```
HelloServer.h HelloServer.cpp Hello.tars HelloImp.h HelloImp.cpp CMakeLists.txt
```

## 编译服务

```
cd ./HelloServer/build/
cmake ..
make -j4
```

## 服务实现

```
cd build
cmake ..
make -j4
make HelloServer-tar
make HelloServer-upload
```

重新make cleanall;make;make tar，会重新生成HelloServer.tgz发布包。

# 客户端同步/异步调用服务

```
mkdir -p /home/tarsproto/TestApp/HelloServer
make HelloServer-release
cd /home/tarsproto/
mkdir TestHelloClient
cd ./TestHelloClient/
touch main.cpp
touch makefile
make
./TestHelloClient
```

创建计算服务：

```
cd /usr/local/tars/cpp/script/
./cmake_tars_server.sh TestCalcApp CalcServer Calc
cd /usr/local/tars/cpp/script/CalcServer/build/
cmake ..
make -j4
make -j4
make CalcServer-tar
make cleanall;make;make tar
mkdir -p /home/tarsproto/TestCalcApp/CalcServer
make CalcServer-release
cd /home/tarsproto/
mkdir -p TestCalcClient
cd ./TestCalcClient/
touch main.cpp
touch makefile
```

