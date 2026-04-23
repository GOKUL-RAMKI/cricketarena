#include "../include/Player.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// Constructors — pass name up to Person base class
Player::Player() 
    : Person(),
      totalRuns(0), totalBalls(0), totalFours(0), totalSixes(0), 
      timesOut(0), totalOvers(0), totalBallsBowled(0), runsGiven(0), 
      wicketsTaken(0), currentRuns(0), currentBalls(0), currentFours(0), 
      currentSixes(0), currentOut(false), currentOvers(0), currentBallsInOver(0),
      currentRunsGiven(0), currentWickets(0)
{
}

Player::Player(string playerName) 
    : Person(playerName),   // <<< passes name to Person base constructor
      totalRuns(0), totalBalls(0), totalFours(0), totalSixes(0), 
      timesOut(0), totalOvers(0), totalBallsBowled(0), runsGiven(0), 
      wicketsTaken(0), currentRuns(0), currentBalls(0), currentFours(0), 
      currentSixes(0), currentOut(false), currentOvers(0), currentBallsInOver(0),
      currentRunsGiven(0), currentWickets(0)
{
}

// Override of pure virtual Person::displayInfo()
void Player::displayInfo() const
{
    cout << "Player: " << name
         << " | Runs: " << totalRuns
         << " | Wickets: " << wicketsTaken << "\n";
}

// Getters (getName() is inherited from Person)
int Player::getTotalRuns() const 
{
    return totalRuns;
}

int Player::getTotalBalls() const 
{
    return totalBalls;
}

int Player::getTotalFours() const 
{
    return totalFours;
}

int Player::getTotalSixes() const 
{
    return totalSixes;
}

int Player::getTimesOut() const 
{
    return timesOut;
}

int Player::getTotalOvers() const 
{
    return totalOvers;
}

int Player::getTotalBallsBowled() const 
{
    return totalBallsBowled;
}

int Player::getRunsGiven() const 
{
    return runsGiven;
}

int Player::getWicketsTaken() const 
{
    return wicketsTaken;
}

// Current match getters
int Player::getCurrentRuns() const 
{
    return currentRuns;
}

int Player::getCurrentBalls() const 
{
    return currentBalls;
}

int Player::getCurrentFours() const 
{
    return currentFours;
}

int Player::getCurrentSixes() const 
{
    return currentSixes;
}

bool Player::isCurrentOut() const 
{
    return currentOut;
}

int Player::getCurrentOvers() const 
{
    return currentOvers;
}

int Player::getCurrentBallsInOver() const 
{
    return currentBallsInOver;
}

int Player::getCurrentRunsGiven() const 
{
    return currentRunsGiven;
}

int Player::getCurrentWickets() const 
{
    return currentWickets;
}

// Calculated stats
double Player::getBattingAverage() const 
{
    if (timesOut == 0)
    {
        return totalRuns;
    }
    return static_cast<double>(totalRuns) / timesOut;
}

double Player::getStrikeRate() const 
{
    if (totalBalls == 0)
    {
        return 0.0;
    }
    return (static_cast<double>(totalRuns) / totalBalls) * 100.0;
}

double Player::getBowlingAverage() const 
{
    if (wicketsTaken == 0)
    {
        return runsGiven;
    }
    return static_cast<double>(runsGiven) / wicketsTaken;
}

double Player::getBowlingEconomy() const 
{
    double totalOversDecimal = totalOvers + (totalBallsBowled / 6.0);
    if (totalOversDecimal == 0)
    {
        return 0.0;
    }
    return runsGiven / totalOversDecimal;
}

// Update methods
void Player::updateBattingStats(int runs, bool isFour, bool isSix) 
{
    currentRuns += runs;
    currentBalls++;
    
    if (isFour)
    {
        currentFours++;
    }
    if (isSix)
    {
        currentSixes++;
    }
}

void Player::updateWicket() 
{
    currentOut = true;
}

void Player::updateBowlingStats(int runs) 
{
    currentRunsGiven += runs;
}

void Player::updateBowlerWicket() 
{
    currentWickets++;
}

void Player::incrementBowlerBall() 
{
    currentBallsInOver++;
}

void Player::completeOver() 
{
    currentOvers++;
    currentBallsInOver = 0;
}

// Reset current match stats
void Player::resetCurrentMatchStats() 
{
    currentRuns = 0;
    currentBalls = 0;
    currentFours = 0;
    currentSixes = 0;
    currentOut = false;
    currentOvers = 0;
    currentBallsInOver = 0;
    currentRunsGiven = 0;
    currentWickets = 0;
}

// Commit current match stats to total
void Player::commitMatchStats() 
{
    totalRuns += currentRuns;
    totalBalls += currentBalls;
    totalFours += currentFours;
    totalSixes += currentSixes;
    
    if (currentOut)
    {
        timesOut++;
    }
    
    totalOvers += currentOvers;
    totalBallsBowled += currentBallsInOver;
    runsGiven += currentRunsGiven;
    wicketsTaken += currentWickets;
}

// CSV format: name,totalRuns,totalBalls,totalFours,totalSixes,timesOut,totalOvers,totalBallsBowled,runsGiven,wicketsTaken
string Player::toCSV() const 
{
    ostringstream oss;
    oss << name << "," 
        << totalRuns << "," 
        << totalBalls << "," 
        << totalFours << "," 
        << totalSixes << "," 
        << timesOut << ","
        << totalOvers << ","
        << totalBallsBowled << ","
        << runsGiven << ","
        << wicketsTaken;
    return oss.str();
}

void Player::fromCSV(const string& line) 
{
    istringstream iss(line);
    string token;
    
    getline(iss, name, ',');   // 'name' accessed via Person's protected member
    getline(iss, token, ','); totalRuns = stoi(token);
    getline(iss, token, ','); totalBalls = stoi(token);
    getline(iss, token, ','); totalFours = stoi(token);
    getline(iss, token, ','); totalSixes = stoi(token);
    getline(iss, token, ','); timesOut = stoi(token);
    getline(iss, token, ','); totalOvers = stoi(token);
    getline(iss, token, ','); totalBallsBowled = stoi(token);
    getline(iss, token, ','); runsGiven = stoi(token);
    getline(iss, token, ','); wicketsTaken = stoi(token);
}

void Player::saveToFile(ofstream& out) const 
{
    out << name << "\n";
    out << currentRuns << " " << currentBalls << " " << currentFours << " " 
        << currentSixes << " " << currentOut << "\n";
    out << currentOvers << " " << currentBallsInOver << " " 
        << currentRunsGiven << " " << currentWickets << "\n";
}

void Player::loadFromFile(ifstream& in) 
{
    getline(in, name);   // 'name' is Person's protected member
    in >> currentRuns >> currentBalls >> currentFours >> currentSixes >> currentOut;
    in >> currentOvers >> currentBallsInOver >> currentRunsGiven >> currentWickets;
    in.ignore();
}
