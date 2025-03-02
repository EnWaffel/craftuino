#include "extension/ExtensionAPIImpl.h"
#include "Sketch.h"

void ExtensionAPIImpl::RegisterCommand(const std::string& name, Command* cmd)
{
    Sketch::CUR_SKETCH->cmdMgr.AddCommand(name, cmd);
    cmd->api = this;
}

std::shared_ptr<Cmd> ExtensionAPIImpl::CompileLine(const std::string &line, const std::string& func, int lineNum)
{
    std::shared_ptr<Cmd> cmd = std::make_shared<Cmd>();
    Sketch::CUR_SKETCH->cmdMgr.CompileLine(Sketch::CUR_SKETCH->prog, line, lineNum, func, true, cmd.get());
    return cmd;
}

void ExtensionAPIImpl::GenerateCommand(Cmd* cmd, Func& func, std::ofstream& out)
{
    Sketch::CUR_SKETCH->cmdMgr.GenerateCommand(Sketch::CUR_SKETCH->prog, func, *cmd, out);
}