//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_MAINWINDOW_H
#define LICENTA_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
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
    int selectedNumberOfCores = 1;
    int quantum = 1;
    QWidget *lastWidget;

    void handleSingleCoreButton();

    void handleMultiCoreButton();

    void handleRealTimeButton();

    void gotoRunning(DES *des, int numCores);

    QWidget* getPlotFromPythonScript(std::string scriptName, std::string imageName, std::string parameters);

    void deleteContent(QWidget *widget);

    void selectNumberOfCores();

    void selectAlgortihmPage(QWidget *parent, const std::vector<std::string>& algorithms, QLayout *layout, bool isMultiCore, bool isRealTime);

    void selectedAlgorithmButton(const std::string& algorithmName, QLayout* layout, bool isMultiCore= false);

    void goBackButton(QLayout* layout, QWidget* parent);

    void selectAlgortihm(const std::string &algortihm, bool isMultiCore = false);

    void clearWidgets(QLayout *layout);

    void setupInputData(QWidget *parent, bool isMultiCore = false);

    void generateDataButton(QLayout *layout, QWidget *parent, bool isMultiCore = false);
//    void se
};


#endif //LICENTA_MAINWINDOW_H
