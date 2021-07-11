# grpc学习

记录grpc学习的内容。

## 参考内容

* [github地址grpc](https://github.com/grpc/grpc)

* [grpc开发手册](https://grpc.io/docs/languages/cpp/quickstart/)

* [g++: internal compiler error: Killed (program cc1plus) 解决办法](https://blog.csdn.net/u013095333/article/details/94188743)

* [cmake对编译器版本和库依赖的问题](https://blog.csdn.net/fpcc/article/details/102664881)

* [CentOS7下安装gRPC（C++环境搭建）](https://blog.csdn.net/cyq6239075/article/details/83037585?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522162591531916780264056520%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=162591531916780264056520&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v2~rank_v29-1-83037585.pc_v2_rank_blog_default&utm_term=grpc&spm=1018.2226.3001.4450)

* [gRPC B站视频](https://www.bilibili.com/video/BV1eE411T7GC?p=6)

  

## 使用方法

**1.YUM安装相关工具**

```bash
yum install -y build-essential autoconf libtool pkg-config
```

**2.下载gRPC源码和相关子模块**

这步可能出现失败或者网速慢的情况（原因你懂的），多执行几次直到完全下载完毕为止。

```bash
git clone https://github.com/grpc/grpc.git
cd grpc
git submodule update --init
```

**3.安装  gRPC, Protocol Buffers, and Abseil**

```bash

export MY_INSTALL_DIR=/home/gRPC/grpc_install
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

export CC=/usr/local/bin/gcc
export CXX=/usr/local/bin/g++

cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make 
make install
popd
mkdir -p third_party/abseil-cpp/cmake/build
pushd third_party/abseil-cpp/cmake/build
cmake -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
      ../..
make 
make install
popd
```

## grpc的消息传输类型

1. #### Unary RPC

   * rpc method(Request) returns (Reply)

2. #### Server streaming RPC

   * rpc method(Request) returns (stream Reply) ，例如请求一个长视频，不断返回缓存视频

3. #### Client streaming RPC

   * rpc method(stream Request) returns (Reply)，例如上传一个大文件，上传完服务端返回OK

4. #### Bidirectional streaming RPC

   * rpc method(stream Request) returns (stream Reply)

## 核心概念

- #### Service definition （.proto)

- #### Using the API (server side:implements the methods;client: has a local object known as *stub*  that implements the same methods as the service)

- #### Synchronous vs. asynchronous

- #### RPC life cycle (Unary RPC,Server streaming RPC,Client streaming RPC,Bidirectional streaming RPC)

- #### Deadlines/Timeouts

- ### Deadlines/Timeouts

- #### RPC termination

- #### Cancelling an RPC

- #### Metadata

- #### Channels

## 问题记录

1.编译内存不足解决方法：

c++: internal compiler error: Killed (program cc1plus)

```bash
主要问题：
机器内存不足

解决办法：
使用机器的swap分区

操作过程：
# 使swapfile停止被占用
sudo swapoff /swapfile
# 重新分配swapfile的大小
#count的大小就是增加的swap空间的大小，64M是块大小，所以空间大小是bs*count=1024MB,可以适当改大count=32
sudo dd if=/dev/zero of=/swapfile bs=64M count=16
# 把这个文件格式化成swap空间
sudo mkswap /swapfile
# 使用刚才创建的swap空间
sudo swapon /swapfile

完成之后，可能想删除swap分区：
操作过程：
sudo swapoff /swapfile
sudo rm /swapfile
```

2.cmake对编译器版本和库依赖的问题 

```bash
error: no matching function for call to ‘StrFormat(const char [45], const double&, double, const double&, const double&)’ moments.skewness, moments.kurtosis)

查询后发现是gcc版本问题：
[root@VM-0-10-centos build]# whereis g++
g++: /usr/bin/g++ /usr/local/bin/g++ /usr/share/man/man1/g++.1.gz

/usr/bin/g++ 是GNU 4.8.5 , /usr/local/bin/g++ 是GNU 9.1.0
手动执行以下两条命令，cmake时会默认加制GNU 9.1.0
export CC=/usr/local/bin/gcc
export CXX=/usr/local/bin/g++

[root@VM-0-10-centos build]# cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..
-- The C compiler identification is GNU 9.1.0
-- The CXX compiler identification is GNU 9.1.0
```

## 使用方法

```bash
grpc.tar.gz  为下载grpc以及第三方库的文件
test.sh为整个执行过程文件，生成对应so和.h

cd /home/gRPC/test/  #上传这两个文件，生成的文件在/home/gRPC/grpc_install，具体看test.sh脚本内容
tar -zxvf grpc.tar.gz
./test.sh
```

测试是否能用：

```bash
export MY_INSTALL_DIR=/home/gRPC/grpc_install

cd examples/cpp/helloworld
mkdir -p cmake/build
pushd cmake/build
cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..
make -j


./greeter_server

./greeter_client
Greeter received: Hello world
```

