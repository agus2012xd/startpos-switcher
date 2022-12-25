#pragma once

#include <cocos2d.h>
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

    extern CCSprite* leftArrow;
    extern CCSprite* rightArrow;

    extern CCMenuItemSpriteExtra* leftArrowButton;
    extern CCMenuItemSpriteExtra* rightArrowButton;

    extern bool direction;
    
    extern bool toSwitch;

    extern bool isInMenu;

    extern bool isCreated;
    
    void switchStartPos(PlayLayer* game);

    void updateIndex(bool increment);

    struct Callbacks {
        void leftArrowButtonCallback(CCObject* obj);
        void rightArrowButtonCallback(CCObject* obj);
    };
}