#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QList>
#include <QListWidgetItem>

#include "objloader.h"
#include "scene.h"
#include "shaderpostprocessfocal.h"
#include "shaderpostprocessripple.h"
#include "shaderpostprocessglow.h"
#include "shaderpostprocesssssdebug.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void drawOpenGL();
    void openFile();
    void shadersChanged(QListWidgetItem* item);

private:
    Ui::MainWindow *ui;
    QTimer* animationTimer;

    Scene* m_scene;
    QMap<QString, RenderObject*> m_objects;
    QMap<QString, ShaderPostProcess*> m_filters;

    QElapsedTimer m_timer;
    int m_frames = 0;

    void setShaders();
};

#endif // MAINWINDOW_H
