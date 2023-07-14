#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "Agent.h"
#include "College.h"
#include "Team.h"
#include "Ground.h"
#include "global_variables.h"

using namespace std;

Team::Team()
{
    status=1;
    user = 0;
    last_id=0;
}

void Team::set_agent(int id , int x , int y , char type)
{

    int index = find_agent(id);

    if(index != -1)   // there "is" such agent with this ID ... just set x & y
    {
        agents[index].x=x;
        agents[index].y=y;
        points[x][y].add_agent(& agents[index]);  // add this agent to existing list of
    }                                                  // the point where it is located

    else   // this ID is new(!) ... create this new agent !
    {

        new_agent = new Agent[1];
        new_agent->set_new_agent(id,x,y,team_number,type);
        agents.push_back((*new_agent));
        if(id>last_id)
        {
            last_id=id;
        }

    }
    points[x][y].add_agent(new_agent);    // add this agent to existing list of
                                                      // the point where it is located
}

int Team::find_agent(int id)    // finds the agent with given ID and
{                                  // returns it's index in the list

    for(unsigned int i=0;i<agents.size();i++)
        if(agents[i].id==id)
            return i;
    return -1;  // not found ! -> index = -1

}
int Team::find_college(int id)  // finds the college with given ID and
{                                    // returns it's index in the list

    for(unsigned int i=0;i<team_colleges.size();i++)
        if(team_colleges[i]->college_number==id)
            return i;
    return -1;  // not found ! -> index = -1

}

void Team::erase_agent (int id)   // when an agent die !
{                                 // erase it from the list
    if(find_agent(id)!=-1)
    {
        agents.erase(agents.begin()+find_agent(id));
    }
    if(agents.size()==0 && team_colleges.size()==0)
    {
        status=0;       // the team has been lost! :(
    }
}
void Team::add_college(int college_number)
{
    team_colleges.push_back(&colleges[college_number]);
}

void Team:: erase_college (int id)     // when a college is lost !
{                                      // erase it from the list
    team_colleges.erase(team_colleges.begin()+find_college(id));
}
void Team::move(int id , char direction)
{
    int index=find_agent(id);
    if(index!=-1)
    {
        if(direction!='m')   // random direction
        {
            agents[index].move(direction);
        }
        else
        {
            srand(time(NULL));
            rand_number=(rand()%8)+1;  // generate a random number from 1 to 8
            switch (rand_number) {                  // for one of 8 directions
            case 1:
                agents[index].move('u');
                break;
            case 2:
                agents[index].move('d');
                break;
            case 3:
                agents[index].move('l');
                break;
            case 4:
                agents[index].move('r');
                break;
            case 5:
                agents[index].move('a');
                break;
            case 6:
                agents[index].move('b');
                break;
            case 7:
                agents[index].move('c');
                break;
            case 8:
                agents[index].move('d');
                break;
            }
        }

    }
}
void Team::move_all_random()     // move all agents in random direction
{
    for(unsigned int i=0;i<agents.size();i++)
    {
            move(agents[i].id,'m');
    }
}

void Team::check_move_agents()
{
    unsigned int current_size;
    for(unsigned int i=0;i<agents.size();i++)
    {
        current_size=agents.size();
        if(agents[i].next_x!=agents[i].x || agents[i].next_y!=agents[i].y)
        {                                       // if the agent has moved ...
            if(agents[i].timer==0)
            {
                agents[i].set_timer();  // reset agent timer for next move
                points[agents[i].x][agents[i].y].remove_agent(& agents[i]);   // remove this agent from the list of
                field_state=points[agents[i].next_x][agents[i].next_y].kind;     // existiongs in previous location
                if(field_state==0)
                {                   // if the next location is part of the field
                    agents[i].x=agents[i].next_x;
                    agents[i].y=agents[i].next_y; // So , the agent moves...
                    points[agents[i].x][agents[i].y].add_agent(& agents[i]); // and add this agent to the list of
                    points[agents[i].x][agents[i].y].check_collision();           //existings in the new location
                                                             // and check all possible collision in this location
                }
                else   // if the agent enters a college
                {
                    colleges[field_state].check_enter(&agents[i]);  // to perform necessary actions when an
                                                                    // agent enters a college
                }

            }
            (agents[i].timer)--;    // reduce timer of this agent for next tick...
            if(agents.size()<current_size)
            {
                i-=current_size-agents.size();
            }
        }
    }
}
void Team::change_product_type(int college_number, char product)
{

    int index=find_college(college_number);

    if(index!=-1)           // check if the college belongs to this team
    {
        team_colleges[index]->product=product;   // to change product of the college
    }
}

void Team::artificial_intelligence()        // general artificial intelligence of team and it do for teams with user=1 that are computer players
{
    int our_num=0;
    int enemy_num=0;
    for(int i=0 ; i<(team_colleges.size())/2;i++)       //setting production type
    {
        change_product_type(team_colleges[i]->college_number,'P');
    }

    for(int i=((team_colleges.size())/2)+1 ; i<team_colleges.size();i++)        //setting production type
    {
        change_product_type(team_colleges[i]->college_number,'S');
    }
    for(int i=0;i<team_colleges.size();i++)     // we defense in front of enemy agents if they are to near to us
    {
        for(int j=0;j<NumOfTeams; j++)
        {
            if(teams[j].team_number!=team_number)
            {
                for(int k=0 ; k<teams[j].agents.size();k++)
                {
                    if(team_colleges[i]->find_college_distance(teams[j].agents[k].x,teams[j].agents[k].y<(map_length+map_width)/10))
                    {
                        enemy_num+=teams[j].agents[k].CapPower;
                    }
                }
            }

        }
        if(enemy_num>team_colleges[i]->life || team_colleges[i]->life<(team_colleges[i]->max_life)/2)
        {
            for(int t=0;t<agents.size();t++)
            {
                agents[t].destination_x=team_colleges[i]->corner_x+(team_colleges[i]->length)/2;
                agents[t].destination_y=team_colleges[i]->corner_y+(team_colleges[i]->width)/2;
            }
            return;
        }
        enemy_num=0;
    }


    for(int i=0;i<team_colleges.size();i++)     //we set moving of agents to the colleges for upgrading them
    {
        for(int k=0 ; k<agents.size();k++)
        {
            if(team_colleges[i]->find_college_distance(agents[k].x,agents[k].y<(map_length+map_width)/10))
            {
                our_num+=agents[k].CapPower;
            }
        }

        if(our_num>=20)
        {
            for(int k=0 ; k<agents.size();k++)
            {
                if(team_colleges[i]->find_college_distance(agents[k].x,agents[k].y<(map_length+map_width)/10))
                {
                    agents[k].destination_x=team_colleges[i]->corner_x+(team_colleges[i]->length)/2;
                    agents[k].destination_y=team_colleges[i]->corner_y+(team_colleges[i]->width)/2;
                }
            }
            return;
        }
        our_num=0;
    }


    for(int i=0; i<NumOfColleges ; i++)     // we attack to our enemies if there are too low agents of enemies across a specific college
    {
        if(find_college(colleges[i].college_number)==-1)
        {
            for(int j=0;j<NumOfTeams; j++)
            {
                if(teams[j].team_number!=team_number)
                {
                    for(int k=0 ; k<teams[j].agents.size();k++)
                    {
                        if(colleges[i].find_college_distance(teams[j].agents[k].x,teams[j].agents[k].y<(map_length+map_width)/10))
                        {
                            enemy_num+=teams[j].agents[k].CapPower;
                        }
                    }
                }

            }
            for(int j=0;j<agents.size();j++)
            {
                our_num+=agents[j].CapPower;
            }
            if(our_num>enemy_num+colleges[i].life)
            {
                for(int t=0;t<agents.size();t++)
                {
                    agents[t].destination_x=colleges[i].corner_x+(colleges[i].length)/2;
                    agents[t].destination_y=colleges[i].corner_y+(colleges[i].width)/2;
                }
                return;
            }
        }
    }




}

void Team::set_destination(int id, int x, int y)        //setting destination of an agent to move to it
{
    int index=find_agent(id);
    agents[index].destination_x=x;
    agents[index].destination_y=y;
}

void Team::check_go_agents()
{
    for(unsigned int i=0;i<agents.size();i++)
    {
        agents[i].check_go(agents[i].destination_x,agents[i].destination_y);
    }
}
