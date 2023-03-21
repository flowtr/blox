#include "lua.h"
#include "lualib.h"
#include "Luau/CodeGen.h"
#include "Luau/Compiler.h"
#include "Luau/BytecodeBuilder.h"
#include "Luau/Parser.h"
#include "Luau/Common.h"
#include "Luau/Ast.h"
#include "Luau/AstJsonEncoder.h"
#include "Luau/Parser.h"
#include "Luau/ParseOptions.h"
#include "Luau/ToString.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <iostream>
#include <string>
#include <bobux/run.hpp>
#include <bobux/state.hpp>
#include <bobux/engine.hpp>

auto main(int argc, char** argv) -> int {
	Luau::Allocator allocator;
	Luau::AstNameTable names(allocator);

	Luau::ParseOptions options;
	options.captureComments = true;
	options.supportContinueStatement = true;
	options.allowTypeAnnotations = true;
	options.allowDeclarationSyntax = true;

	std::ifstream sourceFileStream(argv[argc - 1]); 
	std::stringstream sourceStream;
	sourceStream << sourceFileStream.rdbuf();
	sourceFileStream.close();
	std::string source = sourceStream.str();
	
	Luau::ParseResult parseResult = Luau::Parser::parse(
		source.data(),
		source.size(),
		names,
		allocator,
		options
	);

	if (parseResult.errors.size() > 0)
	{
		fprintf(stderr, "Parse errors were encountered:\n");
		for (const Luau::ParseError& error : parseResult.errors)
		{
			fprintf(stderr,
				"%s - %s\n",
				toString(
					error.getLocation()
				).c_str(),
				error.getMessage().c_str()
			);
		}
		fprintf(stderr, "\n");
		return 1;
	}

	//printf("AST: %s", Luau::toJson(parseResult.root, parseResult.commentLocations).c_str());
	Luau::CompileOptions compile_options = {};
	compile_options.optimizationLevel = 3;
	compile_options.debugLevel = 1;
	compile_options.coverageLevel = 0;

	std::unique_ptr<lua_State, void (*)(lua_State*)> lua_state(
		luaL_newstate(),
		lua_close
	);
	luaL_sandboxthread(lua_state.get());

	lua_pushcfunction(lua_state.get(), bobux::print, "print");
  lua_setglobal(lua_state.get(), "print");
  lua_settop(lua_state.get(), 0);
	
	auto engine = std::make_unique<bobux::engine::Engine>();

	auto workspace = std::make_unique<bobux::engine::Workspace>();
	workspace->setup_lua_state(lua_state.get());
		
	//auto result = bobux::run_code(lua_state.get(), source, compile_options);
	//std::cout << result;

	while (engine->should_run()) {
		glClearColor(0.3, 0.3, 0.3, 1.0);

		engine->handle_post_draw();
	}


	return 0;
}
