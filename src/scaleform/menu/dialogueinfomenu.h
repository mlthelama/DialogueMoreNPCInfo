#pragma once
#include "CLIK/Array.h"
#include "CLIK/MovieClip.h"
#include "CLIK/TextField.h"
#include "data/actordata.h"
#include "handle/dialogueactorhandle.h"
#include "util/util.h"

namespace Scaleform {

    class DialogueInfoMenu : public RE::IMenu {
    public:
        static constexpr std::string_view MENU_NAME = "DialogueInfoMenu";
        static constexpr std::string_view FILE_NAME = MENU_NAME;

        static void Register() {
            RE::UI::GetSingleton()->Register(MENU_NAME, Creator);
            logger::info("Registered {}"sv, MENU_NAME);
        }

        static void Open() {
            if (!DialogueInfoMenu::IsMenuOpen()) {
                logger::debug("Open Menu {}"sv, MENU_NAME);
                RE::UIMessageQueue::GetSingleton()->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
            }
        }

        static void Close() {
            if (DialogueInfoMenu::IsMenuOpen()) {
                logger::debug("Close Menu {}"sv, MENU_NAME);
                RE::UIMessageQueue::GetSingleton()->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            }
        }

        static bool IsMenuOpen() {
            auto isOpen = RE::UI::GetSingleton()->IsMenuOpen(MENU_NAME);
            if (isOpen) {
                logger::trace("Menu {} is open {}"sv, MENU_NAME, isOpen);
            }
            return isOpen;
        }

        void RefreshItems() {
            if (IsMenuOpen()) {
                FillFields();
            }
        }

    protected:
        DialogueInfoMenu() {
            using Context = RE::UserEvents::INPUT_CONTEXT_ID;
            using Flag = RE::UI_MENU_FLAGS;

            auto menu = static_cast<RE::IMenu*>(this);
            auto scaleformManager = RE::BSScaleformManager::GetSingleton();
            [[maybe_unused]] const auto success = scaleformManager->LoadMovieEx(menu,
                FILE_NAME,
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

            const auto* const graficState = RE::BSGraphics::State::GetSingleton();
            logger::trace("Screen Resolution {}x{}"sv, graficState->screenWidth, graficState->screenHeight);

            logger::debug("Loading Menu {} was successful {}"sv, FILE_NAME, success);
            assert(success);
            _view = menu->uiMovie;
            //_view->SetMouseCursorCount(0);
            menu->menuFlags |= Flag::kUsesCursor;

            //menu->depthPriority = 0;
            //menu->inputContext = Context::kNone;

            InitExtensions();

            _isActive = true;
            _view->SetVisible(true);

            logger::debug("Set everything for Menu {}"sv, MENU_NAME);
        }

        DialogueInfoMenu(const DialogueInfoMenu&) = delete;
        DialogueInfoMenu(DialogueInfoMenu&&) = delete;

        ~DialogueInfoMenu() = default;

        DialogueInfoMenu& operator=(const DialogueInfoMenu&) = delete;
        DialogueInfoMenu& operator=(DialogueInfoMenu&&) = delete;

        static stl::owner<RE::IMenu*> Creator() { return new DialogueInfoMenu(); }

        void PostCreate() override { DialogueInfoMenu::OnOpen(); }

        //might not work that well if DialogMenu is open
        RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override {
            /*switch (*a_message.type) {
                case RE::UI_MESSAGE_TYPE::kHide:
                    OnClose();
                    return RE::UI_MESSAGE_RESULTS::kHandled;
                default:
                    return RE::IMenu::ProcessMessage(a_message);
            }*/

            if (a_message.menu == DialogueInfoMenu::MENU_NAME) {
                return RE::UI_MESSAGE_RESULTS::kHandled;
            }
            return RE::UI_MESSAGE_RESULTS::kPassOn;
        }

        void AdvanceMovie(float a_interval, uint32_t a_currentTime) override {
            RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
        }

        void Accept(CallbackProcessor* a_processor) override { a_processor->Process("Log", Log); }

    private:
        class Logger : public RE::GFxLog {
        public:
            void LogMessageVarg(LogMessageType, const char* a_fmt, std::va_list a_argList) override {
                std::string fmt(a_fmt ? a_fmt : "");
                while (!fmt.empty() && fmt.back() == '\n') { fmt.pop_back(); }

                std::va_list args;
                va_copy(args, a_argList);
                std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1));
                std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
                va_end(args);

                logger::info("{}: {}"sv, DialogueInfoMenu::MENU_NAME, buf.data());
            }
        };

        void InitExtensions() {
            const RE::GFxValue boolean(true);
            bool success;

            success = _view->SetVariable("_global.gfxExtensions", boolean);
            assert(success);
            /*success = _view->SetVariable("_global.noInvisibleAdvance", boolean);
            assert(success);*/
        }

        void OnOpen() {
            using element_t = std::pair<std::reference_wrapper<CLIK::Object>, std::string_view>;
            std::array objects{
                element_t{ std::ref(_rootObj), "_root.rootObj"sv },
                element_t{ std::ref(_race), "_root.rootObj.race"sv },
                element_t{ std::ref(_name), "_root.rootObj.name"sv },
                element_t{ std::ref(_gender), "_root.rootObj.gender"sv },
                element_t{ std::ref(_className), "_root.rootObj.className"sv },
                element_t{ std::ref(_level), "_root.rootObj.level"sv },
                element_t{ std::ref(_faction), "_root.rootObj.faction"sv },
                element_t{ std::ref(_trainer), "_root.rootObj.trainer"sv },
                element_t{ std::ref(_vendor), "_root.rootObj.vendor"sv },
                element_t{ std::ref(_morality), "_root.rootObj.morality"sv },
                element_t{ std::ref(_assistence), "_root.rootObj.assistance"sv },
                element_t{ std::ref(_confidence), "_root.rootObj.confidence"sv },
                element_t{ std::ref(_aggression), "_root.rootObj.aggression"sv },
                element_t{ std::ref(_mood), "_root.rootObj.mood"sv },
                element_t{ std::ref(_relation), "_root.rootObj.relation"sv },
                element_t{ std::ref(_nameValue), "_root.rootObj.nameValue"sv },
                element_t{ std::ref(_genderValue), "_root.rootObj.genderValue"sv },
                element_t{ std::ref(_classNameValue), "_root.rootObj.classNameValue"sv },
                element_t{ std::ref(_levelValue), "_root.rootObj.levelValue"sv },
                element_t{ std::ref(_factionValue), "_root.rootObj.factionValue"sv },
                element_t{ std::ref(_trainerValue), "_root.rootObj.trainerValue"sv },
                element_t{ std::ref(_vendorValue), "_root.rootObj.vendorValue"sv },
                element_t{ std::ref(_moralityValue), "_root.rootObj.moralityValue"sv },
                element_t{ std::ref(_assistenceValue), "_root.rootObj.assistanceValue"sv },
                element_t{ std::ref(_confidenceValue), "_root.rootObj.confidenceValue"sv },
                element_t{ std::ref(_aggressionValue), "_root.rootObj.aggressionValue"sv },
                element_t{ std::ref(_moodValue), "_root.rootObj.moodValue"sv },
                element_t{ std::ref(_relationValue), "_root.rootObj.relationValue"sv },
            };

            for (const auto& [object, path] : objects) {
                auto& instance = object.get().GetInstance();
                [[maybe_unused]] const auto success = _view->GetVariable(std::addressof(instance), path.data());
                assert(success && instance.IsObject());
            }
            logger::debug("Loaded all SWF objects successfully for {}"sv, MENU_NAME);

            _rootObj.Visible(false);

            UpdateTitleFields();
            FillFields();

            _view->SetVisible(true);
            _rootObj.Visible(true);

            logger::debug("Processed all Objects for Menu {}"sv, MENU_NAME);
        }

        void UpdateText(CLIK::TextField a_field, std::string_view a_string) {
            a_field.AutoSize(CLIK::Object{ "left" });
            a_field.HTMLText(a_string);
            a_field.Visible(true);
        }

        RE::GFxValue buildGFxValue(std::string a_val) {
            RE::GFxValue value;
            _view->CreateObject(std::addressof(value));
            value.SetMember("displayName", { static_cast<std::string_view>(a_val) });
            return value;
        }

        void UpdateTitleFields() {
            //TODO add translation/config for the strings
            UpdateText(_name, "Name");
            UpdateText(_gender, "Gender");
            UpdateText(_className, "Class");
            UpdateText(_level, "Level");
            UpdateText(_faction, "Faction");
            UpdateText(_trainer, "Trainer");
            UpdateText(_vendor, "Vendor");

            UpdateText(_morality, "Morality");
            UpdateText(_assistence, "Assistance");
            UpdateText(_confidence, "Confidence");
            UpdateText(_aggression, "Aggression");
            UpdateText(_mood, "Mood");
            UpdateText(_relation, "Relationship");
        }

        void FillFields() {
            logger::trace("going to fill the fields with data"sv);

            auto actor = Handle::DialogueActorHandle::GetSingleton()->getActor();
            if (actor) {
                auto actorBase = actor->GetActorBase();
                logger::trace("got Actor {}, formid {}, baseformid {}"sv,
                    actor->GetName(),
                    Util::StringUtil::intToHex(actor->formID),
                    Util::StringUtil::intToHex(actorBase->formID));
                UpdateText(_race, actor->GetRace()->GetName());
                UpdateText(_nameValue, actor->GetName());
                //icon for gender
                UpdateText(_genderValue, ActorData::getGender(actorBase));
                UpdateText(_classNameValue, actorBase->npcClass->GetName());
                UpdateText(_levelValue, std::to_string(actor->GetLevel()));
                UpdateText(_factionValue, ActorData::getFaction(actor));

                auto teaches = ActorData::getIsTrainer(actorBase);
                auto maxTraing = ActorData::getMaxTrainingsLevel(actorBase);
                if (!teaches.empty() && maxTraing > 0) {
                    UpdateText(_trainerValue, fmt::format(FMT_STRING("{}, Max {}"), teaches, maxTraing));
                } else {
                    UpdateText(_trainerValue, "-");
                }
                UpdateText(_vendorValue, ActorData::isVendor(actor) ? "Yes" : "No");

                UpdateText(_moralityValue, ActorData::getMorality(actorBase));
                UpdateText(_assistenceValue, ActorData::getAssistance(actorBase));
                UpdateText(_confidenceValue, ActorData::getConfidence(actorBase));
                UpdateText(_aggressionValue, ActorData::getAggression(actorBase));
                UpdateText(_moodValue, ActorData::getMood(actorBase));
                UpdateText(_relationValue, ActorData::getRelationshipRankString(actor));
            }

            logger::trace("done filling the fields with data"sv);
        }

        void OnClose() { return; }

        static void Log(const RE::FxDelegateArgs& a_params) {
            assert(a_params.GetArgCount() == 1);
            assert(a_params[0].IsString());

            logger::debug("{}: {}"sv, DialogueInfoMenu::MENU_NAME, a_params[0].GetString());
        }

        RE::GPtr<RE::GFxMovieView> _view;
        bool _isActive = false;

        CLIK::MovieClip _rootObj;

        CLIK::TextField _race;

        CLIK::TextField _name;
        CLIK::TextField _gender;
        CLIK::TextField _className;
        CLIK::TextField _level;
        CLIK::TextField _faction;
        CLIK::TextField _trainer;
        CLIK::TextField _vendor;

        CLIK::TextField _morality;
        CLIK::TextField _assistence;
        CLIK::TextField _confidence;
        CLIK::TextField _aggression;
        CLIK::TextField _mood;
        CLIK::TextField _relation;

        CLIK::TextField _nameValue;
        CLIK::TextField _genderValue;
        CLIK::TextField _classNameValue;
        CLIK::TextField _levelValue;
        CLIK::TextField _factionValue;
        CLIK::TextField _trainerValue;
        CLIK::TextField _vendorValue;

        CLIK::TextField _moralityValue;
        CLIK::TextField _assistenceValue;
        CLIK::TextField _confidenceValue;
        CLIK::TextField _aggressionValue;
        CLIK::TextField _moodValue;
        CLIK::TextField _relationValue;
    };
}
