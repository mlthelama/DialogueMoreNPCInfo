#pragma once

#include "CLIK/MovieClip.h"
#include "CLIK/TextField.h"
#include "CLIK/lazydatasetter.h"

namespace scaleform {
    class dialogue_info_menu final : public RE::IMenu {
    public:
        static constexpr std::string_view menu_name = "DialogueInfoMenu";
        static constexpr std::string_view file_name = menu_name;

        static void Register();

        static void open();

        static void close();

        static bool is_menu_open();

        void refresh_items();

        dialogue_info_menu(const dialogue_info_menu&) = delete;
        dialogue_info_menu(dialogue_info_menu&&) = delete;

        dialogue_info_menu& operator=(const dialogue_info_menu&) = delete;
        dialogue_info_menu& operator=(dialogue_info_menu&&) = delete;

    protected:
        dialogue_info_menu();

        ~dialogue_info_menu() override = default;

        static stl::owner<IMenu*> creator();

        void PostCreate() override;

        RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override;

        void AdvanceMovie(float a_interval, uint32_t a_current_time) override;

        void Accept(CallbackProcessor* a_processor) override;

        void RefreshPlatform() override;

    private:
        class Logger : public RE::GFxLog {
        public:
            void LogMessageVarg(LogMessageType, const char* a_fmt, std::va_list a_arg_list) override;
        };

        void init_extensions() const;

        void on_open();

        static void update_text(CLIK::TextField a_field, std::string_view a_string);

        [[nodiscard]] RE::GFxValue build_gfx_value_icon(const std::string& a_key) const;

        [[nodiscard]] RE::GFxValue build_gfx_value_pair(const std::string_view& a_key,
            const std::string_view& a_value) const;

        [[nodiscard]] RE::GFxValue build_gfx_value_pair_with_max(const std::string_view& a_key,
            const std::string_view& a_value,
            const std::string_view& a_max);

        void update_elements();

        static void on_close();

        static void log(const RE::FxDelegateArgs& a_params);

        void set_relationship_data(RE::Actor* a_actor);

        void set_mood_data(RE::TESNPC* a_actor_base);

        void adjust_position();

        void scale_window();

        RE::GPtr<RE::GFxMovieView> view_;
        bool is_active_ = false;

        CLIK::MovieClip root_obj_;

        CLIK::TextField race_;

        CLIK::lazy_data_setter picture_back_;

        CLIK::lazy_data_setter key_value_name_;
        CLIK::lazy_data_setter key_value_gender_;
        CLIK::lazy_data_setter key_value_class_;
        CLIK::lazy_data_setter key_value_level_;
        CLIK::lazy_data_setter key_value_trainer_;
        CLIK::lazy_data_setter key_value_vendor_;
        CLIK::lazy_data_setter key_value_best_skill_first_;
        CLIK::lazy_data_setter key_value_best_skill_second_;

        CLIK::lazy_data_setter key_value_morality_;
        CLIK::lazy_data_setter key_value_assistance_;
        CLIK::lazy_data_setter key_value_confidence_;
        CLIK::lazy_data_setter key_value_aggression_;
        CLIK::lazy_data_setter key_value_mood_;
        CLIK::lazy_data_setter key_value_relation_;
        CLIK::lazy_data_setter key_value_faction_;
    };
}  // scaleform
