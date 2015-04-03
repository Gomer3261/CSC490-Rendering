#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(NULL),
    m_objects(),
    m_filters(),
    m_selected(NULL)
{
    ui->setupUi(this);
    animationTimer = new QTimer(this);

    connect(animationTimer, SIGNAL(timeout()), this, SLOT(drawOpenGL()));
    animationTimer->start(20);

    m_filters.insert("through", new ShaderPostProcess("../../../../filters/Through.vsh", "../../../../filters/Through.fsh", false));
    m_filters.insert("sssdebug", new ShaderPostProcessSSSDebug("../../../../filters/Through.vsh", "../../../../filters/SSSDebug.fsh", false));
    m_filters.insert("depth", new ShaderPostProcess("../../../../filters/Through.vsh", "../../../../filters/Depth.fsh", false));
    m_filters.insert("ssao", new ShaderPostProcess("../../../../filters/Through.vsh", "../../../../filters/SimpleSSAO.fsh", false));
    m_filters.insert("ripple", new ShaderPostProcessRipple("../../../../filters/Through.vsh", "../../../../filters/Ripple.fsh", true));
    m_filters.insert("gaussian", new ShaderPostProcess("../../../../filters/Through.vsh", "../../../../filters/GaussianBlur.fsh", 2, false));
    m_filters.insert("gaussiandof", new ShaderPostProcessFocal("../../../../filters/Through.vsh", "../../../../filters/GaussianDOF.fsh", 2, false));
    m_filters.insert("bokeh", new ShaderPostProcess("../../../../filters/Through.vsh", "../../../../filters/BokehBlur.fsh", false));
    m_filters.insert("bokehdof", new ShaderPostProcess("../../../../filters/Through.vsh", "../../../../filters/BokehDOF.fsh", false));
    m_filters.insert("bokehdofcpu", new ShaderPostProcessFocal("../../../../filters/Through.vsh", "../../../../filters/BokehDOFCPU.fsh", false));
    m_filters.insert("glow", new ShaderPostProcessGlow("../../../../filters/Through.vsh", "../../../../filters/Glow.fsh", false));
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
    //m_scene->addFilter(m_filters["depth"]);
    //m_scene->addFilter(m_filters["ssao"]);
    m_scene->addFilter(m_filters["depth"]);
    m_scene->addFilter(m_filters["depth"]);
    //m_scene->addFilter(m_filters["glow"]);
    //m_scene->addFilter(m_filters["ripple"]);
    //m_scene->addFilter(m_filters["gaussian"]);
    //m_scene->addFilter(m_filters["gaussiandof"]);
    //m_scene->addFilter(m_filters["bokeh"]);
    //m_scene->addFilter(m_filters["bokehdof"]);
    //m_scene->addFilter(m_filters["bokehdofcpu"]);
    //m_scene->addFilter(m_filters["sssdebug"]);
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
            m_objects.insert(object->getName(), object);
            m_scene->addObject(object);
            ui->objectList->addItem(object->getName());
        }
    }
    m_scene->initializeGL();
}
