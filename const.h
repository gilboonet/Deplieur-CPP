#ifndef CONST_H
#define CONST_H

const double epsilon = 0.5;
const double dMin = -999999;
const double dMax =  999999;

int prec (const int n);
int suiv (const int n);
double calc_angle (Vec2 a, Vec2 b, Vec2 c);
bool li (Vec2 l1S, Vec2 l1E, Vec2 l2S, Vec2 l2E);

#endif // CONST_H
