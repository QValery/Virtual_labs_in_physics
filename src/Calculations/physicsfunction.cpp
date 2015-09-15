#include "physicsfunction.h"

PhysicsFunction::PhysicsFunction()
{
    reset();
}

void PhysicsFunction::reset()
{
    _accuracyV = 0;
    _accuracyA = 0;
    _resistanceV = 29;
    _resistanceA = 0.237;
}

void PhysicsFunction::setAccuracy(float V, float A)
{
    if (V>0) _accuracyV = V;
    if (A>0) _accuracyA = A;
}

void PhysicsFunction::setResistance(float A, float V)
{
    if (A>0) _resistanceA = A;
    if (V>0) _resistanceV = V;
}

float PhysicsFunction::amperageReal(float EDS, float R)
{
    return EDS/(R + _resistanceA);
}

/*--------------Статические функции--------------*/

float PhysicsFunction::amperage(float U, float R)
{
    return U/R;
}

float PhysicsFunction::voltage(float I, float R)
{
    return I*R;
}

float PhysicsFunction::resistance(float I, float U)
{
    return U/I;
}

float PhysicsFunction::resistanceWires(float g, float l, float S)
{
    return g*l/S;
}

float PhysicsFunction::countOfHeat(float I, float R, float t)
{
    return I*I*R*t;
}
