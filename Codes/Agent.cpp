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
Agent::Agent()
{

}
double Agent::square(double x)      // giving the square of number
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

void Agent:: set_new_agent (int ID,int X,int Y,int TEAM,char TYPE)
{                                                       // initialize the new agent specifications
    id=ID; x=X; y=Y; team=TEAM; type=TYPE;
    next_x=x; next_y=y; destination_x=x; destination_y=y;
    if(type=='S')
    {
        timer_ratio=3;
        speed=2;
        life=3;
        CapPower=1;
    }
    else if(type=='T')
    {
        timer_ratio=2;
        speed=3;
        life=1;
        CapPower=2;
    }
    else if(type=='P')
    {
        timer_ratio=6;
        speed=1;
        life=2;
        CapPower=3;
    }
    set_timer();

}
void Agent::set_timer()
{
    timer=(timer_ratio)*(timer_factor);   // it is the initial value for the time
}

void Agent::move(char direction)    // set the next location of the agent
{
    if(direction=='l')
    {
        if(y>0)
        {
            next_y=y-1;
        }
    }
    else if(direction=='r')
    {
        if(y<map_length-1)
        {
            next_y=y+1;
        }
    }
    else if(direction=='u')
    {
        if(x>0)
        {
            next_x=x-1;
        }
    }
    else if(direction=='d')
    {
        if(x<map_width-1)
        {
            next_x=x+1;
        }
    }
    else if(direction=='c')     //dl
    {
        if(y>0 && x<map_width-1)
        {
            next_y=y-1;
            next_x=x+1;
        }
    }
    else if(direction=='b')        //ul
    {
        if(y>0 && x>0)
        {
            next_y=y-1;
            next_x=x-1;
        }
    }
    else if(direction=='d')        //dr
    {
        if(y<map_length-1 && x<map_width-1)
        {
            next_y=y+1;
            next_x=x+1;
        }
    }
    else if(direction=='a')        //ur
    {
        if(y<map_length-1 && x>0)
        {
            next_y=y+1;
            next_x=x-1;
        }
    }
}
int Agent::check_points(int prob_x,int prob_y)      // check the near points to the probable point and also itself for specific conditions
{
    int kind=points[destination_x][destination_y].kind;
    if(kind==0)
    {
        if(points[prob_x][prob_y].kind!=0)
        {
            return 1;
        }

        else if(points[prob_x+1][prob_y].kind!=0 || points[prob_x-1][prob_y].kind!=0 || points[prob_x][prob_y+1].kind!=0 || points[prob_x][prob_y-1].kind!=0 )
        {
            if(sqrt(square(double(destination_x-prob_x))+square(double(destination_y-prob_y)))>1.41)
            {
                return 1;
            }

        }
        else if(points[prob_x+1][prob_y+1].kind!=0 || points[prob_x-1][prob_y+1].kind!=0 || points[prob_x+1][prob_y-1].kind!=0 || points[prob_x-1][prob_y-1].kind!=0 )
        {
            if(sqrt(square(double(destination_x-prob_x))+square(double(destination_y-prob_y)))>1.41)
            {
                return 1;
            }
        }
        return 0;
    }
    else
    {
        if(points[prob_x][prob_y].kind==kind)
        {
            return 0;
        }
        else if(points[prob_x][prob_y].kind!=0)
        {
            return 1;
        }
        else if((points[prob_x+1][prob_y].kind!=0 && points[prob_x+1][prob_y].kind!=kind) || (points[prob_x-1][prob_y].kind!=0 && points[prob_x+1][prob_y].kind!=kind))
        {
            if(sqrt(square(double(destination_x-prob_x))+square(double(destination_y-prob_y)))>1.41)
            {
                return 1;
            }

        }
        else if((points[prob_x][prob_y+1].kind!=0 && points[prob_x+1][prob_y].kind!=kind) || (points[prob_x][prob_y-1].kind!=0 && points[prob_x+1][prob_y].kind!=kind))
        {
            if(sqrt(square(double(destination_x-prob_x))+square(double(destination_y-prob_y)))>1.41)
            {
                return 1;
            }
        }
        else if((points[prob_x+1][prob_y+1].kind!=0 && points[prob_x+1][prob_y].kind!=kind) || (points[prob_x-1][prob_y+1].kind!=0 && points[prob_x+1][prob_y].kind!=kind))
        {
            if(sqrt(square(double(destination_x-prob_x))+square(double(destination_y-prob_y)))>1.41)
            {
                return 1;
            }
        }
        else if((points[prob_x+1][prob_y-1].kind!=0 && points[prob_x+1][prob_y].kind!=kind) || (points[prob_x-1][prob_y-1].kind!=0 && points[prob_x+1][prob_y].kind!=kind) )
        {
            if(sqrt(square(double(destination_x-prob_x))+square(double(destination_y-prob_y)))>1.41)
            {
                return 1;
            }
        }
        return 0;
    }
}

void Agent::check_go(int des_x , int des_y)     //this function checks for the nearest points to the destination with the specific conditions to move to them
{

    int choice=0;
    double distance[8][4];
    if(des_x==x && des_y==y)
    {
        return;
    }
    else
    {
        distance[0][0]=sqrt(square(double(des_x-x))+square(double(des_y-y+1)));
        distance[1][0]=sqrt(square(double(des_x-x+1))+square(double(des_y-y+1)));
        distance[2][0]=sqrt(square(double(des_x-x+1))+square(double(des_y-y)));
        distance[3][0]=sqrt(square(double(des_x-x+1))+square(double(des_y-y-1)));
        distance[4][0]=sqrt(square(double(des_x-x))+square(double(des_y-y-1)));
        distance[5][0]=sqrt(square(double(des_x-x-1))+square(double(des_y-y-1)));
        distance[6][0]=sqrt(square(double(des_x-x-1))+square(double(des_y-y)));
        distance[7][0]=sqrt(square(double(des_x-x-1))+square(double(des_y-y+1)));
        for(int i=0;i<8;i++)
        {
            switch (i)
            {
            case 0:
                distance[i][2]=x;
                distance[i][3]=y-1;
                break;
            case 1:
                distance[i][2]=x-1;
                distance[i][3]=y-1;
                break;
            case 2:
                distance[i][2]=x-1;
                distance[i][3]=y;
                break;
            case 3:
                distance[i][2]=x-1;
                distance[i][3]=y+1;
                break;
            case 4:
                distance[i][2]=x;
                distance[i][3]=y+1;
                break;
            case 5:
                distance[i][2]=x+1;
                distance[i][3]=y+1;
                break;
            case 6:
                distance[i][2]=x+1;
                distance[i][3]=y;
                break;
            case 7:
                distance[i][2]=x+1;
                distance[i][3]=y-1;
                break;

            }
            distance[i][1]=check_points(distance[i][2],distance[i][3]);

        }
        for(int i=0;i<8;i++)
        {
            if(distance[i][1]==0)
            {
                choice=i;
            }
        }
        for(int i=0;i<8;i++)
        {
            if(distance[i][1]==0 && (distance[i][0]<=distance[choice][0]))
            {
                if(distance[i][2]!=previous_x || distance[i][3]!=previous_y)
                {
                    choice=i;
                }

            }
        }
        switch (choice)
        {
        case 0:
            next_x=x;
            next_y=y-1;
            break;
        case 1:
            next_x=x-1;
            next_y=y-1;
            break;
        case 2:
            next_x=x-1;
            next_y=y;
            break;
        case 3:
            next_x=x-1;
            next_y=y+1;
            break;
        case 4:
            next_x=x;
            next_y=y+1;
            break;
        case 5:
            next_x=x+1;
            next_y=y+1;
            break;
        case 6:
            next_x=x+1;
            next_y=y;
            break;
        case 7:
            next_x=x+1;
            next_y=y-1;
            break;
        default:
            break;
        }

    }
}
