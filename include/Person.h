#ifndef PERSON_H
#define PERSON_H

#include <string>

// Base class representing a named person
// Player inherits from this class
class Person
{
protected:
    std::string name;

public:
    Person();
    Person(const std::string& personName);
    virtual ~Person();

    // Getter
    std::string getName() const;

    // Setter
    void setName(const std::string& personName);

    // Pure virtual: every person must be displayable
    virtual void displayInfo() const = 0;
};

#endif
