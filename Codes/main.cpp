#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
#include <streambuf>
#include "Agent.h"
#include "College.h"
#include "Team.h"
#include "Ground.h"
#include "global_variables.h"
#include <ncurses.h>
#include <unistd.h>

using namespace std;

void execute (vector<string>);  // executes all commands after a tick
void time_manager();  // performs necessary actions after several ticks
void output_print();  // prints the map informatin in standard format

//////////////////////////////////////////////////////////////////////////////////////////////////

void execute(vector<string> command)  // to execute all commands
{
    string sub;
    char product;
    int team_number,college_number,id,destination_x,destination_y;
    char direction;
    for(unsigned int i=0; i<command.size();i++)
    {
        istringstream S1(command[i]);
        S1 >> sub >> team_number >>sub;
        if(teams[team_number].status==1)   // if this team is not lost :
        {
            if(sub=="move")   // if the command is 'move' for an agent :
            {
                S1>>id>>direction;
                teams[team_number].move(id,direction);
            }
            else if(sub=="go")  // if the command is 'go' for an agent :
            {
                S1>>id>>sub>>destination_x>>destination_y;
                teams[team_number].set_destination(id,destination_x,destination_y);
            }
            else if(sub=="change")  // if the command is 'change' for a college :
            {
                S1>>college_number>>sub>>product;
                teams[team_number].change_product_type(college_number,product);

            }
        }
    }
    time_manager();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void time_manager()
{
    teams[0].move_all_random();  // move all of agents in random direction (DA'ESH!!!)
    for(int i=0;i<NumOfTeams+1;i++)
    {
        if(teams[i].status==1)   // if this team is not lost :
        {
            if(teams[i].user==1)
            {
                teams[i].artificial_intelligence();     //if the user of team is computer we call artificial intelligencr of team
            }
            teams[i].check_go_agents();
            teams[i].check_move_agents();  // is it time to move ?
        }                                  // check it for all agents...
    }
    for(int i=1;i<NumOfColleges+1;i++)
    {
        colleges[i].check_produce();  // is it time to produce new agents ?
    }                                 // check it for all colleges...

}
//////////////////////////////////////////////////////////////////////////////////////////////////
void output_print()     // printing output un the text base format
{


    init_pair(1,COLOR_RED,COLOR_RED);
    init_pair(2,COLOR_GREEN,COLOR_GREEN);
    init_pair(3,COLOR_BLUE,COLOR_BLUE);
    init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
    init_pair(5,COLOR_MAGENTA,COLOR_MAGENTA);
    init_pair(100,COLOR_BLACK,COLOR_WHITE);

    for(int i=1;i<NumOfColleges+1;i++)     // print colleges
    {
        for(int x=colleges[i].corner_x;x<colleges[i].corner_x+colleges[i].width;x++)
            for(int y=colleges[i].corner_y;y<colleges[i].corner_y+colleges[i].length;y++)
            {
                move(x,y);
                int owner=colleges[i].owner;
                if(owner==-1)
                    owner=100;
                attron(COLOR_PAIR(owner));
                addch(ACS_CKBOARD);
                refresh();
            }
    }

    init_pair(6,COLOR_RED,COLOR_BLACK);
    init_pair(7,COLOR_GREEN,COLOR_BLACK);
    init_pair(8,COLOR_BLUE,COLOR_BLACK);
    init_pair(9,COLOR_YELLOW,COLOR_BLACK);
    init_pair(10,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(100,COLOR_WHITE,COLOR_BLACK);

    for(int x=0;x<map_width;x++)
        for(int y=0;y<map_length;y++)
            if(points[x][y].kind==0)
            {
                if(points[x][y].existings.size()==0)
                {
                    move(x,y);
                    attron(COLOR_PAIR(100));
                    addch(' '); // empty point !
                    refresh();
                }
                else
                {
                     move(x,y);
                     attron(COLOR_PAIR(5+points[x][y].existings[0]->team));
                     addch('o');
                     refresh();
                }
            }

    usleep(500000);

/*
    // print map information in standard format
    cout<<endl<<"//////////////////////////////////////////////////////////////"<<endl;
    cout<<map_length<<" "<<map_width<<" "<<NumOfTeams<<endl;
    cout<<NumOfColleges<<endl;
    for(int i=1;i<NumOfColleges+1;i++)
    {
        cout<<colleges[i].corner_x<<" "<<colleges[i].corner_y<<" "<<colleges[i].length
        <<" "<<colleges[i].width<<" "<<"team=";
        if(colleges[i].owner==-1)
        {
            cout<<"x ";
        }
        else
        {
            cout<<colleges[i].owner<<" ";
        }
        cout<<colleges[i].door_x<<" "<<colleges[i].door_y<<" P="<<colleges[i].product<<endl;

    }
    for(int i=1;i<NumOfTeams+1;i++)
    {
        cout<<"team "<<teams[i].team_number<<" "<<teams[i].agents.size()<<endl;
        for(unsigned int j=0;j<teams[i].agents.size();j++)
        {
            cout<<teams[i].agents[j].id<<" "<<teams[i].agents[j].x
            <<" "<<teams[i].agents[j].y<<" "<<teams[i].agents[j].type<<endl;
        }
    }
    cout<<"team "<<"d"<<" "<<teams[0].agents.size()<<endl;
    for(unsigned int j=0;j<teams[0].agents.size();j++)
    {
        cout<<teams[0].agents[j].id<<" "<<teams[0].agents[j].x
        <<" "<<teams[0].agents[j].y<<" "<<teams[0].agents[j].type<<endl;
    }
*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int main ()
{

    timer_factor=1;

    initscr();
    curs_set(FALSE);
    start_color();

    bool Flag=0 ;   // will be set after first data to avoid map, team & college redefining
    vector <string> command; // list of commands before a tick
    string instruction,sub;
    int owner;
    int corner_x,corner_y,length,width,door_x,door_y;
    int team_number,NumOfAgents,agent_id;
    char product,agent_kind;
    int x,y;

    ifstream input("input.txt");  // input file

    while(1)
    {
        getline(input, instruction);
        if(instruction=="map")
        {
            ///////////////////////////// 1st line /////////
            getline(input, instruction);
            if(Flag==0)
            {
                istringstream S1(instruction);
                S1 >> map_length;  // length of map
                S1 >> map_width ;  // width of map
                S1 >> NumOfTeams;  // number of all teams
                points=new Ground* [map_width];
                for(int i=0;i<map_width;i++)    // map is made of 'points' which are objects of 'Ground' class
                    points[i]=new Ground [map_length];
                teams=new Team [NumOfTeams+1];
                for(int i=0;i<NumOfTeams+1;i++)  // team[1] -> team_number=1
                    teams[i].team_number=i;      // team[2] -> team_number=2
                                                 // team[3] -> team_number=3
                                                 // ...
                                                 // team[0] -> DA'ESH !!!
            }
            for(int i=0;i<map_width;i++)        // erase the list of 'existings' for each point
                for(int j=0;j<map_length;j++)     //         whenever map information is received
                    points[i][j].erase();


            ///////////////////////////// 2st line /////////
            getline(input, instruction);
            istringstream S2(instruction);
            if(Flag==0)
            {
                S2>>NumOfColleges;
                colleges=new College [NumOfColleges+1];  // number of colleges starts from 1
                for(int i=1;i<NumOfColleges+1;i++)
                    colleges[i].college_number=i;  // give each college it's id
                Flag = 1;
            }   // Set Flag to avoid map, team & college redefining

            ///////////////// colleges information /////////
            for(int college=1;college<NumOfColleges+1;college++)
            {
                getline(input, instruction);
                istringstream S3(instruction);
                S3 >> corner_x;   //  up-left point of college
                S3 >> corner_y;
                S3 >> length;     //  college length
                S3 >> width;      //  college width
                S3 >> sub;
                sub.erase(0,5);   // erase "team="
                if(sub=="x")
                    owner=-1;     // 'x' -> owner=-1 -> no owner
                else
                    istringstream (sub) >> owner;
                S3 >> door_x;     // door of the college , where new produced agents sit
                S3 >> door_y;
                S3 >> sub;
                product = sub[2]; // product of the college : 'S' or 'T' or 'P'

                for(int i=0;i<length;i++) // each point has a certain 'kind' which shows whether it is
                    for(int j=0;j<width;j++)                          // part of a college or field...
                        points[corner_x+j][corner_y+i].kind=college;

                if(owner!=-1)             // add (reference of) the college to it's owner college_list
                    teams[owner].add_college(college);
                colleges[college].specification(
                    owner,length,width,door_x,door_y,corner_x,corner_y,product);
                                                   // set college specifications
            }


            ///////////////////////// teams agents /////////
            for(int i=0;i<NumOfTeams+1;i++)
            {
                getline(input, instruction);
                istringstream S4(instruction);
                S4 >> sub;
                S4>>sub;
                if(sub=="d")
                    team_number=0;
                else
                    istringstream (sub) >> team_number;
                S4 >> NumOfAgents;
                for(int j=0;j<NumOfAgents;j++)
                {
                    getline(input, instruction);
                    istringstream S5(instruction);
                    S5 >> agent_id;     //
                    S5 >> x >> y;       //  get the agent information : ID , location & type
                    S5 >> agent_kind;   //
                    teams[team_number].set_agent(agent_id,x,y,agent_kind);
                }                       //  send the agent information to it's Team class
            }                                    //  whether it is a new agent or not ...

            for(int i=0;i<map_length;i++)
                for(int j=0;j<map_width;j++)
                {
                    points[i][j].check_collision();
                }
        }

        else if(instruction=="command")
        {
            command.erase(command.begin(),command.begin()+command.size());  // erase command list
            while(1)
            {
                getline(input, instruction);
                if(instruction=="tick")
                    break;                  // get all commands before 'tick'
                command.push_back(instruction);
            }
            execute(command);  // executes all commands after the tick
            output_print();    // print modified map...
        }
    }
    return 0;
}
