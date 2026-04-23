#include "../include/Team.h"

using namespace std;

// Constructors
Team::Team() 
    : teamName(""), totalRuns(0), totalWickets(0), oversDone(0), ballsInCurrentOver(0)
{
}

Team::Team(string name) 
    : teamName(name), totalRuns(0), totalWickets(0), oversDone(0), ballsInCurrentOver(0)
{
}

// Getters
string Team::getTeamName() const 
{
    return teamName;
}

int Team::getTotalRuns() const 
{
    return totalRuns;
}

int Team::getTotalWickets() const 
{
    return totalWickets;
}

int Team::getOversDone() const 
{
    return oversDone;
}

int Team::getBallsInCurrentOver() const 
{
    return ballsInCurrentOver;
}

int Team::getPlayerCount() const 
{
    return players.size();
}

Player& Team::getPlayer(int index) 
{
    return players[index];
}

const Player& Team::getPlayer(int index) const 
{
    return players[index];
}

// Setters
void Team::setTotalRuns(int runs) 
{
    totalRuns = runs;
}

void Team::setTotalWickets(int wickets) 
{
    totalWickets = wickets;
}

void Team::setOversDone(int overs) 
{
    oversDone = overs;
}

void Team::setBallsInCurrentOver(int balls) 
{
    ballsInCurrentOver = balls;
}

// Team management
void Team::addPlayer(const Player& player) 
{
    players.push_back(player);
}

int Team::findPlayerByName(const string& name) const 
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].getName() == name)
        {
            return i;
        }
    }
    return -1;
}

// Match operations
void Team::addRuns(int runs) 
{
    totalRuns += runs;
}

void Team::addWicket() 
{
    totalWickets++;
}

void Team::incrementBall() 
{
    ballsInCurrentOver++;
    if (ballsInCurrentOver == 6)
    {
        oversDone++;
        ballsInCurrentOver = 0;
    }
}

void Team::resetInningsStats() 
{
    totalRuns = 0;
    totalWickets = 0;
    oversDone = 0;
    ballsInCurrentOver = 0;
    
    for (size_t i = 0; i < players.size(); i++)
    {
        players[i].resetCurrentMatchStats();
    }
}

// File I/O
void Team::saveToFile(ofstream& out) const 
{
    out << teamName << "\n";
    out << totalRuns << " " << totalWickets << " " << oversDone << " " << ballsInCurrentOver << "\n";
    out << players.size() << "\n";
    
    for (size_t i = 0; i < players.size(); i++)
    {
        players[i].saveToFile(out);
    }
}

void Team::loadFromFile(ifstream& in) 
{
    getline(in, teamName);
    in >> totalRuns >> totalWickets >> oversDone >> ballsInCurrentOver;
    in.ignore();
    
    int playerCount;
    in >> playerCount;
    in.ignore();
    
    players.clear();
    for (int i = 0; i < playerCount; i++)
    {
        Player p;
        p.loadFromFile(in);
        players.push_back(p);
    }
}
