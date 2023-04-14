#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>
#include <vector>
#include <memory>
#include "batch_command.h"

using BatchCommands = std::vector< std::unique_ptr< BatchCommand > >;

class Tokenizer
{
public:
    Tokenizer(const size_t commandsMaxSize);
    BatchCommands Tokenize( const std::string& str_command );
private:
    size_t m_CommandsMaxSize;
    size_t m_StaticCommandCounter;
    size_t m_Brc;
};


#endif // TOKENIZER_H
