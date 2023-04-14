#ifndef IOUTPUT_H
#define IOUTPUT_H
#include <string>
#include <vector>

class IOutput
{
public:
	virtual ~IOutput() = default;
	virtual void print(const std::vector<std::string>& str_data) = 0;
};
#endif // IOUTPUT_H
