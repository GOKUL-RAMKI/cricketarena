#include "../include/FileManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sys/stat.h>

using namespace std;

// ANSI Color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Constructor
FileManager::FileManager() 
    : playerStatsFile("../data/player_stats.csv"),
      matchStateFile("../data/match_state.txt"),
      rankingsFile("../data/rankings.txt")
{
    ensureDataDirectory();
}

FileManager::FileManager(const string& statsFile, const string& stateFile, const string& ranksFile) 
    : playerStatsFile(statsFile), matchStateFile(stateFile), rankingsFile(ranksFile)
{
    ensureDataDirectory();
}

// Player stats management
void FileManager::savePlayerStats(const vector<Player>& allPlayers) 
{
    ofstream out(playerStatsFile);
    if (!out)
    {
        cerr << RED << "Error: Cannot write to " << playerStatsFile << RESET << endl;
        return;
    }
    
    // Header
    out << "Name,TotalRuns,TotalBalls,Fours,Sixes,TimesOut,Overs,BallsBowled,RunsGiven,Wickets\n";
    
    for (size_t i = 0; i < allPlayers.size(); i++)
    {
        out << allPlayers[i].toCSV() << "\n";
    }
    
    out.close();
}

vector<Player> FileManager::loadPlayerStats() 
{
    vector<Player> players;
    ifstream in(playerStatsFile);
    
    if (!in)
    {
        return players;  // Return empty if file doesn't exist
    }
    
    string line;
    getline(in, line);  // Skip header
    
    while (getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }
        Player p;
        p.fromCSV(line);
        players.push_back(p);
    }
    
    in.close();
    return players;
}

void FileManager::updatePlayerInDatabase(const Player& player) 
{
    vector<Player> allPlayers = loadPlayerStats();
    bool found = false;
    
    for (size_t i = 0; i < allPlayers.size(); i++)
    {
        if (allPlayers[i].getName() == player.getName())
        {
            allPlayers[i] = player;
            found = true;
            break;
        }
    }
    
    if (!found)
    {
        allPlayers.push_back(player);
    }
    
    savePlayerStats(allPlayers);
}

Player FileManager::getPlayerFromDatabase(const string& playerName) 
{
    vector<Player> allPlayers = loadPlayerStats();
    
    for (size_t i = 0; i < allPlayers.size(); i++)
    {
        if (allPlayers[i].getName() == playerName)
        {
            return allPlayers[i];
        }
    }
    
    return Player();  // Return empty player if not found
}

// Rankings generation
void FileManager::generateRankings(const vector<Player>& allPlayers) 
{
    ofstream out(rankingsFile);
    if (!out)
    {
        cerr << RED << "Error: Cannot write rankings" << RESET << endl;
        return;
    }
    
    // Create copies for sorting
    vector<Player> battingByRuns = allPlayers;
    vector<Player> battingByAvg = allPlayers;
    vector<Player> battingBySR = allPlayers;
    vector<Player> bowlingByWickets = allPlayers;
    vector<Player> bowlingByAvg = allPlayers;
    vector<Player> bowlingByEcon = allPlayers;
    
    // Sort by runs (descending)
    for (size_t i = 0; i < battingByRuns.size(); i++)
    {
        for (size_t j = i + 1; j < battingByRuns.size(); j++)
        {
            if (battingByRuns[j].getTotalRuns() > battingByRuns[i].getTotalRuns())
            {
                Player temp = battingByRuns[i];
                battingByRuns[i] = battingByRuns[j];
                battingByRuns[j] = temp;
            }
        }
    }
    
    // Sort by batting average (descending)
    for (size_t i = 0; i < battingByAvg.size(); i++)
    {
        for (size_t j = i + 1; j < battingByAvg.size(); j++)
        {
            if (battingByAvg[j].getBattingAverage() > battingByAvg[i].getBattingAverage())
            {
                Player temp = battingByAvg[i];
                battingByAvg[i] = battingByAvg[j];
                battingByAvg[j] = temp;
            }
        }
    }
    
    // Sort by strike rate (descending)
    for (size_t i = 0; i < battingBySR.size(); i++)
    {
        for (size_t j = i + 1; j < battingBySR.size(); j++)
        {
            if (battingBySR[j].getStrikeRate() > battingBySR[i].getStrikeRate())
            {
                Player temp = battingBySR[i];
                battingBySR[i] = battingBySR[j];
                battingBySR[j] = temp;
            }
        }
    }
    
    // Sort by wickets (descending)
    for (size_t i = 0; i < bowlingByWickets.size(); i++)
    {
        for (size_t j = i + 1; j < bowlingByWickets.size(); j++)
        {
            if (bowlingByWickets[j].getWicketsTaken() > bowlingByWickets[i].getWicketsTaken())
            {
                Player temp = bowlingByWickets[i];
                bowlingByWickets[i] = bowlingByWickets[j];
                bowlingByWickets[j] = temp;
            }
        }
    }
    
    // Sort by bowling average (ascending, but only those with wickets)
    for (size_t i = 0; i < bowlingByAvg.size(); i++)
    {
        for (size_t j = i + 1; j < bowlingByAvg.size(); j++)
        {
            if (bowlingByAvg[j].getWicketsTaken() > 0 && 
                (bowlingByAvg[i].getWicketsTaken() == 0 || 
                 bowlingByAvg[j].getBowlingAverage() < bowlingByAvg[i].getBowlingAverage()))
            {
                Player temp = bowlingByAvg[i];
                bowlingByAvg[i] = bowlingByAvg[j];
                bowlingByAvg[j] = temp;
            }
        }
    }
    
    // Sort by economy (ascending)
    for (size_t i = 0; i < bowlingByEcon.size(); i++)
    {
        for (size_t j = i + 1; j < bowlingByEcon.size(); j++)
        {
            if (bowlingByEcon[j].getTotalOvers() > 0 &&
                (bowlingByEcon[i].getTotalOvers() == 0 ||
                 bowlingByEcon[j].getBowlingEconomy() < bowlingByEcon[i].getBowlingEconomy()))
            {
                Player temp = bowlingByEcon[i];
                bowlingByEcon[i] = bowlingByEcon[j];
                bowlingByEcon[j] = temp;
            }
        }
    }
    
    // Write rankings
    out << "========================================\n";
    out << "         CRICKET PLAYER RANKINGS        \n";
    out << "========================================\n\n";
    
    // Batting Rankings - Most Runs
    out << "TOP BATSMEN BY RUNS:\n";
    out << "----------------------------------------\n";
    out << left << setw(20) << "Player" 
        << right << setw(8) << "Runs" 
        << setw(8) << "Avg" 
        << setw(8) << "SR" << "\n";
    out << "----------------------------------------\n";
    
    for (size_t i = 0; i < battingByRuns.size() && i < 10; i++)
    {
        if (battingByRuns[i].getTotalRuns() > 0)
        {
            out << left << setw(20) << battingByRuns[i].getName()
                << right << setw(8) << battingByRuns[i].getTotalRuns()
                << setw(8) << fixed << setprecision(2) 
                << battingByRuns[i].getBattingAverage()
                << setw(8) << fixed << setprecision(2) 
                << battingByRuns[i].getStrikeRate() << "\n";
        }
    }
    
    out << "\n\nTOP BATSMEN BY AVERAGE:\n";
    out << "----------------------------------------\n";
    out << left << setw(20) << "Player" 
        << right << setw(8) << "Avg" 
        << setw(8) << "Runs" 
        << setw(8) << "Outs" << "\n";
    out << "----------------------------------------\n";
    
    for (size_t i = 0; i < battingByAvg.size() && i < 10; i++)
    {
        if (battingByAvg[i].getTimesOut() > 0 || battingByAvg[i].getTotalRuns() > 0)  // Show players who have batted
        {
            out << left << setw(20) << battingByAvg[i].getName()
                << right << setw(8) << fixed << setprecision(2)
                << battingByAvg[i].getBattingAverage()
                << setw(8) << battingByAvg[i].getTotalRuns()
                << setw(8) << battingByAvg[i].getTimesOut() << "\n";
        }
    }
    
    out << "\n\nTOP BATSMEN BY STRIKE RATE:\n";
    out << "----------------------------------------\n";
    out << left << setw(20) << "Player" 
        << right << setw(8) << "SR" 
        << setw(8) << "Runs" 
        << setw(8) << "Balls" << "\n";
    out << "----------------------------------------\n";
    
    for (size_t i = 0; i < battingBySR.size() && i < 10; i++)
    {
        if (battingBySR[i].getTotalBalls() > 0)  // Show players who have batted
        {
            out << left << setw(20) << battingBySR[i].getName()
                << right << setw(8) << fixed << setprecision(2)
                << battingBySR[i].getStrikeRate()
                << setw(8) << battingBySR[i].getTotalRuns()
                << setw(8) << battingBySR[i].getTotalBalls() << "\n";
        }
    }
    
    // Bowling Rankings
    out << "\n\nTOP BOWLERS BY WICKETS:\n";
    out << "----------------------------------------\n";
    out << left << setw(20) << "Player" 
        << right << setw(8) << "Wickets" 
        << setw(8) << "Avg" 
        << setw(8) << "Econ" << "\n";
    out << "----------------------------------------\n";
    
    for (size_t i = 0; i < bowlingByWickets.size() && i < 10; i++)
    {
        if (bowlingByWickets[i].getWicketsTaken() > 0)
        {
            out << left << setw(20) << bowlingByWickets[i].getName()
                << right << setw(8) << bowlingByWickets[i].getWicketsTaken()
                << setw(8) << fixed << setprecision(2)
                << bowlingByWickets[i].getBowlingAverage()
                << setw(8) << fixed << setprecision(2)
                << bowlingByWickets[i].getBowlingEconomy() << "\n";
        }
    }
    
    out << "\n\nTOP BOWLERS BY AVERAGE:\n";
    out << "----------------------------------------\n";
    out << left << setw(20) << "Player" 
        << right << setw(8) << "Avg" 
        << setw(8) << "Wickets" 
        << setw(8) << "Runs" << "\n";
    out << "----------------------------------------\n";
    
    for (size_t i = 0; i < bowlingByAvg.size() && i < 10; i++)
    {
        if (bowlingByAvg[i].getWicketsTaken() > 0)  // Show bowlers with wickets
        {
            out << left << setw(20) << bowlingByAvg[i].getName()
                << right << setw(8) << fixed << setprecision(2)
                << bowlingByAvg[i].getBowlingAverage()
                << setw(8) << bowlingByAvg[i].getWicketsTaken()
                << setw(8) << bowlingByAvg[i].getRunsGiven() << "\n";
        }
    }
    
    out << "\n\nTOP BOWLERS BY ECONOMY:\n";
    out << "----------------------------------------\n";
    out << left << setw(20) << "Player" 
        << right << setw(8) << "Econ" 
        << setw(8) << "Overs" 
        << setw(8) << "Runs" << "\n";
    out << "----------------------------------------\n";
    
    for (size_t i = 0; i < bowlingByEcon.size() && i < 10; i++)
    {
        if (bowlingByEcon[i].getTotalOvers() > 0)  // Show bowlers who have bowled
        {
            out << left << setw(20) << bowlingByEcon[i].getName()
                << right << setw(8) << fixed << setprecision(2)
                << bowlingByEcon[i].getBowlingEconomy()
                << setw(8) << bowlingByEcon[i].getTotalOvers()
                << setw(8) << bowlingByEcon[i].getRunsGiven() << "\n";
        }
    }
    
    out.close();
}

// Utility
bool FileManager::fileExists(const string& filename) 
{
    ifstream file(filename);
    return file.good();
}

void FileManager::ensureDataDirectory() 
{
    struct stat info;
    if (stat("../data", &info) != 0)
    {
        // Directory doesn't exist, create it
#ifdef _WIN32
        system("mkdir data");
#else
        system("mkdir -p data");
#endif
    }
}
