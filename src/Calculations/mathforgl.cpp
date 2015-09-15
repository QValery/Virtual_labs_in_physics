#include "mathforgl.h"


float MathForGL::circleExternalRad(float lenX, float lenY, float lenZ)
{
    return sqrtf(lenX*lenX + lenY*lenY + lenZ*lenZ);
}

float MathForGL::circleExternalRad(vect3f v)
{
    return circleExternalRad(v.x, v.y, v.z);
}

float MathForGL::crossingParallelepiped(vect6f p1, vect6f p2)
{/*
    vect3f tempPoint;
    float minDist;
    float *tempMas1 = p1.mas(), *tempMas2 = p2.mas();
    int i, j;

    for (i=0; i<4; i++)
    {

    }*/
    return 0;
}

float MathForGL::distanceModels(Model3ds model1, Model3ds model2, float eps)
{
    float extreme_distance[64]; // потому что 8 вершин первого объекта и 8 шт от второго
    vect3f mid_p_ob1, mid_p_ob2;
    float min1_extreme_distance = FLT_MAX; // максимальное дабловское число
    float min2_extreme_distance = FLT_MAX;
    float min3_extreme_distance = FLT_MAX;
    int number_point_ob1[3]={0,0,0},number_point_ob2[3]={0,0,0}; // номера точек, между которыми находим минимум
    vect3f *extreme_points_ob1 = edgesPointModelToPointsParallelepiped(model1.edgesPoints());
    vect3f *extreme_points_ob2 = edgesPointModelToPointsParallelepiped(model2.edgesPoints());

    //эта неадекватная вещь находит 3 кратчайших расстояния от одного объекта до другого
    //это надо для нахождения самого-самого кратчайшего расстояния, описано будет дальше
    for (int i=0;i<8;i++)
        for (int k=0;k<8;k++)
        {
            extreme_distance[(i*k)+k] = extreme_points_ob1[i].distancePoint(extreme_points_ob2[k]);
            if (min1_extreme_distance>extreme_distance[(i*k)+k])
        {
            number_point_ob1[2]=number_point_ob1[1];//переопределение индексов точек, что бы их не потерять и потом обратится к ним, для получения координат
            number_point_ob1[1]=number_point_ob1[0];
            number_point_ob1[0]=i;
            number_point_ob2[2]=number_point_ob2[1];
            number_point_ob2[1]=number_point_ob2[0];
            number_point_ob2[0]=k;

            min3_extreme_distance=min2_extreme_distance;// аналогичное переопределение минимального расстояния
            min2_extreme_distance=min1_extreme_distance;
            min1_extreme_distance=extreme_distance[(i*k)+k];
        }
    }

    float mid_distance;

    //как раз та самая математика... здесь мы делим ребро на пополам(поэтому и называется mid_p - средняя точка) для первого и второго объектов
    //и мы придлижаемся постепенно к идеальному короткому значению этим методом. Это на матане когда то проходили...
    while (abs(min3_extreme_distance-min2_extreme_distance)>eps) //eps- погрешность
    {
    mid_p_ob1=(extreme_points_ob1[number_point_ob1[2]]+extreme_points_ob1[number_point_ob1[1]])/2;
    //или покоординатно, т.е. для каждой пары суммируем координаты и делим на 2, что бы найти середину.
    //складываем Х третьей точки первого объекта с Х второй точки первого объекта и делим на 2=> получаем середину ребра.
    //также для У и Z

    mid_p_ob2=(extreme_points_ob2[number_point_ob2[2]]+extreme_points_ob2[number_point_ob2[1]])/2;

    mid_distance = mid_p_ob1.distancePoint(mid_p_ob2); //расстояние между средними точками на объектах

    //здесь переприсваиваем точки, что бы не мешать программе работать в цикле исправно, если нет ошибки в знаке
    //но надо через отладку будет прогнать и посмотреть, что бы вершины, между которыми наибольшее расстояние,
    //заменились на точки середины ребер параллелепипеда
    if (mid_distance>min3_extreme_distance)
        {
         min3_extreme_distance=mid_distance;
         extreme_points_ob1[number_point_ob1[2]]=mid_p_ob1;
         extreme_points_ob2[number_point_ob2[2]]=mid_p_ob2;
        }
    else
        {
         min2_extreme_distance=mid_distance;
         extreme_points_ob1[number_point_ob1[1]]=mid_p_ob1;
         extreme_points_ob2[number_point_ob2[1]]=mid_p_ob2;
        }
    }
    //возможно это излишне, но это надо для работы программы
    min2_extreme_distance = mid_distance;
    extreme_points_ob1[number_point_ob1[1]]=mid_p_ob1;
    extreme_points_ob2[number_point_ob2[1]]=mid_p_ob2;

    // тоже самое с другим расстоянием, аналогично предыдущему алгоритму
    //вероятно можно сделать всё через цикл, представив эти точки через еще 1 массив, но это геморно и решено расписать.
    //для коомпилятора всё равно без разницы
    while (abs(min2_extreme_distance-min1_extreme_distance)>eps) //eps- погрешность
    {
    mid_p_ob1=(extreme_points_ob1[number_point_ob1[1]]+extreme_points_ob1[number_point_ob1[0]])/2;
    //или покоординатно, т.е. для каждой пары суммируем координаты и делим на 2, что бы найти середину.

    mid_p_ob2=(extreme_points_ob2[number_point_ob2[1]]+extreme_points_ob2[number_point_ob2[0]])/2;

    mid_distance = mid_p_ob1.distancePoint(mid_p_ob2); //расстояние между средними точками на объектах

    if (mid_distance>min2_extreme_distance)
        {
         min2_extreme_distance=mid_distance;
         extreme_points_ob1[number_point_ob1[1]]=mid_p_ob1;
         extreme_points_ob2[number_point_ob2[1]]=mid_p_ob2;
        }
    else
        {
         min1_extreme_distance=mid_distance;
         extreme_points_ob1[number_point_ob1[0]]=mid_p_ob1;
         extreme_points_ob2[number_point_ob2[0]]=mid_p_ob2;
        }
    }
    return mid_distance;
}

vect3f* MathForGL::edgesPointModelToPointsParallelepiped(vect6f edgP)
{
    vect3f *otv = new vect3f[8];
    vect3f min = edgP.x1y1z1(), max = edgP.xyz();
    otv[0] = min;
    otv[1] = vect3f(min.x, max.y, min.z);
    otv[2] = vect3f(max.x, max.y, min.z);
    otv[3] = vect3f(max.x, min.y, min.z);
    otv[4] = vect3f(min.x, min.y, max.z);
    otv[5] = vect3f(min.x, max.y, max.z);
    otv[6] = max;
    otv[7] = vect3f(max.x, min.y, max.z);
    return otv;
}

bool MathForGL::isCrossing(vect3f center, float radSphere, vect3f center2, float radSphere2)
{
    if (center.distancePoint(center2) > (radSphere + radSphere2)) return false;
    else return true;
}

float MathForGL::crossingOfModel3ds(Model3ds model1, Model3ds model2)
{
    float distanc = model1.center().distancePoint(model2.center());
    return distanc - model1.circleExternalRad() - model2.circleExternalRad();
}

bool MathForGL::isCrossing(QList<Model3ds> listMosels, Model3ds model)
{
    for (int i = 0; i<listMosels.size(); i++)
    {
        if (crossingOfModel3ds(listMosels.at(i), model) < 0) return true;
    }
    return false;
}

vect2f MathForGL::projection2Dfor3D(int x, int y, float rotate)
{
    vect2f coord2d;

    double tempd = cos(double(rotate));
    if (tempd == 0) coord2d.x = x;
    else coord2d.x = x/float(tempd);

    coord2d.y = sqrtf(coord2d.x*coord2d.x - x*x);
    if (rotate>0)
    {
        coord2d.y = y - coord2d.y;
    }
    else
    {
        coord2d.y = y + coord2d.y;
    }

    /*
    if (rotate>0)
    {
        coord2d.x = x*cos(double(rotate)) + y*sin(double(rotate));
        coord2d.y = -1*x*sin(double(rotate)) + y*cos(double(rotate));
    }
    else
    {
        coord2d.x = x*cos(double(rotate)) - y*sin(double(rotate));
        coord2d.y = x*sin(double(rotate)) + y*cos(double(rotate));
    }
*/
    return coord2d;
}

vect2f MathForGL::next3PointOf2points(vect2f point1, vect2f point2, unsigned char point1OrPoint2, float distance)
{
    float tangens, b;
    if (point2.y > point1.y)
    {
        tangens = (point2.y - point1.y)/(point2.x - point1.x);
        b = point2.y - tangens*point2.x;
    }
    else
    {
        tangens = (point1.y - point2.y)/(point1.x - point2.x);
        b = point1.y - tangens*point1.x;
    }
    vect2f v;
    return v;
}

void MathForGL::rotationModel(Model3ds *model, vect3f center, float rotX, float rotY, float rotZ)
{
    int i, j;
    QList<vect3f> vert;
    for (i=0; i<model->meshs.size(); i++)
    {
        vert = model->meshs[i].vertex;
        for (j=0; j<vert.size(); j++)
        {
            if (rotX != 0)
            {
                if (vert.at(j).x >= center.x) 			//здесь учитываем положение точки вращения относительно центра
                    vert[j].x = (vert.at(j).x - center.x)*cos(rotX); //объекта, а следовательно и радиус вращения. аналогично для
                else					//других осей
                    vert[j].x = (center.x - vert.at(j).x)*cos(rotX);
            }

            if (rotY != 0)
            {
                if (vert.at(j).y >= center.y)
                    vert[j].y=(vert.at(j).y-center.y)*cos(rotY);
                else
                    vert[j].y = (center.y - vert.at(j).y)*cos(rotY);
            }

            if (rotZ != 0)
            {
                if (vert.at(j).z >= center.z)
                    vert[j].z=(vert.at(j).z-center.z)*cos(rotZ);
                else
                    vert[j].z=(center.z - vert.at(j).z)*cos(rotZ);
            }
        }
        model->meshs[i].vertex = vert;
    }
}
