#ifndef PHYSICSFUNCTION_H
#define PHYSICSFUNCTION_H

class PhysicsFunction
{
public:
    PhysicsFunction();

    //Установка точности приборов вольтметра и амперметра
    void setAccuracy(float V, float A);

    //Установка сопротивления приборов
    void setResistance(float A = -1, float V = -1);

    void reset();

    float amperageReal(float EDS, float R);


    static float amperage(float U, float R);

    static float voltage(float I, float R);

    static float resistance(float I, float U);

    //Сопротивление проводов
    static float resistanceWires(float g, float l, float S);

    //Колличество теплоты
    static float countOfHeat(float I, float R, float t);

private:

    float _accuracyV;
    float _accuracyA;
    float _resistanceV;
    float _resistanceA;
};

#endif // PHYSICSFUNCTION_H
