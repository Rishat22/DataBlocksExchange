#include "batch_command.h"


BatchCommand::BatchCommand(const TypeCommad type, const std::string& arg)
{
	this->m_Type = type;
	this->m_Arg = arg;
}

BatchCommand::BatchCommand(const TypeCommad type)
{
	this->m_Type = type;
}

TypeCommad BatchCommand::type() const
{
	return m_Type;
}

std::string BatchCommand::arg() const
{
	return m_Arg;
}
