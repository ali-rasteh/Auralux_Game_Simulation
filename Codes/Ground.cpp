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
#include <ncurses.h>


using namespace std;


Ground::Ground()
{
    kind = 0;  // default kind for points (field...)
}
void Ground::erase ()  // erase the list of existings
{
    existings.erase(existings.begin(),existings.begin()+existings.size());
}
void Ground::add_agent (Agent * agent)  // add this agent to the list of existings
{
    existings.push_back(agent);
}
void Ground::remove_agent (Agent * agent)  // remove this agent from the list of existings
{
    for(unsigned int i=0;i<existings.size();i++)
    {
        if(existings[i]->team==agent->team && existings[i]->id==agent->id)
        {
            existings.erase(existings.begin()+i);
            break;
        }
    }
}
void Ground::check_collision()  // check all collisions
{

    for(unsigned int i=0;i<existings.size();i++)   // check all agents with each other ...
        for(unsigned int j=i+1;j<existings.size();j++)
            if(existings[i]->team != existings[j]->team)  // if the two agents are in opposite teams
                if(existings[i]->life != 0 && existings[j]->life != 0)
                {
                    if(existings[i]->life >= existings[j]->life)
                    {
                        existings[i]->life -= existings[j]->life;
                        existings[j]->life = 0;
                    }                                           // subtract their life levels ...
                    else
                    {
                        existings[j]->life -= existings[i]->life;
                        existings[i]->life = 0;
                    }
                }

    for(unsigned int i=0;i<existings.size();i++)
    {
        if(existings[i]->life == 0)
        {                               // kill every agent whose life is zero !
            teams[existings[i]->team].erase_agent(existings[i]->id);
            existings.erase(existings.begin()+i);
        }
    }

}
