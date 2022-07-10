#pragma once
#include "util/util.h"

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
            return get_value_from_map(_teachingSkillStringMap, teaches_skill.get());
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

    //TODO add translation/config for the strings
    inline static std::map<RE::SEX, std::string_view> gender_string_map_ = { { RE::SEX::kMale, "Male" },
                                                                             { RE::SEX::kFemale, "Female" } };

    inline static std::map<RE::ACTOR_MORALITY, std::string_view> morality_string_map_ = {
        { RE::ACTOR_MORALITY::kAnyCrime, "Any Crime" },
        { RE::ACTOR_MORALITY::kViolenceAgainstEnemy, "Violence Against Enemies" },
        { RE::ACTOR_MORALITY::kPropertyCrimeOnly, "Property Crime Only" },
        { RE::ACTOR_MORALITY::kNoCrime, "No Crime" } };

    inline static std::map<RE::ACTOR_ASSISTANCE, std::string_view> assistance_string_map_ = {
        { RE::ACTOR_ASSISTANCE::kHelpsNobody, "Helps Nobody" },
        { RE::ACTOR_ASSISTANCE::kHelpsAllies, "Helps Allies" },
        { RE::ACTOR_ASSISTANCE::kHelpsFriends, "Helps Friends" }
    };

    inline static std::map<RE::ACTOR_CONFIDENCE, std::string_view> confidence_string_map_ = {
        { RE::ACTOR_CONFIDENCE::kCowardly, "Cowardly" },
        { RE::ACTOR_CONFIDENCE::kCautious, "Cautious" },
        { RE::ACTOR_CONFIDENCE::kAverage, "Average" },
        { RE::ACTOR_CONFIDENCE::kBrave, "Brave" },
        { RE::ACTOR_CONFIDENCE::kFoolhardy, "Foolhardy" }
    };

    inline static std::map<RE::ACTOR_AGGRESSION, std::string_view> aggression_string_map_ = {
        { RE::ACTOR_AGGRESSION::kCalmed, "Calmed" },
        { RE::ACTOR_AGGRESSION::kUnaggressive, "Unaggressive" },
        { RE::ACTOR_AGGRESSION::kAggressive, "Aggressive" },
        { RE::ACTOR_AGGRESSION::kVeryAggressive, "Very Aggressive" },
        { RE::ACTOR_AGGRESSION::kFrenzied, "Frenzied" }
    };

    inline static std::map<RE::ACTOR_MOOD, std::string_view> mood_string_map_ = {
        { RE::ACTOR_MOOD::kNeutral, "Neutral" },
        { RE::ACTOR_MOOD::kAngry, "Angry" },
        { RE::ACTOR_MOOD::kFear, "Fear" },
        { RE::ACTOR_MOOD::kHappy, "Happy" },
        { RE::ACTOR_MOOD::kSad, "Sad" },
        { RE::ACTOR_MOOD::kSurprised, "Surprised" },
        { RE::ACTOR_MOOD::kPuzzled, "Puzzled" },
        { RE::ACTOR_MOOD::kDisgusted, "Disgusted" },
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
        const REL::Relocation<func_t> func{ REL::ID(53898) };
        return func(a_arg1, a_arg2, a_source, a_target);
    }

    inline static std::map<relationship_ranks, std::string_view> relation_string_map_ = {
        { relationship_ranks::archnemesis, "Archnemesis" },
        { relationship_ranks::enemy, "Enemy" },
        { relationship_ranks::foe, "Foe" },
        { relationship_ranks::rival, "Rival" },
        { relationship_ranks::acquaintance, "Acquaintance" },
        { relationship_ranks::friend_relation, "Friend" },
        { relationship_ranks::confidant, "Confidant" },
        { relationship_ranks::ally, "Ally" },
        { relationship_ranks::lover, "Lover" },
    };

    inline static std::map<RE::CLASS_DATA::Skill, std::string_view> _teachingSkillStringMap = {
        { RE::CLASS_DATA::Skill::kOneHanded, "OneHanded" },
        { RE::CLASS_DATA::Skill::kTwoHanded, "TwoHanded" },
        { RE::CLASS_DATA::Skill::kArchery, "Archery" },
        { RE::CLASS_DATA::Skill::kBlock, "Block" },
        { RE::CLASS_DATA::Skill::kSmithing, "Smithing" },
        { RE::CLASS_DATA::Skill::kHeavyArmor, "HeavyArmor" },
        { RE::CLASS_DATA::Skill::kLightArmor, "LightArmor" },
        { RE::CLASS_DATA::Skill::kPickpocket, "Pickpocket" },
        { RE::CLASS_DATA::Skill::kLockpicking, "Lockpicking" },
        { RE::CLASS_DATA::Skill::kSneak, "Sneak" },
        { RE::CLASS_DATA::Skill::kAlchemy, "Alchemy" },
        { RE::CLASS_DATA::Skill::kSpeech, "Speech" },
        { RE::CLASS_DATA::Skill::kAlteration, "Alteration" },
        { RE::CLASS_DATA::Skill::kConjuration, "Conjuration" },
        { RE::CLASS_DATA::Skill::kDestruction, "Destruction" },
        { RE::CLASS_DATA::Skill::kIllusion, "Illusion" },
        { RE::CLASS_DATA::Skill::kRestoration, "Restoration" },
        { RE::CLASS_DATA::Skill::kEnchanting, "Enchanting" }
    };
};
