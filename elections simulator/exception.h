#pragma once
#include <stdexcept>

namespace election
{
	class salException : public std::exception
	{
		std::string _msg;
	public:
		salException(const std::string& msg) : _msg(msg) {}
		const char* what() const throw()
		{
			return _msg.c_str();
		}
	};
}