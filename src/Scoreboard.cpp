#include "../include/Scoreboard.h"
#include "../include/DisplayBase.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// ANSI Color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

static string formatRunsBalls(int runs, int balls)
{
    ostringstream oss;
    oss << runs << "(" << setw(2) << balls << ")";
    return oss.str();
}

static void printColoredField(const string& text, const string& color, int width)
{
    cout << setw(0);
    cout << color << text << RESET;
    int pad = width - static_cast<int>(text.length());
    if (pad > 0)
    {
        cout << string(pad, ' ');
    }
}

// Override of DisplayBase::clear() — instance method
void Scoreboard::clear() const
{
    Scoreboard::clearScreen();
}

// Override of DisplayBase::showMenu() — instance method
void Scoreboard::showMenu(const vector<string>& options) const
{
    Scoreboard::displayMenu(options);
}

void Scoreboard::clearScreen() 
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Scoreboard::displayLiveScore(const Match& match) 
{
    clearScreen();
    
    const Team* batting = nullptr;
    const Team* bowling = nullptr;
    
    if (match.getCurrentInnings() == 1)
    {
        batting = &match.getTeamA();
        bowling = &match.getTeamB();
    }
    else
    {
        batting = &match.getTeamB();
        bowling = &match.getTeamA();
    }
    
    cout << BOLD << CYAN << "=====================================\n";
    cout << BOLD << WHITE << "   LIVE SCOREBOARD - Innings " << match.getCurrentInnings() << "\n" << RESET;
    cout << BOLD << CYAN << "=====================================\n" << RESET << "\n";
    cout << BOLD << GREEN << batting->getTeamName() << RESET << " vs " << BOLD << YELLOW << bowling->getTeamName() << RESET << "\n";
    cout << BOLD << "Score: " << RESET << batting->getTotalRuns() << "/" << batting->getTotalWickets() 
              << " in " << batting->getOversDone() << "." << batting->getBallsInCurrentOver() 
              << " overs\n";
    
    if (match.getCurrentInnings() == 2)
    {
        cout << BOLD << GREEN << "Target: " << RESET << match.getTargetRuns() + 1 << " runs\n";
        int required = (match.getTargetRuns() + 1) - batting->getTotalRuns();
        int ballsLeft = (match.getTotalOvers() - batting->getOversDone()) * 6 - batting->getBallsInCurrentOver();
        if (required > 0 && ballsLeft > 0)
        {
            cout << BOLD << "Required: " << RESET << required << " runs from " << ballsLeft << " balls\n";
        }
    }
    
    cout << BOLD << CYAN << "-------------------------------------\n" << RESET;
    cout << BOLD << GREEN << "BATSMEN AT CREASE:" << RESET << "\n";
    cout << BOLD << CYAN << "-------------------------------------\n" << RESET;
    cout << left << setw(15) << "Batsman" 
              << right << setw(8) << "R(B)" 
              << setw(6) << "4s" 
              << setw(6) << "6s" << "\n";
    cout << "-------------------------------------\n";
    
    int striker = match.getStrikerIndex();
    int nonStriker = match.getNonStrikerIndex();
    
    if (striker >= 0 && striker < batting->getPlayerCount())
    {
        const Player& p = batting->getPlayer(striker);
        printColoredField(p.getName() + "*", RED, 15);
        cout << right << setw(8) << formatRunsBalls(p.getCurrentRuns(), p.getCurrentBalls())
                  << setw(6) << p.getCurrentFours()
                  << setw(6) << p.getCurrentSixes() << "\n";
    }
    
    if (nonStriker >= 0 && nonStriker < batting->getPlayerCount())
    {
        const Player& p = batting->getPlayer(nonStriker);
        cout << left << setw(15) << p.getName()
                  << right << setw(8) << formatRunsBalls(p.getCurrentRuns(), p.getCurrentBalls())
                  << setw(6) << p.getCurrentFours()
                  << setw(6) << p.getCurrentSixes() << "\n";
    }
    
    cout << "-------------------------------------\n";
    
    int bowlerIdx = match.getCurrentBowlerIndex();
    if (bowlerIdx >= 0 && bowlerIdx < bowling->getPlayerCount())
    {
        const Player& bowler = bowling->getPlayer(bowlerIdx);
        cout << BOLD << YELLOW << "CURRENT BOWLER: " << RESET << bowler.getName() << "\n";
        cout << "O: " << bowler.getCurrentOvers() << "." << bowler.getCurrentBallsInOver()
                  << "  R: " << bowler.getCurrentRunsGiven()
                  << "  W: " << bowler.getCurrentWickets() << "\n";
    }
    
    cout << BOLD << CYAN << "=====================================" << RESET << "\n";
}

void Scoreboard::displayFullScorecard(const Match& match) 
{
    clearScreen();
    
    cout << BOLD << CYAN << "========================================\n";
    cout << BOLD << WHITE << "          FULL SCORECARD\n" << RESET;
    cout << BOLD << CYAN << "========================================" << RESET << "\n\n";
    
    // Display both innings
    const Team* teams[2] = {&match.getTeamA(), &match.getTeamB()};
    
    for (int t = 0; t < 2; t++)
    {
        cout << BOLD << GREEN << teams[t]->getTeamName() << RESET << " Innings: " 
                  << BOLD << teams[t]->getTotalRuns() << "/" << teams[t]->getTotalWickets() << RESET
                  << " in " << teams[t]->getOversDone() << "." << teams[t]->getBallsInCurrentOver() 
                  << " overs\n";
        cout << "----------------------------------------\n";
        cout << left << setw(18) << "Batsman" 
                  << right << setw(8) << "R(B)" 
                  << setw(6) << "4s" 
                  << setw(6) << "6s" 
                  << setw(10) << "Status" << "\n";
        cout << "----------------------------------------\n";
        
        for (int i = 0; i < teams[t]->getPlayerCount(); i++)
        {
            const Player& p = teams[t]->getPlayer(i);
            cout << left << setw(18) << p.getName()
                      << right << setw(8) << formatRunsBalls(p.getCurrentRuns(), p.getCurrentBalls())
                      << setw(6) << p.getCurrentFours()
                      << setw(6) << p.getCurrentSixes()
                      << " ";
            cout << (p.isCurrentOut() ? RED : GREEN)
                      << (p.isCurrentOut() ? "OUT" : "NOT OUT")
                      << RESET << "\n";
        }
        
        cout << "\n" << BOLD << "Bowling:" << RESET << "\n";
        cout << "----------------------------------------\n";
        cout << left << setw(18) << "Bowler" 
                  << right << setw(6) << "O" 
                  << setw(6) << "R" 
                  << setw(6) << "W" << "\n";
        cout << "----------------------------------------\n";
        
        const Team* oppTeam = (t == 0) ? teams[1] : teams[0];
        for (int i = 0; i < oppTeam->getPlayerCount(); i++)
        {
            const Player& p = oppTeam->getPlayer(i);
            if (p.getCurrentOvers() > 0 || p.getCurrentBallsInOver() > 0)
            {
                cout << left << setw(18) << p.getName()
                          << right << setw(3) << p.getCurrentOvers()
                          << "." << p.getCurrentBallsInOver()
                          << setw(6) << p.getCurrentRunsGiven()
                          << setw(6) << BOLD << p.getCurrentWickets() << RESET << "\n";
            }
        }
        cout << "\n\n";
    }
    
    if (match.isMatchComplete())
    {
        cout << BOLD << CYAN << "========================================\n";
        cout << "RESULT: " << YELLOW << match.getResult() << RESET << "\n";
        cout << BOLD << CYAN << "========================================" << RESET << "\n";
    }
}

void Scoreboard::displayPlayerStats(const Player& player) 
{
    cout << BOLD << CYAN << "\n========================================\n";
    cout << "  PLAYER STATS: " << GREEN << player.getName() << RESET << BOLD << CYAN << "\n";
    cout << BOLD << CYAN << "========================================" << RESET << "\n";
    
    cout << BOLD << YELLOW << "\nBATTING:" << RESET << "\n";
    cout << "----------------------------------------\n";
    cout << BOLD << "Total Runs:" << RESET << "       " << player.getTotalRuns() << "\n";
    cout << BOLD << "Total Balls:" << RESET << "      " << player.getTotalBalls() << "\n";
    cout << BOLD << "Fours:" << RESET << "            " << player.getTotalFours() << "\n";
    cout << BOLD << "Sixes:" << RESET << "            " << player.getTotalSixes() << "\n";
    cout << BOLD << "Times Out:" << RESET << "        " << player.getTimesOut() << "\n";
    cout << BOLD << "Batting Average:" << RESET << "  " << fixed << setprecision(2) 
              << player.getBattingAverage() << "\n";
    cout << BOLD << "Strike Rate:" << RESET << "      " << fixed << setprecision(2) 
              << player.getStrikeRate() << "\n";
    
    cout << BOLD << CYAN << "\nBOWLING:" << RESET << "\n";
    cout << "----------------------------------------\n";
    cout << BOLD << "Overs Bowled:" << RESET << "     " << player.getTotalOvers() << "." << player.getTotalBallsBowled() << "\n";
    cout << BOLD << "Runs Given:" << RESET << "       " << player.getRunsGiven() << "\n";
    cout << BOLD << "Wickets Taken:" << RESET << "    " << BOLD << RED << player.getWicketsTaken() << RESET << "\n";
    cout << BOLD << "Bowling Average:" << RESET << "  " << fixed << setprecision(2) 
              << player.getBowlingAverage() << "\n";
    cout << BOLD << "Economy Rate:" << RESET << "     " << fixed << setprecision(2) 
              << player.getBowlingEconomy() << "\n";
    cout << BOLD << CYAN << "========================================" << RESET << "\n";
}

void Scoreboard::displayTeamLineup(const Team& team) 
{
    cout << GREEN << "\n" << team.getTeamName() << " - SQUAD:" << RESET << "\n";
    cout << "----------------------------------------\n";
    
    for (int i = 0; i < team.getPlayerCount(); i++)
    {
        cout << BOLD << (i + 1) << ". " << RESET << team.getPlayer(i).getName() << "\n";
    }
    cout << "----------------------------------------\n";
}

void Scoreboard::displayRankings(const string& rankingsFile) 
{
    clearScreen();
    ifstream in(rankingsFile);
    
    if (!in)
    {
        cout << RED << "Rankings not available yet." << RESET << "\n";
        return;
    }
    
    string line;
    cout << BOLD << CYAN << "========================================\n";
    while (getline(in, line))
    {
        cout << line << "\n";
    }
    cout << "===================================" << RESET << "\n";
    
    in.close();
}

void Scoreboard::displayMatchSummary(const Match& match) 
{
    cout << BOLD << CYAN << "\n========================================\n";
    cout << BOLD << WHITE << "          MATCH SUMMARY\n" << RESET;
    cout << BOLD << CYAN << "========================================\n" << RESET << "\n";
    cout << BOLD << GREEN << match.getTeamA().getTeamName() << RESET << ": " 
              << match.getTeamA().getTotalRuns() << "/" << match.getTeamA().getTotalWickets() 
              << " (" << match.getTeamA().getOversDone() << "." << match.getTeamA().getBallsInCurrentOver() << ")\n";
    cout << BOLD << YELLOW << match.getTeamB().getTeamName() << RESET << ": " 
              << match.getTeamB().getTotalRuns() << "/" << match.getTeamB().getTotalWickets() 
              << " (" << match.getTeamB().getOversDone() << "." << match.getTeamB().getBallsInCurrentOver() << ")\n";
    cout << "----------------------------------------\n";
    cout << BOLD << "RESULT: " << RESET << BOLD << match.getResult() << RESET << "\n";
    cout << BOLD << CYAN << "===================================" << RESET << "\n";
}

void Scoreboard::waitForEnter() 
{
    cout << BOLD << "\nPress Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

void Scoreboard::displayMenu(const vector<string>& options) 
{
    cout << BOLD << "\n";
    for (size_t i = 0; i < options.size(); i++)
    {
        cout << i + 1 << ". " << options[i] << "\n";
    }
    cout << "Enter choice: " << RESET;
}
