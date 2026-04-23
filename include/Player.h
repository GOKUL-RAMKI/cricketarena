#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <fstream>
#include "Person.h"   // <<< INHERITANCE: Player extends Person

// Player inherits from Person (gets 'name' and getName()/setName() from Person)
class Player : public Person
{
private:
    // 'name' is now inherited from Person (protected member)

    // Batting stats
    int totalRuns;
    int totalBalls;
    int totalFours;
    int totalSixes;
    int timesOut;
    
    // Bowling stats
    int totalOvers;
    int totalBallsBowled;
    int runsGiven;
    int wicketsTaken;
    
    // Current match stats
    int currentRuns;
    int currentBalls;
    int currentFours;
    int currentSixes;
    bool currentOut;
    
    // Current bowling stats
    int currentOvers;
    int currentBallsInOver;
    int currentRunsGiven;
    int currentWickets;

public:
    // Constructor
    Player();
    Player(std::string playerName);
    
    // Override pure virtual from Person
    void displayInfo() const override;

    // Getters
    // getName() is inherited from Person - no need to redeclare
    int getTotalRuns() const;
    int getTotalBalls() const;
    int getTotalFours() const;
    int getTotalSixes() const;
    int getTimesOut() const;
    int getTotalOvers() const;
    int getTotalBallsBowled() const;
    int getRunsGiven() const;
    int getWicketsTaken() const;
    
    // Current match getters
    int getCurrentRuns() const;
    int getCurrentBalls() const;
    int getCurrentFours() const;
    int getCurrentSixes() const;
    bool isCurrentOut() const;
    int getCurrentOvers() const;
    int getCurrentBallsInOver() const;
    int getCurrentRunsGiven() const;
    int getCurrentWickets() const;
    
    // Calculated stats
    double getBattingAverage() const;
    double getStrikeRate() const;
    double getBowlingAverage() const;
    double getBowlingEconomy() const;
    
    // Update methods
    void updateBattingStats(int runs, bool isFour, bool isSix);
    void updateWicket();
    void updateBowlingStats(int runs);
    void updateBowlerWicket();
    void incrementBowlerBall();
    void completeOver();
    
    // Reset current match stats
    void resetCurrentMatchStats();
    
    // Commit current match stats to total
    void commitMatchStats();
    
    // File I/O
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
    std::string toCSV() const;
    void fromCSV(const std::string& line);
};

#endif
