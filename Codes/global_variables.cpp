#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include "Agent.h"
#include "College.h"
#include "Team.h"
#include "Ground.h"
#include "global_variables.h"

using namespace std;


int map_length,map_width;
int NumOfTeams,NumOfColleges;
int timer_factor;


Team * teams;
College * colleges;
Ground ** points;
