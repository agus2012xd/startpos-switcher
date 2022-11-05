#pragma once

#include <gd.h>
#include <array>
#include <cstddef>

using namespace gd;
using namespace cocos2d;

namespace switcher {
    std::vector<std::pair<StartPosObject*, CCPoint>> startPoses = {};
    std::array keyBinds = {KEY_Left, KEY_Right};

    std::ptrdiff_t index = 0;

    CCLabelBMFont* text = nullptr;

    bool direction;
    
    bool toSwitch = false;
}