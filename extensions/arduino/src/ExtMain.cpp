#include "ExtensionAPI.h"

#include "DelayCmd.h"
#include "ExecuteCmd.h"
#include "PinModeCmd.h"
#include "SetPinCmd.h"
#include "FunctionCmd.h"
#include "VarCmd.h"
#include "GVarCmd.h"
#include "SetCmd.h"
#include "GetPinCmd.h"
#include "SerialCmd.h"

extern "C" {
__declspec(dllexport) EXT_MAIN
{
    api->RegisterCommand("delay", new DelayCmd);
    api->RegisterCommand("delay", new DelayCmd);
    api->RegisterCommand("pinmode", new PinModeCmd);
    api->RegisterCommand("setpin", new SetPinCmd);
    api->RegisterCommand("function", new FunctionCmd);
    api->RegisterCommand("var", new VarCmd);
    api->RegisterCommand("gvar", new GVarCmd);
    api->RegisterCommand("set", new SetCmd);
    api->RegisterCommand("execute", new ExecuteCmd);
    api->RegisterCommand("getpin", new GetPinCmd);
    api->RegisterCommand("serial", new SerialCmd);
}
}