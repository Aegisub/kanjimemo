
#pragma once

#include "roomaji.h"

namespace Japanese {
	class RoomajiHepburn : public Roomaji {
	public:
		RoomajiHepburn(int type);	// 0 = traditional, 1 = revised, 2 = modified

	protected:
		virtual void AdjustSyllableForPrevious(String &current,const String &previous) const ;
		virtual bool AdjustSyllableForNext(String &current,const String &next) const;

	private:
		int type;
	};
};
