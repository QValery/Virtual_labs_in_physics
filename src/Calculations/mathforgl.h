#ifndef MATHFORGL_H
#define MATHFORGL_H

#include "lib3dstypes.h"
#include "math.h"

#define FLT_MAX         3.402823466e+38F

class MathForGL
{
public:

    /// Радиус описаной окружности или сферы
    static float circleExternalRad(float lenX, float lenY, float lenZ);
    static float circleExternalRad(vect3f v);

    /// Расстояние между двумя параллелепипедами.
    /// Каждый параллелепипед задан даумя точками: с минимальными координатами и с максимальными
    static float crossingParallelepiped(vect6f p1, vect6f p2);

    static float distanceModels(Model3ds model1, Model3ds model2, float eps=0.0001);

    static vect3f* edgesPointModelToPointsParallelepiped(vect6f edgP);

    /// Есть ли пересение параллелепипедов с центром center и радиусом описаной сферы radSphere
    static bool isCrossing(vect3f center, float radSphere, vect3f center2, float radSphere2);

    /// На сколько пикселей одна мдель пересекаед другую?
    /// Если возвращается отриц. число, есть пересечение
    /// Если положительное, то это есть расстояние между описанными окружностями моделей
    static float crossingOfModel3ds(Model3ds model1, Model3ds model2);

    /// Есть ли пересечение модели хотябы с 1 моделью из списка моделей?
    static bool isCrossing(QList<Model3ds> listMosels, Model3ds model);

    static vect2f projection2Dfor3D(int x, int y, float rotate);

    /// Вычисляет следующую отчку по графику из двух первых точек
    static vect2f next3PointOf2points(vect2f point1, vect2f point2, unsigned char point1OrPoint2, float distance);

    /// Вращение модели относительно точки
    static void rotationModel(Model3ds *model, vect3f center, float rotX,float rotY,float rotZ);
};

#endif // MATHFORGL_H
