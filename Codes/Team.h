#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>

using namespace std;


#ifndef TEAM_H
#define TEAM_H
class Team
{
public:
    int team_number,last_id;
    int rand_number;
    int field_state;
    vector <Agent> agents;  // list of agents in the team

    Agent * new_agent;

    vector <College*> team_colleges;  //  list of colleges( with reference ) belonged to the team

    bool user;    // 0 : Human
                  // 1 : Computer

    bool status;  // 1 : Playing
                  // 0 : Lost :(

    Team();
    void set_agent(int id , int x , int y , char type);  // set an agent (new or not) in certain location
    int find_agent(int);  // finds the agent with given ID and returns it's index in the list
    int find_college(int);  // finds the college with given ID and returns it's index in the list
    void erase_agent (int);   // when an agent die !
    void erase_college (int);   // when a college is lost !
    void move(int , char);  // set the next location of an agent
    void move_all_random();  // set the next location of all agents in random directions
    void check_move_agents(); // check whether it is time to move or not
    void add_college(int);  // add a college(reference) to the list
    void change_product_type(int, char);  // chenge product type of a college
    void set_destination(int , int , int);
    void check_go_agents();
    void artificial_intelligence();
};
#endif // TEAM_H
