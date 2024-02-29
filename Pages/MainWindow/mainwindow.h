//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_MAINWINDOW_H
#define LICENTA_MAINWINDOW_H

#include <QMainWindow>
#include "../../Utils/Style.h"
#include "../../Utils/Metrics.h"
#include "../../DiscreteEventSimulator/DES.h"


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

    void handleSingleCoreButton();

    void gotoRunning(DES *des);
};


#endif //LICENTA_MAINWINDOW_H
