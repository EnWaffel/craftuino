#include "command/Command.h"

void Command::SetProgramAndFunc(Program* prog, Func* func)
{
    this->prog = prog;
    this->func = func;
}
