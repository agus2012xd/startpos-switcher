#pragma once

#include <cocos2d.h>
#include <gd.h>

#include <matdash.hpp>

#include <Windows.h>

#include "switcher.hpp"
#include "mods.hpp"

using namespace gd;
using namespace cocos2d;

class ModPlayLayer final : PlayLayer {
    public:
        bool _init(GJGameLevel* lvl) {
            switcher::index = -1;
            switcher::startPoses.clear();

            switcher::isInMenu = false;
            switcher::isCreated = true;

            auto win_size = CCDirector::sharedDirector()->getWinSize();

            switcher::text = CCLabelBMFont::create("0/0", "bigFont.fnt");
            switcher::text->setPosition(win_size.width / 2, 15);
            switcher::text->setScale(0.5f);
            switcher::text->setOpacity(70);
            switcher::text->setZOrder(0x7FFFFFFF);

            switcher::leftArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            switcher::leftArrow->setScale(0.5f);
            switcher::leftArrow->setOpacity(70);

            switcher::rightArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            switcher::rightArrow->setScale(0.5f);
            switcher::rightArrow->setOpacity(70);
            switcher::rightArrow->setFlipX(true);

            auto result = matdash::orig<&ModPlayLayer::_init>(this, lvl);

            switcher::leftArrowButton = CCMenuItemSpriteExtra::create(switcher::leftArrow, this, menu_selector(switcher::Callbacks::leftArrowButtonCallback));
            switcher::leftArrowButton->setPosition(ccp(-30, 0));

            switcher::rightArrowButton = CCMenuItemSpriteExtra::create(switcher::rightArrow, this, menu_selector(switcher::Callbacks::rightArrowButtonCallback));
            switcher::rightArrowButton->setPosition(ccp(30, 0));

            auto menu = CCMenu::create();

            menu->addChild(switcher::leftArrowButton);
            menu->addChild(switcher::rightArrowButton);
            menu->setPosition(ccp(((win_size.width / 2)), 15));
            menu->setZOrder(0x7FFFFFFF);

            switcher::text->setVisible(!switcher::startPoses.empty() && mods::toogle && !(mods::hideInterface));

            switcher::leftArrowButton->setVisible((!switcher::startPoses.empty() && mods::toogle && !(mods::hideInterface) && mods::useArrows));
            switcher::rightArrowButton->setVisible((!switcher::startPoses.empty() && mods::toogle && !(mods::hideInterface) && mods::useArrows));

            addChild(switcher::text);
            addChild(menu);

            return result;
        }

        void addObject(GameObject* game) {
            matdash::orig<&ModPlayLayer::addObject>(this, game);
            
            if (game->m_nObjectID == 31) {
                game->retain();

                switcher::startPoses.push_back({reinterpret_cast<StartPosObject*>(game), game->getPosition()});
                switcher::index++;

                if(switcher::text) {
                    auto label = std::format("{}/{}", 
                            std::to_string(switcher::index + 1), 
                            std::to_string(switcher::startPoses.size())
                        );
                    switcher::text->setString(label.c_str());
                }
                
                game->release();
            }
        }

        void resetLevel(void) {
            m_isTestMode = m_startPos != 0;

            matdash::orig<&ModPlayLayer::resetLevel>(this);

            if (switcher::toSwitch) {
                switcher::switchStartPos(this);
            }
        }


        void onQuit(void) {
            matdash::orig<&ModPlayLayer::onQuit>(this);
            switcher::isInMenu = true;
        }
};