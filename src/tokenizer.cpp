#include "tokenizer.h"


Tokenizer::Tokenizer(const size_t commandsMaxSize)
	: m_CommandsMaxSize(commandsMaxSize)
	, m_StaticCommandCounter(0)
	, m_Brc(0)
{
}

BatchCommands Tokenizer::Tokenize(const std::string& str_command) {
	BatchCommands result_commands;
	if(str_command == "{")
	{
		if(!m_Brc++)
		{
			result_commands.emplace_back( new BatchCommand( TypeCommad::OUT_STATIC ) );
		}
	}
	else if (str_command == "}")
	{
		if(!--m_Brc)
		{
			result_commands.emplace_back( new BatchCommand( TypeCommad::OUT_DYNAMIC ) );
		}
	}
	else
	{
		if(!m_Brc)
		{
			result_commands.emplace_back( new BatchCommand( TypeCommad::ADD_STATIC,  str_command ) );
			if(++m_StaticCommandCounter == m_CommandsMaxSize)
			{
				result_commands.emplace_back( new BatchCommand( TypeCommad::OUT_STATIC ) );
				m_StaticCommandCounter = 0;
			}
		}
		else
			result_commands.emplace_back( new BatchCommand( TypeCommad::ADD_DYNAMIC,  str_command ) );

	}

	return result_commands;
}
