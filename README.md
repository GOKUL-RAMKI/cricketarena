# CricketArena

## Overview  
CricketArena is a C++ based cricket match simulation system. It manages teams, players, scoring, and match progression while storing match state and statistics using file handling.

## Features  
- Team and player management  
- Ball-by-ball score tracking  
- Match state persistence  
- Player statistics storage  
- Scoreboard management  
- File-based data handling  

## Workflow  
The system initializes teams and players from stored data.  

During the match, ball-by-ball inputs update runs, wickets, and match state.  

Scoreboard and player statistics are updated dynamically and saved to files.  

Match state can be resumed using stored data.  

## Tech Stack  
- C++  
- OOP (classes and modular design)  
- File handling  

## Directory Structure  
CricketArena/
│
├── data/
│ ├── match_state.txt
│ ├── player_stats.csv
│ └── rankings.txt
│
├── include/
│ ├── BaseMatch.h
│ ├── DisplayBase.h
│ ├── FileManager.h
│ ├── Match.h
│ ├── Person.h
│ ├── Player.h
│ ├── Scoreboard.h
│ └── Team.h
│
├── src/
│ ├── BaseMatch.cpp
│ ├── FileManager.cpp
│ ├── main.cpp
│ ├── Match.cpp
│ ├── Person.cpp
│ ├── Player.cpp
│ ├── Scoreboard.cpp
│ ├── Team.cpp
│ └── cricketArena.exe


## Compilation  
g++ src/*.cpp -o cricketarena  

## Usage  
./cricketarena  

## Notes  
The project demonstrates structured OOP design and file-based state management for simulating cricket matches.
