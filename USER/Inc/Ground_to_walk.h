#ifndef  _GROUND_TO_WALK_H
#define  _GROUND_TO_WALK_H

#include "RMLibHead.h"

typedef struct {
  float X, Y, Angle;  
} Point_Typedef;

//typedef struct {
//    float V_max;
//    float a;
//    Point_Typedef Targrt;
//    float error;
//} 

typedef struct {
    Point_Typedef   Start;
    Point_Typedef   End;
    
    float   V_max;
    float   S_a;
    float   k;
    
    float   error;
} Straight_Init;


typedef struct {
    Point_Typedef point;
    float   V_max;
    float   S_a;
    
    float   error;
} Segment_Init;

typedef struct {
    int16_t n;
    int16_t k;
    int16_t last_speed;
    float last_Distance;
    float Distance;
    
    Segment_Init seg[50];
	float V_range;
} Piecewise_Init;

void straight_Init(Straight_Init *Run, float V_max, float a, float End_x, float End_y, float error);
uint8_t straight_line(Straight_Init * Run);
uint8_t accelerate_run(Straight_Init * Run);
uint8_t decelerate_run(Straight_Init * Run);
uint8_t uniform_run(Straight_Init * Run);
void piecewise_Init(Piecewise_Init *Run, Segment_Init *seg, int16_t point_num);
int16_t piecewise_line(Piecewise_Init *Run);

#endif
