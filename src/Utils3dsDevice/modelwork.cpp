#include "modelwork.h"

using namespace PinDeviceConstants;

ModelWork::ModelWork()
{
    _error = 0;
}

ModelWork::~ModelWork()
{
    clear();
}

void ModelWork::clear()
{
    _error = 0;
    _texturePath.clear();
    _modelPath.clear();
    _listModel3ds.clear();
}

void ModelWork::setTexturePath(QString path)
{
    if (path.isEmpty()) return;
    if (path.at(path.size()-1) == '/') path.remove(path.size()-1,1);
    _texturePath = path;
}

void ModelWork::setModelPath(QString path)
{
    if (path.isEmpty()) return;
    if (path.at(path.size()-1) == '/') path.remove(path.size()-1,1);
    _modelPath = path;
    if (_texturePath.isEmpty()) _texturePath = _modelPath;
}

QList<Model3ds> ModelWork::models() const
{
    return _listModel3ds;
}

bool ModelWork::loadModels(QStringList files)
{
    if (files.isEmpty()) return false;
    int i;
    Model3ds tempModel;
    if (!_modelPath.isEmpty()) _modelPath+='/';
    for (i=0; i<files.size(); i++)
    {
        tempModel = load3ds(_modelPath + files.at(i));
        if (tempModel.meshs.isEmpty())
        {
            continue;
        }
        else _listModel3ds.append(tempModel);
    }
    if (_listModel3ds.isEmpty()) return false;
    return true;
}

bool ModelWork::loadModel(QString fileName)
{
    if (fileName.isEmpty()) return false;
    Model3ds tempModel;
    if (!_modelPath.isEmpty()) _modelPath+='/';
    tempModel = load3ds(_modelPath + fileName);
    if (tempModel.meshs.isEmpty()) return false;
    else _listModel3ds.append(tempModel);
    return true;
}


QStringList ModelWork::list3dsFiles(bool name)
{
    QStringList list;
    list.append("space.3ds");       // 0
    list.append("ampermeter.3ds");  // 1
    list.append("voltmeter.3ds");   // 2
    list.append("lamp.3ds");        // 3
    list.append("switcher.3ds");    // 4
    list.append("battery.3ds");     // 5
    list.append("wire.3ds");        // 6
    list.append("table.3ds");       // 7
    if (!name) return list;
    QFileInfo inf;
    for (int i=0; i<list.size(); i++)
    {
        inf.setFile(list.at(i));
        list[i] = inf.baseName();
    }
    return list;
}

PinDeviceConstants::PinOfDevice ModelWork::toConstantsDevice(QString meshName)
{
    if (meshName == "BATTERY_PLUS") return PinBattery_Plus;
    if (meshName == "BATTERY_MINUS") return PinBattery_Minus;
    if (meshName == "SWITCH_LEFT") return PinSwitch_Left;
    if (meshName == "SWITCH_RIGHT") return PinSwitch_Right;
    if (meshName == "RHEOSTAT_LEFT") return PinRheostat_Left;
    if (meshName == "RHEOSTAT_RIGHT") return PinRheostat_Right;
    if (meshName == "AMPERMETER_PLUS") return PinAmpermeter_Plus;
    if (meshName == "AMPERMETER_MINUS") return PinAmpermeter_Minus;
    if (meshName == "VOLTMETER_PLUS") return PinVoltmeter_Plus;
    if (meshName == "VOLTMETER_MINUS") return PinVoltmeter_Minus;
    if (meshName == "LAMP_LEFT") return PinLamp_Left;
    if (meshName == "LAMP_RIGHT") return PinLamp_Right;
    return PinOfDeviceNoInfo;
}

QString ModelWork::toMeshName(PinOfDevice pin)
{
    if (pin == PinBattery_Plus) return "BATTERY_PLUS";
    if (pin == PinBattery_Minus) return "BATTERY_MINUS";
    if (pin == PinSwitch_Left) return "SWITCH_LEFT";
    if (pin == PinSwitch_Right) return "SWITCH_RIGHT";
    if (pin == PinRheostat_Left) return "RHEOSTAT_LEFT";
    if (pin == PinRheostat_Right) return "RHEOSTAT_RIGHT";
    if (pin == PinAmpermeter_Plus) return "AMPERMETER_PLUS";
    if (pin == PinAmpermeter_Minus) return "AMPERMETER_MINUS";
    if (pin == PinVoltmeter_Plus) return "VOLTMETER_PLUS";
    if (pin == PinVoltmeter_Minus) return "VOLTMETER_MINUS";
    if (pin == PinLamp_Left) return "LAMP_LEFT";
    if (pin == PinLamp_Right) return "LAMP_RIGHT";
    return QString();
}

PinDeviceConstants::PinType ModelWork::toPinType(PinDeviceConstants::PinOfDevice pin)
{
    if ((pin == PinAmpermeter_Minus)||(pin == PinBattery_Minus)||(pin == PinLamp_Left)||
            (pin == PinRheostat_Left)||(pin == PinSwitch_Left)||(pin == PinVoltmeter_Minus))
        return PinMinus;
    else if (pin == PinOfDeviceNoInfo) return PinTypeNoInfo;
    else return PinPlus;
}


void ModelWork::setCorrect(bool fileTexture, bool tableStartCoordinats, bool devicesOnTable)
{
    if (_listModel3ds.isEmpty()) return;
    int i;
    if ((fileTexture)&&(!_texturePath.isEmpty()))
    {
        for (i=0; i < _listModel3ds.size(); i++)
        {
            replaceStrFileTextures(&_listModel3ds[i]);
        }
    }
    if (tableStartCoordinats)
    {
        QStringList listModels = list3dsFiles(true);
        int tableNumber = listModels.size() - 1;
        vect3f centrSpace;
        for (i=0; i<_listModel3ds.size(); i++)
        {
            if (_listModel3ds.at(i).name == listModels.at(0))
            {
                _listModel3ds[i].alignmentForCoordinat(true,true,true);
                centrSpace = _listModel3ds.at(i).center();
                centrSpace.y = lib3dsMesh("Pol").edgesMaxPoints().y;
            }
            if (_listModel3ds.at(i).name == listModels.at(tableNumber))
            {
                _listModel3ds[i].alignmentForCoordinat(true,true,true);
                _listModel3ds[i].offset(centrSpace);
            }
        }
    }
    if (devicesOnTable) putTheDevicesOnTable();
}

vect6f ModelWork::calculateEdgesPointsOfTables() const
{
    QString tableName = list3dsFiles(true).last();
    for (int i=0; i<_listModel3ds.size(); i++)
    {
        if (_listModel3ds.at(i).name == tableName)
        {
            return _listModel3ds.at(i).edgesPoints();
        }
    }
    return vect6f();
}

float ModelWork::radSphere() const
{
    if (_listModel3ds.isEmpty()) return 0;
    int i;
    vect3f tempfv, size;

    size = _listModel3ds.at(0).size();

    for (i = 1; i<_listModel3ds.size(); i++)
    {
        tempfv = _listModel3ds.at(i).size();
        if (tempfv.x > size.x) size.x = tempfv.x;
        if (tempfv.y > size.y) size.y = tempfv.y;
        if (tempfv.z > size.z) size.z = tempfv.z;
    }
    return MathForGL::circleExternalRad(size);
}

bool ModelWork::isPinOfDevices(int numberMesh) const
{
    register int i, j, count = 0;
    PinOfDevice partDevices;
    for (i=0; i < _listModel3ds.size(); i++)
    {
        for (j=0; j < _listModel3ds.at(i).meshs.size(); j++)
        {
            if (numberMesh == count)
            {
                partDevices = toConstantsDevice(_listModel3ds.at(i).meshs.at(j).name);
                if (partDevices == PinOfDeviceNoInfo) return false;
                else return true;
            }
            count++;
        }
    }
    return false;
}

PinOfDevice ModelWork::toConstantsDevice(int numberMesh) const
{
    return toConstantsDevice(this->lib3dsMesh(numberMesh).name);
}

Lib3dsMesh ModelWork::lib3dsMesh(QString meshName) const
{
    register int i, j;
    for (i=0; i < _listModel3ds.size(); i++)
    {
        for (j=0; j < _listModel3ds.at(i).meshs.size(); j++)
        {
            if (meshName == _listModel3ds.at(i).meshs.at(j).name)
            {
                return _listModel3ds.at(i).meshs.at(j);
            }
        }
    }
    return Lib3dsMesh();
}

Lib3dsMesh ModelWork::lib3dsMesh(int numberMesh) const
{
    register int i, j, count = 0;
    for (i=0; i < _listModel3ds.size(); i++)
    {
        for (j=0; j < _listModel3ds.at(i).meshs.size(); j++)
        {
            if (numberMesh == count)
            {
                return _listModel3ds.at(i).meshs.at(j);
            }
            count++;
        }
    }
    return Lib3dsMesh();
}

bool ModelWork::isDevice(int numberModel) const
{
    if ((numberModel < 0)||(numberModel > _listModel3ds.size())) return false;
    QStringList listModels = list3dsFiles(true);
    for (int i=1; i<listModels.size() - 1; i++)
    {
        if (_listModel3ds.at(numberModel).name == listModels.at(i)) return true;
    }
    return false;
}

int ModelWork::nearestModel3ds(vect3f point, float interval) const
{
/*    float minDist = MathForGL::
    for (int i=0; i<_listModel3ds.size(); i++)
    {

    }*/
    return 0;
}

/*----------------- Приватный методы -----------------*/

Model3ds ModelWork::load3ds(QString fileName)
{
    Model3ds model;
    if (fileName.isEmpty()) return model;
    QFileInfo inf(fileName);
    if (!inf.exists()) return model;
    Lib3ds file3ds(fileName);
    if (!file3ds.load())
    {
        _error = file3ds.errorType();
        return model;
    }
    model = file3ds.model();
    file3ds.clear();
    model.name = inf.baseName();
    return model;
}

void ModelWork::replaceStrFileTextures(Model3ds *model)
{
    if (_texturePath.isEmpty()) return;
    if (model->materials.isEmpty()) return;
    int i;
    QFileInfo inf;
    QString tempStr;
    for (i=0; i<model->materials.size(); i++)
    {
        if (model->materials.at(i).strFile.isEmpty()) continue;
        tempStr = _texturePath+'/'+model->materials.at(i).strFile;
        inf.setFile(tempStr);
        if (!inf.exists())
        {
            model->materials[i].strFile.clear();
        }
        else
        {
            model->materials[i].strFile = tempStr;
        }
    }
}

void ModelWork::putTheDevicesOnTable()
{
    if (_listModel3ds.isEmpty()) return;
    QStringList listModelsName = list3dsFiles(true);
    QString tableName = listModelsName.last();
    QString spaceName = listModelsName.at(0);
    listModelsName.clear();
    int i;
    vect3f temp3F;
    for (i=0; i<_listModel3ds.size(); i++)
    {
        if (_listModel3ds.at(i).name == tableName)
        {
            temp3F = _listModel3ds.at(i).center();
            temp3F.y = _listModel3ds.at(i).edgesMaxPoints().y;
            break;
        }
    }
    if (temp3F == _listModel3ds.at(i).edgesMaxPoints()) return;
    for (i=0; i<_listModel3ds.size(); i++)
    {
        if ((_listModel3ds.at(i).name == tableName)||
            (_listModel3ds.at(i).name == spaceName)) continue;
        _listModel3ds[i].alignmentForCoordinat(true,true,true);
        _listModel3ds[i].offset(temp3F);
    }
}

/*-------------------------------------------------------*/
