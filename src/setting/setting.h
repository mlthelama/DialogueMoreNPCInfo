#pragma once

#include <SimpleIni.h>

class setting {
    inline static const char* ini_path_ = R"(.\Data\SKSE\Plugins\DialogueMoreNPCInfo.ini)";

    inline static int log_level_;
    inline static int avatar_set_;
    inline static bool hide_faction_;
    inline static int toggle_key_;

    inline static int pos_x_;
    inline static int pos_y_;
    inline static int scale_x_;
    inline static int scale_y_;

    inline static bool hand_to_hand_;

    inline static bool show_window_;

public:
    static void load_settings() {
        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path_);

        log_level_ = ini.GetLongValue("General", "iLogLevel", 2);
        avatar_set_ = ini.GetLongValue("General", "iAvatarSet", 0);
        hide_faction_ = ini.GetBoolValue("General", "bHideFaction", false);
        toggle_key_ = ini.GetLongValue("General", "iToggleButton", 34); //G

        pos_x_ = ini.GetLongValue("Window", "iPosX", 405);
        pos_y_ = ini.GetLongValue("Window", "iPosY", 320);
        scale_x_ = ini.GetLongValue("Window", "iScaleX", 100);
        scale_y_ = ini.GetLongValue("Window", "iScaleY", 100);

        hand_to_hand_ = ini.GetBoolValue("Mods", "bHandToHand", false);

        show_window_ = ini.GetBoolValue("Toggle", "bShowWindow", true);
    }

    static void set_show_window(const bool a_show_value) {
        logger::debug("setting new value for {} is {}"sv, "bShowWindow", a_show_value);
        show_window_ = a_show_value;

        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path_);

        ini.SetBoolValue("Toggle", "bShowWindow", a_show_value);

        (void)ini.SaveFile(ini_path_);
    }

    static int get_log_level() {
        return log_level_;
    }

    static int get_avatar_set() {
        return avatar_set_;
    }

    static bool get_hide_faction() {
        return hide_faction_;
    }

    static int get_toggle_key() {
        return toggle_key_;
    }

    static int get_pos_x() {
        return pos_x_;
    }

    static int get_pos_y() {
        return pos_y_;
    }

    static int get_scale_x() {
        return scale_x_;
    }

    static int get_scale_y() {
        return scale_y_;
    }
    
    static bool get_hand_to_hand() {
        return hand_to_hand_;
    }

    static bool get_show_window() {
        return show_window_;
    }
};
