#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
using namespace std;
#ifndef COLLEGE_H
#define COLLEGE_H
class College
{
public:
    int college_number,capacity,absorbed,upgrade,timer,timer_ratio,level,owner;  // owner = -1 means no owner
    int max_life,life;
    int length,width,door_x,door_y,corner_x,corner_y;  // up left corner [x][y]
    char product;

    College();
    void specification (int, int,int ,int ,int ,int,int,char);
    void check_produce();  // check whether it is time to produce a new agent or not
    void set_timer();  // set the college timer to it's initial value
    void check_enter(Agent *);  // perform necessary actions when an agent goes in this college
    double find_college_distance(int, int);
    double square(double);
};
#endif // COLLEGE_H
