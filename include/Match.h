#ifndef MATCH_H
#define MATCH_H

#include <string>
#include "Team.h"
#include "BaseMatch.h"   // <<< INHERITANCE: Match extends BaseMatch

// Match inherits from BaseMatch.
// totalOvers, matchComplete, result are now inherited from BaseMatch.
class Match : public BaseMatch
{
private:
    Team teamA;
    Team teamB;
    int currentInnings;  // 1 or 2
    Team* battingTeam;
    Team* bowlingTeam;
    
    // Current match state
    int strikerIndex;
    int nonStrikerIndex;
    int nextBatsmanIndex;
    int currentBowlerIndex;
    int targetRuns;
    bool lastDeliveryWasWicket;

public:
    // Constructor
    Match();
    Match(const Team& tA, const Team& tB, int overs);
    
    // Getters
    Team& getTeamA();
    Team& getTeamB();
    const Team& getTeamA() const;
    const Team& getTeamB() const;
    // getTotalOvers()   inherited from BaseMatch
    // isMatchComplete() inherited from BaseMatch
    // getResult()       inherited from BaseMatch
    int getCurrentInnings() const;
    Team* getBattingTeam();
    Team* getBowlingTeam();
    int getStrikerIndex() const;
    int getNonStrikerIndex() const;
    int getNextBatsmanIndex() const;
    int getCurrentBowlerIndex() const;
    int getTargetRuns() const;
    bool isCurrentInningsComplete() const;
    
    // Setters
    // setMatchComplete() inherited from BaseMatch
    // setResult()        inherited from BaseMatch
    void setCurrentInnings(int innings);
    void setStrikerIndex(int index);
    void setNonStrikerIndex(int index);
    void setNextBatsmanIndex(int index);
    void setCurrentBowlerIndex(int index);
    void setTargetRuns(int target);
    
    // Match operations
    void startInnings(int inningsNumber);
    void selectOpeningBatsmen(int striker, int nonStriker);
    void selectBowler(const std::string& bowlerName);
    void playDelivery(int runs);
    void handleWicket();
    void swapBatsmen();
    void completeOver();
    void checkMatchEnd();

    // Override pure virtuals from BaseMatch
    void calculateResult() override;
    void saveMatchState(const std::string& filename) override;
    void loadMatchState(const std::string& filename) override;
};

#endif
