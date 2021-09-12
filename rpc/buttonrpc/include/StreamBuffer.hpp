#ifndef __STREAMBUFFER_H_
#define __STREAMBUFFER_H_

#include <vector>
#include <algorithm>
#include <cstdint>
using namespace std;

class StreamBuffer : public vector<char>
{
public:
	/**
    * @brief 构造函数
    */
	StreamBuffer() : _curpos(0) {}

	/**
    * @brief 带入参的构造函数
    */
	StreamBuffer(const char *in, size_t len) : _curpos(0)
	{
		insert(begin(), in, in + len);
	}

	/**
    * @brief 析构函数
    */
	~StreamBuffer() {}

	/**
    * @brief 重置当前字节流位置
    */
	void reset() { _curpos = 0; }

	/**
     * @brief 获取StreamBuffer的所有数据
     *
     * @return const char * 类型的数据
     */
	const char *data() { return &(*this)[0]; }

	/**
     * @brief 获取当前字节流位置开始的StreamBuffer的数据
     *
     * @return const char * 类型的数据
     */
	const char *current() { return &(*this)[_curpos]; }

	/**
    * @brief 从当前字节流位置 偏移 K个单位
    */
	void offset(int k) { _curpos += k; }

	/**
     * @brief 判断是否越界
     *
     * @return 如果越界返回true，否则false
     */
	bool is_eof() { return (_curpos >= size()); }

	/**
     * @brief 从尾部添加数据
     */
	void input(char *in, size_t len) { insert(end(), in, in + len); }

	/**
     * @brief 从当前字节流位置 找c对应的第一个出现位置
	 * 
     * @return c对应的位置序号，若没找到返回-1
     */
	int findc(char c)
	{
		iterator itr = find(begin() + _curpos, end(), c);
		if (itr != end())
		{
			return itr - (begin() + _curpos);
		}
		return -1;
	}

private:
	// 当前字节流位置
	unsigned int _curpos;
};
#endif
