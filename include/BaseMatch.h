#ifndef BASEMATCH_H
#define BASEMATCH_H

#include <string>

// Abstract base class for all match types.
// Match inherits from this class.
class BaseMatch
{
protected:
    int totalOvers;
    bool matchComplete;
    std::string result;

public:
    BaseMatch();
    BaseMatch(int overs);
    virtual ~BaseMatch() {}

    // Getters
    int getTotalOvers() const;
    bool isMatchComplete() const;
    std::string getResult() const;

    // Setters
    void setMatchComplete(bool complete);
    void setResult(const std::string& res);

    // Pure virtual: each match type calculates its result differently
    virtual void calculateResult() = 0;

    // Pure virtual: save/load support
    virtual void saveMatchState(const std::string& filename) = 0;
    virtual void loadMatchState(const std::string& filename) = 0;
};

#endif
