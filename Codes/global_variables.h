#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include "Agent.h"
#include "College.h"
#include "Team.h"
#include "Ground.h"

#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

using namespace std;

extern int map_length,map_width;
extern int NumOfTeams,NumOfColleges;
extern int timer_factor;


extern Team * teams;
extern College * colleges;
extern Ground ** points;


#endif // GLOBAL_VARIABLES_H
