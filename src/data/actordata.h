#pragma once
#include "util/menukeys.h"
#include "util/util.h"

#include "util/offset.h"

class actor_data final {
public:
    static std::string_view get_gender(RE::TESNPC*& a_tesnpc) {
        return get_value_from_map(gender_string_map_, a_tesnpc->GetSex());
    }

    static std::string_view get_morality(RE::TESNPC*& a_tesnpc) {
        return get_value_from_map(morality_string_map_, a_tesnpc->GetMoralityLevel());
    }

    static std::string_view get_assistance(RE::TESNPC*& a_tesnpc) {
        return get_value_from_map(assistance_string_map_, a_tesnpc->GetAssistanceLevel());
    }

    static std::string_view get_confidence(RE::TESNPC*& a_tesnpc) {
        return get_value_from_map(confidence_string_map_, a_tesnpc->GetConfidenceLevel());
    }

    static std::string_view get_aggression(RE::TESNPC*& a_tesnpc) {
        return get_value_from_map(aggression_string_map_, a_tesnpc->GetAggressionLevel());
    }

    static std::string_view get_mood(RE::TESNPC*& a_tesnpc) {
        return get_value_from_map(mood_string_map_, a_tesnpc->GetMoodLevel());
    }

    static std::string_view get_faction(RE::Actor*& a_actor) {
        auto faction = "-";
        if (*setting::hide_faction) {
            return faction;
        }
        a_actor->VisitFactions([&](const RE::TESFaction* a_faction, int8_t a_rank) {
            if (a_faction && a_rank > -1) {
                const auto name(a_faction->GetName());
                //auto rankData(a_faction->rankData); normaly not needed
                //logger::trace("name {}, formId {}, rank {}"sv, name, intToHex(formID), a_rank);
                if (const auto form_id(a_faction->GetFormID());
                    std::ranges::find(faction_form_list_, form_id) != faction_form_list_.end()) {
                    logger::trace("name {}, formId {}, rank {}"sv,
                        name,
                        util::string_util::int_to_hex(form_id),
                        a_rank);
                    faction = name;
                    return true;
                }
            }
            return false;
        });
        return faction;
    }

    static std::string_view get_relationship_rank_string(RE::Actor*& a_target) {
        //we want the relation to the player, so get him/her
        const auto player = RE::PlayerCharacter::GetSingleton();
        //testing has shown we need 2 more arguments in front, so we add them
        return get_value_from_map(relation_string_map_, get_relationship_rank(0, 0, a_target, player));
    }

    static std::string_view get_is_trainer(RE::TESNPC*& a_tesnpc) {
        const auto teaches_skill = a_tesnpc->npcClass->data.teaches;
        if (teaches_skill) {
            // handle hand to hand support
            auto value = get_value_from_map(teaching_skill_string_map_, teaches_skill.get());
            if (*setting::hand_to_hand) {
                switch (teaches_skill.get()) {
                    case RE::CLASS_DATA::Skill::kLockpicking:
                        value = menu_keys::hand_to_hand;
                        break;
                    case RE::CLASS_DATA::Skill::kPickpocket:
                        value = menu_keys::security;
                        break;
                }
            }
            return value;
        }
        return "";
    }

    static uint8_t get_max_trainings_level(RE::TESNPC*& a_tesnpc) {
        return a_tesnpc->npcClass->data.maximumTrainingLevel;
    }

    static bool is_vendor(RE::Actor*& a_actor) {
        if (a_actor->vendorFaction) {
            //we could get more info but for now it will do
            //return vendorFaction->IsVendor();
            return true;
        }
        return false;
    }

    static std::vector<std::pair<std::string_view, float>> get_best_skills(RE::Actor*& a_actor) {
        std::vector<std::pair<std::string_view, float>> actor_value_vec;
        for (auto& [av, av_translation] : actor_value_string_map_) {
            actor_value_vec.emplace_back(av_translation, a_actor->GetActorValue(av));
        }

        std::ranges::sort(actor_value_vec, sort_by_val);

        //keep 2 elements, should be enough, for now, otherwise I need a List in the UI
        if (!actor_value_vec.empty()) {
            actor_value_vec.resize(2);
        }

        for (auto& [actor_value, av_value] : actor_value_vec) {
            logger::trace("Actor {}, AV {}, Value {}"sv, a_actor->GetName(), actor_value, av_value);
        }
        return actor_value_vec;
    }

    actor_data(const actor_data&) = delete;
    actor_data(actor_data&&) = delete;

    actor_data& operator=(const actor_data&) = delete;
    actor_data& operator=(actor_data&&) = delete;
private:
    actor_data() = default;

    ~actor_data() = default;

    template <typename T>
    static std::string_view get_value_from_map(std::map<T, std::string_view>& a_map, T a_key) {
        //in case the value
        if (!a_map.contains(a_key)) {
            return "";
        }
        return a_map.find(a_key)->second;
    }

    static bool sort_by_val(const std::pair<std::string_view, float>& a, const std::pair<std::string_view, float>& b) {
        return a.second > b.second;
    }

    //TODO add translation/config for the strings
    inline static std::map<RE::SEX, std::string_view> gender_string_map_ = { { RE::SEX::kMale, menu_keys::male },
                                                                             { RE::SEX::kFemale, menu_keys::female } };

    inline static std::map<RE::ACTOR_MORALITY, std::string_view> morality_string_map_ = {
        { RE::ACTOR_MORALITY::kAnyCrime, menu_keys::any_crime },
        { RE::ACTOR_MORALITY::kViolenceAgainstEnemy, menu_keys::violence_against_enemy },
        { RE::ACTOR_MORALITY::kPropertyCrimeOnly, menu_keys::property_crime_only },
        { RE::ACTOR_MORALITY::kNoCrime, menu_keys::no_crime } };

    inline static std::map<RE::ACTOR_ASSISTANCE, std::string_view> assistance_string_map_ = {
        { RE::ACTOR_ASSISTANCE::kHelpsNobody, menu_keys::helps_nobody },
        { RE::ACTOR_ASSISTANCE::kHelpsAllies, menu_keys::helps_allies },
        { RE::ACTOR_ASSISTANCE::kHelpsFriends, menu_keys::helps_friends }
    };

    inline static std::map<RE::ACTOR_CONFIDENCE, std::string_view> confidence_string_map_ = {
        { RE::ACTOR_CONFIDENCE::kCowardly, menu_keys::cowardly },
        { RE::ACTOR_CONFIDENCE::kCautious, menu_keys::cautious },
        { RE::ACTOR_CONFIDENCE::kAverage, menu_keys::average },
        { RE::ACTOR_CONFIDENCE::kBrave, menu_keys::brave },
        { RE::ACTOR_CONFIDENCE::kFoolhardy, menu_keys::foolhardy }
    };

    inline static std::map<RE::ACTOR_AGGRESSION, std::string_view> aggression_string_map_ = {
        { RE::ACTOR_AGGRESSION::kCalmed, menu_keys::calmed },
        { RE::ACTOR_AGGRESSION::kUnaggressive, menu_keys::unaggressive },
        { RE::ACTOR_AGGRESSION::kAggressive, menu_keys::aggressive },
        { RE::ACTOR_AGGRESSION::kVeryAggressive, menu_keys::very_aggressive },
        { RE::ACTOR_AGGRESSION::kFrenzied, menu_keys::frenzied }
    };

    inline static std::map<RE::ACTOR_MOOD, std::string_view> mood_string_map_ = {
        { RE::ACTOR_MOOD::kNeutral, menu_keys::neutral },
        { RE::ACTOR_MOOD::kAngry, menu_keys::angry },
        { RE::ACTOR_MOOD::kFear, menu_keys::fear },
        { RE::ACTOR_MOOD::kHappy, menu_keys::happy },
        { RE::ACTOR_MOOD::kSad, menu_keys::sad },
        { RE::ACTOR_MOOD::kSurprised, menu_keys::surprised },
        { RE::ACTOR_MOOD::kPuzzled, menu_keys::puzzled },
        { RE::ACTOR_MOOD::kDisgusted, menu_keys::disgusted },
    };

    inline static std::vector<RE::FormID> faction_form_list_{
        0x000DBF9A, //imperial legion
        0x00029DA9, //thiefs guild
        0x00028713, //tribe orc
        0x0002584B, //pentius
        0x0003D6C5, //again
        0x0001F259, //college
        0x00028849, //stormcloaks
        0x000266C8, //gray
        0x00039F26, //thalmor
        0x0001BDB3, //Db
        0x00048362, //companions
        0x00072834, //blades
        0x000B3292, //vigilant
        0x04031786, //skaal
        0x02003376, //vampire, does not have a faction name
        0x02003375, //dawnguard, does not have a faction name
    };

    enum class relationship_ranks {
        archnemesis = -4,
        enemy = -3,
        foe = -2,
        rival = -1,
        acquaintance = 0,
        friend_relation = 1,
        confidant = 2,
        ally = 3,
        lover = 4
    };

    static relationship_ranks get_relationship_rank(uint32_t a_arg1,
        uint32_t a_arg2,
        RE::Actor* a_source,
        RE::Actor* a_target) {
        using func_t = decltype(&get_relationship_rank);
        const REL::Relocation<func_t> func{ REL::ID(offset::get_relationship_rank) };
        return func(a_arg1, a_arg2, a_source, a_target);
    }

    inline static std::map<relationship_ranks, std::string_view> relation_string_map_ = {
        { relationship_ranks::archnemesis, menu_keys::archnemesis },
        { relationship_ranks::enemy, menu_keys::enemy },
        { relationship_ranks::foe, menu_keys::foe },
        { relationship_ranks::rival, menu_keys::rival },
        { relationship_ranks::acquaintance, menu_keys::acquaintance },
        { relationship_ranks::friend_relation, menu_keys::friend_relation },
        { relationship_ranks::confidant, menu_keys::confidant },
        { relationship_ranks::ally, menu_keys::ally },
        { relationship_ranks::lover, menu_keys::lover },
    };

    inline static std::map<RE::CLASS_DATA::Skill, std::string_view> teaching_skill_string_map_ = {
        { RE::CLASS_DATA::Skill::kOneHanded, menu_keys::one_handed },
        { RE::CLASS_DATA::Skill::kTwoHanded, menu_keys::two_handed },
        { RE::CLASS_DATA::Skill::kArchery, menu_keys::archery },
        { RE::CLASS_DATA::Skill::kBlock, menu_keys::block },
        { RE::CLASS_DATA::Skill::kSmithing, menu_keys::smithing },
        { RE::CLASS_DATA::Skill::kHeavyArmor, menu_keys::heavy_armor },
        { RE::CLASS_DATA::Skill::kLightArmor, menu_keys::light_armor },
        { RE::CLASS_DATA::Skill::kPickpocket, menu_keys::pickpocket },
        { RE::CLASS_DATA::Skill::kLockpicking, menu_keys::lockpicking },
        { RE::CLASS_DATA::Skill::kSneak, menu_keys::sneak },
        { RE::CLASS_DATA::Skill::kAlchemy, menu_keys::alchemy },
        { RE::CLASS_DATA::Skill::kSpeech, menu_keys::speech },
        { RE::CLASS_DATA::Skill::kAlteration, menu_keys::alteration },
        { RE::CLASS_DATA::Skill::kConjuration, menu_keys::conjuration },
        { RE::CLASS_DATA::Skill::kDestruction, menu_keys::destruction },
        { RE::CLASS_DATA::Skill::kIllusion, menu_keys::illusion },
        { RE::CLASS_DATA::Skill::kRestoration, menu_keys::restoration },
        { RE::CLASS_DATA::Skill::kEnchanting, menu_keys::enchanting }
    };

    inline static std::map<RE::ActorValue, std::string_view> actor_value_string_map_ = {
        { RE::ActorValue::kOneHanded, menu_keys::one_handed },
        { RE::ActorValue::kTwoHanded, menu_keys::two_handed },
        { RE::ActorValue::kArchery, menu_keys::archery },
        { RE::ActorValue::kBlock, menu_keys::block },
        { RE::ActorValue::kSmithing, menu_keys::smithing },
        { RE::ActorValue::kHeavyArmor, menu_keys::heavy_armor },
        { RE::ActorValue::kLightArmor, menu_keys::light_armor },
        { RE::ActorValue::kPickpocket, menu_keys::pickpocket },
        { RE::ActorValue::kLockpicking, menu_keys::lockpicking },
        { RE::ActorValue::kSneak, menu_keys::sneak },
        { RE::ActorValue::kAlchemy, menu_keys::alchemy },
        { RE::ActorValue::kSpeech, menu_keys::speech },
        { RE::ActorValue::kAlteration, menu_keys::alteration },
        { RE::ActorValue::kConjuration, menu_keys::conjuration },
        { RE::ActorValue::kDestruction, menu_keys::destruction },
        { RE::ActorValue::kIllusion, menu_keys::illusion },
        { RE::ActorValue::kRestoration, menu_keys::restoration },
        { RE::ActorValue::kEnchanting, menu_keys::enchanting }
    };
};
