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


    QListIterator<QString> it(m_filters.keys());
    while (it.hasNext())
    {
          QListWidgetItem *listItem = new QListWidgetItem(it.next(), ui->shaderList);
          listItem->setCheckState(Qt::Unchecked);
          ui->shaderList->addItem(listItem);
    }
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
    for(int row = 0; row < ui->shaderList->count(); row++)
    {
        QListWidgetItem *item = ui->shaderList->item(row);
        if(item->checkState() == Qt::Checked) {
            m_scene->addFilter(m_filters[item->text()]);
        }
    }
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

void MainWindow::shadersChanged(QListWidgetItem* old_item) {
    if(m_scene != NULL) {
        m_scene->clearFilters();
        for(int row = 0; row < ui->shaderList->count(); row++)
        {
            QListWidgetItem *item = ui->shaderList->item(row);
            if(item->checkState() == Qt::Checked) {
                if(item->text() != old_item->text() || item == old_item) {
                    m_scene->addFilter(m_filters[item->text()]);
                }
            }
        }
        ui->oglview->setScene(m_scene);
        m_scene->initializeGL();
    }
}
