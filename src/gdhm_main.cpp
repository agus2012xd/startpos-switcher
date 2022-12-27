#include "gdhm_main.hpp"

using namespace cocos2d;
using nlohmann::json;

void _header(void) {
    auto updateJson = [](void) {
        json j = {};

        std::fstream fs(".startpos_switcher/settings.json", std::ios::in | std::ios::out);

        fs >> j;

        fs.close();

        fs.open(".startpos_switcher/settings.json", std::ios::out | std::ios::trunc);

        j["Toogle"] = mods::toogle;
        j["Use A/D Keybinds"] = mods::useAD;
        j["Hide Interface"] = mods::hideInterface;
        j["Switch On Death"] = mods::switchOnDeath;
        j["Use Arrows"] = mods::useArrows;

        fs << j;

        fs.close();
    };

    auto setKeybinds = [&](void) {
        if (mods::useAD) {
            switcher::keyBinds = {KEY_A, KEY_D};
        }
        else switcher::keyBinds = {KEY_Left, KEY_Right};

        updateJson();
    };

    auto onToogle = [&](void) {
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
        &mods::switchOnDeath,
        nullptr,
        updateJson,
        "",
        updateJson
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