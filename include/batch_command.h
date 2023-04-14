#ifndef BATCHCOMMAND_H
#define BATCHCOMMAND_H
#include <string>

enum class TypeCommad{
    DEFAULT,
    ADD_STATIC,
    ADD_DYNAMIC,
    OUT_STATIC,
    OUT_DYNAMIC,
    //	WHILE,
    //	END
};

class BatchCommand{
public:
    BatchCommand(const TypeCommad type, const std::string& arg );
    BatchCommand(const TypeCommad type );
    TypeCommad type() const;
    std::string arg() const;

private:
    std::string m_Arg;
    TypeCommad m_Type = TypeCommad::DEFAULT;
};

#endif // BATCHCOMMAND_H
