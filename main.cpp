#include <cstddef>
#include <cstdint>
#include <format>
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <gd.h>
#include <Windows.h>
#include <gdhm.hpp>

namespace {
    using namespace cocos2d;
    using namespace gd;

    std::vector<std::pair<StartPosObject*, CCPoint>> startPoses;
    std::ptrdiff_t startPosIndex;
    CCLabelBMFont* startPosText;

    std::pair<cocos2d::enumKeyCodes, cocos2d::enumKeyCodes> switchKeyBinds = {KEY_Left, KEY_Right};

    bool switchDirection;
    bool toSwitch = false;
    bool switchOnDeath = false;
    bool isToogled = false;
    bool isHideInterface = false;

    class ModPlayLayer : PlayLayer {
        public:
            bool _init(GJGameLevel* lvl) {
                startPosIndex = -1;
                startPoses.clear();

                auto win_size = CCDirector::sharedDirector()->getWinSize();

                startPosText = CCLabelBMFont::create("0/0", "bigFont.fnt");
                startPosText->setPosition(win_size.width / 2, 15);
                startPosText->setScale(0.5f);
                startPosText->setOpacity(70);
                startPosText->setZOrder(0x7FFFFFFF);

                auto result = matdash::orig<&ModPlayLayer::_init>(this, lvl);

                startPosText->setVisible(!startPoses.empty() && isToogled && !(isHideInterface));

                addChild(startPosText);

                return result;
            }

            void destroyPlayer(void) {
                if (toSwitch) {
                    updateIndex(switchDirection);
                    toSwitch = false;
                }

                matdash::orig<&ModPlayLayer::destroyPlayer>(this);
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

                auto opacityPulseBegin = CCFadeTo::create(0.0f, 255);
                auto opacityPulseHold = CCFadeTo::create(1.0f, 255);
                auto opacityPulseEnd = CCFadeTo::create(0.5f, 70);

                auto opacityPulseActions = CCArray::create(opacityPulseBegin, opacityPulseHold, opacityPulseEnd, nullptr);

                auto label = std::format("{}/{}", 
                                std::to_string(startPosIndex + 1), 
                                std::to_string(startPoses.size())
                            );

                startPosText->setString(label.c_str());
                startPosText->runAction(CCSequence::create(opacityPulseActions));

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

                    auto label = std::format("{}/{}", 
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
                
                if ((key == switchKeyBinds.first || key == switchKeyBinds.second) && isToogled) {
                    if (!switchOnDeath) {
                        pModPlayLayer->updateIndex(key == switchKeyBinds.second);
                    }
                    else {
                        switchDirection = (key == switchKeyBinds.second);
                        toSwitch = true;
                    }
                }
            }
    };

    class ModPauseLayer : PauseLayer {
        public:
            void _keyDown(cocos2d::enumKeyCodes key) {
                auto pModPlayLayer = reinterpret_cast<ModPlayLayer*>(PlayLayer::get());

                matdash::orig<&ModPauseLayer::_keyDown>(this, key);
                
                if ((key == switchKeyBinds.first || key == switchKeyBinds.second) && isToogled) {
                    if (!switchOnDeath) {
                        pModPlayLayer->updateIndex(key == switchKeyBinds.second);
                    }
                    else {
                        switchDirection = (key == switchKeyBinds.second);
                        toSwitch = true;
                    }
                }
            }
    };

    void _header(void) {
        static bool isUseAD = false;

        auto setKeybinds = [](void) {
            if (isUseAD) {
                switchKeyBinds = {KEY_A, KEY_D};
            }
            else switchKeyBinds = {KEY_Left, KEY_Right};
        };

        auto onToogle = [&setKeybinds](void) {
            if(startPosText) {
                startPosText->setVisible(isToogled);
            }
        };

        auto onHideInterface = [](void) {
            if (startPosText) {
                startPosText->setVisible(!isHideInterface);
            }
        };

        gdhm::gui::checkbox (
            gdhm::new_id,
            "Toogle",
            &isToogled,
            nullptr,
            onToogle,
            "",
            onToogle
        );

        gdhm::gui::checkbox (
            gdhm::new_id,
            "Use A/D Keybinds",
            &isUseAD,
            nullptr,
            setKeybinds,
            "",
            setKeybinds
        );

        gdhm::gui::checkbox (
            gdhm::new_id,
            "Hide Interface",
            &isHideInterface,
            nullptr,
            onHideInterface,
            "",
            onHideInterface
        );

        gdhm::gui::checkbox (
            gdhm::new_id,
            "Switch On Death",
            &switchOnDeath
            );
    }

    void _footer(void) {
        gdhm::gui::label("");
        gdhm::gui::label("By GD Ephir");
    }
}

void mod_main(HMODULE hModule) {
    matdash::add_hook<&ModPlayLayer::_init, matdash::CallConv::Thiscall>(gd::base + 0x1FB780);
    matdash::add_hook<&ModPlayLayer::resetLevel, matdash::CallConv::Thiscall>(gd::base + 0x20BF00);
    matdash::add_hook<&ModPlayLayer::addObject, matdash::CallConv::Thiscall>(gd::base + 0x2017E0);
    matdash::add_hook<&ModPlayLayer::destroyPlayer, matdash::CallConv::Thiscall>(gd::base + 0x20A1A0);

    matdash::add_hook<&ModUILayer::_keyDown, matdash::CallConv::Thiscall>(gd::base + 0x25f890);
    matdash::add_hook<&ModPauseLayer::_keyDown, matdash::CallConv::Thiscall>(gd::base + 0x1E6580);

    matdash::enable_hooks();

    if (gdhm::is_loaded()) {
        gdhm::gui::window (
            "StartPos Switcher", 
            "", 
            nullptr, 
            [](void){}, 
            _header,
            _footer
        );
    }

}
