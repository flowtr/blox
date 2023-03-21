#pragma once

#include "run.hpp"

namespace bobux::engine {
class Workspace {
public:
	inline auto setup_lua_state(lua_State* lua_state) {
		lua_newtable(lua_state);
		lua_pushstring(lua_state, "Workspace");
		lua_newtable(lua_state);
		lua_settable(lua_state, -3);

		lua_setglobal(lua_state, "game");
		lua_settop(lua_state, 0);
	}
};
}
