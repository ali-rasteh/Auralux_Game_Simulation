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

College::College()
{
    absorbed=0;      // number of absorbed agents for upgrading the college
    capacity=20;     // when 'absorbed' number reaches 'capacity', the college will upgrade
    max_life=20;     // full level of life
    life = 0;        // college life
    level=0;         // shows the number of production in each time
    timer_ratio=6;   // the ratio of initial-timer to timer_factor
}
double College::square(double x)        //giving the square of number
{
    double sqr=x*x;
    if(sqr>=0)
    {
        return sqr;
    }
    else
    {
        return (-sqr);
    }
}
double College::find_college_distance(int x, int y)
{
    double distance=sqrt(square(double(corner_x-x))+square(double(corner_y-y)));
    for(int i=corner_x;i<corner_x+length ;i++)
    {
        for(int j=corner_y; j<corner_y+width; j++)
        {
            if(sqrt(square(double(i-x))+square(double(j-y)))<=distance)
            {
                distance=sqrt(square(double(i-x))+square(double(j-y)));
            }
        }
    }
    return distance;
}

void College:: specification (int s0, int s1,int s2,int s3,int s4,int s5,int s6,char s7)
{                                                          // set college specifications
    owner=s0; length=s1; width=s2; door_x=s3; door_y=s4;
    corner_x=s5; corner_y=s6; product=s7;
    upgrade=length*width; //// it is the maximum level for upgrading the college
    if(owner!=-1)
    {
        life=max_life;
        level=1;
        absorbed = capacity;
    }
    set_timer();
}
void College::set_timer()
{
    timer=timer_factor*timer_ratio;     // it is the initial value for the timer
}

void College::check_produce()
{

    if(timer==0)       // check whether it is time to produce a new agent or not
    {
        set_timer();
        if(owner!=-1)
        {
            for(int i=0;i<level;i++)
            {
                teams[owner].set_agent(teams[owner].last_id+1,door_x,door_y,product);
            }
        }
    }
    timer--;
}
void College::check_enter(Agent * agent)  // necessary actions when an agent goes in this college
{



    if(owner==-1)   // the agent enters to a college which has no owner
    {
        absorbed+=agent->CapPower;
        life+=agent->CapPower;
        if(life>max_life)
            life=max_life;
        if(absorbed>=capacity)
        {
            if(absorbed>=upgrade*capacity)
                absorbed=upgrade*capacity;
            owner=agent->team;
            level++;
            teams[agent->team].add_college(college_number);
        }
        points[agent->x][agent->y].remove_agent(agent);
        teams[agent->team].erase_agent(agent->id);
    }
    else if(agent->team==owner)  // the agent enters to a college which belongs to it's team
    {

        if(level==upgrade && life==max_life)
        {
            return;
        }
        else if(life<max_life)
        {
            life+=agent->CapPower;
            if(life>max_life)
            {
                absorbed+=life-max_life;
                if(absorbed>=(level+1)*capacity)
                {
                    if(level<upgrade)
                    {
                        level++;
                    }
                    else if(absorbed>upgrade*capacity)
                    {
                        absorbed=upgrade*capacity;
                    }
                }
                life=max_life;
            }
            points[agent->x][agent->y].remove_agent(agent);
            teams[owner].erase_agent(agent->id);
        }
        else if(level<upgrade && life==max_life)
        {
            absorbed+=agent->CapPower;

            if(absorbed>=(level+1)*capacity)
            {
                if(absorbed>upgrade*capacity)
                    absorbed=upgrade*capacity;
                level++;
            }
            points[agent->x][agent->y].remove_agent(agent);
            teams[owner].erase_agent(agent->id);
        }
    }
    else if(agent->team!=owner && owner!=-1)
    {                           // the agent enters to a college which belongs to a opposite team
        life-=agent->CapPower;
        points[agent->x][agent->y].remove_agent(agent);
        teams[agent->team].erase_agent(agent->id);
        if(life<=0)
        {
            life=0;
            teams[owner].erase_college(college_number);
            owner=-1;
            level=0;
            absorbed=0;
        }

    }
}
