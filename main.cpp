#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <gdhm.hpp>

#include "ModLayers.hpp"

void _header(void) {
    auto setKeybinds = [](void) {
        if (MenuMods.useAD) {
            switcher::keyBinds = {KEY_A, KEY_D};
        }
        else switcher::keyBinds = {KEY_Left, KEY_Right};
    };

    auto onToogle = [&setKeybinds](void) {
        if(switcher::text) {
            switcher::text->setVisible((!MenuMods.hideInterface) && MenuMods.toogle);
        }
    };

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Toogle",
        &MenuMods.toogle,
        nullptr,
        onToogle,
        "",
        onToogle
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Use A/D Keybinds",
        &MenuMods.useAD,
        nullptr,
        setKeybinds,
        "",
        setKeybinds
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Hide Interface",
        &MenuMods.hideInterface,
        nullptr,
        onToogle,
        "",
        onToogle
    );

    gdhm::gui::checkbox (
        gdhm::new_id,
        "Switch On Death",
        &MenuMods.switchOnDeath
        );
}

void _footer(void) {
    gdhm::gui::label("");
    gdhm::gui::label("By GD Ephir");
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
