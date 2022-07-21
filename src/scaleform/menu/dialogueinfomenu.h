#pragma once
#include "CLIK/keyvalue.h"
#include "CLIK/MovieClip.h"
#include "CLIK/PictureBack.h"
#include "CLIK/TextField.h"
#include "data/actordata.h"
#include "handle/dialogueactorhandle.h"
#include "util/util.h"

namespace scaleform {

    class dialogue_info_menu final : public RE::IMenu {
    public:
        static constexpr std::string_view menu_name = "DialogueInfoMenu";
        static constexpr std::string_view file_name = menu_name;

        static void Register() {
            RE::UI::GetSingleton()->Register(menu_name, creator);
            logger::info("Registered {}"sv, menu_name);
        }

        static void open() {
            if (!is_menu_open()) {
                logger::debug("Open Menu {}"sv, menu_name);
                RE::UIMessageQueue::GetSingleton()->AddMessage(menu_name, RE::UI_MESSAGE_TYPE::kShow, nullptr);
            }
        }

        static void close() {
            if (is_menu_open()) {
                logger::debug("Close Menu {}"sv, menu_name);
                RE::UIMessageQueue::GetSingleton()->AddMessage(menu_name, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            }
        }

        static bool is_menu_open() {
            auto is_open = RE::UI::GetSingleton()->IsMenuOpen(menu_name);
            if (is_open) {
                logger::trace("Menu {} is open {}"sv, menu_name, is_open);
            }
            return is_open;
        }

        void refresh_items() {
            if (is_menu_open()) {
                fill_fields();
            }
        }

        dialogue_info_menu(const dialogue_info_menu&) = delete;
        dialogue_info_menu(dialogue_info_menu&&) = delete;

        dialogue_info_menu& operator=(const dialogue_info_menu&) = delete;
        dialogue_info_menu& operator=(dialogue_info_menu&&) = delete;
    protected:
        dialogue_info_menu() {
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

            const auto* const graphic_state = RE::BSGraphics::State::GetSingleton();
            logger::trace("Screen Resolution {}x{}"sv, graphic_state->screenWidth, graphic_state->screenHeight);

            logger::debug("Loading Menu {} was successful {}"sv, file_name, success);
            assert(success);
            view_ = menu->uiMovie;
            //_view->SetMouseCursorCount(0);
            menu->menuFlags |= flag::kUsesCursor;

            //menu->depthPriority = 0;
            //menu->inputContext = Context::kNone;

            init_extensions();

            is_active_ = true;
            view_->SetVisible(true);

            logger::debug("Set everything for Menu {}"sv, menu_name);
        }

        ~dialogue_info_menu() override = default;

        static stl::owner<IMenu*> creator() { return new dialogue_info_menu(); }

        void PostCreate() override { on_open(); }

        //might not work that well if DialogMenu is open
        RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override {
            /*switch (*a_message.type) {
                case RE::UI_MESSAGE_TYPE::kHide:
                    OnClose();
                    return RE::UI_MESSAGE_RESULTS::kHandled;
                default:
                    return RE::IMenu::ProcessMessage(a_message);
            }*/

            if (a_message.menu == menu_name) {
                return RE::UI_MESSAGE_RESULTS::kHandled;
            }
            return RE::UI_MESSAGE_RESULTS::kPassOn;
        }

        void AdvanceMovie(const float a_interval, const uint32_t a_current_time) override {
            IMenu::AdvanceMovie(a_interval, a_current_time);
        }

        void Accept(CallbackProcessor* a_processor) override { a_processor->Process("Log", log); }

    private:
        class Logger : public RE::GFxLog {
        public:
            void LogMessageVarg(LogMessageType, const char* a_fmt, const std::va_list a_arg_list) override {
                std::string fmt(a_fmt ? a_fmt : "");
                while (!fmt.empty() && fmt.back() == '\n') { fmt.pop_back(); }

                std::va_list args;
                va_copy(args, a_arg_list);
                std::vector<char>
                    buf(static_cast<std::size_t>(std::vsnprintf(nullptr, 0, fmt.c_str(), a_arg_list) + 1));
                std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
                va_end(args);

                logger::info("{}: {}"sv, menu_name, buf.data());
            }
        };

        void init_extensions() const {
            const RE::GFxValue boolean(true);

            [[maybe_unused]] const bool success = view_->SetVariable("_global.gfxExtensions", boolean);
            assert(success);
            /*success = _view->SetVariable("_global.noInvisibleAdvance", boolean);
            assert(success);*/
        }

        void on_open() {
            using element_t = std::pair<std::reference_wrapper<CLIK::Object>, std::string_view>;

            for (std::array objects{
                     element_t{ std::ref(root_obj_), "_root.rootObj"sv },
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
                 }; const auto& [object, path] : objects) {
                auto& instance = object.get().GetInstance();
                [[maybe_unused]] const auto success = view_->GetVariable(std::addressof(instance), path.data());
                assert(success && instance.IsObject());
            }
            logger::debug("Loaded all SWF objects successfully for {}"sv, menu_name);

            root_obj_.Visible(false);

            fill_fields();

            view_->SetVisible(true);
            root_obj_.Visible(true);

            logger::debug("Processed all Objects for Menu {}"sv, menu_name);
        }

        static void update_text(CLIK::TextField a_field, const std::string_view a_string) {
            a_field.AutoSize(CLIK::Object{ "center" });
            a_field.HTMLText(a_string);
            a_field.Visible(true);
        }

        [[nodiscard]] RE::GFxValue build_gfx_value_icon(const std::string& a_key) const {
            RE::GFxValue value;
            view_->CreateObject(std::addressof(value));
            value.SetMember("iconKey", { static_cast<std::string_view>(a_key) });
            return value;
        }

        [[nodiscard]] RE::GFxValue build_gfx_value_pair(const std::string_view& a_key,
            const std::string_view& a_value) const {
            RE::GFxValue value;
            view_->CreateObject(std::addressof(value));
            value.SetMember("keyText", { a_key });
            value.SetMember("valueText", { a_value });
            return value;
        }

        void fill_fields() {
            logger::trace("going to fill the fields with data"sv);

            if (auto actor = handle::dialogue_actor_handle::get_singleton()->get_actor()) {
                auto actor_base = actor->GetActorBase();
                logger::trace("got Actor {}, formid {}, baseformid {}"sv,
                    actor->GetName(),
                    util::string_util::int_to_hex(actor->formID),
                    util::string_util::int_to_hex(actor_base->formID));

                const auto gender = actor_data::get_gender(actor_base);
                const auto race = actor->GetRace()->GetName();

                update_text(race_, race); //center value
                //icon for gender
                key_value_name_.data_provider(CLIK::Object{ build_gfx_value_pair(menu_keys::name, actor->GetName()) });

                auto icon_key = util::avatar_util::get_avatar_key(race, static_cast<std::string>(gender));
                logger::trace("icon key is: {}"sv, icon_key);

                picture_back_.data_provider(CLIK::Object{ build_gfx_value_icon(icon_key) });

                key_value_gender_.data_provider(CLIK::Object{ build_gfx_value_pair(menu_keys::gender, gender) });
                key_value_class_.data_provider(
                    CLIK::Object{ build_gfx_value_pair(menu_keys::class_name, actor_base->npcClass->GetName()) });
                key_value_level_.data_provider(
                    CLIK::Object{ build_gfx_value_pair(menu_keys::level, std::to_string(actor->GetLevel())) });
                //maybe hide with cfg because it could spoiler some things
                key_value_faction_.data_provider(
                    CLIK::Object{ build_gfx_value_pair(menu_keys::faction, actor_data::get_faction(actor)) });

                auto teaches = actor_data::get_is_trainer(actor_base);
                if (auto max_training = actor_data::get_max_trainings_level(actor_base);
                    !teaches.empty() && max_training > 0) {
                    key_value_trainer_.data_provider(CLIK::Object{
                        build_gfx_value_pair(menu_keys::trainer,
                            fmt::format(FMT_STRING("{}, Max {}"), teaches, max_training)) });
                } else {
                    key_value_trainer_.data_provider(CLIK::Object{ build_gfx_value_pair(menu_keys::trainer, "-") });
                }

                key_value_vendor_.data_provider(CLIK::Object{
                    build_gfx_value_pair(menu_keys::vendor,
                        actor_data::is_vendor(actor) ? menu_keys::yes : menu_keys::no) });

                key_value_morality_.data_provider(
                    CLIK::Object{ build_gfx_value_pair(menu_keys::morality, actor_data::get_morality(actor_base)) });
                key_value_assistance_.data_provider(
                    CLIK::Object{
                        build_gfx_value_pair(menu_keys::assistence, actor_data::get_assistance(actor_base)) });
                key_value_confidence_.data_provider(
                    CLIK::Object{
                        build_gfx_value_pair(menu_keys::confidence, actor_data::get_confidence(actor_base)) });
                key_value_aggression_.data_provider(
                    CLIK::Object{
                        build_gfx_value_pair(menu_keys::aggression, actor_data::get_aggression(actor_base)) });
                key_value_mood_.data_provider(
                    CLIK::Object{ build_gfx_value_pair(menu_keys::mood, actor_data::get_mood(actor_base)) });
                key_value_relation_.data_provider(CLIK::Object{
                    build_gfx_value_pair(menu_keys::relation, actor_data::get_relationship_rank_string(actor)) });
            }

            logger::trace("done filling the fields with data"sv);
        }

        static void on_close() { }

        static void log(const RE::FxDelegateArgs& a_params) {
            assert(a_params.GetArgCount() == 1);
            assert(a_params[0].IsString());

            logger::debug("{}: {}"sv, menu_name, a_params[0].GetString());
        }

        RE::GPtr<RE::GFxMovieView> view_;
        bool is_active_ = false;

        CLIK::MovieClip root_obj_;

        CLIK::TextField race_;

        CLIK::picture_back picture_back_;

        CLIK::key_value key_value_name_;
        CLIK::key_value key_value_gender_;
        CLIK::key_value key_value_class_;
        CLIK::key_value key_value_level_;
        CLIK::key_value key_value_faction_;
        CLIK::key_value key_value_trainer_;
        CLIK::key_value key_value_vendor_;

        CLIK::key_value key_value_morality_;
        CLIK::key_value key_value_assistance_;
        CLIK::key_value key_value_confidence_;
        CLIK::key_value key_value_aggression_;
        CLIK::key_value key_value_mood_;
        CLIK::key_value key_value_relation_;

    };
}
