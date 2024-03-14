//
// Created by Cata on 2/27/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <string>
#include <QFileDialog>
#include <iostream>
#include <QMovie>
#include <QScrollArea>
#include <QMenu>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->singlecore, &QPushButton::clicked, this, &MainWindow::handleSingleCoreButton);

    connect(ui->multicore, &QPushButton::clicked, this, &MainWindow::handleMultiCoreButton);

    ui->stackedWidget->setCurrentWidget(ui->StartPage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleSingleCoreButton() {
    ui->stackedWidget->setCurrentWidget(ui->SingleCore);
    lastWidget = ui->StartPage;

    auto layout = new QVBoxLayout();
    layout->setSpacing(0);
    if (ui->SingleCore->layout() == nullptr) {
        ui->SingleCore->setLayout(layout);
    }

    for (const auto& algorithm : ImplementedAlgorithms::getSingleCoreAlgorithms()) {
        auto *button = new QPushButton(QString::fromStdString(algorithm));
        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

        QFont font;
        font.setPointSize(12);
        button->setFont(font);

        connect(button, &QPushButton::clicked, this, [=]() {
            selectedAlgorithm = algorithm;

            int quantum = 0;
            if (selectedAlgorithm == "Round Robin") {
                bool ok = false;
                quantum = QInputDialog::getInt(this, tr("Enter Quantum"), tr("Quantum:"), 1, 1, 100, 1, &ok);
            }

            ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
            lastWidget = ui->SingleCore;

            connect(ui->maximumTime, &QSlider::valueChanged, this, [=] (int value){
                ui->timpselectat->setText(QString::fromStdString(to_string(value)));
            });

            connect(ui->numarprocese, &QSlider::valueChanged, this, [=](int value) {
                ui->proceseselectate->setText(QString::fromStdString(to_string(value)));
            });

            DES *des = new DES(selectedAlgorithm);
            des->setRoundRobinQuant(quantum);

            connect(ui->generate, &QPushButton::clicked, this, [=]() {
                string inputData = des->generateInputData(ui->proceseselectate->text().toInt(), ui->timpselectat->text().toInt());
                ui->stackedWidget->setCurrentWidget(ui->InputData);
                lastWidget = ui->RunSimulation;


                if (ui->InputData->layout() == nullptr) {

                    auto layout = new QVBoxLayout();
                    layout->setSpacing(0);
                    ui->InputData->setLayout(layout);

                    auto *button = new QPushButton(QString::fromStdString("Download Input Data"));
                    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                    button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

                    QFont font;
                    font.setPointSize(12);
                    button->setFont(font);

                    connect(button, &QPushButton::clicked, this, [=]() {
                        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                                        tr("Text files (*.txt);;All Files (*)"));

                        if (!fileName.isEmpty()) {
                            QFile file(fileName);
                            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                QTextStream out(&file);
                                out << QString::fromStdString(inputData);
                                file.close();
                                gotoRunning(des);
                            } else {
                            }
                        }
                    });

                    auto *button1 = new QPushButton(QString::fromStdString("Run Simulation"));
                    button1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                    button1->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

                    QFont font1;
                    font1.setPointSize(12);
                    button1->setFont(font1);

                    connect(button1, &QPushButton::clicked, this, [=]() {
                        gotoRunning(des);
                    });


                    ui->InputData->layout()->addWidget(button);
                    ui->InputData->layout()->addWidget(button1);

                    auto *button3 = new QPushButton(QString::fromStdString("Go back"));
                    button3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                    button3->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

                    button3->setFont(font);

                    connect(button3, &QPushButton::clicked, this, [=]() {
                        ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
                    });

                    ui->InputData->layout()->addWidget(button3);
                }
            });

            connect(ui->usefileasinput, &QPushButton::clicked, this, [=]() {
                QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

                if (!fileName.isEmpty())
                {
                    des->readInputDataFromFile(fileName.toStdString());
                    gotoRunning(des);
                }
            });

            connect(ui->goback, &QPushButton::clicked, this, [=]() {
                deleteContent(ui->SingleCore);
                handleSingleCoreButton();
            });
        });

        ui->SingleCore->layout()->addWidget(button);

    }

    auto *button = new QPushButton(QString::fromStdString("Go back"));
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

    QFont font;
    font.setPointSize(12);
    button->setFont(font);

    connect(button, &QPushButton::clicked, this, [=]() {
        deleteContent(ui->SingleCore);
        ui->stackedWidget->setCurrentWidget(lastWidget);
    });

    ui->SingleCore->layout()->addWidget(button);
}

void MainWindow::handleMultiCoreButton() {
    ui->stackedWidget->setCurrentWidget(ui->MultiCore);
    int coreValues[] = {2, 4, 8, 16, 32};
    int selectedNumberOfCores = 2;
    ui->selectednumberofcores->setText(QString::fromStdString(to_string(2)));
    connect(ui->coreselector, &QSlider::valueChanged, this, [=, &selectedNumberOfCores](int value) {
        selectedNumberOfCores = coreValues[value];
        ui->selectednumberofcores->setText(QString::fromStdString(to_string(coreValues[value])));
    });

    connect(ui->back, &QPushButton::clicked, this, [&]() {
       ui->stackedWidget->setCurrentWidget(ui->StartPage);
    });

    connect(ui->next, &QPushButton::clicked, this, [&]() {
        ui->stackedWidget->setCurrentWidget(ui->SelectMultiCoreAlgorithm);

        auto layout = new QVBoxLayout();
        layout->setSpacing(0);

        ui->SelectMultiCoreAlgorithm->setLayout(layout);

        for (const auto& algorithm : ImplementedAlgorithms::getMultiCoreAlgortihms()) {
            auto *button = new QPushButton(QString::fromStdString(algorithm));
            button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

            QFont font;
            font.setPointSize(12);
            button->setFont(font);

            ui->SelectMultiCoreAlgorithm->layout()->addWidget(button);
        }
    });

}

void MainWindow::gotoRunning(DES *des) {
    ui->stackedWidget->setCurrentWidget(ui->running);
    lastWidget = ui->InputData;

    auto layout = new QVBoxLayout();
    layout->setSpacing(0);
    ui->running->setLayout(layout);

    auto *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    auto *containerWidget = new QWidget;
    auto *containerLayout = new QVBoxLayout(containerWidget);

    vector<Metrics> metrics = des->startSimulation(1);

    string chart_parameters = "";
    string gantt_parameters = "";

    for (auto metric : metrics) {
        des->setPartialMetricsInput(des->getPartialMetricsInput(metric.getCore()) + metric.getMetrics() + " ", metric.getCore());
        string algo = des->getAlgorithm();
        std::transform(algo.begin(), algo.end(), algo.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        des->addToGantt({"\"" + des->getAlgorithm() + "\" " + metric.getGanttData(), algo}, metric.getCore());
        gantt_parameters += std::to_string(metric.getCore());
        for (auto gantt: des->getGantt(metric.getCore())) {
            gantt_parameters += " " + gantt.first;
//            auto ganttChartWidget = getPlotFromPythonScript("gantt_chart.py", "gantt_chart_" + gantt.second + ".png", gantt.first);
//            if (des->isUsedFileAsInput()) {
//                containerLayout->addWidget(ganttChartWidget);
//            }
        }
        chart_parameters += std::to_string(metric.getCore()) + " " + des->getPartialMetricsInput(metric.getCore());
//        auto metricsChartWidget = getPlotFromPythonScript("metrics_chart.py", "performance_metrics_plot.png", std::to_string(metric.getCore()) + " " + des->getPartialMetricsInput(metric.getCore()));
        auto metricsTableWidget = getPlotFromPythonScript("metrics_table.py", "performance_metrics_table.png", std::to_string(metric.getCore()) + " " + des->getPartialMetricsInput(metric.getCore()));

//        containerLayout->addWidget(metricsChartWidget);
        containerLayout->addWidget(metricsTableWidget);
    }
    cout << chart_parameters << "\n";
    cout << gantt_parameters << "\n";
    // exemplu input pentru chart + table
//    https://matplotlib.org/stable/gallery/lines_bars_and_markers/barchart.html#sphx-glr-gallery-lines-bars-and-markers-barchart-py
//    https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.subplots.html
    // 0 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000
    // 0 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000 1 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000

    // pentru gantt
    // 0 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]]
    // 0 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]] 1 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]]

    auto metricsChartWidget = getPlotFromPythonScript("metrics_chart.py", "performance_metrics_plot.png", chart_parameters);
    containerLayout->addWidget(metricsChartWidget);


    scrollArea->setWidget(containerWidget);
    ui->running->layout()->addWidget(scrollArea);

    auto *menuButton = new QPushButton(QString::fromStdString("Run and compare results with other algorithm"));

    connect(menuButton, &QPushButton::clicked, this, [=]() {
        QMenu menu(this);

        vector<string> alreadyTried = DES::getAlgorithms();
        for (auto algorithm : ImplementedAlgorithms::getSingleCoreAlgorithms()) {
            if (std::find(alreadyTried.begin(), alreadyTried.end(), algorithm) == alreadyTried.end()) {
                QAction *action = menu.addAction(QString::fromStdString(algorithm));

                connect(action, &QAction::triggered, this, [=](){
                    des->setAlgorithm(algorithm);
                    des->setInputFromString(des->getInput());
                    deleteContent(ui->running);
                    gotoRunning(des);
                });
            }
        }
        menu.exec(QCursor::pos());
        menu.show();
    });

    ui->running->layout()->addWidget(menuButton);
}

QWidget *MainWindow::getPlotFromPythonScript(std::string scriptName, std::string imageName, std::string parameters) {
    std::string source_dir = __FILE__;
    source_dir = source_dir.substr(0, source_dir.find_last_of("\\/") + 1);

    std::string python_executable = "../../Python/plotting/Scripts/python.exe";
    std::string python_script = "../../Python/plotting/" + scriptName;

    std::string command = source_dir + python_executable + " " + source_dir + python_script + " " + parameters;
    system(command.c_str());

    QPixmap pixmap(QString::fromStdString(imageName));
    auto *imageLabel = new QLabel;
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);

    return imageLabel;
}

void MainWindow::deleteContent(QWidget *widget) {
    QLayoutItem *item;

    while ((item = widget->layout()->takeAt(0)) != NULL) {
        delete item->widget();
        delete item;
    }
}