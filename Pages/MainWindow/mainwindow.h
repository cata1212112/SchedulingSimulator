//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_MAINWINDOW_H
#define LICENTA_MAINWINDOW_H

#include <QMainWindow>
#include "../../Utils/Style.h"
#include "../../Utils/Metrics.h"
#include "../../DiscreteEventSimulator/DES.h"
#include "../../Utils/UUniFastDiscard.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    std::string selectedAlgorithm;
    QWidget *lastWidget;

    void handleSingleCoreButton();

    void handleMultiCoreButton();

    void handleRealTimeButton();

    void gotoRunning(DES *des, int numCores);

    QWidget* getPlotFromPythonScript(std::string scriptName, std::string imageName, std::string parameters);

    void deleteContent(QWidget *widget);
};


#endif //LICENTA_MAINWINDOW_H
