#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Match.h"
#include "DisplayBase.h"   // <<< INHERITANCE: Scoreboard extends DisplayBase

// Scoreboard inherits from DisplayBase.
// It overrides the pure virtual clear() and showMenu() methods.
class Scoreboard : public DisplayBase
{
public:
    // Overrides of pure virtuals from DisplayBase (instance-level)
    void clear() const override;
    void showMenu(const std::vector<std::string>& options) const override;

    // Static convenience wrappers (original interface — unchanged)
    static void clearScreen();
    static void displayLiveScore(const Match& match);
    static void displayFullScorecard(const Match& match);
    static void displayPlayerStats(const Player& player);
    static void displayTeamLineup(const Team& team);
    static void displayRankings(const std::string& rankingsFile);
    static void displayMatchSummary(const Match& match);
    
    static void waitForEnter();
    static void displayMenu(const std::vector<std::string>& options);
};

#endif
