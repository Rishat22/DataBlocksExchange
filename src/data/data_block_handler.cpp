#include <sstream>
#include <string>
#include "data_block_handler.h"
#include "data_block.h"

DataBlockHandler::DataBlockHandler(std::shared_ptr<IDeviceReader> device_reader)
	: m_TaskReady(false)
	, m_isWorking(false)
	, m_DeviceReader(device_reader)
{
}

DataBlockHandler::~DataBlockHandler()
{
	stop_work();
}

void DataBlockHandler::request_data(const std::vector<size_t>& hashes)
{
	{
		std::lock_guard lock{m_Mtx};
		m_CmdTasks.push(hashes);
		m_TaskReady = true;
	}
	m_ConditionVar.notify_one();
}

std::vector<size_t> DataBlockHandler::wait_for_task(bool& task_ready)
{
	std::unique_lock lock(m_Mtx);
	m_ConditionVar.wait(lock, [&task_ready]() { return task_ready; });
	const auto task_data = m_CmdTasks.front();
	m_CmdTasks.pop();
	task_ready = false;
	lock.unlock();
	return task_data;
}

void DataBlockHandler::start_work()
{
	m_isWorking = true;
	for (auto& thread : m_ProcessingThreads)
	{
		thread = std::thread([&]()
		{
			while (m_isWorking && !m_CmdTasks.empty())
			{
				const auto task_data = wait_for_task(m_TaskReady);
				send_data(task_data);
			}
		});
	}
}

void DataBlockHandler::send_data(const std::vector<size_t>& hashes)
{
	for(const size_t hash : hashes)
	{
		const std::string str_hash = std::to_string(hash);
		const auto block_num = m_DeviceReader->get_block_number(str_hash);
		const auto buffer_size = m_DeviceReader->get_block_size(str_hash);
		char* data = nullptr;
		m_DeviceReader->get_block_data(block_num, data, buffer_size);

		DataBlock block(hash, data, buffer_size);
	}
}

void DataBlockHandler::stop_work()
{
	m_isWorking = false;
	for (auto& thread : m_ProcessingThreads)
	{
		thread.join();
	}
}

