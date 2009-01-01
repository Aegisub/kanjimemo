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

#pragma once

#include <random>

namespace Halley {
	typedef std::tr1::mt19937 gen_type;
	class Random {
	private:
		//gen_type generator;
		//std::tr1::variate_generator<gen_type& , std::tr1::uniform_real<float> > rng;

	public:
		Random();

		template <typename T>
		T Get(T max)
		{
			return (T)(GetDouble()*max);
		}

		template <typename T>
		T Get(T min, T max)
		{
			return (T)(GetDouble()*(max-min) + min);
		}

		double GetDouble();
	};
}