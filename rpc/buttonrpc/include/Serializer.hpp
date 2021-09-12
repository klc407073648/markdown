#ifndef __SERIALIZER_H_
#define __SERIALIZER_H_

#include "StreamBuffer.hpp"
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <utility>
#include <tuple>
using namespace std;

//字节序定义
enum ByteOrder
{
	BigEndian,
	LittleEndian
};

class Serializer
{
public:
	/**
    * @brief 构造函数
    */
	Serializer() : _byteorder(LittleEndian) {}

	/**
    * @brief 带入参的构造函数
    */
	Serializer(StreamBuffer dev, int byteorder = LittleEndian) : _iodevice(dev), _byteorder(byteorder) {}

	/**
    * @brief 析构函数
    */
	~Serializer() {}

	/**
    * @brief 重置dev的指针指向位置
    */
	void reset() { _iodevice.reset(); }

	/**
     * @brief 获取dev的大小
     *
     * @return int dev的大小
     */
	int size() { return _iodevice.size(); }

	/**
     * @brief 跳过原始数据的k个位置，使得dev指向_curpos+k
     */
	void skip_raw_data(int k) { _iodevice.offset(k); }

	/**
     * @brief 获取dev的全部数据
     *
     * @return const char * 类型的数据
     */
	const char *data() { return _iodevice.data(); }

	/**
     * @brief 调整字节序，若为大端字节序，则字符串逆序
     */
	void byte_orser(char *in, int len)
	{
		if (_byteorder == BigEndian)
		{
			reverse(in, in + len);
		}
	}

	/**
     * @brief 写入数据，并调整dev的指针指向
     */
	void write_raw_data(char *in, int len)
	{
		_iodevice.input(in, len);
		_iodevice.offset(len);
	}

	/**
     * @brief 获取当前字节流位置开始的dev的数据
     *
     * @return const char * 类型的数据
     */
	const char *current() { return _iodevice.current(); }

	/**
     * @brief 清空dev的内容，调整位置为0
     */
	void clear()
	{
		_iodevice.clear();
		reset();
	}

	/**
     * @brief 返回当前位置以后len个字节数据
     */
	void get_length_mem(char *p, int len)
	{
		memcpy(p, _iodevice.current(), len);
		_iodevice.offset(len);
	}

public:
	/**
     * @brief 输出类型
     */
	template <typename T>
	void output_type(T &t);

	/**
     * @brief 输入类型
     */
	template <typename T>
	void input_type(T t);

	/**
     * @brief 操作符重载 >>
     */
	template <typename T>
	Serializer &operator>>(T &i)
	{
		output_type(i);
		return *this;
	}

	/**
     * @brief 操作符重载 <<
     */
	template <typename T>
	Serializer &operator<<(T i)
	{
		input_type(i);
		return *this;
	}

	/**
     * @brief 从Serializer获取Tuple(t)的值
     */
	template <typename Tuple, std::size_t Id>
	void getv(Serializer &ds, Tuple &t)
	{
		ds >> std::get<Id>(t);
	}

	/**
     * @brief 获取tuple
     */
	template <typename Tuple, std::size_t... I>
	Tuple get_tuple(std::index_sequence<I...>)
	{
		Tuple t;
		initializer_list<int>{((getv<Tuple, I>(*this, t)), 0)...};
		return t;
	}

private:
	//字节序
	int _byteorder;

	//iodev
	StreamBuffer _iodevice;
};

/**
* @brief 从StreamBuffer的当前位置开始，取sizeof(T)个数据，赋值给t
*/
template <typename T>
inline void Serializer::output_type(T &t)
{
	int len = sizeof(T);
	char *d = new char[len];
	if (!_iodevice.is_eof())
	{
		memcpy(d, _iodevice.current(), len);
		_iodevice.offset(len);
		byte_orser(d, len);
		t = *reinterpret_cast<T *>(&d[0]);
	}
	delete[] d;
}

/**
* @brief string类型偏特化处理: 从StreamBuffer的当前位置开始，取sizeof(T)个数据，赋值给t
*/
template <>
inline void Serializer::output_type(std::string &in)
{
	int marklen = sizeof(uint16_t);
	char *d = new char[marklen];
	memcpy(d, _iodevice.current(), marklen);
	byte_orser(d, marklen);
	int len = *reinterpret_cast<uint16_t *>(&d[0]);
	_iodevice.offset(marklen);
	delete[] d;
	if (len == 0)
		return;
	in.insert(in.begin(), _iodevice.current(), _iodevice.current() + len);
	_iodevice.offset(len);
}

/**
* @brief 将sizeof(T)个数据t，添加到StreamBuffer末尾
*/
template <typename T>
inline void Serializer::input_type(T t)
{
	int len = sizeof(T);
	char *d = new char[len];
	const char *p = reinterpret_cast<const char *>(&t);
	memcpy(d, p, len);
	byte_orser(d, len);
	_iodevice.input(d, len);
	delete[] d;
}

/**
* @brief string类型偏特化处理: 将sizeof(T)个数据t，添加到StreamBuffer末尾
*/
template <>
inline void Serializer::input_type(std::string in)
{
	// 先存入字符串长度
	uint16_t len = in.size();
	char *p = reinterpret_cast<char *>(&len);
	byte_orser(p, sizeof(uint16_t));
	_iodevice.input(p, sizeof(uint16_t));

	// 存入字符串
	if (len == 0)
		return;
	char *d = new char[len];
	memcpy(d, in.c_str(), len);
	_iodevice.input(d, len);
	delete[] d;
}

/**
* @brief const char *in类型偏特化处理: 将sizeof(T)个数据t，添加到StreamBuffer末尾
*/
template <>
inline void Serializer::input_type(const char *in)
{
	input_type<std::string>(std::string(in));
}

#endif