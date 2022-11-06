#include <gdhm.hpp>

#include "mods.hpp"
#include "switcher.hpp"

#include "gdhm_main.hpp"

void _header(void) {
    auto setKeybinds = [](void) {
        if (mods::useAD) {
            switcher::keyBinds = {KEY_A, KEY_D};
        }
        else switcher::keyBinds = {KEY_Left, KEY_Right};
    };

    auto onToogle = [&setKeybinds](void) {
        if(switcher::text) {
            switcher::text->setVisible((!mods::hideInterface) && mods::toogle && (!switcher::startPoses.empty()));
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
}

void _footer(void) {
    gdhm::gui::label("");
    gdhm::gui::label("By GD Ephir");
}