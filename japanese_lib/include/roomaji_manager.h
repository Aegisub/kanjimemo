#pragma once

#include "roomaji.h"

namespace Japanese {
	class RoomajiManager {
	public:
		static Roomaji* GetRoomaji();

	private:
		static Roomaji* roomaji;
	};
}