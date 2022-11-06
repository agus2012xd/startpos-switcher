#pragma once

#include <gd.h>
#include <array>
#include <cstddef>

using namespace gd;
using namespace cocos2d;

namespace switcher {
    extern std::vector<std::pair<StartPosObject*, CCPoint>> startPoses;
    extern std::array<enumKeyCodes, 2> keyBinds;

    extern std::ptrdiff_t index;

    extern CCLabelBMFont* text;

    extern bool direction;
    
    extern bool toSwitch;
}