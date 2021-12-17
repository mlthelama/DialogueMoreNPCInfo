#pragma once
#include "util/util.h"

class ActorData {
public:
    static std::string_view getGender(RE::TESNPC*& a_tesnpc) {
        return getValueFromMap(_genderStringMap, a_tesnpc->GetSex());
    }

    static std::string_view getMorality(RE::TESNPC*& a_tesnpc) {
        return getValueFromMap(_moralityStringMap, a_tesnpc->GetMoralityLevel());
    }

    static std::string_view getAssistance(RE::TESNPC*& a_tesnpc) {
        return getValueFromMap(_assistanceStringMap, a_tesnpc->GetAssistanceLevel());
    }

    static std::string_view getConfidence(RE::TESNPC*& a_tesnpc) {
        return getValueFromMap(_confidenceStringMap, a_tesnpc->GetConfidenceLevel());
    }

    static std::string_view getAggression(RE::TESNPC*& a_tesnpc) {
        return getValueFromMap(_aggressionStringMap, a_tesnpc->GetAggressionLevel());
    }

    static std::string_view getMood(RE::TESNPC*& a_tesnpc) {
        return getValueFromMap(_moodStringMap, a_tesnpc->GetMoodLevel());
    }

    static std::string_view getFaction(RE::Actor*& a_actor) {
        auto faction = "-";
        a_actor->VisitFactions([&](RE::TESFaction* a_faction, int8_t a_rank) {
            if (a_faction && a_rank > -1) {
                const auto name(a_faction->GetName());
                const auto formID(a_faction->GetFormID());
                //auto rankData(a_faction->rankData); normaly not needed
                //logger::trace("name {}, formId {}, rank {}"sv, name, intToHex(formID), a_rank);
                if (std::find(_factionFormList.begin(), _factionFormList.end(), formID) != _factionFormList.end()) {
                    logger::trace("name {}, formId {}, rank {}"sv, name, Util::StringUtil::intToHex(formID), a_rank);
                    faction = name;
                    return true;
                }
            }
            return false;
        });
        return faction;
    }

    static std::string_view getRelationshipRankString(RE::Actor*& a_target) {
        //we want the relation to the player, so get him/her
        auto player = RE::PlayerCharacter::GetSingleton();
        //testing has shown we need 2 more arguments in front, so we add them
        return getValueFromMap(_relationStringMap, GetRelationshipRank(0, 0, a_target, player));
    }

private:
    ActorData() = default;
    ActorData(const ActorData&) = delete;
    ActorData(ActorData&&) = delete;
    virtual ~ActorData() = default;

    ActorData& operator=(const ActorData&) = delete;
    ActorData& operator=(ActorData&&) = delete;

    template <typename T>
    static std::string_view getValueFromMap(std::map<T, std::string_view>& a_map, T a_key) {
        //in case the value
        if (a_map.find(a_key) == a_map.end()) {
            return "";
        } else {
            return a_map.find(a_key)->second;
        }
    }

    //TODO add translation/config for the strings
    inline static std::map<RE::SEX, std::string_view> _genderStringMap = { { RE::SEX::kMale, "Male" },
        { RE::SEX::kFemale, "Female" } };

    inline static std::map<RE::ACTOR_MORALITY, std::string_view> _moralityStringMap = { { RE::ACTOR_MORALITY::kAnyCrime,
                                                                                            "Any Crime" },
        { RE::ACTOR_MORALITY::kViolenceAgainstEnemy, "Violence Against Enemies" },
        { RE::ACTOR_MORALITY::kPropertyCrimeOnly, "Property Crime Only" },
        { RE::ACTOR_MORALITY::kNoCrime, "No Crime" } };

    inline static std::map<RE::ACTOR_ASSISTANCE, std::string_view> _assistanceStringMap = {
        { RE::ACTOR_ASSISTANCE::kHelpsNobody, "Helps Nobody" },
        { RE::ACTOR_ASSISTANCE::kHelpsAllies, "Helps Allies" },
        { RE::ACTOR_ASSISTANCE::kHelpsFriends, "Helps Friends" }
    };

    inline static std::map<RE::ACTOR_CONFIDENCE, std::string_view> _confidenceStringMap = {
        { RE::ACTOR_CONFIDENCE::kCowardly, "Cowardly" },
        { RE::ACTOR_CONFIDENCE::kCautious, "Cautious" },
        { RE::ACTOR_CONFIDENCE::kAverage, "Average" },
        { RE::ACTOR_CONFIDENCE::kBrave, "Brave" },
        { RE::ACTOR_CONFIDENCE::kFoolhardy, "Foolhardy" }
    };

    inline static std::map<RE::ACTOR_AGGRESSION, std::string_view> _aggressionStringMap = {
        { RE::ACTOR_AGGRESSION::kCalmed, "Calmed" },
        { RE::ACTOR_AGGRESSION::kUnaggressive, "Unaggressive" },
        { RE::ACTOR_AGGRESSION::kAggressive, "Aggressive" },
        { RE::ACTOR_AGGRESSION::kVeryAggressive, "Very Aggressive" },
        { RE::ACTOR_AGGRESSION::kFrenzied, "Frenzied" }
    };

    inline static std::map<RE::ACTOR_MOOD, std::string_view> _moodStringMap = {
        { RE::ACTOR_MOOD::kNeutral, "Neutral" },
        { RE::ACTOR_MOOD::kAngry, "Angry" },
        { RE::ACTOR_MOOD::kFear, "Fear" },
        { RE::ACTOR_MOOD::kHappy, "Happy" },
        { RE::ACTOR_MOOD::kSad, "Sad" },
        { RE::ACTOR_MOOD::kSurprised, "Surprised" },
        { RE::ACTOR_MOOD::kPuzzled, "Puzzled" },
        { RE::ACTOR_MOOD::kDisgusted, "Disgusted" },
    };

    inline static std::vector<RE::FormID> _factionFormList{
        { 0x000DBF9A },  //imperial legion
        { 0x00029DA9 },  //thiefs guild
        { 0x00028713 },  //tribe orc
        { 0x0002584B },  //pentius
        { 0x0003D6C5 },  //again
        { 0x0001F259 },  //college
        { 0x00028849 },  //stormcloaks
        { 0x000266C8 },  //gray
        { 0x00039F26 },  //thalmor
        { 0x0001BDB3 },  //Db
        { 0x00048362 },  //companions
        { 0x00072834 },  //blades
        { 0x000B3292 },  //vigilant
        { 0x04031786 },  //skaal
        { 0x02003376 },  //vampire, does not have a faction name
        { 0x02003375 },  //dawnguard, does not have a faction name
    };

    enum class RelationshipRanks {
        Archnemesis = -4,
        Enemy = -3,
        Foe = -2,
        Rival = -1,
        Acquaintance = 0,
        Friend = 1,
        Confidant = 2,
        Ally = 3,
        Lover = 4
    };

    static RelationshipRanks
        GetRelationshipRank(uint32_t a_arg1, uint32_t a_arg2, RE::Actor* a_source, RE::Actor* a_target) {
        using func_t = decltype(&GetRelationshipRank);
        REL::Relocation<func_t> func{ REL::ID(53898) };
        return static_cast<RelationshipRanks>(func(a_arg1, a_arg2, a_source, a_target));
    }

    inline static std::map<RelationshipRanks, std::string_view> _relationStringMap = {
        { RelationshipRanks::Archnemesis, "Archnemesis" },
        { RelationshipRanks::Enemy, "Enemy" },
        { RelationshipRanks::Foe, "Foe" },
        { RelationshipRanks::Rival, "Rival" },
        { RelationshipRanks::Acquaintance, "Acquaintance" },
        { RelationshipRanks::Friend, "Friend" },
        { RelationshipRanks::Confidant, "Confidant" },
        { RelationshipRanks::Ally, "Ally" },
        { RelationshipRanks::Lover, "Lover" },
    };
};
