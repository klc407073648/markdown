#ifndef __BUTTONRPC_H_
#define __BUTTONRPC_H_

#include <string>
#include <map>
#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include <zmq.hpp>
#include "Serializer.hpp"

class Serializer;

//模板类型定义
template <typename T>
struct type_xx
{
	typedef T type;
};

template <>
struct type_xx<void>
{
	typedef int8_t type;
};

// 打包帮助模板
//以tuple数据t导入到Serializerd对象ds中
template <typename Tuple, std::size_t... Is>
void package_params_impl(Serializer &ds, const Tuple &t, std::index_sequence<Is...>)
{
	initializer_list<int>{((ds << std::get<Is>(t)), 0)...};
}

template <typename... Args>
void package_params(Serializer &ds, const std::tuple<Args...> &t)
{
	package_params_impl(ds, t, std::index_sequence_for<Args...>{});
	//定义辅助类模板 std::index_sequence_for ，以转换任何类型参数包为同长度的下标序列：
}

// 用tuple做参数调用函数模板类
template <typename Function, typename Tuple, std::size_t... Index>
decltype(auto) invoke_impl(Function &&func, Tuple &&t, std::index_sequence<Index...>)
{
	//解包tuple 参数，传递给func函数调用
	return func(std::get<Index>(std::forward<Tuple>(t))...);
}

template <typename Function, typename Tuple>
decltype(auto) invoke(Function &&func, Tuple &&t)
{
	constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
	return invoke_impl(std::forward<Function>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}
//make_index_sequence它可以帮助我们完成在编译期间生成上面的invoke_impl中的std::size_t... Index

// 调用帮助类，主要用于返回是否void的情况

/*
std::is_same 判断类型是否一致
bool isInt = std::is_same<int, int>::value; //为true

enable_if 的定义类似于下面的代码：（只有 Cond = true 时定义了 type）

template<bool Cond, class T = void> struct enable_if {};
template<class T> struct enable_if<true, T> { typedef T type; };
*/

template <typename R, typename F, typename ArgsTuple>
typename std::enable_if<std::is_same<R, void>::value, typename type_xx<R>::type>::type
call_helper(F f, ArgsTuple args)
{
	invoke(f, args);
	return 0;
}

template <typename R, typename F, typename ArgsTuple>
typename std::enable_if<!std::is_same<R, void>::value, typename type_xx<R>::type>::type
call_helper(F f, ArgsTuple args)
{
	return invoke(f, args);
}

//RPC角色
enum rpc_role
{
	RPC_CLIENT,
	RPC_SERVER
};

//RPC错误码
enum rpc_err_code
{
	RPC_ERR_SUCCESS = 0,
	RPC_ERR_FUNCTIION_NOT_BIND,
	RPC_ERR_RECV_TIMEOUT
};

// wrap return value
template <typename T>
class value_t
{
public:
	typedef typename type_xx<T>::type type;
	typedef std::string msg_type;
	typedef uint16_t code_type;

	value_t()
	{
		_code = 0;
		_msg.clear();
	}
	bool valid() { return (_code == 0 ? true : false); }
	int error_code() { return _code; }
	std::string error_msg() { return _msg; }
	type val() { return _val; }

	void set_val(const type &val) { _val = val; }
	void set_code(code_type code) { _code = code; }
	void set_msg(msg_type msg) { _msg = msg; }

	friend Serializer &operator>>(Serializer &in, value_t<T> &d)
	{
		in >> d._code >> d._msg;
		if (d._code == 0)
		{
			in >> d._val;
		}
		return in;
	}
	friend Serializer &operator<<(Serializer &out, value_t<T> d)
	{
		out << d._code << d._msg << d._val;
		return out;
	}

private:
	code_type _code;
	msg_type _msg;
	type _val;
};

// rpc 类定义
class buttonrpc
{
public:
	/**
     * @brief 构造函数
     */
	buttonrpc();

	/**
     * @brief 析构函数
     */
	~buttonrpc();

	// network

	/**
	* @brief 以ZeroMQ做网络层，创建客户端连接
	*/
	void as_client(const std::string &ip, int port);

	/**
     * @brief 客户端设置超时时间
     */
	void set_timeout(uint32_t ms);

	/**
	* @brief 以ZeroMQ做网络层，创建服务端
	*/
	void as_server(int port);

	/**
     * @brief 服务端循环处理: 接收消息，调用函数，返回结果(发送消息)
     */
	void run();

	/**
     * @brief ZeroMQ发送消息
     */
	void send(zmq::message_t &data);

	/**
     * @brief ZeroMQ接收消息
     */
	void recv(zmq::message_t &data);

public:
	// server

	/**
     * @brief 将函数名与普通函数绑定
     */
	template <typename F>
	void bind(const std::string &name, F func);

	/**
     * @brief 将函数名与类成员函数绑定，需要传递类对象
     */
	template <typename F, typename S>
	void bind(const std::string &name, F func, S *s);

	// client

	/**
     * @brief 客户端通过call函数(client.call<int>("foo_3", 10)),将所调用的函数名和参数信息封装成Serializer对象，再通过ZeroMQ发送出去
     *
     * @return value_t<R> 类型的数据，包含_code(是否有效),_msg(消息内容),_val(返回值)
     */
	template <typename R, typename... Params>
	value_t<R> call(const std::string &name, Params... ps)
	{
		using args_type = std::tuple<typename std::decay<Params>::type...>; // 获取可变参数类型，例如 using args_type = std::tuple<int, char, string>
		args_type args = std::make_tuple(ps...);							//例如 std::tuple<int, char, string> args = std::make_tuple(10, 'a',"hello")

		Serializer ds;
		ds << name;
		package_params(ds, args); // 将函数名name,以及可变参数的数据依次打包传递给Serializer对象ds

		return net_call<R>(ds);
	}

	/**
     * @brief 客户端通过call函数调用无参数的函数
     *
     * @return value_t<R> 类型的数据，包含_code(是否有效),_msg(消息内容),_val(返回值)
     */
	template <typename R>
	value_t<R> call(const std::string &name)
	{
		Serializer ds;
		ds << name;
		return net_call<R>(ds);
	}

private:
	/**
     * @brief 服务端通过call_函数,调用已经注册的函数
     *
     * @return value_t<R> 类型的数据
     */
	Serializer *call_(const std::string &name, const char *data, int len);

	/**
     * @brief 客户端通过ZerMQ的调用获取返回结果
     *
     * @return value_t<R> 类型的数据
     */
	template <typename R>
	value_t<R> net_call(Serializer &ds);

	/**
     * @brief 调用普通函数代理
     */
	template <typename F>
	void callproxy(F fun, Serializer *pr, const char *data, int len);

	/**
     * @brief 调用类成员函数代理
     */
	template <typename F, typename S>
	void callproxy(F fun, S *s, Serializer *pr, const char *data, int len);

	// 函数指针
	template <typename R, typename... Params>
	void callproxy_(R (*func)(Params...), Serializer *pr, const char *data, int len)
	{
		callproxy_(std::function<R(Params...)>(func), pr, data, len);
	}

	// 类成员函数指针
	template <typename R, typename C, typename S, typename... Params>
	void callproxy_(R (C::*func)(Params...), S *s, Serializer *pr, const char *data, int len)
	{

		using args_type = std::tuple<typename std::decay<Params>::type...>;

		Serializer ds(StreamBuffer(data, len));
		constexpr auto N = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = ds.get_tuple<args_type>(std::make_index_sequence<N>{});

		auto ff = [=](Params... ps) -> R
		{
			return (s->*func)(ps...);
		};
		typename type_xx<R>::type r = call_helper<R>(ff, args);

		value_t<R> val;
		val.set_code(RPC_ERR_SUCCESS);
		val.set_val(r);
		(*pr) << val;
	}

	// functional
	template <typename R, typename... Params>
	void callproxy_(std::function<R(Params... ps)> func, Serializer *pr, const char *data, int len)
	{

		using args_type = std::tuple<typename std::decay<Params>::type...>;

		Serializer ds(StreamBuffer(data, len));
		constexpr auto N = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = ds.get_tuple<args_type>(std::make_index_sequence<N>{});

		typename type_xx<R>::type r = call_helper<R>(func, args);

		value_t<R> val;
		val.set_code(RPC_ERR_SUCCESS);
		val.set_val(r);
		(*pr) << val;
	}

private:
	//调用函数名与调用形式，客户端调用函数call会将函数名以及入参信息封装成Serializer结构，通过ZeroMQ发送给服务端，
	//服务端解析出函数名，将调用call_函数，最终也是返回Serializer类型内容
	std::map<std::string, std::function<void(Serializer *, const char *, int)>> _handlers;

	//ZeroMQ的套接字
	std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t *)>> _socket;

	//ZeroMQ的上下文
	zmq::context_t _context;

	//rpc错误码
	rpc_err_code _error_code;

	//角色
	int _role;
};

inline buttonrpc::buttonrpc() : _context(1), _error_code(RPC_ERR_SUCCESS)
{
}

inline buttonrpc::~buttonrpc()
{
	_context.close();
}

// network
//unique_ptr<T, D> m_socket;  //T对应构造，D对应删除器

inline void buttonrpc::as_client(const std::string &ip, int port)
{
	_role = RPC_CLIENT;

	auto deleter = [](zmq::socket_t *sock)
	{
		sock->close(); //关闭连接
		delete sock;   //删除对象指针
		sock = nullptr;
	};

	_socket = std::unique_ptr<zmq::socket_t, decltype(deleter)>(new zmq::socket_t(_context, ZMQ_REQ), deleter);

	ostringstream os;
	os << "tcp://" << ip << ":" << port;
	_socket->connect(os.str());
}

inline void buttonrpc::as_server(int port)
{
	_role = RPC_SERVER;

	auto deleter = [](zmq::socket_t *sock)
	{
		sock->close(); //关闭连接
		delete sock;   //删除对象指针
		sock = nullptr;
	};

	_socket = std::unique_ptr<zmq::socket_t, decltype(deleter)>(new zmq::socket_t(_context, ZMQ_REP), deleter);

	ostringstream os;
	os << "tcp://*:" << port;
	_socket->bind(os.str());
}

inline void buttonrpc::send(zmq::message_t &data)
{
	_socket->send(data);
}

inline void buttonrpc::recv(zmq::message_t &data)
{
	_socket->recv(&data);
}

inline void buttonrpc::set_timeout(uint32_t ms)
{
	// only client can set
	if (_role == RPC_CLIENT)
	{
		_socket->setsockopt(ZMQ_RCVTIMEO, ms);
	}
}

inline void buttonrpc::run()
{
	// only server can call
	if (_role != RPC_SERVER)
	{
		return;
	}
	while (1)
	{
		zmq::message_t data;
		recv(data);
		StreamBuffer iodev((char *)data.data(), data.size());
		Serializer ds(iodev);

		std::string funname;
		ds >> funname;
		Serializer *r = call_(funname, ds.current(), ds.size() - funname.size());

		zmq::message_t retmsg(r->size());
		memcpy(retmsg.data(), r->data(), r->size());
		send(retmsg);
		delete r;
	}
}

// 服务端通过call_函数,调用已经注册的函数
inline Serializer *buttonrpc::call_(const std::string &name, const char *data, int len)
{
	Serializer *ds = new Serializer();
	if (_handlers.find(name) == _handlers.end())
	{
		(*ds) << value_t<int>::code_type(RPC_ERR_FUNCTIION_NOT_BIND);
		(*ds) << value_t<int>::msg_type("function not bind: " + name);
		return ds;
	}
	auto fun = _handlers[name];
	fun(ds, data, len);
	ds->reset();
	return ds;
}

//服务端bind函数和调用代理函数
template <typename F>
inline void buttonrpc::bind(const std::string &name, F func)
{
	_handlers[name] = std::bind(&buttonrpc::callproxy<F>, this, func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

template <typename F, typename S>
inline void buttonrpc::bind(const std::string &name, F func, S *s)
{
	_handlers[name] = std::bind(&buttonrpc::callproxy<F, S>, this, func, s, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

template <typename F>
inline void buttonrpc::callproxy(F fun, Serializer *pr, const char *data, int len)
{
	callproxy_(fun, pr, data, len);
}

template <typename F, typename S>
inline void buttonrpc::callproxy(F fun, S *s, Serializer *pr, const char *data, int len)
{
	callproxy_(fun, s, pr, data, len);
}

/**
* @brief 客户端的网络调用，将封装函数名和函数参数信息的Serializer对象，转换成zmq::message_t对象，并通过send函数发送。
		 服务端recv函数收到消息内容后，会先解出函数名，再看是否注册过相应函数，若注册过，则调用函数并填入对应参数。
		 最终，将函数结果通过send函数返回给客户端，同时客户端和服务端均需要实现相同的 Serializer& operator >>  和 Serializer& operator <<
		 便于按照指定格式解析数据。
*
* @return value_t<R> 类型的数据
*/
template <typename R>
inline value_t<R> buttonrpc::net_call(Serializer &ds)
{
	zmq::message_t request(ds.size() + 1);
	memcpy(request.data(), ds.data(), ds.size());
	if (_error_code != RPC_ERR_RECV_TIMEOUT)
	{
		send(request);
	}
	zmq::message_t reply;
	recv(reply);
	value_t<R> val;
	if (reply.size() == 0)
	{
		// timeout
		_error_code = RPC_ERR_RECV_TIMEOUT;
		val.set_code(RPC_ERR_RECV_TIMEOUT);
		val.set_msg("recv timeout");
		return val;
	}
	_error_code = RPC_ERR_SUCCESS;
	ds.clear();
	ds.write_raw_data((char *)reply.data(), reply.size());
	ds.reset();

	ds >> val;
	return val;
}

#endif