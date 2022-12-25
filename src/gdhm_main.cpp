#include <gdhm.hpp>

#include "mods.hpp"
#include "switcher.hpp"

#include "gdhm_main.hpp"

#include <cocos2d.h>

using namespace cocos2d;

void _header(void) {
    auto setKeybinds = [](void) {
        if (mods::useAD) {
            switcher::keyBinds = {KEY_A, KEY_D};
        }
        else switcher::keyBinds = {KEY_Left, KEY_Right};
    };

    auto onToogle = [](void) {
        if(switcher::isCreated){
            if(switcher::isInMenu == false) {
                switcher::text->setVisible((!mods::hideInterface) && mods::toogle && !(switcher::startPoses.empty()));
                switcher::leftArrowButton->setVisible((!mods::hideInterface) && mods::toogle && !(switcher::startPoses.empty()) && mods::useArrows);
                switcher::rightArrowButton->setVisible((!mods::hideInterface) && mods::toogle && !(switcher::startPoses.empty()) && mods::useArrows);
            }
        }
    };

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Toogle",
        &mods::toogle,
        nullptr,
        onToogle,
        "",
        onToogle
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Use A/D Keybinds",
        &mods::useAD,
        nullptr,
        setKeybinds,
        "",
        setKeybinds
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Hide Interface",
        &mods::hideInterface,
        nullptr,
        onToogle,
        "",
        onToogle
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Switch On Death",
        &mods::switchOnDeath
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Use Arrows",
        &mods::useArrows,
        nullptr,
        onToogle,
        "",
        onToogle
    );
}

void _footer(void) {
    gdhm::gui::label("");
    gdhm::gui::label("By GD Ephir");
}