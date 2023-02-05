#pragma once

#include <cstddef>

#include <array>

#include <cocos2d.h>
#include <gd.h>

namespace switcher {
    extern std::vector<std::pair<gd::StartPosObject*, cocos2d::CCPoint>> startPoses;
    extern std::array<cocos2d::enumKeyCodes, 2> keyBinds;
    extern std::ptrdiff_t index;
    extern cocos2d::CCLabelBMFont* text;
    extern cocos2d::CCSprite* leftArrow;
    extern cocos2d::CCSprite* rightArrow;
    extern gd::CCMenuItemSpriteExtra* leftArrowButton;
    extern gd::CCMenuItemSpriteExtra* rightArrowButton;
    extern bool direction;
    extern bool toSwitch;
    extern bool isInMenu;
    extern bool isCreated;
    void switchStartPos(gd::PlayLayer* game);
    void updateIndex(bool increment);

    struct Callbacks {
        void leftArrowButtonCallback(cocos2d::CCObject* obj);
        void rightArrowButtonCallback(cocos2d::CCObject* obj);
    };
}