#include <fstream>

#include <cocos2d.h>
#include <gdhm.hpp>
#undef isnan
#undef snprintf
#include <nlohmann/json.hpp>

#include "mods.hpp"
#include "switcher.hpp"

#define CHECKBOX(name, value, onActivate) \
    gdhm::gui::checkbox ( \
        gdhm::new_id, \
        name, \
        value, \
        nullptr, \
        onActivate, \
        "", \
        onActivate \
    )

USING_NS_CC;

using nlohmann::json;

void GDHMHeader(void) {
    auto updateJson = [](void) {
        json j = {};
        std::fstream fs(".startpos_switcher/settings.json", std::ios::in | std::ios::out);

        if(fs.is_open()) {
            fs >> j;
        }

        fs.close();
        fs.open(".startpos_switcher/settings.json", std::ios::out | std::ios::trunc);

        if(fs.is_open()) {
            j["Toogle"] = mods::toogle;
            j["Use A/D Keybinds"] = mods::useAD;
            j["Hide Interface"] = mods::hideInterface;
            j["Switch On Death"] = mods::switchOnDeath;
            j["Use Arrows"] = mods::useArrows;
            fs << j;
        }

        fs.close();
    };

    auto setKeybinds = [&updateJson](void) {
        if (mods::useAD) {
            switcher::keyBinds = {KEY_A, KEY_D};
        }
        else switcher::keyBinds = {KEY_Left, KEY_Right};

        updateJson();
    };

    auto onToogle = [&updateJson](void) {
        if(switcher::isCreated){
            if(switcher::isInMenu == false) {
                switcher::text->setVisible((!mods::hideInterface) && mods::toogle && !(switcher::startPoses.empty()));
                switcher::leftArrowButton->setVisible((!mods::hideInterface) && mods::toogle && !(switcher::startPoses.empty()) && mods::useArrows);
                switcher::rightArrowButton->setVisible((!mods::hideInterface) && mods::toogle && !(switcher::startPoses.empty()) && mods::useArrows);
            }
        }

        updateJson();
    };

    setKeybinds();
    onToogle();
    CHECKBOX("Toogle", &mods::toogle, onToogle);
    CHECKBOX("Use A/D Keybinds", &mods::useAD, setKeybinds);
    CHECKBOX("Hide Interface", &mods::hideInterface, onToogle);
    CHECKBOX("Switch On Death", &mods::switchOnDeath, updateJson);
    CHECKBOX("Use Arrows", &mods::useArrows, onToogle);
}

void GDHMFooter(void) {
    gdhm::gui::label("");
    gdhm::gui::label("By GD Ephir");
}
