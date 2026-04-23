#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "../include/Player.h"
#include "../include/Team.h"
#include "../include/Match.h"
#include "../include/FileManager.h"
#include "../include/Scoreboard.h"

using namespace std;

// ANSI Color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Forward declarations
void runOrganizer();
void runViewer();
void organizerMenu(Match& match, FileManager& fileManager);
void viewerMenu(FileManager& fileManager);
void playMatch(Match& match, FileManager& fileManager);
void playInnings(Match& match, int inningsNumber, FileManager& fileManager);
void updatePlayerDatabase(const Team& team, FileManager& fileManager);void displayLiveMatchWithRefresh(Match& match);
int main() 
{
    while (true)
    {
        Scoreboard::clearScreen();
        
        cout << BOLD << CYAN << "========================================\n";
        cout << BOLD << WHITE << "    CRICKET SCOREBOARD SYSTEM\n" << RESET;
        cout <<BOLD << CYAN << "========================================\n" << RESET << "\n\n";
        
        cout << BOLD << WHITE << "Select Role:\n" << RESET;
        cout << "1. " << GREEN << "Organizer (Manage Match)" << RESET << "\n";
        cout << "2. " << YELLOW << "Viewer (View Live Scores & Rankings)" << RESET << "\n";
        cout << "3. " << RED << "Exit" << RESET << "\n";
        cout << BOLD << CYAN << "Enter choice: " << RESET;
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1)
        {
            runOrganizer();
        }
        else if (choice == 2)
        {
            runViewer();
        }
        else if (choice == 3)
        {
            cout << BOLD << RED << "Exiting..." << RESET << "\n";
            break;
        }
        else
        {
            cout << RED << "Invalid choice. Try again." << RESET << "\n";
            Scoreboard::waitForEnter();
        }
    }
    
    return 0;
}

void runOrganizer() 
{
    FileManager fileManager;
    Match match;
    
    Scoreboard::clearScreen();
    cout << BOLD << CYAN << "========================================\n";
    cout << BOLD << WHITE << "       ORGANIZER MODE\n" << RESET;
    cout << BOLD << CYAN << "========================================\n"<< RESET << "\n\n";
    
    cout << BOLD << WHITE << "1. Start New Match\n";
    cout << BOLD << WHITE << "2. Continue Existing Match\n";
    cout << BOLD << CYAN << "Enter choice: " << RESET;
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 2 && fileManager.fileExists("../data/match_state.txt"))
    {
        match.loadMatchState("../data/match_state.txt");
        cout << GREEN << "Match loaded successfully!\n" << RESET;
        Scoreboard::waitForEnter();
    }
    else
    {
        // Setup new match
        Scoreboard::clearScreen();
        cout << BOLD << CYAN << "========================================\n";
        cout << BOLD << WHITE << "       NEW MATCH SETUP\n" << RESET;
        cout << BOLD << CYAN << "========================================" << RESET << "\n\n";
        
        int totalOvers;
        cout << BOLD << WHITE << "Enter total overs: " << RESET;
        cin >> totalOvers;
        cin.ignore();
        
        string teamAName, teamBName;
        cout << BOLD << WHITE << "Enter Team A name: " << RESET;
        getline(cin, teamAName);
        
        cout << BOLD << WHITE << "Enter Team B name: " << RESET;
        getline(cin, teamBName);
        
        Team teamA(teamAName);
        Team teamB(teamBName);
        
        // Enter Team A players
        cout << BOLD << WHITE << "\nEnter 11 players for " << GREEN << teamAName << RESET << BOLD << WHITE << ":\n" << RESET;
        for (int i = 0; i < 11; i++)
        {
            string playerName;
            cout << BOLD << CYAN << "Player " << (i + 1) << ": " << RESET;
            getline(cin, playerName);
            
            Player p = fileManager.getPlayerFromDatabase(playerName);
            if (p.getName().empty())
            {
                p = Player(playerName);
            }
            p.resetCurrentMatchStats();
            teamA.addPlayer(p);
        }
        
        // Enter Team B players
        cout << BOLD << WHITE << "\nEnter 11 players for " << YELLOW << teamBName << RESET << BOLD << WHITE << ":\n" << RESET;
        for (int i = 0; i < 11; i++)
        {
            string playerName;
            cout << BOLD << CYAN << "Player " << (i + 1) << ": " << RESET;
            getline(cin, playerName);
            
            Player p = fileManager.getPlayerFromDatabase(playerName);
            if (p.getName().empty())
            {
                p = Player(playerName);
            }
            p.resetCurrentMatchStats();
            teamB.addPlayer(p);
        }
        
        match = Match(teamA, teamB, totalOvers);
        cout << GREEN << "\nMatch setup complete!\n" << RESET;
        Scoreboard::waitForEnter();
    }
    
    organizerMenu(match, fileManager);
}

void organizerMenu(Match& match, FileManager& fileManager) 
{
    while (true)
    {
        Scoreboard::clearScreen();
        cout << BOLD << CYAN << "========================================\n";
        cout << BOLD << WHITE << "       ORGANIZER MENU\n" << RESET;
        cout << BOLD << CYAN << "========================================" << RESET << "\n\n";
        
        vector<string> options;
        options.push_back(GREEN "Start/Continue Match" RESET);
        options.push_back(YELLOW "View Full Scorecard" RESET);
        options.push_back(RED "Save & Exit" RESET);
        
        Scoreboard::displayMenu(options);
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1)
        {
            playMatch(match, fileManager);
        }
        else if (choice == 2)
        {
            Scoreboard::displayFullScorecard(match);
            Scoreboard::waitForEnter();
        }
        else if (choice == 3)
        {
            match.saveMatchState("../data/match_state.txt");
            cout << GREEN << "Match saved. Returning to main menu..." << RESET << "\n";
            Scoreboard::waitForEnter();
            return;
        }
    }
}

void playMatch(Match& match, FileManager& fileManager) 
{
    if (match.getCurrentInnings() == 0)
    {
        // Ask which team bats first
        Scoreboard::clearScreen();
        cout << BOLD << WHITE << "Which team bats first?\n" << RESET;
        cout << "1. " << GREEN << match.getTeamA().getTeamName() << RESET << "\n";
        cout << "2. " << YELLOW << match.getTeamB().getTeamName() << RESET << "\n";
        cout << BOLD << CYAN << "Enter choice: " << RESET;
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1)
        {
            playInnings(match, 1, fileManager);
        }
        else
        {
            // Swap teams
            Team temp = match.getTeamA();
            match.getTeamA() = match.getTeamB();
            match.getTeamB() = temp;
            playInnings(match, 1, fileManager);
        }
    }
    else if (match.getCurrentInnings() == 1 && match.isCurrentInningsComplete())
    {
        // First innings complete, start second
        playInnings(match, 2, fileManager);
    }
    else if (match.getCurrentInnings() == 2 && match.isMatchComplete())
    {
        // Match complete
        Scoreboard::displayFullScorecard(match);
        cout << BOLD << RED << "\n\nMATCH COMPLETE!" << RESET << "\n";
        
        // Update player database
        updatePlayerDatabase(match.getTeamA(), fileManager);
        updatePlayerDatabase(match.getTeamB(), fileManager);
        
        // Generate rankings
        vector<Player> allPlayers = fileManager.loadPlayerStats();
        fileManager.generateRankings(allPlayers);
        
        Scoreboard::waitForEnter();
    }
    else
    {
        // Continue current innings
        playInnings(match, match.getCurrentInnings(), fileManager);
    }
}

void playInnings(Match& match, int inningsNumber, FileManager& fileManager) 
{
    if (match.getCurrentInnings() != inningsNumber)
    {
        match.startInnings(inningsNumber);
        
        // Select opening batsmen
        Team* batting = match.getBattingTeam();
        Scoreboard::clearScreen();
        cout << BOLD << CYAN << "=============================================\n";
        cout << BOLD << WHITE << "   INNINGS " << inningsNumber << " - " << GREEN << batting->getTeamName() << RESET << BOLD << WHITE << " BATTING\n" << RESET;
        cout << BOLD << CYAN << "=============================================" << RESET << "\n\n";
        
        Scoreboard::displayTeamLineup(*batting);
        
        cout << BOLD << WHITE << "\nSelect Opening Batsmen:\n" << RESET;
        int striker, nonStriker;
        
        cout << BOLD << CYAN << "Striker (1-11): " << RESET;
        cin >> striker;
        striker--;
        
        cout << BOLD << CYAN << "Non-Striker (1-11): " << RESET;
        cin >> nonStriker;
        nonStriker--;
        cin.ignore();
        
        match.selectOpeningBatsmen(striker, nonStriker);
        
        cout << GREEN << "\nOpeners selected. Starting innings...\n" << RESET;
        Scoreboard::waitForEnter();
    }
    
    // Play deliveries
    while (!match.isMatchComplete() && !match.isCurrentInningsComplete())
    {
        Team* batting = match.getBattingTeam();
        Team* bowling = match.getBowlingTeam();
        
        // Select bowler at start of over
        if (batting->getBallsInCurrentOver() == 0)
        {
            Scoreboard::clearScreen();
            cout << BOLD << YELLOW << "Over " << (batting->getOversDone() + 1) << " begins." << RESET << "\n";
            Scoreboard::displayTeamLineup(*bowling);
            
            string bowlerName;
            cout << BOLD << CYAN << "\nEnter bowler name: " << RESET;
            getline(cin, bowlerName);
            
            match.selectBowler(bowlerName);
            
            if (match.getCurrentBowlerIndex() == -1)
            {
                cout << RED << "Invalid bowler! Try again." << RESET << "\n";
                Scoreboard::waitForEnter();
                continue;
            }
        }
        
        // Display live score
        Scoreboard::displayLiveScore(match);
        
        cout << BOLD << CYAN << "\nEnter runs (0-6) or -1 for WICKET: " << RESET;
        int runs;
        cin >> runs;
        cin.ignore();
        
        // Validate input
        if (runs < -1 || runs > 6)
        {
            cout << RED << "Invalid input! Try again." << RESET << "\n";
            Scoreboard::waitForEnter();
            continue;
        }
        
        match.playDelivery(runs);
        
        if (runs == -1 && batting->getTotalWickets() < 10 && !match.isCurrentInningsComplete())
        {
            Scoreboard::clearScreen();
            cout << BOLD << RED << "WICKET! Select next batsman:\n" << RESET;
            Scoreboard::displayTeamLineup(*batting);
            
            int nextBatsman;
            while (true)
            {
                cout << BOLD << CYAN << "\nNext batsman (1-11): " << RESET;
                cin >> nextBatsman;
                nextBatsman--;
                cin.ignore();

                if (nextBatsman >= 0 && nextBatsman < batting->getPlayerCount() && !batting->getPlayer(nextBatsman).isCurrentOut())
                {
                    match.setStrikerIndex(nextBatsman);
                    match.setNextBatsmanIndex(nextBatsman + 1);
                    break;
                }
                cout << RED << "Invalid batsman selection. Choose a not-out player.\n" << RESET;
            }
        }
        
        // Save match state after each delivery
        match.saveMatchState("../data/match_state.txt");
        
        // Check if innings complete
        if (batting->getTotalWickets() >= 10 || batting->getOversDone() >= match.getTotalOvers())
        {
            match.checkMatchEnd();
            
            if (inningsNumber == 1)
            {
                cout << GREEN << "\nFirst innings complete!\n" << RESET;
                cout << BOLD << GREEN << batting->getTeamName() << RESET << " scored " 
                          << BOLD << batting->getTotalRuns() 
                          << "/" << batting->getTotalWickets() << RESET << "\n";
                Scoreboard::waitForEnter();
                return;
            }
        }
        
        // Check if chasing team won
        if (inningsNumber == 2 && batting->getTotalRuns() > match.getTargetRuns())
        {
            match.checkMatchEnd();
            break;
        }
    }
    
    if (match.isMatchComplete() && inningsNumber == 2)
    {
        cout << BOLD << RED << "\n\nMATCH COMPLETE!" << RESET << "\n";
        match.calculateResult();
        Scoreboard::displayMatchSummary(match);
        
        // Update player database
        updatePlayerDatabase(match.getTeamA(), fileManager);
        updatePlayerDatabase(match.getTeamB(), fileManager);
        
        // Generate rankings
        vector<Player> allPlayers = fileManager.loadPlayerStats();
        fileManager.generateRankings(allPlayers);
        
        Scoreboard::waitForEnter();
    }
}

void displayLiveMatchWithRefresh(Match& match)
{
    while (true)
    {
        Scoreboard::clearScreen();
        Scoreboard::displayLiveScore(match);
        
        if (match.isMatchComplete())
        {
            cout << BOLD << RED << "\n\nMATCH COMPLETE!\n" << RESET;
            match.calculateResult();
            cout << BOLD << WHITE << "RESULT: " << GREEN << match.getResult() << RESET << "\n";
        }
        
        cout << BOLD << WHITE << "\nOptions:\n" << RESET;
        cout << BOLD << GREEN << "1. Refresh\n" << RESET;
        cout << BOLD << YELLOW << "2. Back to Viewer Menu\n" << RESET;
        cout << BOLD << CYAN << "Enter choice: " << RESET;
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1)
        {
            // Reload match state
            FileManager fm;
            if (fm.fileExists("../data/match_state.txt"))
            {
                match.loadMatchState("../data/match_state.txt");
            }
            else
            {
                cout << RED << "Match no longer available.\n" << RESET;
                Scoreboard::waitForEnter();
                break;
            }
        }
        else if (choice == 2)
        {
            break;
        }
        else
        {
            cout << RED << "Invalid choice.\n" << RESET;
            Scoreboard::waitForEnter();
        }
    }
}

void updatePlayerDatabase(const Team& team, FileManager& fileManager) 
{
    for (int i = 0; i < team.getPlayerCount(); i++)
    {
        Player updatedPlayer = team.getPlayer(i);
        updatedPlayer.commitMatchStats();
        fileManager.updatePlayerInDatabase(updatedPlayer);
    }
}

void runViewer() 
{
    FileManager fileManager;
    viewerMenu(fileManager);
}

void viewerMenu(FileManager& fileManager) 
{
    while (true)
    {
        Scoreboard::clearScreen();
        cout << BOLD << CYAN << "========================================\n";
        cout << BOLD << WHITE << "         VIEWER MODE\n" << RESET;
        cout << BOLD << CYAN << "========================================\n" << RESET << "\n\n";
        
        vector<string> options;
        options.push_back("View Live Match");
        options.push_back("View Full Scoreboard");
        options.push_back("View Player Rankings");
        options.push_back("Back to Main Menu");
        
        Scoreboard::displayMenu(options);
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1)
        {
            if (fileManager.fileExists("../data/match_state.txt"))
            {
                Match match;
                match.loadMatchState("../data/match_state.txt");
                displayLiveMatchWithRefresh(match);
            }
            else
            {
                cout << YELLOW << "No live match in progress.\n" << RESET;
                Scoreboard::waitForEnter();
            }
        }
        else if (choice == 2)
        {
            if (fileManager.fileExists("../data/match_state.txt"))
            {
                Match match;
                match.loadMatchState("../data/match_state.txt");
                Scoreboard::displayFullScorecard(match);
                Scoreboard::waitForEnter();
            }
            else
            {
                cout << YELLOW << "No live match in progress.\n" << RESET;
                Scoreboard::waitForEnter();
            }
        }
        else if (choice == 3)
        {
            if (fileManager.fileExists("../data/rankings.txt"))
            {
                Scoreboard::displayRankings("../data/rankings.txt");
                Scoreboard::waitForEnter();
            }
            else
            {
                cout << YELLOW << "Rankings not available yet.\n" << RESET;
                Scoreboard::waitForEnter();
            }
        }
        else if (choice == 4)
        {
            cout << GREEN << "Returning to main menu...\n" << RESET;
            Scoreboard::waitForEnter();
            return;
        }
    }
}
