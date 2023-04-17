#include <sstream>
#include <string>
#include "data_block_handler.h"

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
				request_data_from_device_reader(task_data);
			}
		});
	}
}

std::vector<DataBlock> DataBlockHandler::request_data_from_device_reader(const std::vector<size_t>& hashes)
{
	std::vector<DataBlock> data_blocks;
	data_blocks.reserve(hashes.size());
	for(const size_t hash : hashes)
	{
		const std::string str_hash = std::to_string(hash);
		const auto block_num = m_DeviceReader->get_block_number(str_hash);
		const auto buffer_size = m_DeviceReader->get_block_size(str_hash);
		char* data = nullptr;
		m_DeviceReader->get_block_data(block_num, data, buffer_size);

		data_blocks.emplace_back(hash, data, buffer_size);
	}

	return data_blocks;
}

void DataBlockHandler::stop_work()
{
	m_isWorking = false;
	for (auto& thread : m_ProcessingThreads)
	{
		thread.join();
	}
}

