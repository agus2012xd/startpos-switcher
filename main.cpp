#include <cstddef>
#include <format>
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <gd.h>
#include <Windows.h>

namespace {
    using namespace cocos2d;
    using namespace gd;

    std::vector<std::pair<StartPosObject*, CCPoint>> startPoses;
    std::ptrdiff_t startPosIndex;
    CCLabelBMFont* startPosText;

    class ModPlayLayer : PlayLayer {
        public:
            bool _init(GJGameLevel* lvl) {
                startPosIndex = -1;
                startPoses.clear();

                auto corner = CCDirector::sharedDirector()->getScreenTop();

                startPosText = CCLabelBMFont::create("StartPos 0/0", "bigFont.fnt");

                auto result = matdash::orig<&ModPlayLayer::_init>(this, lvl);

                startPosText->setPosition(5, corner - 25);
                startPosText->setScale(0.4f);
                startPosText->setAnchorPoint({0, 0.5f});
                startPosText->setOpacity(50);
                startPosText->setZOrder(0x2021AF);

                startPosText->setVisible(!startPoses.empty());

                addChild(startPosText);

                return result;
            }

            void updateIndex(bool increment) noexcept {
                if (startPoses.empty() || m_isPracticeMode) return;

                startPosIndex += increment;
                startPosIndex -= !increment;

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

                m_startPosCheckpoint = nullptr;

                if (startPosIndex == -1) {
                    m_startPos = nullptr;
                    m_playerStartPosition = ccp(0, 105);
                } else {
                    m_startPos = startPoses[startPosIndex].first;
                    m_playerStartPosition = startPoses[startPosIndex].second;
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

                    startPoses.push_back({reinterpret_cast<StartPosObject*>(game), game->getPosition()});
                    startPosIndex++;

                    auto label = std::format("StartPos {}/{}", 
                                std::to_string(startPosIndex + 1), 
                                std::to_string(startPoses.size())
                            );
                    startPosText->setString(label.c_str());

                    game->release();
                }

            }
            void resetLevel(void) {
                m_isTestMode = m_startPos != 0;

                matdash::orig<&ModPlayLayer::resetLevel>(this);
            }
    };


    class ModUILayer : UILayer {
        public:
            void _keyDown(cocos2d::enumKeyCodes key) {
                auto pModPlayLayer = reinterpret_cast<ModPlayLayer*>(gd::PlayLayer::get());

                matdash::orig<&ModUILayer::_keyDown>(this, key);
                
                if (key == KEY_Left || key == KEY_Right) {;
                    pModPlayLayer->updateIndex(key == KEY_Right);
                }
            }
    };

    class ModPauseLayer : PauseLayer {
        public:
            void _keyDown(cocos2d::enumKeyCodes key) {
                auto pModPlayLayer = reinterpret_cast<ModPlayLayer*>(PlayLayer::get());

                matdash::orig<&ModPauseLayer::_keyDown>(this, key);
                
                if (key == KEY_Left || key == KEY_Right) {;
                    pModPlayLayer->updateIndex(key == KEY_Right);
                }
            }
    };
}

void mod_main(HMODULE hModule) {
    matdash::add_hook<&ModPlayLayer::_init, matdash::CallConv::Thiscall>(gd::base + 0x1FB780);
    matdash::add_hook<&ModPlayLayer::resetLevel, matdash::CallConv::Thiscall>(gd::base + 0x20BF00);
    matdash::add_hook<&ModPlayLayer::addObject, matdash::CallConv::Thiscall>(gd::base + 0x2017E0);

    matdash::add_hook<&ModUILayer::_keyDown, matdash::CallConv::Thiscall>(gd::base + 0x25f890);
    matdash::add_hook<&ModPauseLayer::_keyDown, matdash::CallConv::Thiscall>(gd::base + 0x1E6580);

    matdash::enable_hooks();
}
