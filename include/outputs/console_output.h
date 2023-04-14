#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H
#include <string>
#include <vector>
#include "ioutput.h"

class ConsoleOutput : public IOutput
{
public:
	virtual ~ConsoleOutput() = default;
	void print(const std::vector<std::string>& str_data) override;
};

#endif // CONSOLEOUTPUT_H
