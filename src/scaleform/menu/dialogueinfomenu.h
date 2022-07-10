#pragma once
#include "CLIK/Array.h"
#include "CLIK/MovieClip.h"
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
                     element_t{ std::ref(name_), "_root.rootObj.name"sv },
                     element_t{ std::ref(gender_), "_root.rootObj.gender"sv },
                     element_t{ std::ref(class_name_), "_root.rootObj.className"sv },
                     element_t{ std::ref(level_), "_root.rootObj.level"sv },
                     element_t{ std::ref(faction_), "_root.rootObj.faction"sv },
                     element_t{ std::ref(trainer_), "_root.rootObj.trainer"sv },
                     element_t{ std::ref(vendor_), "_root.rootObj.vendor"sv },
                     element_t{ std::ref(morality_), "_root.rootObj.morality"sv },
                     element_t{ std::ref(assistence_), "_root.rootObj.assistance"sv },
                     element_t{ std::ref(confidence_), "_root.rootObj.confidence"sv },
                     element_t{ std::ref(aggression_), "_root.rootObj.aggression"sv },
                     element_t{ std::ref(mood_), "_root.rootObj.mood"sv },
                     element_t{ std::ref(relation_), "_root.rootObj.relation"sv },
                     element_t{ std::ref(name_value_), "_root.rootObj.nameValue"sv },
                     element_t{ std::ref(gender_value_), "_root.rootObj.genderValue"sv },
                     element_t{ std::ref(class_name_value_), "_root.rootObj.classNameValue"sv },
                     element_t{ std::ref(level_value_), "_root.rootObj.levelValue"sv },
                     element_t{ std::ref(faction_value_), "_root.rootObj.factionValue"sv },
                     element_t{ std::ref(trainer_value_), "_root.rootObj.trainerValue"sv },
                     element_t{ std::ref(vendor_value_), "_root.rootObj.vendorValue"sv },
                     element_t{ std::ref(morality_value_), "_root.rootObj.moralityValue"sv },
                     element_t{ std::ref(assistence_value_), "_root.rootObj.assistanceValue"sv },
                     element_t{ std::ref(confidence_value_), "_root.rootObj.confidenceValue"sv },
                     element_t{ std::ref(aggression_value_), "_root.rootObj.aggressionValue"sv },
                     element_t{ std::ref(mood_value_), "_root.rootObj.moodValue"sv },
                     element_t{ std::ref(relation_value_), "_root.rootObj.relationValue"sv },
                     element_t{ std::ref(default_portrait_), "_root.rootObj.defaultIcon"sv }
                 }; const auto& [object, path] : objects) {
                auto& instance = object.get().GetInstance();
                [[maybe_unused]] const auto success = view_->GetVariable(std::addressof(instance), path.data());
                assert(success && instance.IsObject());
            }
            logger::debug("Loaded all SWF objects successfully for {}"sv, menu_name);

            root_obj_.Visible(false);

            update_title_fields();
            fill_fields();

            view_->SetVisible(true);
            root_obj_.Visible(true);

            logger::debug("Processed all Objects for Menu {}"sv, menu_name);
        }

        static void update_text(CLIK::TextField a_field, const std::string_view a_string) {
            a_field.AutoSize(CLIK::Object{ "left" });
            a_field.HTMLText(a_string);
            a_field.Visible(true);
        }

        [[nodiscard]] RE::GFxValue build_gfx_value(const std::string& a_val) const {
            RE::GFxValue value;
            view_->CreateObject(std::addressof(value));
            value.SetMember("displayName", { static_cast<std::string_view>(a_val) });
            return value;
        }

        void update_title_fields() const {
            //TODO add translation/config for the strings
            update_text(name_, "Name");
            update_text(gender_, "Gender");
            update_text(class_name_, "Class");
            update_text(level_, "Level");
            update_text(faction_, "Faction");
            update_text(trainer_, "Trainer");
            update_text(vendor_, "Vendor");

            update_text(morality_, "Morality");
            update_text(assistence_, "Assistance");
            update_text(confidence_, "Confidence");
            update_text(aggression_, "Aggression");
            update_text(mood_, "Mood");
            update_text(relation_, "Relationship");
        }

        void fill_fields() {
            logger::trace("going to fill the fields with data"sv);

            if (auto actor = handle::dialogue_actor_handle::get_singleton()->get_actor()) {
                auto actor_base = actor->GetActorBase();
                logger::trace("got Actor {}, formid {}, baseformid {}"sv,
                    actor->GetName(),
                    util::string_util::int_to_hex(actor->formID),
                    util::string_util::int_to_hex(actor_base->formID));
                update_text(race_, actor->GetRace()->GetName());
                update_text(name_value_, actor->GetName());
                //icon for gender
                if (actor->GetName() == "Alvor"sv) {
                    default_portrait_.Visible(true);
                }

                update_text(gender_value_, actor_data::get_gender(actor_base));
                update_text(class_name_value_, actor_base->npcClass->GetName());
                update_text(level_value_, std::to_string(actor->GetLevel()));
                update_text(faction_value_, actor_data::get_faction(actor));

                auto teaches = actor_data::get_is_trainer(actor_base);
                if (auto max_training = actor_data::get_max_trainings_level(actor_base);
                    !teaches.empty() && max_training > 0) {
                    update_text(trainer_value_, fmt::format(FMT_STRING("{}, Max {}"), teaches, max_training));
                } else {
                    update_text(trainer_value_, "-");
                }

                update_text(vendor_value_, actor_data::is_vendor(actor) ? "Yes" : "No");

                update_text(morality_value_, actor_data::get_morality(actor_base));
                update_text(assistence_value_, actor_data::get_assistance(actor_base));
                update_text(confidence_value_, actor_data::get_confidence(actor_base));
                update_text(aggression_value_, actor_data::get_aggression(actor_base));
                update_text(mood_value_, actor_data::get_mood(actor_base));
                update_text(relation_value_, actor_data::get_relationship_rank_string(actor));
            }

            logger::trace("done filling the fields with data"sv);
        }

        void handle_portrait() { default_portrait_.Visible(false); }

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

        CLIK::TextField name_;
        CLIK::TextField gender_;
        CLIK::TextField class_name_;
        CLIK::TextField level_;
        CLIK::TextField faction_;
        CLIK::TextField trainer_;
        CLIK::TextField vendor_;

        CLIK::TextField morality_;
        CLIK::TextField assistence_;
        CLIK::TextField confidence_;
        CLIK::TextField aggression_;
        CLIK::TextField mood_;
        CLIK::TextField relation_;

        CLIK::TextField name_value_;
        CLIK::TextField gender_value_;
        CLIK::TextField class_name_value_;
        CLIK::TextField level_value_;
        CLIK::TextField faction_value_;
        CLIK::TextField trainer_value_;
        CLIK::TextField vendor_value_;

        CLIK::TextField morality_value_;
        CLIK::TextField assistence_value_;
        CLIK::TextField confidence_value_;
        CLIK::TextField aggression_value_;
        CLIK::TextField mood_value_;
        CLIK::TextField relation_value_;

        CLIK::MovieClip default_portrait_;
    };
}
