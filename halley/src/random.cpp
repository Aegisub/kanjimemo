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

  Copyright (c) 2008 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#include "random.h"
#include <ctime>
#include <cstdlib>
using namespace Halley;

Random::Random()
//: generator((unsigned)rand()),
//  rng(generator, std::tr1::uniform_real<float>(0, 1))
{

}

double Random::GetDouble()
{
	/*
	double value = rng();
	assert (value >= 0);
	assert (value < 1);
	return value;
	*/
	return ((double)rand())/RAND_MAX;
}
