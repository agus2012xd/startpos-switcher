#pragma once

#include <format>

#include <gd.h>

#include "switcher.hpp"
#include "MenuMods.hpp"

using namespace gd;
using namespace cocos2d;

class ModPlayLayer final : PlayLayer {
    public:
        bool _init(GJGameLevel* lvl) {
            switcher::index = -1;
            switcher::startPoses.clear();

            auto win_size = CCDirector::sharedDirector()->getWinSize();

            switcher::text = CCLabelBMFont::create("0/0", "bigFont.fnt");
            switcher::text->setPosition(win_size.width / 2, 15);
            switcher::text->setScale(0.5f);
            switcher::text->setOpacity(70);
            switcher::text->setZOrder(0x7FFFFFFF);

            auto result = matdash::orig<&ModPlayLayer::_init>(this, lvl);

            switcher::text->setVisible(!switcher::startPoses.empty() && MenuMods.toogle && !(MenuMods.hideInterface));

            addChild(switcher::text);

            return result;
        }

        void destroyPlayer(void) {
            if (switcher::toSwitch) {
                updateIndex(switcher::direction);
                switcher::toSwitch = false;
            }

            matdash::orig<&ModPlayLayer::destroyPlayer>(this);
        }

        void updateIndex(bool increment) noexcept {
            if (switcher::startPoses.empty() || m_isPracticeMode) return;

            switcher::index += increment;
            switcher::index -= !increment;

            if (switcher::index == switcher::startPoses.size()) {
                switcher::index = -1;
            } else if (switcher::index < -1) {
                switcher::index = switcher::startPoses.size() - 1;
            }

            auto opacityPulseBegin = CCFadeTo::create(0.0f, 255);
            auto opacityPulseHold = CCFadeTo::create(1.0f, 255);
            auto opacityPulseEnd = CCFadeTo::create(0.5f, 70);

            auto opacityPulseActions = CCArray::create(opacityPulseBegin, opacityPulseHold, opacityPulseEnd, nullptr);

            auto label = std::format("{}/{}", 
                            std::to_string(switcher::index + 1), 
                            std::to_string(switcher::startPoses.size())
                        );

            switcher::text->setString(label.c_str());
            switcher::text->runAction(CCSequence::create(opacityPulseActions));

            m_startPosCheckpoint = nullptr;

            if (switcher::index == -1) {
                m_startPos = nullptr;
                m_playerStartPosition = ccp(0, 105);
            } else {
                m_startPos = switcher::startPoses[switcher::index].first;
                m_playerStartPosition = switcher::startPoses[switcher::index].second;
            }

            resetLevel();

            if (m_isPaused) {
                GameSoundManager::sharedState()->stopBackgroundMusic();
            }
        }
        void addObject(GameObject* game) {
            matdash::orig<&ModPlayLayer::addObject>(this, game);
            
            if (game->m_nObjectID == 31) {
                game->retain();

                switcher::startPoses.push_back({reinterpret_cast<StartPosObject*>(game), game->getPosition()});
                switcher::index++;

                auto label = std::format("{}/{}", 
                            std::to_string(switcher::index + 1), 
                            std::to_string(switcher::startPoses.size())
                        );
                switcher::text->setString(label.c_str());

                game->release();
            }

        }
        void resetLevel(void) {
            m_isTestMode = m_startPos != 0;

            matdash::orig<&ModPlayLayer::resetLevel>(this);
        }
};