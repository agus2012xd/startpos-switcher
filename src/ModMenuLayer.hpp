#pragma once

#include <Windows.h>

#include <cocos2d.h>
#include <gd.h>

#include <matdash.hpp>

using namespace gd;
using namespace cocos2d;

class ModMenuLayer {
    public:
        void onMoreGames(CCObject* obj) {
            matdash::orig<&ModMenuLayer::onMoreGames>(this, obj);

            static auto counter = 0;

            if (counter == 4) {
                counter = 0;

                MessageBoxA(NULL, "/ / lesaparamonov", "Easter egg", MB_OK);
            }

            counter++;
        }
};