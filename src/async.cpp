#include <memory>
#include <sstream>
#include <map>
#include <ctime>
#include "async.h"
#include "outputs/console_output.h"
#include "outputs/logfile_output.h"
#include "batch_command_handler.h"

namespace async {

template <typename T, typename... Args> class Singleton
{
public:
	static T& Instance()
	{
		static T Instance(m_Arg);
		return Instance;
	}
	static void setArgs(const size_t arg)
	{
		m_Arg = arg;
	}
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
private:
	Singleton(const T&);
	Singleton& operator=(const T&);
private:
	static size_t m_Arg;
};

int createContext()
{
	std::srand(std::time(nullptr));
	int random_variable = std::rand();
	return random_variable;
}

std::map<int, std::unique_ptr<BatchCommandHandler>> BATCH_COMMAND_HANDLER_LIST;

int connect(const std::size_t bulk_size)
{
//	Singleton<BatchCommandHandler>::setArgs(bulk_size);
	auto batch_command_handler = std::make_unique<BatchCommandHandler>(bulk_size);
	batch_command_handler->addOutputPrinter( new ConsoleOutput() );
	auto logfile_output = new LogFileOutput();
	logfile_output->startWork();
	batch_command_handler->addOutputPrinter( logfile_output );

	auto context = createContext();
	BATCH_COMMAND_HANDLER_LIST[context] = std::move(batch_command_handler);
	return context;
}

void receive(const char* data, const std::size_t length, const int context)
{
	const auto inputData = std::string{data, length};
	std::stringstream ss(inputData);
	std::string command;

	while(std::getline(ss, command, '\n')){
		BATCH_COMMAND_HANDLER_LIST[context]->processCommand(command);
	}
}

void disconnect(const int context)
{
	if(BATCH_COMMAND_HANDLER_LIST.find(context) != BATCH_COMMAND_HANDLER_LIST.end() )
		BATCH_COMMAND_HANDLER_LIST.erase(context);

	if(BATCH_COMMAND_HANDLER_LIST.empty())
		std::cout << "No more connections" << std::endl;
}

}
