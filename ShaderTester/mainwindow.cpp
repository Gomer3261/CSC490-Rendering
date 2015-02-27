#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(NULL),
    m_objects(),
    m_shaders(),
    m_filters(),
    m_selected(NULL)
{
    ui->setupUi(this);
    animationTimer = new QTimer(this);

    connect(animationTimer, SIGNAL(timeout()), this, SLOT(drawOpenGL()));
    animationTimer->start(20);

    m_shaders.insert("test", new Shader("../../../../shaders/Basic.vsh", "../../../../shaders/Basic.fsh"));
    m_filters.insert("through", new ShaderPostProcess(1, 1, "../../../../filters/Through.vsh", "../../../../filters/Through.fsh"));
    m_filters.insert("depth", new ShaderPostProcess(1, 1, "../../../../filters/Through.vsh", "../../../../filters/Depth.fsh"));
    m_filters.insert("depth2", new ShaderPostProcess(1, 1, "../../../../filters/Through.vsh", "../../../../filters/Depth.fsh"));
    m_filters.insert("ssao", new ShaderPostProcess(1, 1, "../../../../filters/Through.vsh", "../../../../filters/SSAO.fsh"));
    m_filters.insert("ssao2", new ShaderPostProcess(1, 1, "../../../../filters/Through.vsh", "../../../../filters/SimpleSSAO.fsh"));
    m_filters.insert("ripple", new ShaderPostProcessRipple(1, 1, "../../../../filters/Through.vsh", "../../../../filters/Ripple.fsh"));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_scene != NULL) {
        delete m_scene;
    }
}

void MainWindow::drawOpenGL()
{
    ui->oglview->repaint();
}

void MainWindow::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("Obj Files (*.obj)"));
    if(!fileNames.empty() && m_scene != NULL) {
        delete m_scene;
        m_objects.clear();
        ui->objectList->clear();
    }
    m_scene = new Scene();
    m_scene->addFilter(m_filters["ssao2"]);
    m_scene->addFilter(m_filters["depth"]);
    //m_scene->addFilter(m_filters["depth2"]);
    //m_scene->addFilter(m_filters["ripple"]);
    ui->oglview->setScene(m_scene);
    foreach (QString file, fileNames) {
        QString root;
        if(fileNames.length() > 1) {
            root = QString(file);
            root.truncate(file.lastIndexOf('.'));
            root =  root.right(root.length() - root.lastIndexOf('/') - 1);
            root += '/';
        } else {
            root = QString("");
        }
        ObjLoader loader = ObjLoader(file, root);
        while(loader.hasObject()) {
            RenderObject* object = loader.popObject();
            object->setShader(m_shaders["test"]);
            m_objects.insert(object->getName(), object);
            m_scene->addObject(object);
            ui->objectList->addItem(object->getName());
        }
    }
}
