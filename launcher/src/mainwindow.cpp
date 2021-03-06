//------------------------------------------------------------------------------
//
//      RSS launcher main window
//      (c) maisvendoo, 17/12/2018
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief RSS launcher main window
 * \copyright maisvendoo
 * \author maisvendoo
 * \date 17/12/2018
 */

#include    "mainwindow.h"
#include    "ui_mainwindow.h"

#include    <QPushButton>
#include    <QDir>
#include    <QDirIterator>
#include    <QStringList>
#include    <QComboBox>
#include    <QTextStream>

#include    "filesystem.h"
#include    "CfgReader.h"

#include    "platform.h"
#include    "styles.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    connect(ui->lwRoutes, &QListWidget::itemSelectionChanged,
            this, &MainWindow::onRouteSelection);

    connect(ui->lwTrains, &QListWidget::itemSelectionChanged,
            this, &MainWindow::onTrainSelection);

    connect(ui->btnStart, &QPushButton::pressed,
            this, &MainWindow::onStartPressed);

    connect(&simulatorProc, &QProcess::started,
            this, &MainWindow::onSimulatorStarted);

    connect(&simulatorProc, QOverload<int>::of(&QProcess::finished),
            this, &MainWindow::onSimulatorFinished);

    connect(&viewerProc, QOverload<int>::of(&QProcess::finished),
            this, &MainWindow::onViewerFinished);

    connect(ui->cbStations, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStationSelected);

    connect(ui->cbDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onDirectionSelected);

    setCentralWidget(ui->twMain);

    setFocusPolicy(Qt::ClickFocus);

    loadTheme();

    QIcon icon(":/images/images/RRS_logo.png");
    setWindowIcon(icon);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::init()
{
    FileSystem &fs = FileSystem::getInstance();

    loadRoutesList(fs.getRouteRootDir());
    loadTrainsList(fs.getTrainsDir());
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadRoutesList(const std::string &routesDir)
{
    QDir routes(QString(routesDir.c_str()));
    QDirIterator route_dirs(routes.path(), QStringList(), QDir::NoDotAndDotDot | QDir::Dirs);

    while (route_dirs.hasNext())
    {
        route_info_t route_info;
        route_info.route_dir = route_dirs.next();

        CfgReader cfg;

        if (cfg.load(route_info.route_dir + QDir::separator() + "description.xml"))
        {
            QString secName = "Route";

            cfg.getString(secName, "Title", route_info.route_title);
            cfg.getString(secName, "Description", route_info.route_description);
        }

        routes_info.push_back(route_info);
    }

    for (auto it = routes_info.begin(); it != routes_info.end(); ++it)
    {
        ui->lwRoutes->addItem((*it).route_title);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadTrainsList(const std::string &trainsDir)
{
    QDir    trains(QString(trainsDir.c_str()));
    QDirIterator train_files(trains.path(), QStringList() << "*.xml", QDir::NoDotAndDotDot | QDir::Files);

    while (train_files.hasNext())
    {
        train_info_t train_info;
        QString fullPath = train_files.next();
        QFileInfo fileInfo(fullPath);

        train_info.train_config_path = fileInfo.baseName();

        CfgReader cfg;

        if (cfg.load(fullPath))
        {
            QString secName = "Common";

            cfg.getString(secName, "Title", train_info.train_title);
            cfg.getString(secName, "Description", train_info.description);
        }

        trains_info.push_back(train_info);
        ui->lwTrains->addItem(train_info.train_title);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::setRouteScreenShot(const QString &path)
{
    QFileInfo info(path);

    if (!info.exists())
    {
        ui->lRouteScreenShot->setText(tr("No screenshot"));
        return;
    }

    QImage image(ui->lRouteScreenShot->width(), ui->lRouteScreenShot->height(), QImage::Format_ARGB32);
    image.load(path);
    ui->lRouteScreenShot->setPixmap(QPixmap::fromImage(image));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::startSimulator()
{
    FileSystem &fs = FileSystem::getInstance();
    QString simPath = SIMULATOR_NAME + EXE_EXP;

    QStringList args;
    args << "--train-config=" + selectedTrain;
    args << "--route=" + selectedRoutePath;

    if (isBackward())
    {
        args << "--direction=-1";
    }
    else
    {
        args << "--direction=1";
    }

    if (ui->cbStations->count() != 0)
    {        
        double init_coord = ui->dsbOrdinate->value() / 1000.0;
        args << "--init-coord=" + QString("%1").arg(init_coord, 0, 'f', 2);
    }

    simulatorProc.setWorkingDirectory(QString(fs.getBinaryDir().c_str()));
    simulatorProc.start(simPath, args);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::startViewer()
{
    FileSystem &fs = FileSystem::getInstance();
    QString viewerPath = VIEWER_NAME + EXE_EXP;

    QStringList args;
    args << "--route" << selectedRoutePath;
    args << "--train" << selectedTrain;
    args << "--direction";

    if (isBackward())
    {
        args << "-1";
    }
    else
    {
        args << "1";
    }

    viewerProc.setWorkingDirectory(QString(fs.getBinaryDir().c_str()));
    viewerProc.start(viewerPath, args);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadStations(QString &routeDir)
{
    ui->cbStations->clear();
    waypoints.clear();

    QFile file(routeDir + QDir::separator() + "waypoints.conf");

    if (!file.open(QIODevice::ReadOnly))
        return;

    while (!file.atEnd())
    {
        QString line = file.readLine();

        waypoint_t waypoint;

        QTextStream ss(&line);

        ss >> waypoint.name >> waypoint.forward_coord >> waypoint.backward_coord;

        ui->cbStations->addItem(waypoint.name);
        waypoints.push_back(waypoint);
    }

    ui->cbStations->setCurrentIndex(0);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool MainWindow::isBackward()
{
    switch (ui->cbDirection->currentIndex())
    {
    case 0: return false;

    case 1: return true;
    }

    return false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadTheme()
{
    FileSystem &fs = FileSystem::getInstance();
    std::string cfg_dir = fs.getConfigDir();
    std::string cfg_path = fs.combinePath(cfg_dir, "launcher.xml");

    CfgReader cfg;

    if ( cfg.load(QString(cfg_path.c_str())) )
    {
        QString secName = "Launcher";
        QString theme_name = "";

        if (!cfg.getString(secName, "Theme", theme_name))
        {
            theme_name = "dark-jedy";
        }

        std::string theme_dir = fs.getThemeDir();
        std::string theme_path = fs.combinePath(theme_dir, theme_name.toStdString() + ".qss");
        QString style_sheet = readStyleSheet(QString(theme_path.c_str()));

        this->setStyleSheet(style_sheet);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onRouteSelection()
{
    size_t item_idx = static_cast<size_t>(ui->lwRoutes->currentRow());

    ui->ptRouteDescription->clear();
    selectedRoutePath = routes_info[item_idx].route_dir;
    ui->ptRouteDescription->appendPlainText(routes_info[item_idx].route_description);

    loadStations(selectedRoutePath);

    onStationSelected(ui->cbStations->currentIndex());

    setRouteScreenShot(selectedRoutePath + QDir::separator() + "shotcut.png");
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onTrainSelection()
{
    size_t item_idx = static_cast<size_t>(ui->lwTrains->currentRow());

    ui->ptTrainDescription->clear();
    selectedTrain = trains_info[item_idx].train_config_path;
    ui->ptTrainDescription->appendPlainText(trains_info[item_idx].description);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onStartPressed()
{
    // Check is train selected
    if (selectedTrain.isEmpty())
    {
        return;
    }

    // Check is route selected
    if (selectedRoutePath.isEmpty())
    {
        return;
    }

    startSimulator();    
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onSimulatorStarted()
{
    ui->btnStart->setEnabled(false);    

    startViewer();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onSimulatorFinished(int exitCode)
{
    Q_UNUSED(exitCode)

    ui->btnStart->setEnabled(true);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onViewerFinished(int exitCode)
{
    Q_UNUSED(exitCode)

    simulatorProc.kill();
    setFocusPolicy(Qt::StrongFocus);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onStationSelected(int index)
{
    size_t idx = static_cast<size_t>(index);

    if (idx < waypoints.size())
    {
        if (isBackward())
            ui->dsbOrdinate->setValue(waypoints[idx].backward_coord);
        else
            ui->dsbOrdinate->setValue(waypoints[idx].forward_coord);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onDirectionSelected(int index)
{
    Q_UNUSED(index)

    int station_idx = ui->cbStations->currentIndex();

    onStationSelected(station_idx);
}
