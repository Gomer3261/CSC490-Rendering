#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QListWidgetItem>

#include "objloader.h"
#include "scene.h"
#include "shaderpostprocessfocal.h"
#include "shaderpostprocessripple.h"

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

private:
    Ui::MainWindow *ui;
    QTimer* animationTimer;

    Scene* m_scene;
    QMap<QString, RenderObject*> m_objects;
    QMap<QString, Shader*> m_shaders;
    QMap<QString, ShaderPostProcess*> m_filters;

    QListWidgetItem *m_selected;
};

#endif // MAINWINDOW_H
