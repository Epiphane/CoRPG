#include <string>

#include "main.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include "LuaBridge/Source/LuaBridge/LuaBridge.h"

void run_script(std::string filename);
