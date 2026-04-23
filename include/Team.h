#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>
#include "Player.h"

class Team 
{
private:
    std::string teamName;
    std::vector<Player> players;
    int totalRuns;
    int totalWickets;
    int oversDone;
    int ballsInCurrentOver;

public:
    // Constructor
    Team();
    Team(std::string name);
    
    // Getters
    std::string getTeamName() const;
    int getTotalRuns() const;
    int getTotalWickets() const;
    int getOversDone() const;
    int getBallsInCurrentOver() const;
    int getPlayerCount() const;
    Player& getPlayer(int index);
    const Player& getPlayer(int index) const;
    
    // Setters
    void setTotalRuns(int runs);
    void setTotalWickets(int wickets);
    void setOversDone(int overs);
    void setBallsInCurrentOver(int balls);
    
    // Team management
    void addPlayer(const Player& player);
    int findPlayerByName(const std::string& name) const;
    
    // Match operations
    void addRuns(int runs);
    void addWicket();
    void incrementBall();
    void resetInningsStats();
    
    // File I/O
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif
