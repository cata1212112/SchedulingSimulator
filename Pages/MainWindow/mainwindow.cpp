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

    connect(ui->realtime, &QPushButton::clicked, this, &MainWindow::handleRealTimeButton);

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

        connect(button, &QPushButton::clicked, this, [=, this]() {
            selectedAlgorithm = algorithm;

            int quantum = 0;
            if (selectedAlgorithm == "Round Robin") {
                bool ok = false;
                quantum = QInputDialog::getInt(this, tr("Enter Quantum"), tr("Quantum:"), 1, 1, 100, 1, &ok);
            }

            ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
            lastWidget = ui->SingleCore;

            connect(ui->maximumTime, &QSlider::valueChanged, this, [=, this] (int value){
                ui->timpselectat->setText(QString::fromStdString(to_string(value)));
            });

            connect(ui->numarprocese, &QSlider::valueChanged, this, [=, this](int value) {
                ui->proceseselectate->setText(QString::fromStdString(to_string(value)));
            });

            connect(ui->horizontalSlider, &QSlider::valueChanged, this, [=, this](int value) {
                ui->label_5->setText(QString::fromStdString(to_string(value)));
            });

            connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, [=, this](int value) {
                ui->label_6->setText(QString::fromStdString(to_string(value)));
            });

            DES *des = new DES(selectedAlgorithm);
            des->setRoundRobinQuant(quantum);

            connect(ui->generate, &QPushButton::clicked, this, [=, this]() {
                int mean = ui->label_5->text().toInt();
                int std = ui->label_6->text().toInt();
                string inputData = des->generateInputData(ui->proceseselectate->text().toInt(), ui->timpselectat->text().toInt(), mean, std);
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

                    connect(button, &QPushButton::clicked, this, [=, this]() {
                        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                                        tr("Text files (*.txt);;All Files (*)"));

                        if (!fileName.isEmpty()) {
                            QFile file(fileName);
                            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                QTextStream out(&file);
                                out << QString::fromStdString(inputData);
                                file.close();
                                gotoRunning(des, 1);
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

                    connect(button1, &QPushButton::clicked, this, [=, this]() {
                        gotoRunning(des, 1);
                    });


                    ui->InputData->layout()->addWidget(button);
                    ui->InputData->layout()->addWidget(button1);

                    auto *button3 = new QPushButton(QString::fromStdString("Go back"));
                    button3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                    button3->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

                    button3->setFont(font);

                    connect(button3, &QPushButton::clicked, this, [=, this]() {
                        ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
                    });

                    ui->InputData->layout()->addWidget(button3);
                }
            });

            connect(ui->usefileasinput, &QPushButton::clicked, this, [=, this]() {
                QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

                if (!fileName.isEmpty())
                {
                    des->readInputDataFromFile(fileName.toStdString());
                    gotoRunning(des, 1);
                }
            });

            connect(ui->goback, &QPushButton::clicked, this, [=, this]() {
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

    connect(button, &QPushButton::clicked, this, [=, this]() {
        deleteContent(ui->SingleCore);
        ui->stackedWidget->setCurrentWidget(lastWidget);
    });

    ui->SingleCore->layout()->addWidget(button);
}

void MainWindow::handleMultiCoreButton() {
    ui->stackedWidget->setCurrentWidget(ui->MultiCore);
    int coreValues[] = {2, 4, 8, 16, 32};
    ui->selectednumberofcores->setText(QString::fromStdString(to_string(2)));
    connect(ui->coreselector, &QSlider::valueChanged, this, [=, this](int value) {
        ui->selectednumberofcores->setText(QString::fromStdString(to_string(coreValues[value])));
    });

    connect(ui->back, &QPushButton::clicked, this, [&]() {
       ui->stackedWidget->setCurrentWidget(ui->StartPage);
    });

    connect(ui->next, &QPushButton::clicked, this, [&]() {
        int selectedNumberOfCores = ui->selectednumberofcores->text().toInt();
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

            connect(button, &QPushButton::clicked, this, [=, this]() {
                selectedAlgorithm = algorithm;

                int quantum = 10;
                if (selectedAlgorithm == "Round Robin") {
                    bool ok = false;
                    quantum = QInputDialog::getInt(this, tr("Enter Quantum"), tr("Quantum:"), 1, 1, 100, 1, &ok);
                }

                ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
                lastWidget = ui->SingleCore;

                connect(ui->maximumTime, &QSlider::valueChanged, this, [=, this] (int value){
                    ui->timpselectat->setText(QString::fromStdString(to_string(value)));
                });

                connect(ui->numarprocese, &QSlider::valueChanged, this, [=, this](int value) {
                    ui->proceseselectate->setText(QString::fromStdString(to_string(value)));
                });

                DES *des = new DES(selectedAlgorithm);
                des->setRoundRobinQuant(quantum);

                connect(ui->generate, &QPushButton::clicked, this, [=, this]() {
                    int mean = ui->label_5->text().toInt();
                    int std = ui->label_6->text().toInt();
                    string inputData = des->generateInputData(ui->proceseselectate->text().toInt(), ui->timpselectat->text().toInt(), mean, std);
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

                        connect(button, &QPushButton::clicked, this, [=, this]() {
                            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                                            tr("Text files (*.txt);;All Files (*)"));

                            if (!fileName.isEmpty()) {
                                QFile file(fileName);
                                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                    QTextStream out(&file);
                                    out << QString::fromStdString(inputData);
                                    file.close();
                                    gotoRunning(des, selectedNumberOfCores);
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

                        connect(button1, &QPushButton::clicked, this, [=, this]() {
                            gotoRunning(des, selectedNumberOfCores);
                        });


                        ui->InputData->layout()->addWidget(button);
                        ui->InputData->layout()->addWidget(button1);

                        auto *button3 = new QPushButton(QString::fromStdString("Go back"));
                        button3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                        button3->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

                        button3->setFont(font);

                        connect(button3, &QPushButton::clicked, this, [=, this]() {
                            ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
                        });

                        ui->InputData->layout()->addWidget(button3);
                    }
                });

                connect(ui->usefileasinput, &QPushButton::clicked, this, [=, this]() {
                    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

                    if (!fileName.isEmpty())
                    {
                        des->readInputDataFromFile(fileName.toStdString());
                        gotoRunning(des, selectedNumberOfCores);
                    }
                });

                connect(ui->goback, &QPushButton::clicked, this, [=, this]() {
                    deleteContent(ui->SingleCore);
                    handleSingleCoreButton();
                });
            });

            ui->SelectMultiCoreAlgorithm->layout()->addWidget(button);
        }
    });

}

void MainWindow::gotoRunning(DES *des, int numCores) {
    ui->stackedWidget->setCurrentWidget(ui->running);
    lastWidget = ui->InputData;

    auto layout = new QVBoxLayout();
    layout->setSpacing(0);
    ui->running->setLayout(layout);

    auto *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    auto *containerWidget = new QWidget;
    auto *containerLayout = new QVBoxLayout(containerWidget);

    vector<Metrics> metrics = des->startSimulation(numCores);

    string chart_parameters = "";
    string gantt_parameters = "";

    for (auto metric : metrics) {
        des->setPartialMetricsInput(des->getPartialMetricsInput(metric.getCore()) + metric.getMetrics() + " ", metric.getCore());
        string algo = des->getAlgorithm();
        std::transform(algo.begin(), algo.end(), algo.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        des->addToGantt({"\"" + des->getAlgorithm() + "\" " + metric.getGanttData(), algo}, metric.getCore());
        gantt_parameters += " --core " + std::to_string(metric.getCore());
        for (auto gantt: des->getGantt(metric.getCore())) {
            gantt_parameters += " " + gantt.first;
        }
        chart_parameters += " --core " + std::to_string(metric.getCore()) + " " + des->getPartialMetricsInput(metric.getCore());
    }

    auto metricsChartWidget = getPlotFromPythonScript("metrics_chart.py", "performance_metrics_plot.png", chart_parameters);
    containerLayout->addWidget(metricsChartWidget);

    auto metricsTableWidget = getPlotFromPythonScript("metrics_table.py", "performance_metrics_table.png", chart_parameters);
    containerLayout->addWidget(metricsTableWidget);

    auto ganttChart = getPlotFromPythonScript("gantt_chart.py", "gantt_chart.png", gantt_parameters);
    containerLayout->addWidget(ganttChart);


    scrollArea->setWidget(containerWidget);
    ui->running->layout()->addWidget(scrollArea);

    auto *menuButton = new QPushButton(QString::fromStdString("Run and compare results with other algorithm"));

    connect(menuButton, &QPushButton::clicked, this, [=, this]() {
        QMenu menu(this);

        vector<string> alreadyTried = DES::getAlgorithms();
        for (auto algorithm : ImplementedAlgorithms::getSingleCoreAlgorithms()) {
            if (algorithm == "Round Robin" || std::find(alreadyTried.begin(), alreadyTried.end(), algorithm) == alreadyTried.end()) {
                QAction *action = menu.addAction(QString::fromStdString(algorithm));

                connect(action, &QAction::triggered, this, [=, this](){
                    int quantum = 0;
                    if (algorithm == "Round Robin") {
                        bool ok;
                        quantum = QInputDialog::getInt(this, tr("Enter Quantum"), tr("Quantum:"), 1, 1, 100, 1, &ok);
                    }
                    des->setAlgorithm(algorithm);
                    des->setInputFromString(des->getInput());
                    des->setRoundRobinQuant(quantum);
                    deleteContent(ui->running);
                    gotoRunning(des, numCores);
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

void MainWindow::handleRealTimeButton() {
    ui->stackedWidget->setCurrentWidget(ui->MultiCore);
    int coreValues[] = {2, 4, 8, 16, 32};
    ui->selectednumberofcores->setText(QString::fromStdString(to_string(2)));
    connect(ui->coreselector, &QSlider::valueChanged, this, [=, this](int value) {
        ui->selectednumberofcores->setText(QString::fromStdString(to_string(coreValues[value])));
    });

    connect(ui->back, &QPushButton::clicked, this, [&]() {
        ui->stackedWidget->setCurrentWidget(ui->StartPage);
    });

    connect(ui->next, &QPushButton::clicked, this, [&]() {
        int selectedNumberOfCores = ui->selectednumberofcores->text().toInt();
        ui->stackedWidget->setCurrentWidget(ui->SelectRealTimeAlgorithm);
        auto layout = new QVBoxLayout();
        layout->setSpacing(0);

        ui->SelectRealTimeAlgorithm->setLayout(layout);
        for (const auto& algorithm : ImplementedAlgorithms::getRealTimeAlgortihms()) {
            auto *button = new QPushButton(QString::fromStdString(algorithm));
            button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

            QFont font;
            font.setPointSize(12);
            button->setFont(font);

            connect(button, &QPushButton::clicked, this, [=, this]() {
                selectedAlgorithm = algorithm;

                ui->stackedWidget->setCurrentWidget(ui->GenerateRealTimeData);
                ui->label_8->setText(QString::fromStdString(to_string(selectedNumberOfCores * 2)));

                ui->horizontalSlider_3->setMinimum(selectedNumberOfCores);
                ui->horizontalSlider_3->setMaximum(selectedNumberOfCores * 10);
                connect(ui->horizontalSlider_3, &QSlider::valueChanged, this, [=, this](int value) {
                    ui->label_8->setText(QString::fromStdString(to_string(value)));
                });

                DES *des = new DES(selectedAlgorithm);
                des->setRealTime(true);

                connect(ui->pushButton, &QPushButton::clicked, this, [=, this]() {
                    int numberOfTasks = ui->label_8->text().toInt();
                    string inputData = des->generateInputData(numberOfTasks, selectedNumberOfCores);

                    ui->stackedWidget->setCurrentWidget(ui->InputData);

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

                        connect(button, &QPushButton::clicked, this, [=, this]() {
                            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                                            tr("Text files (*.txt);;All Files (*)"));

                            if (!fileName.isEmpty()) {
                                QFile file(fileName);
                                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                    QTextStream out(&file);
                                    out << QString::fromStdString(inputData);
                                    file.close();
                                    gotoRunning(des, selectedNumberOfCores);
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

                        connect(button1, &QPushButton::clicked, this, [=, this]() {
                            gotoRunning(des, selectedNumberOfCores);
                        });


                        ui->InputData->layout()->addWidget(button);
                        ui->InputData->layout()->addWidget(button1);

                        auto *button3 = new QPushButton(QString::fromStdString("Go back"));
                        button3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                        button3->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

                        button3->setFont(font);

                        connect(button3, &QPushButton::clicked, this, [=, this]() {
                            ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
                        });

                        ui->InputData->layout()->addWidget(button3);
                    }

                });

                connect(ui->pushButton_2, &QPushButton::clicked, this, [=, this]() {
                    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

                    if (!fileName.isEmpty())
                    {
                        des->readInputDataFromFile(fileName.toStdString(), true);
                        gotoRunning(des, selectedNumberOfCores);
                    }
                });
            });
            ui->SelectRealTimeAlgorithm->layout()->addWidget(button);
            lastWidget = ui->RunSimulation;
        }
    });
}
