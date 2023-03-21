#pragma once

#include "Luau/CodeGen.h"
#include "Luau/Compiler.h"
#include "lua.h"
#include "lualib.h"
#include <iostream>
#include <string>

namespace bobux {
inline auto print(lua_State* lua_state) -> int
{
    int nargs = lua_gettop(lua_state);
    std::string string = "";
    for (int i = 1; i <= nargs; i++)
    {
        if (i > 1) string += " ";
        switch (lua_type(lua_state, i))
        {
        case LUA_TSTRING:
            string += (std::string)lua_tostring(lua_state, i);
        case LUA_TNUMBER:
            string += (int)lua_tonumber(lua_state, i);
        case LUA_TBOOLEAN:
            string += (bool)lua_toboolean(lua_state, i);
        }
    }
    std::cout << string << "\n";
    return 0;
}

inline std::string run_code(lua_State* lua_state, const std::string& source, const Luau::CompileOptions& compile_options)
{
	std::string bytecode = Luau::compile(source, compile_options);

	if (luau_load(lua_state, "=stdin", bytecode.data(), bytecode.size(), 0) != 0)
	{
		size_t len;
		const char* msg = lua_tolstring(lua_state, -1, &len);

		std::string error(msg, len);
		lua_pop(lua_state, 1);

		return error;
	}

	Luau::CodeGen::compile(lua_state, -1);

	lua_State* lua_thread = lua_newthread(lua_state);

	lua_pushvalue(lua_state, -2);
	lua_remove(lua_state, -3);
	lua_xmove(lua_state, lua_thread, 1);

	int status = lua_resume(lua_thread, NULL, 0);

	if (status == 0)
	{

		lua_pop(lua_state, 1);
		return std::string();
	}
	else
	{
		std::string error;

		if (status == LUA_YIELD)
		{
			error = "thread yielded unexpectedly";
		}
		else if (const char* str = lua_tostring(lua_thread, -1))
		{
			error = str;
		}

		error += "\nstack backtrace:\n";
		error += lua_debugtrace(lua_thread);

		lua_pop(lua_state, 1);
		return error;
	}
}
}
