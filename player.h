#include "gameinfo.h"

class Player : public GameObject {
public:
	Player() { setProperty("level", "1"); };

	int getLevel();
};
