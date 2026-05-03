/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
--------------------------------------------------------
	Valid Arguments:
	<shader path> <dev path>
	
	for default Path:
	-d
======================================================*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "SystemTypes.hpp"

#define TUDO_NO_LOGGER
#include "FileSystem.hpp"

#define SHDC_DEFAULT_SHADERPATH "Data/Shaders"
#define SHDC_DEFAULT_DEVPATH	"Data/Development/Shaders"

using namespace Tudo;

static strg CompileShader(strgv args);
static strg CompileAllShaders(strgv sdir, strgv dir);

int main(int argc, char* argv[])
{
	bool ready		= false;
	strg shdpath	= SHDC_DEFAULT_SHADERPATH;
	strg devpath	= SHDC_DEFAULT_DEVPATH;

	if (argc == 3)
	{
		ready	= true;
		shdpath = strg(argv[1]);
		devpath = strg(argv[2]);
	}
	else if (argc == 2)
	{
		if (strg(argv[1]) == "-d") ready = true;
		else std::cout << "Invalid Argument" << std::endl;
	}
	else std::cout << "Invalid Arguments" << std::endl;

	if (ready)
	{
		if (!FileSystem::Exists(shdpath))
			FileSystem::DirectoryCreate(shdpath);

#if _WIN32
		const strg d3dpath(shdpath + "/D3D");
		const strg vkpath(shdpath + "/SPIRV");

		if (!FileSystem::Exists(d3dpath))
			FileSystem::DirectoryCreate(d3dpath);
		if (!FileSystem::Exists(vkpath))
			FileSystem::DirectoryCreate(vkpath);
#else
		const strg mtpath(shdpath + "/METAL");

		if (!FileSystem::Exists(mtpath))
			FileSystem::DirectoryCreate(mtpath);
#endif

		std::cout << CompileAllShaders(shdpath, devpath) << std::endl;
	}
	return 0;
}

strg CompileShader(strgv args)
{
	std::ostringstream command;

#if _WIN32
	command << "Shaderc.exe " << args;
#elif __APPLE__
	const strg exepath = FileSystem::GetResourcePath("shadercRelease").string() + " ";
	command << exepath << args;
#endif

#if _WIN32
	FILE* pipe = _popen(command.str().c_str(), "r");
#elif __APPLE__
	FILE* pipe = popen(command.str().c_str(), "r");
#endif
	if (!pipe)
	{
		const strg errmsg = "Failed opening Shaderc: " + command.str();
		return errmsg;
	}

	std::ostringstream output;
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		output << buffer;

#if _WIN32
	if (_pclose(pipe) == -1)
#elif __APPLE__
	if (pclose(pipe) == -1)
#endif
	{
		const strg errmsg = "Failed closing Shaderc: " + command.str();
		return errmsg;
	}

	return output.str();
}

strg CompileAllShaders(strgv sdir, strgv dir)
{
	const strg ssdir(sdir);
	strg result;

	const std::vector<strg> shaders = FileSystem::SubDirectories(dir);
	const strg ishaders = " -i " + strg(dir) + "/Shared";

	for (auto& it : shaders)
	{
		std::filesystem::path path(it);
		const strg name = path.filename().string();
		if (name == "Shared") continue;

		const strg inpath = strg(dir) + "/" + name + "/" + name;

#if _WIN32
		const strg outpathD3D = ssdir + "/D3D/" + name;
		const strg outpathVK = ssdir + "/SPIRV/" + name;

		const strg arg0 = "-f " + inpath + ".vs -o " + outpathD3D + ".vsb --type v --platform windows -p s_5_0 -O 3" + ishaders;
		result += CompileShader(arg0) + "\n";

		const strg arg1 = "-f " + inpath + ".fs -o " + outpathD3D + ".fsb --type f --platform windows -p s_5_0 -O 3" + ishaders;
		result += CompileShader(arg1) + "\n";

		const strg arg2 = "-f " + inpath + ".vs -o " + outpathVK + ".vsb --type v --platform windows -p spirv" + ishaders;
		result += CompileShader(arg2) + "\n";

		const strg arg3 = "-f " + inpath + ".fs -o " + outpathVK + ".fsb --type f --platform windows -p spirv" + ishaders;
		result += CompileShader(arg3) + "\n";

#elif __APPLE__
		const strg outpathMT = ssdir + "/METAL/" + name;

		const strg arg4 = "-f " + inpath + ".vs -o " + outpathMT + ".vsb --type v --platform osx -p metal" + ishaders;
		result += CompileShader(arg4) + "\n";

		const strg arg5 = "-f " + inpath + ".fs -o " + outpathMT + ".fsb --type f --platform osx -p metal" + ishaders;
		result += CompileShader(arg5) + "\n";
#endif
	}

	return result;
}
