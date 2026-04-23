#include "../include/Match.h"
#include <fstream>
#include <iostream>

using namespace std;

// Constructors — pass totalOvers up to BaseMatch
Match::Match() 
    : BaseMatch(),   // <<< delegates to BaseMatch default constructor
      currentInnings(0), battingTeam(nullptr), bowlingTeam(nullptr),
      strikerIndex(0), nonStrikerIndex(1), nextBatsmanIndex(2), currentBowlerIndex(-1),
      targetRuns(0), lastDeliveryWasWicket(false)
{
}

Match::Match(const Team& tA, const Team& tB, int overs) 
    : BaseMatch(overs),   // <<< delegates totalOvers to BaseMatch
      teamA(tA), teamB(tB), currentInnings(0), 
      battingTeam(nullptr), bowlingTeam(nullptr),
      strikerIndex(0), nonStrikerIndex(1), nextBatsmanIndex(2), currentBowlerIndex(-1),
      targetRuns(0), lastDeliveryWasWicket(false)
{
}

// Getters
Team& Match::getTeamA() 
{
    return teamA;
}

Team& Match::getTeamB() 
{
    return teamB;
}

const Team& Match::getTeamA() const 
{
    return teamA;
}

const Team& Match::getTeamB() const 
{
    return teamB;
}

// getTotalOvers() is inherited from BaseMatch

int Match::getCurrentInnings() const 
{
    return currentInnings;
}

Team* Match::getBattingTeam() 
{
    return battingTeam;
}

Team* Match::getBowlingTeam() 
{
    return bowlingTeam;
}

int Match::getStrikerIndex() const 
{
    return strikerIndex;
}

int Match::getNonStrikerIndex() const 
{
    return nonStrikerIndex;
}

int Match::getNextBatsmanIndex() const 
{
    return nextBatsmanIndex;
}

int Match::getCurrentBowlerIndex() const 
{
    return currentBowlerIndex;
}

int Match::getTargetRuns() const 
{
    return targetRuns;
}

// isMatchComplete() is inherited from BaseMatch

bool Match::isCurrentInningsComplete() const
{
    if (currentInnings == 0 || battingTeam == nullptr)
    {
        return false;
    }
    return battingTeam->getTotalWickets() >= 10 || battingTeam->getOversDone() >= totalOvers;
}

// getResult() is inherited from BaseMatch

// Setters
// setMatchComplete() and setResult() are inherited from BaseMatch

void Match::setCurrentInnings(int innings) 
{
    currentInnings = innings;
}

void Match::setStrikerIndex(int index) 
{
    strikerIndex = index;
}

void Match::setNonStrikerIndex(int index) 
{
    nonStrikerIndex = index;
}

void Match::setNextBatsmanIndex(int index) 
{
    nextBatsmanIndex = index;
}

void Match::setCurrentBowlerIndex(int index) 
{
    currentBowlerIndex = index;
}

void Match::setTargetRuns(int target) 
{
    targetRuns = target;
}

// Match operations
void Match::startInnings(int inningsNumber) 
{
    currentInnings = inningsNumber;
    matchComplete = false;   // BaseMatch protected member
    result = "";             // BaseMatch protected member
    
    if (inningsNumber == 1)
    {
        battingTeam = &teamA;
        bowlingTeam = &teamB;
        targetRuns = 0;
    }
    else
    {
        battingTeam = &teamB;
        bowlingTeam = &teamA;
        targetRuns = teamA.getTotalRuns();
    }
    
    battingTeam->resetInningsStats();
    strikerIndex = 0;
    nonStrikerIndex = 1;
    nextBatsmanIndex = 2;
    currentBowlerIndex = -1;
}

void Match::selectOpeningBatsmen(int striker, int nonStriker) 
{
    strikerIndex = striker;
    nonStrikerIndex = nonStriker;
    nextBatsmanIndex = 0;
    
    for (int i = 0; i < battingTeam->getPlayerCount(); i++)
    {
        if (i != strikerIndex && i != nonStrikerIndex)
        {
            nextBatsmanIndex = i;
            break;
        }
    }
}

void Match::selectBowler(const string& bowlerName) 
{
    int bowlerIdx = bowlingTeam->findPlayerByName(bowlerName);
    if (bowlerIdx != -1)
    {
        currentBowlerIndex = bowlerIdx;
    }
}

void Match::playDelivery(int runs) 
{
    if (matchComplete || currentBowlerIndex == -1)
    {
        return;
    }
    
    lastDeliveryWasWicket = (runs == -1);
    
    if (runs == -1)
    {
        handleWicket();
    }
    else
    {
        bool isFour = (runs == 4);
        bool isSix = (runs == 6);
        battingTeam->getPlayer(strikerIndex).updateBattingStats(runs, isFour, isSix);
        battingTeam->addRuns(runs);
        bowlingTeam->getPlayer(currentBowlerIndex).updateBowlingStats(runs);
        if (runs % 2 == 1)
        {
            swapBatsmen();
        }
    }
    
    bowlingTeam->getPlayer(currentBowlerIndex).incrementBowlerBall();
    battingTeam->incrementBall();
    
    if (battingTeam->getBallsInCurrentOver() == 0)
    {
        completeOver();
    }
    
    checkMatchEnd();
}

void Match::handleWicket() 
{
    battingTeam->getPlayer(strikerIndex).updateWicket();
    battingTeam->addWicket();
    bowlingTeam->getPlayer(currentBowlerIndex).updateBowlerWicket();
    
    if (battingTeam->getTotalWickets() < 10 && nextBatsmanIndex < battingTeam->getPlayerCount())
    {
        for (int i = nextBatsmanIndex; i < battingTeam->getPlayerCount(); i++)
        {
            if (!battingTeam->getPlayer(i).isCurrentOut())
            {
                strikerIndex = i;
                nextBatsmanIndex = i + 1;
                break;
            }
        }
    }
}

void Match::swapBatsmen() 
{
    int temp = strikerIndex;
    strikerIndex = nonStrikerIndex;
    nonStrikerIndex = temp;
}

void Match::completeOver() 
{
    bowlingTeam->getPlayer(currentBowlerIndex).completeOver();
    if (!lastDeliveryWasWicket)
    {
        swapBatsmen();
    }
    currentBowlerIndex = -1;
    lastDeliveryWasWicket = false;
}

void Match::checkMatchEnd() 
{
    if (battingTeam->getTotalWickets() >= 10)
    {
        if (currentInnings == 2)
        {
            matchComplete = true;
            calculateResult();
        }
        else
        {
            matchComplete = false;
        }
        return;
    }
    
    if (battingTeam->getOversDone() >= totalOvers)
    {
        if (currentInnings == 2)
        {
            matchComplete = true;
            calculateResult();
        }
        else
        {
            matchComplete = false;
        }
        return;
    }
    
    if (currentInnings == 2 && battingTeam->getTotalRuns() > targetRuns)
    {
        matchComplete = true;
        calculateResult();
    }
}

// Override of pure virtual BaseMatch::calculateResult()
void Match::calculateResult() 
{
    if (currentInnings == 1)
    {
        result = "First innings complete";
        return;
    }
    
    if (teamA.getTotalRuns() > teamB.getTotalRuns())
    {
        result = teamA.getTeamName() + " won by " + 
                 to_string(teamA.getTotalRuns() - teamB.getTotalRuns()) + " runs";
    }
    else if (teamB.getTotalRuns() > teamA.getTotalRuns())
    {
        result = teamB.getTeamName() + " won by " + 
                 to_string(10 - teamB.getTotalWickets()) + " wickets";
    }
    else
    {
        result = "Match tied";
    }
}

// Override of pure virtual BaseMatch::saveMatchState()
void Match::saveMatchState(const string& filename) 
{
    ofstream out(filename);
    if (!out)
    {
        return;
    }
    
    out << totalOvers << "\n";
    out << currentInnings << "\n";
    out << strikerIndex << " " << nonStrikerIndex << " " << nextBatsmanIndex << "\n";
    out << currentBowlerIndex << "\n";
    out << targetRuns << "\n";
    out << matchComplete << "\n";
    out << result << "\n";
    
    teamA.saveToFile(out);
    teamB.saveToFile(out);
    
    out.close();
}

// Override of pure virtual BaseMatch::loadMatchState()
void Match::loadMatchState(const string& filename) 
{
    ifstream in(filename);
    if (!in)
    {
        return;
    }
    
    in >> totalOvers;
    in >> currentInnings;
    in >> strikerIndex >> nonStrikerIndex >> nextBatsmanIndex;
    in >> currentBowlerIndex;
    in >> targetRuns;
    in >> matchComplete;
    in.ignore();
    getline(in, result);
    
    teamA.loadFromFile(in);
    teamB.loadFromFile(in);
    
    if (currentInnings == 1)
    {
        battingTeam = &teamA;
        bowlingTeam = &teamB;
    }
    else
    {
        battingTeam = &teamB;
        bowlingTeam = &teamA;
    }
    
    in.close();
}
