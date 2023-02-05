#include "switcher.hpp"

#include "mods.hpp"

USING_NS_CC;

using namespace gd;

std::vector<std::pair<StartPosObject*, CCPoint>> switcher::startPoses = {};
std::array<enumKeyCodes, 2> switcher::keyBinds = {KEY_Left, KEY_Right};
std::ptrdiff_t switcher::index = 0;
CCLabelBMFont* switcher::text = nullptr;
CCSprite* switcher::leftArrow = nullptr;
CCSprite* switcher::rightArrow = nullptr;
CCMenuItemSpriteExtra* switcher::leftArrowButton = nullptr;
CCMenuItemSpriteExtra* switcher::rightArrowButton = nullptr;
bool switcher::isInMenu = false;
bool switcher::isCreated = false;

namespace {
    void pulseObjects(void) {
        switcher::leftArrow->runAction(CCSequence::create(CCFadeTo::create(0.0f, 255), CCFadeTo::create(1.0f, 255), CCFadeTo::create(0.5f, 70), nullptr));
        switcher::rightArrow->runAction(CCSequence::create(CCFadeTo::create(0.0f, 255), CCFadeTo::create(1.0f, 255), CCFadeTo::create(0.5f, 70), nullptr));
        switcher::text->runAction(CCSequence::create(CCFadeTo::create(0.0f, 255), CCFadeTo::create(1.0f, 255), CCFadeTo::create(0.5f, 70), nullptr));
    }
}

void switcher::switchStartPos(PlayLayer* game) {
    switcher::toSwitch = false;

    if (switcher::startPoses.empty() || game->m_isPracticeMode) return;

    game->m_startPosCheckpoint = nullptr;

    if (switcher::index == -1) {
        game->m_startPos = nullptr;
        game->m_playerStartPosition = ccp(0, 105);
    } else {
        game->m_startPos = switcher::startPoses[switcher::index].first;
        game->m_playerStartPosition = switcher::startPoses[switcher::index].second;
    }

    game->resetLevel();

    if (game->m_bIsPaused) {
        GameSoundManager::sharedState()->stopBackgroundMusic();
    }
}

void switcher::updateIndex(bool increment) {
    switcher::index += increment;
    switcher::index -= !increment;

    if (switcher::index == switcher::startPoses.size()) {
        switcher::index = -1;
    } else if (switcher::index < -1) {
        switcher::index = switcher::startPoses.size() - 1;
    }

    auto label = std::format("{}/{}", 
                    std::to_string(switcher::index + 1), 
                    std::to_string(switcher::startPoses.size())
    );
    switcher::text->setString(label.c_str());
    pulseObjects();
}

bool switcher::direction;
bool switcher::toSwitch = false;

void switcher::Callbacks::leftArrowButtonCallback(CCObject* obj) {
    auto pPlayLayer = PlayLayer::get();

    switcher::updateIndex(false);

    if (!mods::switchOnDeath) {
        switcher::switchStartPos(pPlayLayer);
    }
    else {
        switcher::direction = false;
        switcher::toSwitch = true;
    }
}

void switcher::Callbacks::rightArrowButtonCallback(CCObject* obj) {
    auto pPlayLayer = PlayLayer::get();

    switcher::updateIndex(true);

    if (!mods::switchOnDeath) {
        switcher::switchStartPos(pPlayLayer);
    }
    else {
        switcher::direction = true;
        switcher::toSwitch = true;
    }
}
