#ifndef LOGFILEOUTPUT_H
#define LOGFILEOUTPUT_H
#include <vector>
#include <array>
#include <queue>
#include <thread>
#include <condition_variable>
#include "data/idevice_reader.h"

const size_t THREADS_COUNT = 2;

class DataBlockHandler
{
public:
	DataBlockHandler(std::shared_ptr<IDeviceReader> device_reader);
	virtual ~DataBlockHandler();
	void start_work();
	void stop_work();
	void request_data(const std::vector<size_t>& hashes);
private:
	void send_data(const std::vector<size_t>& hashes);
	std::vector<size_t> wait_for_task(bool& task_ready);
private:
	std::queue<std::vector<size_t>> m_CmdTasks;
	std::condition_variable m_ConditionVar;
	std::mutex m_Mtx;
	bool m_TaskReady;
	bool m_isWorking;
	std::array<std::thread, THREADS_COUNT> m_ProcessingThreads;
	std::shared_ptr<IDeviceReader> m_DeviceReader;
};

#endif // LOGFILEOUTPUT_H
