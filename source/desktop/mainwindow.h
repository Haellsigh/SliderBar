#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace SliderBar {
class Manager;
class SerialInterface;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    void loadQuickSettings();
    void saveQuickSettings();

    void handleActionConnect();

    void connected();
    void disconnected();

private:
    void initialiseConnections();

    Ui::MainWindow* ui       = nullptr;
    QAction* m_actionPlugins = nullptr;

    SliderBar::Manager* m_sliderbar;
    bool m_connected = false;

    SliderBar::SerialInterface* m_dataInterface;
};

#endif // MAINWINDOW_H
