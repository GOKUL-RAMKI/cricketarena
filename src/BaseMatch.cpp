#include "../include/BaseMatch.h"

using namespace std;

BaseMatch::BaseMatch() : totalOvers(0), matchComplete(false), result("") {}

BaseMatch::BaseMatch(int overs) : totalOvers(overs), matchComplete(false), result("") {}

int BaseMatch::getTotalOvers() const
{
    return totalOvers;
}

bool BaseMatch::isMatchComplete() const
{
    return matchComplete;
}

string BaseMatch::getResult() const
{
    return result;
}

void BaseMatch::setMatchComplete(bool complete)
{
    matchComplete = complete;
}

void BaseMatch::setResult(const string& res)
{
    result = res;
}
