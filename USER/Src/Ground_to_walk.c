#include "Ground_to_walk.h"
#include "arm_math.h"
#include <math.h>

float Rectify(float num) {
    if (num < -1.0f) return -1;
    else if (num > 1.0f) return 1;
    else return num;
}

float distance_P2P(const Point_Typedef *P1, const Point_Typedef *P2) {
    float d;
    arm_sqrt_f32((P1->X - P2->X) * (P1->X - P2->X) + (P1->Y - P2->Y) * (P1->Y - P2->Y), &d);
    return d;
}

__weak Point_Typedef GetLocation() {
    Point_Typedef p = {0, 0};
    return p;
}

__weak void SetSpeed(float Vx, float Vy) {
    UNUSED(Vx);
    UNUSED(Vy);
}

void straight_Init(Straight_Init *Run, float V_max, float a, float End_x, float End_y, float error) {
    Point_Typedef End = {End_x, End_y};
    Run->Start = GetLocation();
    Run->End = End;
    Run->error = error;
    Run->V_max = V_max;
    
    float Distance = distance_P2P(&Run->Start, &Run->End);
    
    Run->S_a = V_max * V_max / (2 * a);
    Run->k = V_max / Run->S_a;
    
    if (Distance < Run->S_a * 2)
        Run->S_a = Distance / 2;
}

uint8_t straight_line(Straight_Init * Run) {
    Point_Typedef Now = GetLocation();
    float End_Distance = distance_P2P(&Now, &Run->End);
    float Start_Distance = distance_P2P(&Now, &Run->Start);
    float speed = 0;
	
    if (End_Distance < Run->error) {
        SetSpeed(0, 0);
        return 1;
    } if (Start_Distance < Run->S_a) {
        speed = Start_Distance * Run->k * 0.9f + Run->V_max * 0.1f;
    } else if (End_Distance < Run->S_a) {
        speed = End_Distance * Run->k;
    } else speed = Run->V_max;
    
    float _cos = (Run->End.X - Now.X) / End_Distance;
    float _sin = (Run->End.Y - Now.Y) / End_Distance;
    
    float _vx = speed * _cos;
    float _vy = speed * _sin;
    
    SetSpeed(_vx, _vy);
    return 0;
}

uint8_t accelerate_run(Straight_Init * Run) {
	Point_Typedef Now = GetLocation();
    float End_Distance = distance_P2P(&Now, &Run->End);
    float Start_Distance = distance_P2P(&Now, &Run->Start);
    float speed = 0;
	
	if (End_Distance < Run->error) {
        return 1;
    } if (Start_Distance < Run->S_a) {
        speed = Start_Distance * Run->k * 0.9f + Run->V_max * 0.1f;
    } else speed = Run->V_max;
    
    float _cos = (Run->End.X - Now.X) / End_Distance;
    float _sin = (Run->End.Y - Now.Y) / End_Distance;
    
    float _vx = speed * _cos;
    float _vy = speed * _sin;
    
    SetSpeed(_vx, _vy);
	return 0;
}

uint8_t decelerate_run(Straight_Init * Run) {
    Point_Typedef Now = GetLocation();

    float End_Distance = distance_P2P(&Now, &Run->End);
    float Start_Distance = distance_P2P(&Now, &Run->Start);
    float speed = 0;
	
    if (End_Distance < Run->error) {
        SetSpeed(0, 0);
        return 1;
    } else if (End_Distance < Run->S_a) {
        speed = End_Distance * Run->k;
    } else speed = Run->V_max;
    
    float _cos = (Run->End.X - Now.X) / End_Distance;
    float _sin = (Run->End.Y - Now.Y) / End_Distance;
    
    float _vx = speed * _cos;
    float _vy = speed * _sin;
    
    SetSpeed(_vx, _vy);
    return 0;
}

uint8_t uniform_run(Straight_Init * Run) {
    Point_Typedef Now = GetLocation();
    float End_Distance = distance_P2P(&Now, &Run->End);
    float Start_Distance = distance_P2P(&Now, &Run->Start);
    float speed = 0;
	
    if (End_Distance < Run->error) {
        return 1;
    } 
    else speed = Run->V_max;
    
    float _cos = (Run->End.X - Now.X) / End_Distance;
    float _sin = (Run->End.Y - Now.Y) / End_Distance;
    
    float _vx = speed * _cos;
    float _vy = speed * _sin;
    
    SetSpeed(_vx, _vy);
    return 0;
}

static int16_t i = 0;
void piecewise_Init(Piecewise_Init *Run, Segment_Init *seg, int16_t point_num) {
    i = 1;
    Segment_Init Start = {.point = GetLocation()};
    Run->seg[0] = Start;
    memcpy(Run->seg + 1, seg, point_num * sizeof(Segment_Init));
    Run->last_speed = 0;
    Run->n = point_num;
    Run->k = seg[0].V_max / seg[0].S_a;
	float Distance = distance_P2P(&Run->seg[point_num - 1].point, &Run->seg[point_num].point);
    if (!(Distance > 2 * Run->seg[point_num].S_a)) {
        Run->seg[point_num].S_a = Distance;
    }
}

int16_t piecewise_line(Piecewise_Init *Run) {
    Point_Typedef Now = GetLocation();
    float speed = 0;
    Run->last_Distance = distance_P2P(&Now, &Run->seg[i - 1].point);
    Run->Distance = distance_P2P(&Now, &Run->seg[i].point); 
        
    if (Run->Distance < Run->seg[i].error) {
        if(i == Run->n) {
            SetSpeed(0, 0);
            return 0xff;
        } else {
            Run->last_speed = Run->last_Distance * Run->k + Run->last_speed;
            i++;
            if(i == Run->n)  Run->k = Run->seg[i].V_max / Run->seg[i].S_a;
            else Run->k = (Run->seg[i].V_max - Run->last_speed) / Run->seg[i].S_a;
            return i;
        }
    } 
    if (Run->Distance < Run->seg[i].S_a && i == Run->n) {
        speed = Run->Distance * Run->k;
    } else if (Run->last_Distance < Run->seg[i].S_a) {
        if(i == 1) speed = Run->last_Distance * Run->k * 0.9f + Run->seg[i].V_max * 0.1f;
        else speed = Run->last_Distance * Run->k + Run->last_speed;
    } 
    else speed = Run->seg[i].V_max;
    
    float _cos = (Run->seg[i].point.X - Now.X) / Run->Distance;
    float _sin = (Run->seg[i].point.Y - Now.Y) / Run->Distance;
    
    float _vx = speed * _cos;
    float _vy = speed * _sin;
    
    SetSpeed(_vx, _vy);
    return 0;
}






