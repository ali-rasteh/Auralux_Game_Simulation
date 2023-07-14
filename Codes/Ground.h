#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
using namespace std;
#ifndef GROUND_H
#define GROUND_H
class Ground
{
public:

    vector <Agent*> existings;  // agents who already exist in this point

    int kind;   // the point is part of a college or field
                // 0 : field
                // 1 : college 1
                // 2 : college 2
                // 3 : college 3
                // ...
    Ground();
    void erase ();  // erase the list of existings
    void add_agent (Agent *);   // add an agent to the list of existings
    void remove_agent(Agent *);   // remove an agent to the list of existings
    void check_collision();   // check all collisions

};
#endif // GROUND_H
