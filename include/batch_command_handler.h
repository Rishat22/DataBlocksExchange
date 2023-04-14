#ifndef BATCHCOMMANDHANDLER_H
#define BATCHCOMMANDHANDLER_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "outputs/ioutput.h"
#include "tokenizer.h"

class BatchCommandHandler
{
public:
    BatchCommandHandler(const size_t commandsMaxSize);
    ~BatchCommandHandler();
    void processCommand( const std::string& str_command );
    void addOutputPrinter( IOutput* output_printer );
private:
    void processCommand(const std::vector<std::unique_ptr<BatchCommand> >& batch_commands);
    void printCommands(const std::vector< std::string > str_commands);
private:
    Tokenizer m_Tokenizer;
    std::vector< std::unique_ptr< IOutput > > m_OutputPrinters;
    std::vector< std::string > m_StaticCommands;
    std::vector< std::string > m_DynamicCommands;
};

#endif // BATCHCOMMANDHANDLER_H
