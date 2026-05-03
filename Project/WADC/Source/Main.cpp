/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
--------------------------------------------------------
	Valid Arguments:
	-m  <inpath>   <outloc> <saveExt>
	-ma <fileroot> <outloc> <saveExt>
======================================================*/
#include <iostream>
#include <fstream>
#include <vector>
#include "SystemTypes.hpp"
#include "Globals.hpp"
#include "FileSystem.hpp"
#include "WADMaker.hpp"

using namespace Tudo;

int main(int argc, char* argv[])
{
	if (argc >= 3)
	{
		strg command(argv[1]);
		
		if (command == "-m") // Make
		{
			strg inpath(argv[2]);
			strg outloc(argv[3]);
			bool saveExt((bool)(::atoi(argv[4])));
			WADMaker::Make(inpath, outloc, saveExt);
		}
		else if (command == "-ma") // MakeAll
		{
			strg fileroot(argv[2]);
			strg outloc(argv[3]);
			bool saveExt((bool)(::atoi(argv[4])));
			WADMaker::MakeAll(fileroot, outloc, saveExt);
		}
		else
		{
			std::cout << "Invalid Command!" << std::endl;
			return 0;
		}
		std::cout << "Done!" << std::endl;
	}
	else std::cout << "Invalid Arguments" << std::endl;
	return 0;
}
