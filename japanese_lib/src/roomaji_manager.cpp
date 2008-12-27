#include "roomaji_manager.h"
#include "roomaji_hepburn.h"
using namespace Japanese;

Roomaji* RoomajiManager::roomaji = NULL;

Roomaji* RoomajiManager::GetRoomaji()
{
	if (!roomaji) roomaji = new RoomajiHepburn(1);
	return roomaji;
}
