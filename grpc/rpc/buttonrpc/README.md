
# buttonrpc - modern rpc framework for C++
- ZeroMQ 作为网络层
- 使用c++14开发

## Features
- 轻量级，跨平台，简单易用
- 服务端可以绑定自由函数，类成员函数，std::function对象
- 服务端可以绑定参数是任意自定义类型的函数
- 客户端与服务端自动重连机制
- 客户端调用超时选项

## Example
server:

```c++
#include "buttonrpc.hpp"

int foo(int age, int mm){
	return age + mm;
}

int main()
{
	buttonrpc server;
	server.as_server(5555);

	server.bind("foo", foo);
	server.run();

	return 0;
}
```

client: 

```c++
#include <iostream>
#include "buttonrpc.hpp"

int main()
{
	buttonrpc client;
	client.as_client("127.0.0.1", 5555);
	int a = client.call<int>("foo", 2, 3).val();
	std::cout << "call foo result: " << a << std::endl;
	system("pause");
	return 0;
}

// output: call foo result: 5

```

## Dependences
- [ZeroMQ](http://zguide.zeromq.org/page:all)


## Building
- windows vs2015 或者更高版本,  linux 添加编译选项：-std=c++1z

## Usage

- 1： 更多例子在目录 example/ 下



# 知识点

```
m_socket = std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>>(new zmq::socket_t(m_context, ZMQ_REP), [](zmq::socket_t* sock){ sock->close(); delete sock; sock =nullptr;});

unique_ptr<T, D> m_socket;  //T对应构造，D对应删除器
void Deleter(zmq::socket_t* sock)
{
     sock->close();  //关闭连接
     delete sock; 	//删除对象指针
     sock = nullptr;
 }
 
 //删除器的类型的函数指针： decltype(Deleter)* 等效于 void (*) (zmq::socket_t*);
 m_socket = std::unique_ptr<zmq::socket_t, decltype(Deleter)*>>(new zmq::socket_t(m_context, ZMQ_REP), Deleter);


```



智能指针(smarter pointer)自定义删除器(deleter) 的方法:

```
智能指针包含两种"shared_ptr"和"unique_ptr", 由于两种指针的实现方式不同, 所以传递删除器的方式也不同;

"shared_ptr"的传递删除器(deleter)方式比较简单, 只需要在参数中添加具体的删除器函数名, 即可; 注意是单参数函数;
"unique_ptr"的删除器是函数模板(function template), 所以需要在模板类型传递删除器的类型(即函数指针(function pointer)), 再在参数中添加具体删除器;

定义函数指针的类型, 包含三种方法(typedef, typedef decltype, using), 也可以直接传递decltype;
```

 

```c++
#include <iostream>
#include <memory>

using namespace std;

void deleter (int* ptr) {
	delete ptr;
	ptr = nullptr;
	std::clog << "shared_ptr delete the pointer." << std::endl;
}

int main (void) {

	//定义函数类型
	typedef void (*tp) (int*);
	typedef decltype (deleter)* dp;
	using up = void (*) (int*);

	std::shared_ptr<int> spi(new int(10), deleter);
	std::shared_ptr<int> spi2(new int, deleter);
	spi2 = std::make_shared<int>(15);

	std::cout << "*spi = " << *spi << std::endl;
	std::cout << "*spi2 = " << *spi2 << std::endl;

	//unique_ptr是模板函数需要删除器(deleter)类型, 再传入具体的删除器
	std::unique_ptr<int, decltype(deleter)*> upi(new int(20), deleter);
	std::unique_ptr<int, tp> upi2(new int(25), deleter);
	std::unique_ptr<int, dp> upi3(new int(30), deleter);
	std::unique_ptr<int, up> upi4(new int(35), deleter);

	std::cout << "*upi = " << *upi << std::endl;
	std::cout << "*upi2 = " << *upi2 << std::endl;
	std::cout << "*upi3 = " << *upi3 << std::endl;
	std::cout << "*upi4 = " << *upi4 << std::endl;

	return 0;

}
```

运行结果：

```
[root@iZuf61kbf845xt6tz10abgZ ptr]# g++ -o ptr_main -std=c++11 ptr_main.cpp
[root@iZuf61kbf845xt6tz10abgZ ptr]# ./ptr_main
shared_ptr delete the pointer.
*spi = 10
*spi2 = 15
*upi = 20
*upi2 = 25
*upi3 = 30
*upi4 = 35
shared_ptr delete the pointer.
shared_ptr delete the pointer.
shared_ptr delete the pointer.
shared_ptr delete the pointer.
shared_ptr delete the pointer.

```



- **std::is_same 判断类型是否一致**

位于头文件`<type_traits>`中

这个结构体作用很简单，就是两个一样的类型会返回true

```cpp
bool isInt = std::is_same<int, int>::value; //为true
```

- enable_if 的原理

enable_if 的定义类似于下面的代码：（只有 Cond = true 时定义了 type）

```
template<bool Cond, class T = void> struct enable_if {};
template<class T> struct enable_if<true, T> { typedef T type; };
```

- std::forward 完美转发的原理

http://www.voidcn.com/article/p-uvuqppah-xy.html

https://blog.csdn.net/hzhsan/article/details/46342651

- typedef typename std::vector<T>::size_type size_type;

```
typedef创建了存在类型的别名，而typename告诉编译器std::vector<T>::size_type是一个类型而不是一个成员。
```

- template<typename T>

```
template<typename T>
表示T是一个类型， 在模版实例化时可以替换任意类型，不仅包括内置类型（int等），也包括自定义类型class。 换句话说，在template<typename Y>和template<class Y>中， 
typename和class的意义完全一样。 
```

```
template<typename... Args>
void package_params(Serializer& ds, const std::tuple<Args...>& t)
{
	package_params_impl(ds, t, std::index_sequence_for<Args...>{});
}

中间  std::index_sequence_for  ——> make_index_sequence

定义辅助类模板 std::index_sequence_for ，以转换任何类型参数包为同长度的下标序列：

template<class... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;
	
	

```

- std::decay

```
Obtains the decay type of T.
If T is a reference type, the type referrered to by T.
Otherwise, T.
std::decay就是对一个类型进行退化处理

```
