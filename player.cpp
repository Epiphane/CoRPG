#include <sstream>

#include "player.h"

using namespace std;

int Player::getLevel() {
	return atoi(getProperty("level").c_str());
}
