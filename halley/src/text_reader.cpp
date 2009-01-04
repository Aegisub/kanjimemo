/*********************************************************\
         __      __           __  __
        / /     / / _____    / / / / ______   __     __
       / /     / / /___  \  / / / / / ___  | / /    / /
      / /_____/ / _____| / / / / / / /__/ / / /    / /
     / ______  / / ___  / / / / / / _____/ / /    / /
    / /     / / / /__/ / / / / / / |____  / |____/ /
   /_/     /_/ |______/ /_/ /_/  \_____/  \_____  /
                                         ______/ /
            High Level Game Framework   /_______/

  ------------------------------------------------------

  Copyright (c) 2009 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#include "halley.h"
#include "text_reader.h"
#include <fstream>
#include <algorithm>
using namespace Halley;

StringArray TextReader::ReadFile(String filename)
{
	StringArray result;
	std::ifstream stream(filename.c_str(), std::ios::in);
	if (stream.is_open()) {
		std::string line;
		while (!stream.eof()) {
			getline(stream, line);
			result.push_back(line);
		}
	}

	return result;
}
