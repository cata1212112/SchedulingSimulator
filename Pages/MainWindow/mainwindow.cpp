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

    std::thread pipeThread([](){

        std::string source_dir = __FILE__;
        source_dir = source_dir.substr(0, source_dir.find_last_of("\\/") + 1);

        std::string python_executable = "../../Python/plotting/Scripts/python.exe";
        std::string python_script = "../../Python/plotting/pipe.py";

        std::string command = source_dir + python_executable + " " + source_dir + python_script;

        std::system(command.c_str());
    });

    cout << "ID-ul threadului este " << pipeThread.get_id() << "\n";

    pipeThread.detach();

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
    selectedNumberOfCores = 1;
    ui->stackedWidget->setCurrentWidget(ui->SingleCore);

    auto* layout = qobject_cast<QVBoxLayout*>(ui->SingleCore->layout());

    if (!layout) {
        layout = new QVBoxLayout(ui->SingleCore);
        layout->setSpacing(0);
        ui->SingleCore->setLayout(layout);
    }

    clearWidgets(layout);

    selectAlgortihmPage(ui->StartPage, ImplementedAlgorithms::getSingleCoreAlgorithms(), layout, false, false);
}

void MainWindow::handleMultiCoreButton() {
    ui->stackedWidget->setCurrentWidget(ui->MultiCore);

    auto* layout = qobject_cast<QVBoxLayout*>(ui->MultiCore->layout());

    if (!layout) {
        layout = new QVBoxLayout(ui->MultiCore);
        layout->setSpacing(0);
        ui->MultiCore->setLayout(layout);
    }

//    clearWidgets(layout);
    selectNumberOfCores();
}


void MainWindow::gotoRunning(DES *des, int numCores) {
//    cout << des->getAlgorithm() << "\n";
//    cout << des->isRealTime() << "\n";

    ui->stackedWidget->setCurrentWidget(ui->running);
    lastWidget = ui->InputData;

    auto* layout = qobject_cast<QVBoxLayout*>(ui->running->layout());

    if (!layout) {
        layout = new QVBoxLayout(ui->running);
        layout->setSpacing(0);
        ui->running->setLayout(layout);
    }

    clearWidgets(layout);

    auto *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    auto *containerWidget = new QWidget;
    auto *containerLayout = new QVBoxLayout(containerWidget);

    vector<Metrics> metrics = des->startSimulation(numCores);

    string stats = metrics[0].getMetrics() + "\n";
    for (auto x:des->getGeneratedBurstByDistributionId()) {
        string distributionString = to_string(x.first) + " " + to_string(des->getGeneratedBurstByDistributionMean()[x.first]) + " " +
                to_string(des->getGeneratedBurstByDistributionStd()[x.first]) + " ";
        for (auto xx:x.second) {
            distributionString += to_string(xx) + " ";
        }
        distributionString += "\n";
        stats += distributionString;
    }

    HANDLE hPipe = CreateNamedPipe(
            L"\\\\.\\pipe\\plotting_pipe",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, 65536, 65536,
            0,
            NULL
    );

    HANDLE hFeedbackPipe = CreateNamedPipe(
            L"\\\\.\\pipe\\plotting_pipe_feedback",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, 65536, 65536,
            0,
            NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create main pipe. " << std::endl;
        std::cerr << GetLastError() << "\n";
    }

    if (hFeedbackPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create feedback pipe." << std::endl;
        std::cerr << GetLastError() << "\n";

        CloseHandle(hPipe);
    }
    ConnectNamedPipe(hPipe, NULL);
    DWORD dwWritten;
    if (!WriteFile(hPipe, stats.c_str(), stats.length(), &dwWritten, NULL)) {
        std::cerr << "Failed to write to main pipe." << std::endl;
        std::cerr << GetLastError() << std::endl;

        CloseHandle(hPipe);
        CloseHandle(hFeedbackPipe);
        return;
    }

    ConnectNamedPipe(hFeedbackPipe, NULL);

    char buffer[128];
    DWORD dwRead;
    if (ReadFile(hFeedbackPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
        buffer[dwRead] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to read from feedback pipe." << std::endl;
        std::cerr << GetLastError() << "\n";
    }

    CloseHandle(hPipe);
    CloseHandle(hFeedbackPipe);




    QPixmap pixmap(QString::fromStdString("statistici.png"));
    auto *imageLabel = new QLabel;
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);

    containerLayout->addWidget(imageLabel);
//
//    auto metricsTableWidget = getPlotFromPythonScript("metrics_table.py", "performance_metrics_table.png", chart_parameters);
//    containerLayout->addWidget(metricsTableWidget);
//
//    auto ganttChart = getPlotFromPythonScript("gantt_chart.py", "gantt_chart.png", gantt_parameters);
//    containerLayout->addWidget(ganttChart);


    scrollArea->setWidget(containerWidget);
    ui->running->layout()->addWidget(scrollArea);

    auto *menuButton = new QPushButton(QString::fromStdString("Run and compare results with other algorithm"));

    connect(menuButton, &QPushButton::clicked, this, [=, this]() {
        QMenu menu(this);

        vector<string> alreadyTried = DES::getAlgorithms();
        for (auto algorithm : ImplementedAlgorithms::getSingleCoreAlgorithms()) {
            if (algorithm == "Round Robin" || algorithm == "Mean Threshold Shortest Job Round Robin" || std::find(alreadyTried.begin(), alreadyTried.end(), algorithm) == alreadyTried.end()) {
                QAction *action = menu.addAction(QString::fromStdString(algorithm));

                connect(action, &QAction::triggered, this, [=, this](){
                    int quantum = 0;
                    bool ok;
                    if (algorithm == "Round Robin" || algorithm == "Mean Threshold Shortest Job Round Robin") {
                        quantum = QInputDialog::getInt(this, tr("Enter Quantum"), tr("Quantum:"), 1, 1, 100, 1, &ok);
                        if (ok) {
                            des->setAlgorithm(algorithm);
                            des->setInputFromString(des->getInput());
                            des->setRoundRobinQuant(quantum);
                            deleteContent(ui->running);
                            gotoRunning(des, numCores);
                        }
                    } else {
                        des->setAlgorithm(algorithm);
                        des->setInputFromString(des->getInput());
                        des->setRoundRobinQuant(quantum);
                        deleteContent(ui->running);
                        gotoRunning(des, numCores);
                    }
                });
            }
        }
        menu.exec(QCursor::pos());
        menu.show();
    });

    ui->running->layout()->addWidget(menuButton);
    goBackButton(ui->running->layout(), ui->StartPage);
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
//                    selectedNumberOfCores = 3;
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
//                                    gotoRunning(des, selectedNumberOfCores);
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

        auto *button = new QPushButton(QString::fromStdString("Evaluate all the above"));
        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

        QFont font;
        font.setPointSize(12);
        button->setFont(font);

        connect(button, &QPushButton::clicked, this, [=, this]() {

            for (double i=2.1; i<=2 * selectedNumberOfCores; i+=0) {
                vector<vector<pair<int,int>>> taskSet;// = DES::generateTaskSet(int(10 * i / 1), i);

                auto f = [&taskSet, i, selectedNumberOfCores](string algname) {
                    int scheduled = 0;
                    DES des(algname);
                    des.setRealTime(true);
                    int cnt = 0;
                    int valid = 0;
                    int validScheduled = 0;
                    for (auto &tasks:taskSet) {

                        double util = 0;
                        for (auto y:tasks) {
                            util += (y.first + 0.0) / (y.second + 0.0);
                        }
//                        cout << util << "\n";

                        if (!(abs(util - 3.9) < 0.0001)) {
                            continue;
                        }

//                        cout << util << "\n";
                        string inputData;
                        priority_queue<Event> *events = new priority_queue<Event>();

                        for (auto task:tasks) {
                            Process p(task.first, task.second);
                            inputData += to_string(p.getId()) + " " + to_string(task.first) + " " + to_string(task.second) + "\n";
                            p.setArrivalTime(0);

                            Event event = Event(ARRIVAL, 0, p);

                            events->push(event);
                        }

                        int hyperPeriod = std::lcm(tasks[0].second, tasks[1].second);
                        for (int j=2; j<tasks.size(); j++) {
                            hyperPeriod = std::lcm(hyperPeriod, tasks[j].second);
                        }
                        des.setEvents(events);
                        des.setToStop(hyperPeriod);

//                        cout << hyperPeriod << "\n";
                        auto m = des.startSimulation(selectedNumberOfCores);
//                        if (m[m.size() - 1].getContextSwitches() == 0) {
//                            scheduled += 1;
//                        } else {
//                            ofstream out("out.txt");
//                            for (auto t:tasks) {
//                                out << t.first << " " << t.second << "\n";
//                            }
//                            out.close();
//                        }

                        if (m[m.size() - 1].getContextSwitches() == 0) {
                            validScheduled += 1;
                            ofstream out("outPoate.txt");
                            for (auto t:tasks) {
                                out << t.first << " " << t.second << "\n";
                            }
                            out.close();
//                            exit(0);
                        } else {
                            ofstream out("out.txt");
                            for (auto t:tasks) {
                                out << t.first << " " << t.second << "\n";
                            }
                            out.close();
                        }
                        valid += 1;
//                        cout << util << " " << (m[m.size() - 1].getContextSwitches() == 0) << "\n";
//                        cout << scheduled << "\n";

//                        cout << "Done " << ++cnt << "/1000\n";
                    }
                    cout << i << " " << algname << " " << validScheduled << "/" << valid << "\n";
                };

                int a_min = 6;
                int b_min = 38;
                for (int cnt = a_min; cnt <= b_min; cnt+=2) {
                    for (int j=0; j<10; j++) {
                        cout << "Numarul de taskuri este " << cnt << "\n";
                        taskSet = DES::generateTaskSet(cnt, 4.0);
                        auto newTaskset = DES::generateTaskSet(cnt, 4.1);
                        taskSet.insert(taskSet.end(), newTaskset.begin(), newTaskset.end());

                        newTaskset = DES::generateTaskSet(cnt, 4.2);
                        taskSet.insert(taskSet.end(), newTaskset.begin(), newTaskset.end());

                        newTaskset = DES::generateTaskSet(cnt, 4.3);
                        taskSet.insert(taskSet.end(), newTaskset.begin(), newTaskset.end());

                        std::thread t1(f, ImplementedAlgorithms::getRealTimeAlgortihms()[0]);
                        std::thread t2(f, ImplementedAlgorithms::getRealTimeAlgortihms()[1]);
                        std::thread t3(f, ImplementedAlgorithms::getRealTimeAlgortihms()[2]);
                        t1.join();
                        t2.join();
                        t3.join();
                    }
                }
                cout << "DONEEEE\n";
                exit(0);
//                std::thread t2(f, ImplementedAlgorithms::getRealTimeAlgortihms()[1]);
//                std::thread t3(f, ImplementedAlgorithms::getRealTimeAlgortihms()[2]);

//                t2.join();
//                t3.join();
//                for (auto &s:ImplementedAlgorithms::getRealTimeAlgortihms()) {
//                    int scheduled = 0;
//                    DES des(s);
//                    des.setRealTime(true);
//                    for (auto &tasks:taskSet) {
//
//                        string inputData;
//                        priority_queue<Event> *events = new priority_queue<Event>();
//
//                        for (auto task:tasks) {
//                            Process p(task.first, task.second);
//                            inputData += to_string(p.getId()) + " " + to_string(task.first) + " " + to_string(task.second) + "\n";
//                            p.setArrivalTime(0);
//
//                            Event event = Event(ARRIVAL, 0, p);
//
//                            events->push(event);
//                        }
//
//                        int hyperPeriod = std::lcm(tasks[0].second, tasks[1].second);
//                        for (int j=2; j<tasks.size(); j++) {
//                            hyperPeriod = std::lcm(hyperPeriod, tasks[j].second);
//                        }
//                        des.setEvents(events);
//                        des.setToStop(hyperPeriod);
//
//                        auto m = des.startSimulation(selectedNumberOfCores);
//                        if (m[m.size() - 1].getContextSwitches() == 0) {
//                            scheduled += 1;
//                        }
////                        cout << scheduled << "\n";
//                    }
//                    cout << i << " " << s << " " << scheduled << "\n";
//                }
            }
        });

        ui->SelectRealTimeAlgorithm->layout()->addWidget(button);

    });
}

void MainWindow::selectNumberOfCores() {
    selectedNumberOfCores = 2;
    int coreValues[] = {2, 4, 8, 16, 32};
    ui->selectednumberofcores->setText(QString::fromStdString(to_string(2)));
    connect(ui->coreselector, &QSlider::valueChanged, this, [=, this](int value) {
        ui->selectednumberofcores->setText(QString::fromStdString(to_string(coreValues[value])));
        selectedNumberOfCores = ui->selectednumberofcores->text().toInt();

    });
    connect(ui->back, &QPushButton::clicked, this, [&]() {
        ui->stackedWidget->setCurrentWidget(ui->StartPage);
    });
    connect(ui->next, &QPushButton::clicked, this, [&] {

        auto* layout = qobject_cast<QVBoxLayout*>(ui->SelectMultiCoreAlgorithm->layout());

        if (!layout) {
            layout = new QVBoxLayout(ui->SelectMultiCoreAlgorithm);
            layout->setSpacing(0);
            ui->SelectMultiCoreAlgorithm->setLayout(layout);
        }

        clearWidgets(layout);
        ui->stackedWidget->setCurrentWidget(ui->SelectMultiCoreAlgorithm);

        selectAlgortihmPage(ui->StartPage, ImplementedAlgorithms::getMultiCoreAlgortihms(), ui->SelectMultiCoreAlgorithm->layout(), true, false);
    });
}

void MainWindow::selectAlgortihmPage(QWidget *parent, const vector<std::string> &algorithms, QLayout *layout, bool isMultiCore, bool isRealTime) {
    for (const auto &algo : algorithms) {
        selectedAlgorithmButton(algo, layout, isMultiCore);
    }
    goBackButton(layout, parent);
}

void MainWindow::selectedAlgorithmButton(const string &algorithmName, QLayout *layout, bool isMultiCore) {
    auto *button = new QPushButton(QString::fromStdString(algorithmName));
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

    QFont font;
    font.setPointSize(12);
    button->setFont(font);

    connect(button, &QPushButton::clicked, this, [this, algorithmName, isMultiCore]() {
        selectAlgortihm(algorithmName, isMultiCore);
    });

    layout->addWidget(button);
}

void MainWindow::goBackButton(QLayout *layout, QWidget *parent) {
    auto *goBackButton = new QPushButton("Go Back");
    goBackButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    goBackButton->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

    QFont font;
    font.setPointSize(12);
    goBackButton->setFont(font);

    connect(goBackButton, &QPushButton::clicked, this, [this, parent, &layout]() {
        ui->stackedWidget->setCurrentWidget(parent);
    });

    layout->addWidget(goBackButton);
}

void MainWindow::selectAlgortihm(const string &algortihm, bool isMultiCore) {
    selectedAlgorithm = algortihm;
    bool ok = false;
    if (selectedAlgorithm == "Round Robin" || selectedAlgorithm == "Mean Threshold Shortest Job Round Robin") {

        quantum = QInputDialog::getInt(this, tr("Enter Quantum"), tr("Quantum:"), 1, 1, 100, 1, &ok);
        if (ok) {
            setupInputData(ui->SingleCore, isMultiCore);
            ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
        }
    } else {
        setupInputData(ui->SingleCore, isMultiCore);
        ui->stackedWidget->setCurrentWidget(ui->RunSimulation);
    }
}

void MainWindow::clearWidgets(QLayout *layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::setupInputData(QWidget *parent, bool isMultiCore) {

    connect(ui->maximumTime, &QSlider::valueChanged, this, [=, this] (int value){
        ui->timpselectat->setText(QString::fromStdString(to_string(value)));

    });


    QLayoutItem *child;
    while ((child = ui->parametriiDistributiii->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, [=, this](int value) {
        ui->label_6->setText(QString::fromStdString(to_string(value)));
        if (value == 1) {
            ui->medie2->setText(QString::fromStdString("Distributia 2 are media N/A"));
            ui->std2->setText(QString::fromStdString("Distributia 2 are deviatia standard N/A"));
            ui->numproc2->setText(QString::fromStdString("Din distributia 2 generez N/A procese"));

            ui->medie3->setText(QString::fromStdString("Distributia 3 are media N/A"));
            ui->std3->setText(QString::fromStdString("Distributia 3 are deviatia standard N/A"));
            ui->numproc3->setText(QString::fromStdString("Din distributia 3 generez N/A procese"));

        } else if (value == 2) {
            ui->medie3->setText(QString::fromStdString("Distributia 3 are media N/A"));
            ui->std3->setText(QString::fromStdString("Distributia 3 are deviatia standard N/A"));
            ui->numproc3->setText(QString::fromStdString("Din distributia 3 generez N/A procese"));
        }

        QLayoutItem *child;
        while ((child = ui->parametriiDistributiii->layout()->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        for (int i = 0; i < value; ++i) {
            QPushButton *selectButton = new QPushButton(QString("Select Mean, Std Dev for Distribution %1 and the number of processes").arg(i + 1));

            ui->parametriiDistributiii->layout()->addWidget(selectButton);
            connect(selectButton, &QPushButton::clicked, this, [=]() {
                double mean = QInputDialog::getDouble(this, tr("Input Mean"), tr("Mean for Distribution %1:").arg(i + 1), 0, 1, 10000, 2);
                double stddev = QInputDialog::getDouble(this, tr("Input Std Dev"), tr("Standard Deviation for Distribution %1:").arg(i + 1), 1, 0, 10000, 2);
                int numberOfProcs = QInputDialog::getInt(this, tr("Input number processes"), tr("Number of processes generated from Distribution %1").arg(i+1), 1, 1, 1000);

                if (i == 0) {
                    ui->medie1->setText(QString::fromStdString("Distributia 1 are media " + to_string(mean)));
                    ui->std1->setText(QString::fromStdString("Distributia 1 are deviatia standard " + to_string(stddev)));
                    ui->numproc1->setText(QString::fromStdString("Din distributia 1 generez " + to_string(numberOfProcs) +  " procese"));
                } else if (i == 1) {
                    ui->medie2->setText(QString::fromStdString("Distributia 2 are media " + to_string(mean)));
                    ui->std2->setText(QString::fromStdString("Distributia 2 are deviatia standard " + to_string(stddev)));
                    ui->numproc2->setText(QString::fromStdString("Din distributia 2 generez " + to_string(numberOfProcs) +  " procese"));

                } else if (i == 2) {
                    ui->medie3->setText(QString::fromStdString("Distributia 3 are media " + to_string(mean)));
                    ui->std3->setText(QString::fromStdString("Distributia 3 are deviatia standard " + to_string(stddev)));
                    ui->numproc3->setText(QString::fromStdString("Din distributia 3 generez " + to_string(numberOfProcs) +  " procese"));

                }
            });
        }

    });

    connect(ui->goback, &QPushButton::clicked, this, [=, this]() {
        ui->stackedWidget->setCurrentWidget(parent);
    });

    connect(ui->generate, &QPushButton::clicked, this, [=, this]() {
        auto* layout = qobject_cast<QVBoxLayout*>(ui->InputData->layout());

        if (!layout) {
            layout = new QVBoxLayout(ui->InputData);
            layout->setSpacing(0);
            ui->InputData->setLayout(layout);
        }

        clearWidgets(layout);

        generateDataButton(ui->InputData->layout(), ui->RunSimulation, isMultiCore);
    });

    connect(ui->usefileasinput, &QPushButton::clicked, this, [=, this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

        if (!fileName.isEmpty())
        {
            DES *des = new DES(selectedAlgorithm);
            des->setRoundRobinQuant(quantum);
            des->setIsMultiCore(isMultiCore);
            des->readInputDataFromFile(fileName.toStdString(), false);


//            selectedNumberOfCores = 1;


            gotoRunning(des, selectedNumberOfCores);
        }
    });
}

void MainWindow::generateDataButton(QLayout *layout, QWidget *parent, bool isMultiCore) {

    DES *des = new DES(selectedAlgorithm);
    des->setRoundRobinQuant(quantum);
    des->setIsMultiCore(isMultiCore);

    ui->stackedWidget->setCurrentWidget(ui->InputData);

    int mean1 = 0;
    int std1 = 0;
    int numProc1 = 0;

    int mean2 = 0;
    int std2 = 0;
    int numProc2 = 0;


    int mean3 = 0;
    int std3 = 0;
    int numProc3 = 0;


    std::regex regexMedie("Distributia \\d+ are media ([+-]?\\d*\\.?\\d+)");
    std::regex regexStdDev("Distributia \\d+ are deviatia standard ([+-]?\\d*\\.?\\d+)");
    std::regex regexNumarProc("Din distributia (\\d+) generez (\\d+) procese");
    std::smatch match;

    string inputMedie1 = ui->medie1->text().toStdString();
    if (std::regex_search(inputMedie1, match, regexMedie)) {
        if (match.size() > 1) {
            std::string xStr = match[1].str();
            double x = std::stod(xStr);
            mean1 = x;
        }
    }

    string inputStd1 = ui->std1->text().toStdString();
    if (std::regex_search(inputStd1, match, regexStdDev)) {
        if (match.size() > 1) {
            std::string xStr = match[1].str();
            double x = std::stod(xStr);
            std1 = x;
        }
    }

    string inputNrProc1 = ui->numproc1->text().toStdString();
    if (std::regex_search(inputNrProc1, match, regexNumarProc)) {
        if (match.size() > 1) {
            int processes = std::stoi(match[2].str());
            numProc1 = processes;
        }
    }

    string inputMedie2 = ui->medie2->text().toStdString();
    if (std::regex_search(inputMedie2, match, regexMedie)) {
        if (match.size() > 1) {
            std::string xStr = match[1].str();
            double x = std::stod(xStr);
            mean2 = x;
        }
    }

    string inputStd2 = ui->std2->text().toStdString();
    if (std::regex_search(inputStd2, match, regexStdDev)) {
        if (match.size() > 1) {
            std::string xStr = match[1].str();
            double x = std::stod(xStr);
            std2 = x;
        }
    }

    string inputNrProc2 = ui->numproc2->text().toStdString();
    if (std::regex_search(inputNrProc2, match, regexNumarProc)) {
        if (match.size() > 2) {
            int distribution = std::stoi(match[1].str());
            int processes = std::stoi(match[2].str());
            numProc2 = processes;
        }
    }

    string inputMedie3 = ui->medie3->text().toStdString();
    if (std::regex_search(inputMedie3, match, regexMedie)) {
        if (match.size() > 1) {
            std::string xStr = match[1].str();
            double x = std::stod(xStr);
            mean3 = x;
        }
    }

    string inputStd3 = ui->std3->text().toStdString();
    if (std::regex_search(inputStd3, match, regexStdDev)) {
        if (match.size() > 1) {
            std::string xStr = match[1].str();
            double x = std::stod(xStr);
            std3 = x;
        }
    }

    string inputNrProc3 = ui->numproc3->text().toStdString();
    if (std::regex_search(inputNrProc3, match, regexNumarProc)) {
        if (match.size() > 2) {
            int distribution = std::stoi(match[1].str());
            int processes = std::stoi(match[2].str());
            numProc3 = processes;
        }
    }

    string inputData = des->generateInputData({numProc1, numProc2, numProc3}, ui->timpselectat->text().toInt(), {mean1, mean2, mean3}, {std1, std2, std3});

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


    goBackButton(layout, parent);
}

