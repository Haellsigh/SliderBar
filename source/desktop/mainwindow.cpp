#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

#include "sliderbar/serialinterface.h"
#include "sliderbar/sliderbar.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_actionPlugins = new QAction("Plugins");
    ui->menuBar->addAction(m_actionPlugins);

    m_sliderbar     = new SliderBar(this);
    m_dataInterface = new SerialInterface();

    m_sliderbar->setTransmitter(m_dataInterface);
    m_dataInterface->setReceiver(m_sliderbar);

    loadSettings();

    initialiseActions();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_actionPlugins;
}

void MainWindow::loadSettings()
{
    QSettings settings;
    ui->actionAutoconnect->setChecked(settings.value("sliderbar/autoconnect", false).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("sliderbar/autoconnect", ui->actionAutoconnect->isChecked());
}

void MainWindow::displayCalibrationData(uint32_t minpos, uint32_t maxpos, float maxvel)
{
    ui->eMinPos->setValue(minpos);
    ui->eMaxPos->setValue(maxpos);
    ui->eMaxVel->setValue(maxvel);
}

void MainWindow::initialiseActions()
{
    connect(ui->actionExit, &QAction::triggered,
            this, &MainWindow::close);

    // Connect SliderBar
    connect(ui->actionConnect, &QAction::triggered,
            m_sliderbar, &SliderBar::connect);
    connect(ui->actionAutoconnect, &QAction::triggered,
            m_sliderbar, &SliderBar::autoconnect);
    connect(ui->actionSettings, &QAction::triggered,
            m_sliderbar, &SliderBar::manageSettings);
    connect(m_actionPlugins, &QAction::triggered,
            m_sliderbar, &SliderBar::managePlugins);
    connect(m_sliderbar, &SliderBar::settingsChanged,
            this, &MainWindow::loadSettings);

    // Save settings when changed from quick settings
    connect(ui->actionAutoconnect, &QAction::triggered,
            this, &MainWindow::saveSettings);

    connect(ui->bCalibrate, &QPushButton::clicked,
            m_sliderbar, &SliderBar::requestCalibration);

    connect(m_sliderbar, &SliderBar::calibrationData,
            this, &MainWindow::displayCalibrationData);
}
