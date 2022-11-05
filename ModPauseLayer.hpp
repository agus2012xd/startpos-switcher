#pragma once

#include <gd.h>

#include "ModPlayLayer.hpp"

#include "switcher.hpp"
#include "MenuMods.hpp"

using namespace gd;
using namespace cocos2d;

class ModPauseLayer final : PauseLayer {
    public:
        void _keyDown(enumKeyCodes key) {
            auto pModPlayLayer = reinterpret_cast<ModPlayLayer*>(PlayLayer::get());

            matdash::orig<&ModPauseLayer::_keyDown>(this, key);
            
            if ((key == switcher::keyBinds[0] || key == switcher::keyBinds[1]) && MenuMods.toogle) {
                if (!MenuMods.switchOnDeath) {
                    pModPlayLayer->updateIndex(key == switcher::keyBinds[1]);
                }
                else {
                    switcher::direction = (key == switcher::keyBinds[1]);
                    switcher::toSwitch = true;
                }
            }
        }
};