#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>

#include <gdhm.hpp>

#include "ModLayers.hpp"

#include "gdhm_main.hpp"

void mod_main(HMODULE hModule) {
    matdash::add_hook<&ModPlayLayer::_init, matdash::CallConv::Thiscall>(gd::base + 0x1FB780);
    matdash::add_hook<&ModPlayLayer::resetLevel, matdash::CallConv::Thiscall>(gd::base + 0x20BF00);
    matdash::add_hook<&ModPlayLayer::addObject, matdash::CallConv::Thiscall>(gd::base + 0x2017E0);
    matdash::add_hook<&ModPlayLayer::onQuit, matdash::CallConv::Thiscall>(gd::base + 0x20D810);

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
