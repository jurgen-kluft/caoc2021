local GlobExtension = require("tundra.syntax.glob")
Build {
	ReplaceEnv = {
		OBJECTROOT = "target",
	},
	Env = {
		CPPDEFS = {
			{ "TARGET_PC", "PLATFORM_64BIT"; Config = "win64-*-*-*" },
			{ "TARGET_MAC_DEV_DEBUG", "TARGET_MAC", "PLATFORM_64BIT"; Config = "macosx-*-debug-dev" },
			{ "TARGET_MAC_DEV_RELEASE", "TARGET_MAC", "PLATFORM_64BIT"; Config = "macosx-*-release-dev" },
			{ "TARGET_MAC_TEST_DEBUG", "TARGET_MAC", "PLATFORM_64BIT"; Config = "macosx-*-debug-test" },
			{ "TARGET_MAC_TEST_RELEASE", "TARGET_MAC", "PLATFORM_64BIT"; Config = "macosx-*-release-test" },
		},
	},
	Units = function ()
		-- Recursively globs for source files relevant to current build-id
		local function SourceGlob(dir)
			return FGlob {
				Dir = dir,
				Extensions = { ".c", ".cpp", ".s", ".asm" },
				Filters = {
					{ Pattern = "_win32"; Config = "win64-*-*" },
					{ Pattern = "_mac"; Config = "macosx-*-*" },
					{ Pattern = "_test"; Config = "*-*-*-test" },
				}
			}
		end
		local xentry_library = StaticLibrary {
			Name = "xentry",
			Config = "*-*-*-*",
			Sources = { SourceGlob("../xentry/source/main/cpp") },
			Includes = { "../xentry/source/main/include" },
		}
		local xbase_library = StaticLibrary {
			Name = "xbase",
			Config = "*-*-*-*",
			Sources = { SourceGlob("../xbase/source/main/cpp") },
			Includes = { "../xbase/source/main/include" },
		}
		local xtext_library = StaticLibrary {
			Name = "xtext",
			Config = "*-*-*-*",
			Sources = { SourceGlob("../xtext/source/main/cpp") },
			Includes = { "../xtext/source/main/include", "../xbase/source/main/include" },
		}
		local xfile_library = StaticLibrary {
			Name = "xfile",
			Config = "*-*-*-*",
			Sources = { SourceGlob("../xfile/source/main/cpp") },
			Includes = { "../xfile/source/main/include", "../xbase/source/main/include" },
		}
		local xgenerics_library = StaticLibrary {
			Name = "xgenerics",
			Config = "*-*-*-*",
			Sources = { SourceGlob("../xgenerics/source/main/cpp") },
			Includes = { "../xgenerics/source/main/include", "../xbase/source/main/include" },
		}
		local xaoc2021_library = StaticLibrary {
			Name = "xaoc2021",
			Config = "*-*-*-*",
			Sources = { SourceGlob("source/main/cpp") },
			Includes = { "source/main/include", "../xbase/source/main/include", "../xtext/source/main/include", "../xfile/source/main/include", "../xgenerics/source/main/include" },
		}
		
		local xunittest_library = StaticLibrary {
			Name = "xunittest",
			Config = "*-*-*-test",
			Sources = { SourceGlob("../xunittest/source/main/cpp") },
			Includes = { "../xunittest/source/main/include" },
		}
		local unittest = Program {
			Name = "xaoc2021_test",
			Config = "*-*-*-test",
			Sources = { SourceGlob("source/test/cpp") },
			Includes = { "source/main/include","source/test/include","../xunittest/source/main/include","../xentry/source/main/include","../xbase/source/main/include", "../xtext/source/main/include", "../xfile/source/main/include", "../xgenerics/source/main/include" },
			Depends = { xbase_library,xtext_library,xfile_library,xgenerics_library,xunittest_library },
		}
		Default(unittest)
	end,
	Configs = {
		Config {
			Name = "macosx-clang",
			Env = {
			PROGOPTS = { "-lc++" },
			CXXOPTS = {
				"-std=c++11",
				"-arch x86_64",
				"-g",
				"-Wno-new-returns-null",
				"-Wno-missing-braces",
				"-Wno-unused-function",
				"-Wno-unused-variable",
				"-Wno-unused-result",
				"-Wno-write-strings",
				"-Wno-c++11-compat-deprecated-writable-strings",
				"-Wno-null-dereference",
				"-Wno-format",
				"-fno-strict-aliasing",
				"-fno-omit-frame-pointer",
			},
		},
		DefaultOnHost = "macosx",
		Tools = { "clang" },
		},
		Config {
			ReplaceEnv = {
				OBJECTROOT = "target",
			},
			Name = "linux-gcc",
			DefaultOnHost = "linux",
			Tools = { "gcc" },
		},
		Config {
			ReplaceEnv = {
				OBJECTROOT = "target",
			},
			Name = "win64-msvc",
			Env = {
				PROGOPTS = { "/SUBSYSTEM:CONSOLE" },
				CXXOPTS = { },
			},
			DefaultOnHost = "windows",
			Tools = { "msvc-vs2017" },
		},
	},
	SubVariants = { "dev", "test" },
}
