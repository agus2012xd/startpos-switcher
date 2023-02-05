#pragma once

#include <gd.h>

#include "ModPlayLayer.hpp"
#include "mods.hpp"
#include "switcher.hpp"

USING_NS_CC;

using namespace gd;

class ModPauseLayer final {
    public:
        void _keyDown(enumKeyCodes key) {
            auto pPlayLayer = PlayLayer::get();

            matdash::orig<&ModPauseLayer::_keyDown>(this, key);
            
            if ((key == switcher::keyBinds[0] || key == switcher::keyBinds[1]) && mods::toogle) {
                switcher::updateIndex(key == switcher::keyBinds[1]);
                
                if (!mods::switchOnDeath) {
                   switcher::switchStartPos(pPlayLayer);
                }
                else {
                    switcher::direction = (key == switcher::keyBinds[1]);
                    switcher::toSwitch = true;
                }
            }
        }
};
