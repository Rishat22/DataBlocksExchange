#ifndef LOGFILEOUTPUT_H
#define LOGFILEOUTPUT_H
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <thread>
#include <condition_variable>
#include "ioutput.h"

const size_t THREADS_COUNT = 2;

class LogFileOutput : public IOutput
{
public:
	LogFileOutput();
	virtual ~LogFileOutput();
	void startWork();
	void stopWork();
	void print(const std::vector<std::string>& str_data) override;
private:
	void saveData(const std::vector<std::string>& str_data);
	std::string getFileName();
	std::vector<std::string> wait_for_task(bool& task_ready);
private:
	std::queue<std::vector<std::string>> m_CmdTasks;
	std::condition_variable m_ConditionVar;
	std::mutex m_Mtx;
	bool m_TaskReady;
	bool m_isWorking;
	std::array<std::thread, THREADS_COUNT> m_ProcessingThreads;
};

#endif // LOGFILEOUTPUT_H
