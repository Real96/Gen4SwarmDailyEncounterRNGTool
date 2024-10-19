#include <iostream>
#include "mt.h"
#include "lcrng.h"
#include <math.h>
#include <string_view>
#include <array>
#include <regex>

using namespace std;

#define DP_MAX_SWARM (28)
#define Pt_MAX_SWARM (22)
#define HGSS_MAX_SWARM (20)

short sanitizeGameVersionInput() {
    string gameVersion;
    const regex gameVersionRegex("^(dp|DP|Dp|dP|pt|PT|Pt|pT|hgss|HGSS|HGss|hgSS)$");

    while ((cout << "Game version? (dp/pt/hgss) ") && (!(cin >> gameVersion) || !regex_match(gameVersion, gameVersionRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    transform(gameVersion.begin(), gameVersion.end(), gameVersion.begin(), ::toupper);

    return gameVersion == "DP" ? 0 : gameVersion == "PT" ? 1 : 2;
}

void printTableEncounterIndexes(const short game, const short limit) {
    static constexpr array<string_view, DP_MAX_SWARM> swarmEncounterDPTable{
        "Doduo", "Zigzagoon", "Cubone", "Nosepass", "Phanpy", "Dunsparce", "Snubbull", "Absol", "Spoink", "Drowzee",
        "Delibird", "Swinub", "Voltorb", "Farfetch'd", "Skitty", "Natu", "Makuhita", "Krabby", "Spinda", "Beldum",
        "Pidgey", "Corsola", "Surskit", "Lickitung", "Smoochum", "Electrike", "Slakoth", "Magnemite" };

    static constexpr array<string_view, Pt_MAX_SWARM> swarmEncounterPtTable{
        "Doduo", "Zigzagoon", "Cubone", "Larvitar", "Phanpy", "Dunsparce", "Snubbull", "Spoink", "Drowzee", "Delibird",
        "Voltorb", "Farfetch'd", "Skitty", "Natu", "Makuhita", "Krabby", "Spinda", "Beldum", "Pinsir", "Corsola",
        "Electrike", "Slakoth" };

    static constexpr array<string_view, HGSS_MAX_SWARM> swarmEncounterHGSSTable{
        "Poochyena", "Baltoy (HG) / Gulpin (SS)", "Sableye (HG) / Mawile (SS)", "Relicanth", "Chansey", "Clamperl",
        "Qwilfish", "Buneary", "Luvdisc", "Ralts", "Yanma", "Snubbull", "Remoraid", "Swablu", "Ditto", "Marill",
        "Dunsparce", "Kricketot", "Wingull", "Whiscash" };

    static constexpr array<string_view, DP_MAX_SWARM> swarmLocationDPTable{
        "Route 201", "Route 202", "Route 203", "Route 206", "Route 207", "Route 208", "Route 209", "Route 213",
        "Route 214", "Route 215", "Route 216", "Route 217", "Route 218", "Route 221", "Route 222", "Route 224",
        "Route 225", "Route 226", "Route 227", "Route 228", "Route 229", "Route 230", "Lake Verity", "Lake Valor",
        "Lake Acuity", "Valley Windworks", "Eterna Forest", "Fuego Ironworks" };

    static constexpr array<string_view, Pt_MAX_SWARM> swarmLocationPtTable{
        "Route 201", "Route 202", "Route 203", "Route 206", "Route 207", "Route 208", "Route 209", "Route 214",
        "Route 215", "Route 217", "Route 218", "Route 221", "Route 222", "Route 224", "Route 225", "Route 226",
        "Route 227", "Route 228", "Route 229", "Route 230", "Valley Windworks", "Eterna Forest" };

    static constexpr array<string_view, HGSS_MAX_SWARM> swarmLocationHGSSTable{
        "Route 1", "Route 3", "Route 9", "Route 12 (Fishing)", "Route 13", "Route 19 (Surfing)", "Route 32 (Fishing)",
        "Route 25", "Route 27 (Surfing)", "Route 34", "Route 35", "Route 38", "Route 44 (Fishing)", "Route 45", "Route 47",
        "Mt. Mortar", "Dark Cave", "Viridian Forest", "Vermilion City (Surfing)", "Violet City (Fishing)" };

    static constexpr array<const string_view*, 3> pokemonNameTables{ swarmEncounterDPTable.data(), swarmEncounterPtTable.data(), swarmEncounterHGSSTable.data() };
    static constexpr array<const string_view*, 3> locationNameTables{ swarmLocationDPTable.data(), swarmLocationPtTable.data(), swarmLocationHGSSTable.data() };

    cout << "\n\nSwarm encounters:\n\n";

    for (short i = 0; i < limit; i++) {
        cout << i + 1 << " " << pokemonNameTables[game][i] << " - " << locationNameTables[game][i] << "\n";
    }

    cout << "\n\n";
}

template <typename T>
void sanitizeInput(const string output, T &value, const T lowLimit, const T highLimit) {
    while ((cout << output) && (!(cin >> value) || (value < lowLimit || value > highLimit))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool isWantedEncounterCheck(const uint32_t seed, const short limit, const short encounter) {
    return seed % limit == encounter - 1;
}

void findEncounterSeed(const short limit) {
    short encounter, minDelay, hour, minMTAdvances;
    const short maxDelay = 10000;

    sanitizeInput<short>("Insert the wanted encounter index: ", encounter, 1, limit);
    sanitizeInput<short>("Insert the min delay: ", minDelay, 600, 9999);
    sanitizeInput<short>("Insert the min hours (0-23): ", hour, 0, 23);
    sanitizeInput<short>("Insert the needed MT advances: ", minMTAdvances, 0, 9999);

    for (short ab = 0; ab < 256; ab++) {
        for (short cd = hour; cd < 24; cd++) {
            for (short efgh = minDelay; efgh < maxDelay; efgh++) {
                const uint32_t seed = ((ab << 24) | (cd << 16)) + efgh;
                mt MT(seed);
                lcrng LCRNG(MT.advance(minMTAdvances));

                if (isWantedEncounterCheck(LCRNG.next(2), limit, encounter)) {
                    printf("\n\nTarget seed: %08X\n\n------------------------------------------------\n\n", seed);
                    return;
                }
            }
        }
    }
}

int main() {
    while (true) {
        const short game = sanitizeGameVersionInput(), limit = game == 0 ? DP_MAX_SWARM : game == 1 ? Pt_MAX_SWARM : HGSS_MAX_SWARM;
        printTableEncounterIndexes(game, limit);
        findEncounterSeed(limit);
    }
}