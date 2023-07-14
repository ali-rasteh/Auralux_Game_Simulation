#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
using namespace std;
#ifndef AGENT_H
#define AGENT_H
class Agent
{
public:
    Agent();
    int x,y,next_x,next_y,previous_x,previous_y,destination_x,destination_y;
    int id , team , life , speed , timer , CapPower;
    int timer_ratio;
    char type;   //  'S' or 'T' or 'P'
    void set_new_agent (int,int,int,int ,char);  // initialize the new agent specifications
    void set_timer();  // send the agent information to it's Team class...
    void move(char);   // set the next location of the agent
    int check_points(int ,int);
    void check_go(int , int);
    double square(double);
};

#endif // AGENT_H
