#ifndef MODELWORK_H
#define MODELWORK_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include "lib3d.h"
#include "lib3dstypes.h"
#include "Calculations/mathforgl.h"
#include "Constants.h"
#include "pindevicecostants.h"

#include <QDebug>

class ModelWork
{
public:
    ModelWork();
    ~ModelWork();
    void clear();
    void setModelPath(QString path);
    void setTexturePath(QString path);

    bool loadModels(QStringList files);
    bool loadModel(QString fileName);

    ///Создаёт список ссылок на файлы 3D моделей
    /// Усли name = true, то выдаст список просто имён
    /// 1. Стол
    /// 2. Амперметер
    /// 3. Вольтметер
    /// 4. Лампа
    /// 5. Выключатель
    /// 6. Акумулятор
    /// 7. Провод
    static QStringList list3dsFiles(bool name = false);

    static PinDeviceConstants::PinOfDevice toConstantsDevice(QString meshName);

    static QString toMeshName(PinDeviceConstants::PinOfDevice pin);

    static PinDeviceConstants::PinType toPinType(PinDeviceConstants::PinOfDevice pin);

    /// Возвращает ближайшую модель от выбранной точки в радиусе interval
    static int nearestModel3ds(vect3f point, QList<Model3ds> models, float interval = 10);


    QList<Model3ds> models() const;

    void setCorrect(bool fileTexture, bool tableStartCoordinats = false, bool devicesOnTable = false);

    vect6f calculateEdgesPointsOfTables() const;

    float radSphere() const;

    /// Если номер мэша это контакт какого либо девайса,
    /// то true
    bool isPinOfDevices(int numberMesh) const;

    PinDeviceConstants::PinOfDevice toConstantsDevice(int numberMesh) const;

    Lib3dsMesh lib3dsMesh(QString meshName) const;

    Lib3dsMesh lib3dsMesh(int numberMesh) const;

    bool isDevice(int numberModel) const;

    int nearestModel3ds(vect3f point, float interval = 10) const;


private:
    unsigned char _error;

    QString _texturePath;
    QString _modelPath;
    QList<Model3ds> _listModel3ds;

    Model3ds load3ds(QString fileName);

    void replaceStrFileTextures(Model3ds *model);

    /// Изменение координат для нужного отображения
    void putTheDevicesOnTable();
};

#endif // MODELWORK_H
