#include <string>
#include <iostream>
#include "buttonrpc.hpp"

// 测试例子
void testFun()
{
	std::cout << "call testFun" << std::endl;
}

void testInputValue(int arg)
{
	std::cout << "call testInputValue" << std::endl;
}

int testAdd(int a, int b)
{
	std::cout << "call testAdd " << std::endl;
	return a + b;
}

class Student
{
public:
	std::string toString(std::string name, int age, std::string address)
	{
		std::cout << "call Student::toString" << std::endl;
		return "name:" + name + " age:" + std::to_string(age) + " address:" + address;
	}
};

struct PersonInfo
{
	int age;
	std::string name;
	float height;

	// must implement
	friend Serializer &operator>>(Serializer &in, PersonInfo &d)
	{
		in >> d.age >> d.name >> d.height;
		return in;
	}
	friend Serializer &operator<<(Serializer &out, PersonInfo d)
	{
		out << d.age << d.name << d.height;
		return out;
	}
};

PersonInfo personFun(PersonInfo d, int hight)
{
	std::cout << "call personFun" << std::endl;
	PersonInfo ret;
	ret.age = d.age + 10;
	ret.name = d.name + " is good";
	ret.height = d.height + hight;
	return ret;
}

int main()
{
	buttonrpc server;
	server.as_server(5555);

	server.bind("testFun", testFun);
	server.bind("testInputValue", testInputValue);
	server.bind("testAdd", std::function<int(int, int)>(testAdd));

	Student stu;
	server.bind("toString", &Student::toString, &stu);

	server.bind("personFun", personFun);

	std::cout << "run rpc server on: " << 5555 << std::endl;
	server.run();

	return 0;
}
