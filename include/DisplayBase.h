#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H

#include <string>
#include <vector>

// Abstract base class for all display/UI classes.
// Scoreboard inherits from this class.
class DisplayBase
{
public:
    virtual ~DisplayBase() {}

    // Pure virtual: subclasses must implement screen clearing
    virtual void clear() const = 0;

    // Pure virtual: subclasses must implement a menu display
    virtual void showMenu(const std::vector<std::string>& options) const = 0;
};

#endif
