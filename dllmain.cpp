#include <cstdint>
#include <format>
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <gd.h>
#include <Windows.h>


using namespace cocos2d;

std::vector<std::pair<gd::StartPosObject*, CCPoint>> startPoses;
int startPosIndex;
CCLabelBMFont* startPosText;

class ModPlayLayer : gd::PlayLayer {
    public:
        bool _init(gd::GJGameLevel* lvl) {
            startPosText = CCLabelBMFont::create("StartPos 0/0", "bigFont.fnt");
            startPosIndex = -1;
            startPoses = {};

            auto corner = CCDirector::sharedDirector()->getScreenTop();

            startPosText->setPosition(5, corner - 25);
            startPosText->setScale(0.4f);
            startPosText->setAnchorPoint({0, 0.5f});
            startPosText->setOpacity(50);

            startPosText->setVisible(startPoses.empty());
            
            this->addChild(startPosText, 1000);

            return matdash::orig<&ModPlayLayer::_init>(this, lvl);
        }

        void updateIndex(bool increment) {
            if (startPoses.empty() || this->m_isPracticeMode) return;

            if (increment) {
                startPosIndex++;
            }
            else startPosIndex--;

            if (startPosIndex == startPoses.size()) {
                startPosIndex = -1;
            } else if (startPosIndex < -1) {
                startPosIndex = startPoses.size() - 1;
            }

            auto colorPulseBegin = CCTintTo::create(0.0, 0, 255, 0);
            auto colorPulseEnd = CCTintTo::create(0.5, 255, 255, 255);

            auto opacityPulseBegin = CCFadeTo::create(0.0, 255);
            auto opacityPulseEnd = CCFadeTo::create(0.5, 50);

            auto label = std::format("StartPos {}/{}", 
                            std::to_string(startPosIndex + 1), 
                            std::to_string(startPoses.size())
                        );

            startPosText->setString(label.c_str());

            startPosText->runAction(CCSequence::create(colorPulseBegin, colorPulseEnd, nullptr));
            startPosText->runAction(CCSequence::create(opacityPulseBegin, opacityPulseEnd, nullptr));

            this->m_startPosCheckpoint = nullptr;
            
            if (startPosIndex == -1) {
                this->m_startPos = nullptr;
                this->m_playerStartPosition = ccp(0, 105);
            } else {
                this->m_startPos = startPoses[startPosIndex].first;
                this->m_playerStartPosition = startPoses[startPosIndex].second;
            }

            this->resetLevel();

            if (this->m_isPaused) {
                gd::GameSoundManager::sharedState()->stopBackgroundMusic();
            }
        }
        void addObject(gd::GameObject* game) {
            matdash::orig<&ModPlayLayer::addObject>(this, game);
            
            if (game->m_nObjectID == 31) {
                game->retain();

                startPoses.push_back({reinterpret_cast<gd::StartPosObject*>(game), game->getPosition()});
                startPosIndex += 1;

                auto label = std::format("StartPos {}/{}", 
                            std::to_string(startPosIndex + 1), 
                            std::to_string(startPoses.size())
                        );
                startPosText->setString(label.c_str());
            }

        }
        void resetLevel(void) {
            this->m_isTestMode = this->m_startPos != 0;

            matdash::orig<&ModPlayLayer::resetLevel>(this);
        }
};


class ModUILayer : gd::UILayer {
    public:
        void _keyDown(cocos2d::enumKeyCodes key) {
            auto pModPlayLayer = reinterpret_cast<ModPlayLayer*>(gd::PlayLayer::get());

            matdash::orig<&ModUILayer::_keyDown>(this, key);
            
            if (key == KEY_Left || key == KEY_Right) {;
                pModPlayLayer->updateIndex(key == KEY_Right);
            }
        }
};

class ModPauseLayer : gd::PauseLayer {
    public:
        void _keyDown(cocos2d::enumKeyCodes key) {
            auto pModPlayLayer = reinterpret_cast<ModPlayLayer*>(gd::PlayLayer::get());

            matdash::orig<&ModPauseLayer::_keyDown>(this, key);
            
            if (key == KEY_Left || key == KEY_Right) {;
                pModPlayLayer->updateIndex(key == KEY_Right);
            }
        }
};

void mod_main(HMODULE hModule) {
    matdash::add_hook<&ModPlayLayer::_init, matdash::CallConv::Thiscall>(gd::base + 0x1FB780);
    matdash::add_hook<&ModPlayLayer::resetLevel, matdash::CallConv::Thiscall>(gd::base + 0x20BF00);
    matdash::add_hook<&ModPlayLayer::addObject, matdash::CallConv::Thiscall>(gd::base + 0x2017E0);

    matdash::add_hook<&ModUILayer::_keyDown, matdash::CallConv::Thiscall>(gd::base + 0x25f890);
    matdash::add_hook<&ModPauseLayer::_keyDown, matdash::CallConv::Thiscall>(gd::base + 0x1E6580);
}