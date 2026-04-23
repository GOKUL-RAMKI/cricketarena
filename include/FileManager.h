#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Player.h"
#include "Team.h"

class FileManager 
{
private:
    std::string playerStatsFile;
    std::string matchStateFile;
    std::string rankingsFile;

public:
    // Constructor
    FileManager();
    FileManager(const std::string& statsFile, const std::string& stateFile, const std::string& ranksFile);
    
    // Player stats management
    void savePlayerStats(const std::vector<Player>& allPlayers);
    std::vector<Player> loadPlayerStats();
    void updatePlayerInDatabase(const Player& player);
    Player getPlayerFromDatabase(const std::string& playerName);
    
    // Rankings generation
    void generateRankings(const std::vector<Player>& allPlayers);
    
    // Utility
    bool fileExists(const std::string& filename);
    void ensureDataDirectory();
};

#endif
