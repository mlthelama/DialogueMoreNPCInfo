#include "dialogue_info_menu.h"
#include "data/actordata.h"
#include "handle/dialogueactorhandle.h"
#include "util/menukeys.h"
#include "util/type_util.h"

namespace scaleform {

    void dialogue_info_menu::Register() {
        RE::UI::GetSingleton()->Register(menu_name, creator);
        logger::info("registered {}"sv, menu_name);
    }

    void dialogue_info_menu::open() {
        if (!is_menu_open()) {
            logger::debug("open menu {}"sv, menu_name);
            RE::UIMessageQueue::GetSingleton()->AddMessage(menu_name, RE::UI_MESSAGE_TYPE::kShow, nullptr);
        }
    }

    void dialogue_info_menu::close() {
        if (is_menu_open()) {
            logger::debug("close menu {}"sv, menu_name);
            RE::UIMessageQueue::GetSingleton()->AddMessage(menu_name, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }
    }

    bool dialogue_info_menu::is_menu_open() { return RE::UI::GetSingleton()->IsMenuOpen(menu_name); }

    //TODO
    void dialogue_info_menu::refresh_items() {
        if (is_menu_open()) {
            if (const auto actor = handle::dialogue_actor_handle::get_singleton()->get_actor()) {
                set_relationship_data(actor);
                const auto actor_base = actor->GetActorBase();
                set_mood_data(actor_base);
            }
        }
    }

    dialogue_info_menu::dialogue_info_menu() {
        using context = RE::UserEvents::INPUT_CONTEXT_ID;
        using flag = RE::UI_MENU_FLAGS;

        const auto menu = static_cast<IMenu*>(this);
        const auto scaleform_manager = RE::BSScaleformManager::GetSingleton();
        [[maybe_unused]] const auto success = scaleform_manager->LoadMovieEx(menu,
            file_name,
            RE::BSScaleformManager::ScaleModeType::kExactFit,
            [&](RE::GFxMovieDef* a_def) -> void {
                fxDelegate.reset(new RE::FxDelegate);
                fxDelegate->RegisterHandler(this);
                a_def->SetState(RE::GFxState::StateType::kExternalInterface, fxDelegate.get());
                fxDelegate->Release();

                logger::trace("FPS: {}, Width: {}, Height: {}"sv,
                    a_def->GetFrameRate(),
                    a_def->GetWidth(),
                    a_def->GetHeight());
                a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
            });

        logger::debug("loading menu {} was successful {}"sv, file_name, success);
        view_ = menu->uiMovie;

        view_->SetMouseCursorCount(0);

        //menu->menuFlags |= flag::kUsesCursor;
        menu->menuFlags |= flag::kAssignCursorToRenderer;

        //menu->depthPriority = 0;
        //menu->inputContext = Context::kNone;

        init_extensions();

        is_active_ = true;
        view_->SetVisible(true);

        logger::debug("Set everything for Menu {}"sv, menu_name);
    }

    RE::IMenu* dialogue_info_menu::creator() { return new dialogue_info_menu(); }

    void dialogue_info_menu::PostCreate() { on_open(); }

    //might not work that well if DialogMenu is open
    RE::UI_MESSAGE_RESULTS dialogue_info_menu::ProcessMessage(RE::UIMessage& a_message) {
        if (a_message.menu == menu_name) {
            if (*a_message.type == RE::UI_MESSAGE_TYPE::kUpdateController) {
                RefreshPlatform();
                return RE::UI_MESSAGE_RESULTS::kPassOn;
            }
            return RE::UI_MESSAGE_RESULTS::kHandled;
        }
        return RE::UI_MESSAGE_RESULTS::kPassOn;
    }

    void dialogue_info_menu::AdvanceMovie(const float a_interval, const uint32_t a_current_time) {
        IMenu::AdvanceMovie(a_interval, a_current_time);
    }

    void dialogue_info_menu::Accept(CallbackProcessor* a_processor) { a_processor->Process("Log", log); }

    void dialogue_info_menu::RefreshPlatform() { IMenu::RefreshPlatform(); }

    void dialogue_info_menu::Logger::LogMessageVarg(RE::GFxLogConstants::LogMessageType,
        const char* a_fmt,
        const std::va_list a_arg_list) {
        std::string fmt(a_fmt ? a_fmt : "");
        while (!fmt.empty() && fmt.back() == '\n') {
            fmt.pop_back();
        }

        std::va_list args;
        va_copy(args, a_arg_list);
        std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(nullptr, 0, fmt.c_str(), a_arg_list) + 1));
        std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
        va_end(args);

        logger::info("{}: {}"sv, menu_name, buf.data());
    }

    void dialogue_info_menu::init_extensions() const {
        const RE::GFxValue boolean(true);
        view_->SetVariable("_global.gfxExtensions", boolean);
        view_->SetVariable("_global.noInvisibleAdvance", boolean);
    }

    void dialogue_info_menu::on_open() {
        using element_t = std::pair<std::reference_wrapper<CLIK::Object>, std::string_view>;

        for (std::array objects{ element_t{ std::ref(root_obj_), "_root.rootObj"sv },
                 element_t{ std::ref(race_), "_root.rootObj.race"sv },
                 element_t{ std::ref(picture_back_), "_root.rootObj.picture"sv },
                 element_t{ std::ref(key_value_name_), "_root.rootObj.keyValueName"sv },
                 element_t{ std::ref(key_value_gender_), "_root.rootObj.keyValueGender"sv },
                 element_t{ std::ref(key_value_class_), "_root.rootObj.keyValueClass"sv },
                 element_t{ std::ref(key_value_level_), "_root.rootObj.keyValueLevel"sv },
                 element_t{ std::ref(key_value_faction_), "_root.rootObj.keyValueFaction"sv },
                 element_t{ std::ref(key_value_trainer_), "_root.rootObj.keyValueTrainer"sv },
                 element_t{ std::ref(key_value_vendor_), "_root.rootObj.keyValueVendor"sv },
                 element_t{ std::ref(key_value_morality_), "_root.rootObj.keyValueMorality"sv },
                 element_t{ std::ref(key_value_assistance_), "_root.rootObj.keyValueAssistance"sv },
                 element_t{ std::ref(key_value_confidence_), "_root.rootObj.keyValueConfidence"sv },
                 element_t{ std::ref(key_value_aggression_), "_root.rootObj.keyValueAggression"sv },
                 element_t{ std::ref(key_value_mood_), "_root.rootObj.keyValueMood"sv },
                 element_t{ std::ref(key_value_relation_), "_root.rootObj.keyValueRelation"sv },
                 element_t{ std::ref(key_value_best_skill_first_), "_root.rootObj.keyValueBestSkillFirst"sv },
                 element_t{ std::ref(key_value_best_skill_second_), "_root.rootObj.keyValueBestSkillSecond"sv } };
             const auto& [object, path] : objects) {
            auto& instance = object.get().GetInstance();
            view_->GetVariable(std::addressof(instance), path.data());
        }
        logger::debug("loaded all SWF objects successfully for {}"sv, menu_name);

        root_obj_.Visible(false);
        adjust_position();
        scale_window();

        update_elements();

        view_->SetVisible(true);
        root_obj_.Visible(true);

        logger::debug("processed all objects for menu {}"sv, menu_name);
    }

    void dialogue_info_menu::update_text(CLIK::TextField a_field, const std::string_view a_string) {
        a_field.AutoSize(CLIK::Object{ "center" });
        a_field.HTMLText(a_string);
        a_field.Visible(true);
    }

    RE::GFxValue dialogue_info_menu::build_gfx_value_icon(const std::string& a_key) const {
        RE::GFxValue value;
        view_->CreateObject(std::addressof(value));
        value.SetMember("iconKey", { static_cast<std::string_view>(a_key) });
        return value;
    }

    RE::GFxValue dialogue_info_menu::build_gfx_value_pair(const std::string_view& a_key,
        const std::string_view& a_value) const {
        RE::GFxValue value;
        view_->CreateObject(std::addressof(value));
        value.SetMember("keyText", { a_key });
        value.SetMember("valueText", { a_value });
        return value;
    }

    RE::GFxValue dialogue_info_menu::build_gfx_value_pair_with_max(const std::string_view& a_key,
        const std::string_view& a_value,
        const std::string_view& a_max) {
        RE::GFxValue value;
        view_->CreateObject(std::addressof(value));
        value.SetMember("keyText", { a_key });
        value.SetMember("valueText", { a_value });
        value.SetMember("valueMaxText", { a_max });
        return value;
    }

    void dialogue_info_menu::update_elements() {
        logger::trace("going to fill the fields with data"sv);

        if (auto actor = handle::dialogue_actor_handle::get_singleton()->get_actor()) {
            auto actor_base = actor->GetActorBase();
            logger::trace("got Actor {}, formid {}, baseformid {}"sv,
                actor->GetName(),
                util::type_util::int_to_hex(actor->formID),
                util::type_util::int_to_hex(actor_base->formID));


            logger::trace("Race formid is {}, name is {}, Key is {}",
                util::type_util::int_to_hex(actor->GetRace()->GetFormID()),
                actor->GetRace()->GetName(),
                actor_data::get_race_translation_name(actor->GetRace()->formID));

            update_text(race_, actor_data::get_race_translation_name(actor->GetRace()->formID));
            //icon for gender
            key_value_name_.data_provider(CLIK::Object{ build_gfx_value_pair(menu_keys::name, actor->GetName()) });

            auto icon_key =
                util::avatar_util::get_avatar_key(actor_data::get_race_name_for_icon(actor->GetRace()->formID),
                    actor_data::get_gender_name_for_icon(actor_base));
            logger::trace("icon key is: {}"sv, icon_key);

            picture_back_.data_provider(CLIK::Object{ build_gfx_value_icon(icon_key) });

            key_value_gender_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::gender, actor_data::get_gender(actor_base)) });
            key_value_class_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::class_name, actor_base->npcClass->GetName()) });
            key_value_level_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::level, std::to_string(actor->GetLevel())) });
            //maybe hide with cfg because it could spoiler some things
            key_value_faction_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::faction, actor_data::get_faction(actor)) });

            const auto teaches = actor_data::get_is_trainer(actor_base);
            if (const auto max_training = actor_data::get_max_trainings_level(actor_base);
                !teaches.empty() && max_training > 0) {
                const auto max_string = fmt::format(FMT_STRING("({})"), max_training);
                key_value_trainer_.data_provider(
                    CLIK::Object{ build_gfx_value_pair_with_max(menu_keys::trainer, teaches, max_string) });
            } else {
                key_value_trainer_.data_provider(CLIK::Object{ build_gfx_value_pair(menu_keys::trainer, "-") });
            }

            key_value_vendor_.data_provider(CLIK::Object{ build_gfx_value_pair(menu_keys::vendor,
                actor_data::is_vendor(actor) ? menu_keys::yes : menu_keys::no) });

            key_value_morality_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::morality, actor_data::get_morality(actor_base)) });
            key_value_assistance_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::assistence, actor_data::get_assistance(actor_base)) });
            key_value_confidence_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::confidence, actor_data::get_confidence(actor_base)) });
            key_value_aggression_.data_provider(
                CLIK::Object{ build_gfx_value_pair(menu_keys::aggression, actor_data::get_aggression(actor_base)) });

            set_mood_data(actor_base);

            set_relationship_data(actor);

            if (auto av_list = actor_data::get_best_skills(actor); !av_list.empty()) {
                auto a_value = fmt::format(FMT_STRING("({})"), av_list.at(0).second);
                key_value_best_skill_first_.data_provider(CLIK::Object{
                    build_gfx_value_pair_with_max(menu_keys::best_skills, av_list.at(0).first, a_value) });

                a_value = fmt::format(FMT_STRING("({})"), av_list.at(1).second);
                key_value_best_skill_second_.data_provider(
                    CLIK::Object{ build_gfx_value_pair_with_max(" "sv, av_list.at(1).first, a_value) });
            }
        }

        logger::trace("done filling the fields with data"sv);
    }

    void dialogue_info_menu::on_close() {}

    void dialogue_info_menu::log(const RE::FxDelegateArgs& a_params) {
        logger::debug("{}: {}"sv, menu_name, a_params[0].GetString());
    }

    void dialogue_info_menu::set_relationship_data(RE::Actor* a_actor) {
        key_value_relation_.data_provider(CLIK::Object{
            build_gfx_value_pair(menu_keys::relation, actor_data::get_relationship_rank_string(a_actor)) });
    }

    void dialogue_info_menu::set_mood_data(RE::TESNPC* a_actor_base) {
        key_value_mood_.data_provider(
            CLIK::Object{ build_gfx_value_pair(menu_keys::mood, actor_data::get_mood(a_actor_base)) });
    }

    void dialogue_info_menu::adjust_position() {
        root_obj_.X(setting::get_pos_x());
        root_obj_.Y(setting::get_pos_y());
        logger::trace("Current Position is: X {}, Y {}"sv, root_obj_.X(), root_obj_.Y());
    }

    void dialogue_info_menu::scale_window() {
        root_obj_.XScale(setting::get_scale_x());
        root_obj_.YScale(setting::get_scale_y());
        logger::trace("Scaled Window");
    }


}  // scaleform
