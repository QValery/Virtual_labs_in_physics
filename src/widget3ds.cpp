#include "widget3ds.h"

Widget3ds::Widget3ds(QWidget* pwgt) : QGLWidget(pwgt)
{
    _scale = true;
    _selectedDevice = -1;
    _isSelectedWire = false;
    _navigationWidget = false;
    _mouseStatusMove = NoInfoMove;
    _tempPinDevice = 0;
    _stepRotationScene = 2;
    this->setAutoBufferSwap(false);
//    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);

//    QFile::remove("E:/вуз/Дипломник/физика & математика/Орбита для камеры/coord.txt");
}

Widget3ds::~Widget3ds()
{
    clear();
}

void Widget3ds::clear()
{
    _scale = true;
    _selectedDevice = -1;
    _stepRotationScene = 2;
    _isSelectedWire = false;
    _navigationWidget = false;
    _mouseStatusMove = NoInfoMove;
    _tempPinDevice = 0;
    _listsComplete.append(_listWires.listsCompleteWires);
    _listsComplete.append(_listsCompleteForSelectBuf);
    QList<unsigned int> texturList;
    register int i, j;
    for (i=0; i<_listsComplete.size(); i++)
    {
        glDeleteLists(_listsComplete.at(i),1);
    }
    for (i=0; i<_model3ds.size(); i++)
    {
        for (j=0; j < _model3ds.at(i).materials.size(); j++)
        {
            if (_model3ds.at(i).materials.at(j).strFile.isEmpty()) continue;
            texturList.append(_model3ds.at(i).materials.at(j).texureID);
        }
    }
    if (!texturList.isEmpty())
    {
        GLuint *text = new GLuint[texturList.size()];
        for (i=0; i < texturList.size(); i++)
        {
            text[i] = texturList.at(i);
        }
        glDeleteTextures(texturList.size(), text);
        delete[] text;
    }
    texturList.clear();
    _colorBuf.clear();
    _listsComplete.clear();
    _listWires.listsCompleteWires.clear();
    _listWires.dev1.clear();
    _listWires.dev2.clear();
    _listsCompleteForSelectBuf.clear();
    _limitsCoordinatDevices.clear();
    _model3ds.clear();
    _modelWork.clear();
    _camera.clear();

    setNavigationWidget(false);
}

void Widget3ds::redrawScene()
{
    clear();
    initializeGL();
    updateDispley();
}

void Widget3ds::initializeGL()
{
    qglClearColor(Qt::white);
    //z буфер(глубина)
    glEnable(GL_DEPTH_TEST);
    //Приведение нормалей к единичной длине
    glEnable(GL_NORMALIZE);
    // включаем мультивыборку
//    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    //Если мы, не включили закрашивание материала, текстуры всегда будут своего первоначального цвета.
    //glColor3f(r,g,b) не будет действовать на расцвечивание
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Сглаживание полигонов
//    glEnable(GL_POLYGON_SMOOTH);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    _modelWork.setModelPath("./device");
    _modelWork.setTexturePath("./device/textures1");
    if (!_modelWork.loadModels(ModelWork::list3dsFiles())) return;
    _modelWork.setCorrect(true, true, true);
    _model3ds = _modelWork.models();
    if (_model3ds.size() != ModelWork::list3dsFiles().size())
    {
        qDebug()<<"Загружено "<<_model3ds.size()<<" файлов из "<<ModelWork::list3dsFiles().size();
//        exit(20);
    }
    _radSphere = _modelWork.radSphere();
    setDefaultCameraSettings();
    allocationDevice();
    loadTextures();
    drawModels();
    _limitsCoordinatDevices = _modelWork.calculateEdgesPointsOfTables();
}

void Widget3ds::resizeGL(int nWidth, int nHeight)
{
    if (_model3ds.isEmpty()) return;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1*nWidth/2, nWidth, -1*nHeight/2, nHeight, -500, 10000.0);

    vect3f eye = _camera.eye(), center = _camera.center();
//    qDebug()<<eye.x<<eye.y<<eye.z;
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, 0,1,0);
    emit cameraPosition(vect6f(eye, center));

    // извлекаем матрицу поля просмотра в viewport
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glGetIntegerv(GL_VIEWPORT, _viewport);
}

void Widget3ds::paintGL()
{
    if (_model3ds.isEmpty()) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int sizeList = _listsComplete.size() + _listWires.listsCompleteWires.size();
    GLuint *tempList = new GLuint[sizeList];
    register int i;
    for (i=0; i<_listsComplete.size(); i++)
    {
        tempList[i] = _listsComplete.at(i);
    }
    for (int j=0; j<_listWires.listsCompleteWires.size(); j++)
    {
        tempList[j] = _listWires.listsCompleteWires.at(j);
    }
    glCallLists(sizeList, GL_UNSIGNED_INT, tempList);
    delete[] tempList;
    this->swapBuffers();
}

void Widget3ds::updateDispley()
{
    resizeGL(this->width(), this->height());
    updateGL();
//    updateSelectBuffers();
}

/*---------------- Выбор объекта ---------------------------*/

int Widget3ds::selectObject(QPoint p)
{
    if (_model3ds.isEmpty()) return -1;

    unsigned char poscol[3];

    glReadPixels(p.x(),_viewport[3]-p.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, poscol);

    int tempI = _colorBuf.select(poscol);
    if (tempI != -1)
    {
        return tempI;
    }
    else
    {
        return -1;
    }
}

void Widget3ds::updateSelectBuffers()
{
    if (_model3ds.isEmpty()) return;
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resizeGL(this->width(), this->height());

    GLuint *listsComplete = new GLuint[_listsCompleteForSelectBuf.size()];

    for (register int i=0; i<_listsCompleteForSelectBuf.size(); i++)
    {
        listsComplete[i] = _listsCompleteForSelectBuf.at(i);
    }

    glCallLists(_listsCompleteForSelectBuf.size(), GL_UNSIGNED_INT, listsComplete);

    delete[] listsComplete;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void Widget3ds::updateListCompleteSelectBuf(bool intoWire)
{
    int i;
    for (i=0; i<_listsCompleteForSelectBuf.size(); i++)
    {
        glDeleteLists(_listsCompleteForSelectBuf.at(i), 1);
    }

    _listsCompleteForSelectBuf.clear();
    _colorBuf.clear();
    QStringList listNames = ModelWork::list3dsFiles(true);
    QString tableName, spaceName;
    tableName = listNames.last();
    spaceName = listNames.at(0);
    if (intoWire) listNames.clear();
    for (i=0; i<_model3ds.size(); i++)
    {
        if ((_model3ds.at(i).name == tableName)||(_model3ds.at(i).name == spaceName)) continue;
        if (!intoWire)
        {
            if (_model3ds.at(i).name == listNames.at(6)) continue;
        }
        _colorBuf.pushGroupObjectName(i);
        _listsCompleteForSelectBuf.append(drawModelOnlyFigure(_model3ds.at(i), &_colorBuf));
        _colorBuf.endGroupNames();
    }
}

GLuint Widget3ds::drawModelOnlyFigure(Model3ds model, ColorBuffer *buf)
{
    if (model.meshs.isEmpty()) return false;
    GLuint n = glGenLists(1);
    int i, j;
    word index;
    vect3uch color;

    glNewList(n, GL_COMPILE);
        glBegin(GL_TRIANGLES);

            for (i=0; i<model.meshs.size(); i++)
            {
                color = buf->pushRandColor(model.meshs.at(i).name);
                glColor3ub(color.r, color.g, color.b);
                for (j=0; j<model.meshs.at(i).faces.size(); j++)
                {
                    index = model.meshs.at(i).faces.at(j).index.A;
                    glNormal3f(GLfloat(model.meshs.at(i).normals.at(index).x), GLfloat(model.meshs.at(i).normals.at(index).y), GLfloat(model.meshs.at(i).normals.at(index).z));
                    glVertex3f(GLfloat(model.meshs.at(i).vertex.at(index).x), GLfloat(model.meshs.at(i).vertex.at(index).y), GLfloat(model.meshs.at(i).vertex.at(index).z));

                    index = model.meshs.at(i).faces.at(j).index.B;
                    glNormal3f(GLfloat(model.meshs.at(i).normals.at(index).x), GLfloat(model.meshs.at(i).normals.at(index).y), GLfloat(model.meshs.at(i).normals.at(index).z));
                    glVertex3f(GLfloat(model.meshs.at(i).vertex.at(index).x), GLfloat(model.meshs.at(i).vertex.at(index).y), GLfloat(model.meshs.at(i).vertex.at(index).z));

                    index = model.meshs.at(i).faces.at(j).index.C;
                    glNormal3f(GLfloat(model.meshs.at(i).normals.at(index).x), GLfloat(model.meshs.at(i).normals.at(index).y), GLfloat(model.meshs.at(i).normals.at(index).z));
                    glVertex3f(GLfloat(model.meshs.at(i).vertex.at(index).x), GLfloat(model.meshs.at(i).vertex.at(index).y), GLfloat(model.meshs.at(i).vertex.at(index).z));
                }
            }

        glEnd();

    glEndList();

    return n;
}

/*-----------------------------------------------------*/

/*-------------------- Навигация ----------------------*/

void Widget3ds::mousePressEvent(QMouseEvent* pe)
{
    if (_listWires.listsCompleteWires.isEmpty())
        updateListCompleteSelectBuf();
    switch(pe->button())
    {
    case Qt::MidButton:
        _mouseStatusMove = RotateScene;
        updateSelectBuffers();
        _selectedDevice = _colorBuf.selectOfGroup(selectObject(pe->pos()));
        if (!_modelWork.isDevice(_selectedDevice))
        {
            _selectedDevice = -1;
            _mouseStatusMove = NoInfoMove;
            return;
        }
        break;
    case Qt::LeftButton:
        _mouseStatusMove = DeviceMoving;
        updateSelectBuffers();
        _selectedDevice = _colorBuf.selectOfGroup(selectObject(pe->pos()));
        break;
    default: _mouseStatusMove = NoInfoMove; return;
    }

    _positionPoint = pe->pos();
}

void Widget3ds::mouseReleaseEvent(QMouseEvent *pe)
{
    if (_mouseStatusMove == DeviceMoving)
    {
        updateListCompleteSelectBuf();
        updateSelectBuffers();
        int sel = selectObject(pe->pos());
        if (_modelWork.isPinOfDevices(sel))
        {
            if (_wireConnections != NULL)
                _wireConnections->connectDeviceDoubleStep(_modelWork.toConstantsDevice(sel), _modelWork.lib3dsMesh(sel).center());
        }
    }
    _mouseStatusMove = NoInfoMove;
}

void Widget3ds::wheelEvent(QWheelEvent *e)
{
    return;
    if (!_scale) return;
    if (e->delta()>0)
    {
//        _loop = _loop*2;
    }
    else
    {
//        _loop = _loop/2;
    }
    resizeGL(this->width(), this->height());
    updateGL();
}

void Widget3ds::mouseMoveEvent(QMouseEvent* pe)
{
    if (_mouseStatusMove == NoInfoMove)
    {
        PinDeviceConstants::PinType type = ModelWork::toPinType(_modelWork.toConstantsDevice(selectObject(pe->pos())));
        if (type == PinDeviceConstants::PinMinus)
        {
            if (this->cursor().shape() != Qt::BitmapCursor)
                this->setCursor(CursorModification::cursor(CursorModification::CursorTypeMinus));
        }
        else if (type == PinDeviceConstants::PinPlus)
        {
            if (this->cursor().shape() != Qt::BitmapCursor)
                this->setCursor(CursorModification::cursor(CursorModification::CursorTypePlus));
        }
        else
        {
            if (this->cursor().shape() != Qt::ArrowCursor)
                this->setCursor(QCursor(Qt::ArrowCursor));
        }

        return;
    }
    QPoint tempPoint = pe->pos();
//    vect2f temp3f, temp3f2;
    switch(_mouseStatusMove)
    {
    case RotateScene:
//        float _xRotate = 180 * (GLfloat)(pe->y() - _positionPoint.y()) / height();
//        _yRotate += 180 * (GLfloat)(pe->x() - _positionPoint.x()) / width();
        rotateDevice(_selectedDevice, 0, 180 * (GLfloat)(pe->x() - _positionPoint.x()) / width(),0);
//        rotateDevice(_selectedDevice, 0, 0,25);
        break;

    case DeviceMoving:
        if (_selectedDevice < 0) return;
//        temp3f = MathForGL::projection2Dfor3D(_positionPoint.x()-tempPoint.x(), tempPoint.y() - _positionPoint.y(), _xRotate, _yRotate);
//        temp3f = MathForGL::projection2Dfor3D(_positionPoint.x(), _positionPoint.y(), 0);
//        temp3f2 = MathForGL::projection2Dfor3D(tempPoint.x(), tempPoint.y(), 0);
        offsetDevice(_selectedDevice, _positionPoint.x()-tempPoint.x(), 0, tempPoint.y()-_positionPoint.y());
        break;
    default: return;
    }

    updateGL();
    _positionPoint = tempPoint;
}

void Widget3ds::keyPressEvent(QKeyEvent *k)
{
    switch(k->key())
    {
    case Qt::Key_Left:
        navigationButtonClickedLeftEye();
        break;
    case Qt::Key_Right:
        navigationButtonClickedRightEye();
        break;
    case Qt::Key_Up:
        navigationButtonClickedUpEye();
        break;
    case Qt::Key_Down:
        navigationButtonClickedDownEye();
        break;
    default: return;
    }
}

void Widget3ds::setDefaultCameraSettings()
{
    QString tableName = ModelWork::list3dsFiles(true).last();
    vect3f temp3f, temp3f2;
    int i;
    for (i=0; i<_model3ds.size(); i++)
    {
        if (_model3ds.at(i).name == tableName)
        {
            temp3f = _model3ds.at(i).center();
            temp3f.y = _model3ds.at(i).edgesMaxPoints().y;
            _camera.setCenter(temp3f);

            _camera.setEye(0,temp3f.y+200,0);

            temp3f = _model3ds.at(i).edgesMinPoints();
            temp3f.y = _model3ds.at(i).edgesMaxPoints().y - 100;
            temp3f2 = _model3ds.at(i).edgesMaxPoints();
            temp3f2.y += 500;
            _camera.setEdgesPoint(temp3f, temp3f2);
            break;
        }
    }
}

/*-------------------------------------------------------*/

/*------------------ Настройки --------------------------*/

void Widget3ds::setScale(bool on)
{
    _scale = on;
}

void Widget3ds::setStepRotationScene(int step)
{
    _stepRotationScene = step;
}

void Widget3ds::setWiresConnectionClass(WireConnection *w)
{
    if (_wireConnections != NULL)
    {
        disconnect(_wireConnections,SIGNAL(connectedDeviceOfDoubleStep(vect3f,vect3f,PinDeviceConstants::PinType)),
                    this,SLOT(connectDevice(vect3f,vect3f,PinDeviceConstants::PinType)));
        disconnect(_wireConnections,SIGNAL(disConnectedDevice(PinDeviceConstants::PinOfDevice,PinDeviceConstants::PinOfDevice)),
                   this,SLOT(disconnectDevice(PinDeviceConstants::PinOfDevice,PinDeviceConstants::PinOfDevice)));
    }
    _wireConnections = w;
    if (w == NULL) return;
    connect(_wireConnections,SIGNAL(connectedDeviceOfDoubleStep(vect3f,vect3f,PinDeviceConstants::PinType)),
            this,SLOT(connectDevice(vect3f,vect3f,PinDeviceConstants::PinType)));
    connect(_wireConnections,SIGNAL(disConnectedDevice(PinDeviceConstants::PinOfDevice,PinDeviceConstants::PinOfDevice)),
            this,SLOT(disconnectDevice(PinDeviceConstants::PinOfDevice,PinDeviceConstants::PinOfDevice)));
}

QString Widget3ds::toString()
{
    const GLubyte* str = glGetString(GL_VERSION);
    const GLubyte* str2 = glGetString(GL_VENDOR);
    const GLubyte* str3 = glGetString(GL_RENDERER);
    QString strvers;
    int i;
    strvers.append("Версия: ");
    for (i=0; i<256; i++)
    {
        if (str[i]!='\0') strvers.append(QChar::fromLatin1(str[i]));
        else break;
    }
    strvers.append("\nПоставщик: ");
    for (i=0; i<256; i++)
    {
        if (str2[i]!='\0') strvers.append(QChar::fromLatin1(str2[i]));
        else break;
    }
    strvers.append("\nТорговая марка: ");
    for (i=0; i<256; i++)
    {
        if (str3[i]!='\0') strvers.append(QChar::fromLatin1(str3[i]));
        else break;
    }
    delete[] str;
    delete[] str2;
    delete[] str3;
//    qDebug()<<QGLFormat::openGLVersionFlags();
    return strvers;
}

/*--------------------------------------------------------*/

/*------------------- Прорисовка моделей -----------------*/

void Widget3ds::loadTextures()
{
    if (_model3ds.isEmpty()) return;
    int i;
    for (i=0; i<_model3ds.size(); i++)
    {
        loadTexture(&_model3ds[i]);
    }
}

bool Widget3ds::loadTexture(Model3ds *model)
{
    if (model->materials.isEmpty()) return false;
    register int i;
    unsigned int temp;
    for (i=0; i<model->materials.size(); i++)
    {
        if (!model->materials.at(i).strFile.isEmpty())
        {
            temp = loadTextureData(model->materials.at(i));
            if (temp == 0) continue;
            model->materials[i].texureID = temp;
        }
    }
    return true;
}

GLuint Widget3ds::loadTextureData(MaterialInfo3ds mat)
{
    if (mat.strFile.isEmpty()) return 0;
    QImage img;
    QFileInfo info;
    info.setFile(mat.strFile);
    if (!info.exists()) return 0;
    if (!img.load(mat.strFile)) return 0;
    img=QGLWidget::convertToGLFormat(img);
    GLuint id;
    id = bindTexture(img, GL_TEXTURE_2D);

    // создание имён для текстурных объектов
//    glGenTextures(1, &id);
    // создаём и связываем текстурные объекты с состоянием текстуры
//    glBindTexture(GL_TEXTURE_2D, id);
    // связываем текстурный объект с изображением
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)img.width(), (GLsizei)img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.bits());
    // дополнительные параметры текстурного объекта

    // задаём линейную фильтрацию вблизи:
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // задаём линейную фильтрацию вдали:
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // задаём: цвет текселя полностью замещает цвет фрагмента фигуры
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    return id;
}

void Widget3ds::drawModels()
{
    if (_model3ds.isEmpty()) return;
    int i;
    for (i=0; i<_model3ds.size(); i++)
    {
        _listsComplete.append(drawModel(_model3ds.at(i)));
    }
}

GLuint Widget3ds::drawModel(Model3ds model)
{
    if (model.meshs.isEmpty()) return false;
    GLuint n = glGenLists(1);
    int i, j, material = -1;
    unsigned int textureID = 0;
    bool isTexture = false;
    float /*tempfv,*/ transparency = 1;
    word index;
    vect3f bf;

    glNewList(n, GL_COMPILE);

      //      qglColor(Qt::white);
            for (i=0; i<model.meshs.size(); i++)
            {
                for (j=0; j<model.meshs.at(i).faces.size(); j++)
                {
                    if (material != model.meshs.at(i).faces.at(j).material)
                    {
                        material = model.meshs.at(i).faces.at(j).material;
                        if ((material>=0)&&(material<model.materials.size()))
                        {
                            if (model.materials.at(material).hasTransparency)
                            {
                                transparency = 1-model.materials.at(material).transparency;
                            }
                            else transparency = 1;

                            glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT);
                            if (model.materials.at(material).hasAmbientColor)
                            {
                                bf = model.materials.at(material).ambientColor;
                                glColor4f(bf.x, bf.y, bf.z,transparency);
                            }
                            else glColor4f(0, 0, 0,transparency);

                            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
                            if (model.materials.at(material).hasDiffuseColor)
                            {
                                bf = model.materials.at(material).diffuseColor;
                                glColor4f(bf.x, bf.y, bf.z,transparency);
//                                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tempfv);
                            }
                            else glColor4f(0, 0, 0,transparency);

                            glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
                            if (model.materials.at(material).hasSpecular)
                            {
                                bf = model.materials.at(material).specular;
                                glColor4f(bf.x, bf.y, bf.z,transparency);
//                                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tempfv);
                            }
                            else glColor4f(0, 0, 0,transparency);
                            if (model.materials.at(material).hasShininess)
                            {
                                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, model.materials.at(material).shininess);
                            }
                            else glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
                            if (!model.materials.at(material).strFile.isEmpty())
                            {
                                if (textureID != model.materials.at(material).texureID)
                                {
//                                    qglColor(Qt::white);
                                    textureID = model.materials.at(material).texureID;
                                    glBindTexture(GL_TEXTURE_2D, textureID);
                                }
                                isTexture = true;
                            }
                            else isTexture = false;
                        }
                    }

                    glBegin(GL_TRIANGLES);

                    index = model.meshs.at(i).faces.at(j).index.A;
                    glNormal3f(GLfloat(model.meshs.at(i).normals.at(index).x), GLfloat(model.meshs.at(i).normals.at(index).y), GLfloat(model.meshs.at(i).normals.at(index).z));
                    if (isTexture) glTexCoord2f(GLfloat(model.meshs.at(i).textures.at(index).x), GLfloat(model.meshs.at(i).textures.at(index).y));
                    glVertex3f(GLfloat(model.meshs.at(i).vertex.at(index).x), GLfloat(model.meshs.at(i).vertex.at(index).y), GLfloat(model.meshs.at(i).vertex.at(index).z));

                    index = model.meshs.at(i).faces.at(j).index.B;
                    glNormal3f(GLfloat(model.meshs.at(i).normals.at(index).x), GLfloat(model.meshs.at(i).normals.at(index).y), GLfloat(model.meshs.at(i).normals.at(index).z));
                    if (isTexture) glTexCoord2f(GLfloat(model.meshs.at(i).textures.at(index).x), GLfloat(model.meshs.at(i).textures.at(index).y));
                    glVertex3f(GLfloat(model.meshs.at(i).vertex.at(index).x), GLfloat(model.meshs.at(i).vertex.at(index).y), GLfloat(model.meshs.at(i).vertex.at(index).z));

                    index = model.meshs.at(i).faces.at(j).index.C;
                    glNormal3f(GLfloat(model.meshs.at(i).normals.at(index).x), GLfloat(model.meshs.at(i).normals.at(index).y), GLfloat(model.meshs.at(i).normals.at(index).z));
                    if (isTexture) glTexCoord2f(GLfloat(model.meshs.at(i).textures.at(index).x), GLfloat(model.meshs.at(i).textures.at(index).y));
                    glVertex3f(GLfloat(model.meshs.at(i).vertex.at(index).x), GLfloat(model.meshs.at(i).vertex.at(index).y), GLfloat(model.meshs.at(i).vertex.at(index).z));

                    glEnd();
                }
            }

    glEndList();

    return n;
}


void Widget3ds::connectDevice(vect3f dev1, vect3f dev2, PinDeviceConstants::PinType type)
{
    _listWires.listsCompleteWires.append(drawWire(dev1, dev2, WireConnection::colorOfPinType(type)));
    updateDispley();
}

void Widget3ds::disconnectDevice(PinDeviceConstants::PinOfDevice dev1, PinDeviceConstants::PinOfDevice dev2)
{
    if (_listWires.listsCompleteWires.isEmpty()) return;
    if (_listWires.listsCompleteWires.size() == 1)
    {
        _listWires.listsCompleteWires.clear();
        return;
    }
    for (int i=0; i<_listWires.listsCompleteWires.size(); i++)
    {
        if ((_listWires.dev1.at(i) == dev1)&&(_listWires.dev2.at(i) == dev2))
            _listWires.listsCompleteWires.removeAt(i);
    }
}

GLuint Widget3ds::drawWire(vect3f p1, vect3f p2, vect3uch color)
{
    GLuint n = glGenLists(1);

    glNewList(n, GL_COMPILE);
    glLineWidth(3.0);
        glBegin(GL_LINES);

            glColor3ub(color.r, color.g, color.b);
            glVertex3f(GLfloat(p1.x), GLfloat(p1.y), GLfloat(p1.z));
            glVertex3f(GLfloat(p2.x), GLfloat(p2.y), GLfloat(p2.z));

        glEnd();

    glEndList();

    return n;
}

/*----------------------------------------------------------*/

/*------------------ Местоположение объектов ------------------*/

void Widget3ds::allocationDevice()
{
    int i, j;
    float tempf;
    Model3ds model;
    QStringList listModelNames = ModelWork::list3dsFiles(true);
    QString tableName = listModelNames.last();
    QString spaceName = listModelNames.at(0);

    for (i=0; i<_model3ds.size(); i++)
    {
        if ((_model3ds.at(i).name == tableName)||(_model3ds.at(i).name == spaceName)) continue;
        for (j=0; j<_model3ds.size(); j++)
        {
//            qDebug()<<MathForGL::distanceModels(_model3ds.at(i), _model3ds.at(j));
            if ((j==i)||(_model3ds.at(j).name == tableName)||(_model3ds.at(j).name == spaceName)) continue;

            if (MathForGL::crossingOfModel3ds(_model3ds.at(i), _model3ds.at(j)) > 0) continue;
            model = _model3ds.at(j);
            tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
            if (tempf < 0)
            {
                model.offset(0,0,-1*tempf);
                tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
                if (tempf < 0) model.offset(0,0,tempf);
            }

            tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
            if (tempf < 0)
            {
                model.offset(0,0,tempf);
                tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
                if (tempf < 0) model.offset(0,0,-1*tempf);
            }

            tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
            if (tempf < 0)
            {
                model.offset(tempf,0,0);
                tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
                if (tempf < 0) model.offset(-1*tempf,0,0);
            }

            tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
            if (tempf < 0)
            {
                model.offset(-1*tempf,0,0);
                tempf = MathForGL::crossingOfModel3ds(_model3ds.at(i), model);
                if (tempf < 0) model.offset(tempf,0,0);
            }

            _model3ds[j] = model;
        }
    }
}

void Widget3ds::offsetDevice(int modelIndex, int x, int y, int z)
{
    Model3ds tempModel;
    tempModel = _model3ds.at(modelIndex);
    tempModel.offset(x,y,z);
 //   if (MathForGL::isCrossing(_model3ds, tempModel)) return;
    vect6f tempv6f = tempModel.edgesPoints();
    if (tempv6f.x > _limitsCoordinatDevices.x) return;
    if (tempv6f.z > _limitsCoordinatDevices.z) return;
    if (tempv6f.x1 < _limitsCoordinatDevices.x1) return;
    if (tempv6f.z1 < _limitsCoordinatDevices.z1) return;
    _model3ds[modelIndex].offset(x,y,z);
    glDeleteLists(_listsComplete.at(modelIndex),1);
    _listsComplete.replace(modelIndex,drawModel(_model3ds.at(modelIndex)));
}

void Widget3ds::rotateDevice(int modelIndex, float rotX, float rotY, float rotZ)
{
    MathForGL::rotationModel(&_model3ds[modelIndex], _model3ds.at(modelIndex).center(), rotX, rotY, rotZ);

    glDeleteLists(_listsComplete.at(modelIndex),1);
    _listsComplete.replace(modelIndex,drawModel(_model3ds.at(modelIndex)));
}

/*------------------------------------------------*/

/*---------------- Виджет навигации -------------------*/

void Widget3ds::setNavigationWidget(bool on)
{
    if (_navigationWidget == on) return;
    if (on)
        createNavigationWidgetButtons();
    else
        deleteNavigationWidgetButton();
    _navigationWidget = on;
}

void Widget3ds::setNavigationWidget()
{
    if (_navigationWidget)
    {
        _navigationWidget = false;
        deleteNavigationWidgetButton();
    }
    else
    {
        createNavigationWidgetButtons();
        _navigationWidget = true;
    }
}

void Widget3ds::createNavigationWidgetButtons()
{
    QPushButton *butUpEye = new QPushButton(this);
    butUpEye->setToolTip("Поднять камеру вверх");
    butUpEye->setObjectName("NavigationButton");
    connect(butUpEye,SIGNAL(clicked()),this,SLOT(navigationButtonClickedUpEye()));
    QPixmap map("E:/recourses/strelka-up.png");
    QSize size = map.size();
    map.load("E:/recourses/orig/strelka-up.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butUpEye->setGeometry(map.height() + 20,20,map.width(),map.height());
    butUpEye->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-up.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-left.png); }");
    butUpEye->setMask(map.mask());
    butUpEye->setAutoRepeat(true);
    butUpEye->setAutoRepeatInterval(20);
    butUpEye->show();

    QPushButton *butDownEye = new QPushButton(this);
    butDownEye->setToolTip("Опустить камеру вниз");
    butDownEye->setObjectName("NavigationButton");
    connect(butDownEye,SIGNAL(clicked()),this,SLOT(navigationButtonClickedDownEye()));
    map.load("E:/recourses/orig/strelka-down.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butDownEye->setGeometry(map.height() + 20,20+map.height(),map.width(),map.height());
    butDownEye->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-down.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-left.png); }" );
    butDownEye->setMask(map.mask());
    butDownEye->setAutoRepeat(true);
    butDownEye->setAutoRepeatInterval(20);
    butDownEye->show();

    QPushButton *butLeftEye = new QPushButton(this);
    butLeftEye->setToolTip("Переместить камеру влево");
    butLeftEye->setObjectName("NavigationButton");
    connect(butLeftEye,SIGNAL(clicked()),this,SLOT(navigationButtonClickedLeftEye()));
    map.load("E:/recourses/strelka-left.png");
    size = map.size();
    map.load("E:/recourses/orig/strelka-left.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butLeftEye->setGeometry(20,map.width()+10,map.width(),map.height());
    butLeftEye->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-left.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-down.png); }" );
    butLeftEye->setMask(map.mask());
    butLeftEye->setAutoRepeat(true);
    butLeftEye->setAutoRepeatInterval(20);
    butLeftEye->show();

    QPushButton *butRightEye = new QPushButton(this);
    butRightEye->setToolTip("Переместить камеру вправо");
    butRightEye->setObjectName("NavigationButton");
    connect(butRightEye,SIGNAL(clicked()),this,SLOT(navigationButtonClickedRightEye()));
    map.load("E:/recourses/orig/strelka-right.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butRightEye->setGeometry(map.width() + map.height() + 20, map.width()+10,map.width(),map.height());
    butRightEye->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-right.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-down.png); }" );
    butRightEye->setMask(map.mask());
    butRightEye->setAutoRepeat(true);
    butRightEye->setAutoRepeatInterval(20);
    butRightEye->show();

    /*--------------------------------------------------------------------------------------------*/

    QPushButton *butUp = new QPushButton(this);
    butUp->setToolTip("Поднять точку обзора вверх");
    butUp->setObjectName("NavigationButton");
    connect(butUp,SIGNAL(clicked()),this,SLOT(navigationButtonClickedUp()));
    map.load("E:/recourses/strelka-up.png");
    size = map.size();
    map.load("E:/recourses/orig/strelka-up.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butUp->setGeometry(butUpEye->x(),20 + butDownEye->y() + butDownEye->height(),map.width(),map.height());
    butUp->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-up.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-left.png); }");
    butUp->setMask(map.mask());
    butUp->setAutoRepeat(true);
    butUp->setAutoRepeatInterval(20);
    butUp->show();

    QPushButton *butDown = new QPushButton(this);
    butDown->setToolTip("Опустить точку обзора вниз");
    butDown->setObjectName("NavigationButton");
    connect(butDown,SIGNAL(clicked()),this,SLOT(navigationButtonClickedDown()));
    map.load("E:/recourses/orig/strelka-down.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butDown->setGeometry(butUp->x(),butUp->y() + butUp->height(),map.width(),map.height());
    butDown->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-down.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-left.png); }" );
    butDown->setMask(map.mask());
    butDown->setAutoRepeat(true);
    butDown->setAutoRepeatInterval(20);
    butDown->show();

    QPushButton *butLeft = new QPushButton(this);
    butLeft->setToolTip("Переместить точку обзора влево");
    butLeft->setObjectName("NavigationButton");
    connect(butLeft,SIGNAL(clicked()),this,SLOT(navigationButtonClickedLeft()));
    map.load("E:/recourses/strelka-left.png");
    size = map.size();
    map.load("E:/recourses/orig/strelka-left.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butLeft->setGeometry(butLeftEye->x(),butUp->y() + butUp->height() - 10,map.width(),map.height());
    butLeft->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-left.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-down.png); }" );
    butLeft->setMask(map.mask());
    butLeft->setAutoRepeat(true);
    butLeft->setAutoRepeatInterval(20);
    butLeft->show();

    QPushButton *butRight = new QPushButton(this);
    butRight->setToolTip("Переместить точку обзора вправо");
    butRight->setObjectName("NavigationButton");
    connect(butRight,SIGNAL(clicked()),this,SLOT(navigationButtonClickedRight()));
    map.load("E:/recourses/orig/strelka-right.png");
    map = map.scaled(size,Qt::KeepAspectRatio);
    butRight->setGeometry(butRightEye->x(), butLeft->y(),map.width(),map.height());
    butRight->setStyleSheet( "QPushButton { border-image: url(E:/recourses/orig/strelka-right.png); background-color: #21afea} QPushButton::pressed { border-image: url(E:/recourses/strelka-down.png); }" );
    butRight->setMask(map.mask());
    butRight->setAutoRepeat(true);
    butRight->setAutoRepeatInterval(20);
    butRight->show();
}

void Widget3ds::deleteNavigationWidgetButton()
{
    QList<QPushButton*> listBut = this->findChildren<QPushButton*>("NavigationButton");
    for (int i=0; i<listBut.size(); i++)
    {
        listBut.at(i)->deleteLater();
    }
}

void Widget3ds::navigationButtonClickedUpEye()
{
    _camera.offsetEye(0,_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedDownEye()
{
    _camera.offsetEye(0,-_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedLeftEye()
{
    _camera.offsetEye(-_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedRightEye()
{
    _camera.offsetEye(_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedUp()
{
    _camera.offsetCenter(0,0,_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedDown()
{
    _camera.offsetCenter(0,0,-_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedLeft()
{
    _camera.offsetCenter(-_stepRotationScene);
    updateDispley();
}

void Widget3ds::navigationButtonClickedRight()
{
    _camera.offsetCenter(_stepRotationScene);
    updateDispley();
}

QList<vect2f> Widget3ds::calculateCircle(float rad, float countPart, float x, float y)
{
    QList<vect2f> list;
    if ((countPart<=0)||(rad<=0)) return list;
    int i;
    float x1 = x-rad, step = 3.141592*2*rad/countPart;
    vect2f tempVect;
    for (i=0; x1<=x+rad; i++)
    {
        tempVect.x = x1;
        tempVect.y = sqrtf(rad*rad - (x1 - x)*(x1 - x)) + y;
        x1+=step;
        list.append(tempVect);
    }
    for (i=0; x1>=x-rad; i++)
    {
        tempVect.x = x1;
        tempVect.y = -1*sqrtf(rad*rad - (x1 - x)*(x1 - x)) + y;
        x1-=step;
        list.append(tempVect);
    }
    return list;
}

GLuint Widget3ds::drawCircle(QList<vect2f> vertex)
{
    GLuint n = glGenLists(1);
    int i;
    glNewList(n, GL_COMPILE);
        glLineWidth(60);
        glColor3i(0,0,0);
        glBegin(GL_LINE);
        for (i=0; i<vertex.size(); i++)
        {
            glVertex3f(vertex.at(i).x, vertex.at(i).y, 50);
        }
        glEnd();
    glEndList();
    return n;
}

/*-----------------------------------------------------*/
