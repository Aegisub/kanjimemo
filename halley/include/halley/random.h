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

namespace Halley {
	class Random {
	public:
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