#include "../include/Person.h"
#include <iostream>

using namespace std;

Person::Person() : name("") {}

Person::Person(const string& personName) : name(personName) {}

Person::~Person() {}

string Person::getName() const
{
    return name;
}

void Person::setName(const string& personName)
{
    name = personName;
}
