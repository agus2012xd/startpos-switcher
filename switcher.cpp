#include "switcher.hpp"

using namespace gd;
using namespace cocos2d;

std::vector<std::pair<StartPosObject*, CCPoint>> switcher::startPoses = {};
std::array<enumKeyCodes, 2> switcher::keyBinds = {KEY_Left, KEY_Right};

std::ptrdiff_t switcher::index = 0;

CCLabelBMFont* switcher::text = nullptr;

bool switcher::direction;

bool switcher::toSwitch = false;