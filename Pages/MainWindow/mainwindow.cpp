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


    if (des->isRealTime()) {
        bool plotGantt = false;


        string allGantt = selectedAlgorithm + "\n";
        for (auto m:metrics) {
            if (m.isPromtGantt()) {
                plotGantt = true;
                allGantt = allGantt + m.getGantt();
            }
        }
        if (plotGantt) {
            sendInstructionToPipeAndWait("realtime", "\n"+allGantt);

            QPixmap pixmapBurst(QString::fromStdString("realtime.png"));
            auto *imageLabelBurst = new QLabel;
            imageLabelBurst->setPixmap(pixmapBurst);
            imageLabelBurst->setAlignment(Qt::AlignCenter);

            containerLayout->addWidget(imageLabelBurst);
        }

    } else if (selectedNumberOfCores <= 1){
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

        sendInstructionToPipeAndWait("singlecore", stats);

        QPixmap pixmapBurst(QString::fromStdString("histograms.png"));
        auto *imageLabelBurst = new QLabel;
        imageLabelBurst->setPixmap(pixmapBurst);
        imageLabelBurst->setAlignment(Qt::AlignCenter);

        containerLayout->addWidget(imageLabelBurst);

        QPixmap pixmap(QString::fromStdString("statistici.png"));
        auto *imageLabel = new QLabel;
        imageLabel->setPixmap(pixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        containerLayout->addWidget(imageLabel);

        bool plotGantt = false;
        for (auto m:metrics) {
            if (m.isPromtGantt()) {
                plotGantt = true;
                sendInstructionToPipeAndWait("gantt", "\n"+to_string(m.getCore()) + "\n" + m.getGantt());
            }
        }

        if (plotGantt) {
            QPixmap pixmap(QString::fromStdString("burstTimes.png"));
            auto *imageLabel = new QLabel;
            imageLabel->setPixmap(pixmap);
            imageLabel->setAlignment(Qt::AlignCenter);
            containerLayout->addWidget(imageLabel);
        }
    } else {
        string vtimeDiffs = "";
        for (auto x:metrics[metrics.size() - 1].getMaximumLoadDifference()) {
            vtimeDiffs += to_string(x) + " ";
        }
        sendInstructionToPipeAndWait("fairness", "\n"+vtimeDiffs + "\n" + metrics[metrics.size() - 1].getAlgorithm());
        QPixmap pixmap(QString::fromStdString("fairness.png"));
        auto *imageLabel = new QLabel;
        imageLabel->setPixmap(pixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        containerLayout->addWidget(imageLabel);

    }

    scrollArea->setWidget(containerWidget);
    ui->running->layout()->addWidget(scrollArea);

    auto *menuButton = new QPushButton(QString::fromStdString("Run and compare results with other algorithm"));

    connect(menuButton, &QPushButton::clicked, this, [=, this]() {
        QMenu menu(this);

        vector<string> alreadyTried = DES::getAlgorithms();

        if (des->isRealTime()) {
            for (auto algorithm : ImplementedAlgorithms::getRealTimeAlgortihms()) {
                if (std::find(alreadyTried.begin(), alreadyTried.end(), algorithm) == alreadyTried.end()) {
                    QAction *action = menu.addAction(QString::fromStdString(algorithm));
                    connect(action, &QAction::triggered, this, [=, this](){
                        des->setAlgorithm(algorithm);
                        if (des->filename != "") {
                            des->setInputFromString(des->getInput());
                        } else {
                            des->readInputDataFromFile(des->filename, true);

                        }
                        des->setRoundRobinQuant(quantum);
                        deleteContent(ui->running);
                        gotoRunning(des, numCores);
                    });
                }

            }
            menu.exec(QCursor::pos());
            menu.show();
        } else if (selectedNumberOfCores <= 1){
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
        } else {
            for (auto algorithm : ImplementedAlgorithms::getMultiCoreAlgortihms()) {
                if ( std::find(alreadyTried.begin(), alreadyTried.end(), algorithm) == alreadyTried.end()) {
                    QAction *action = menu.addAction(QString::fromStdString(algorithm));

                    connect(action, &QAction::triggered, this, [=, this](){
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
        }

    });

    if (!des->isRealTime()) {
        ui->running->layout()->addWidget(menuButton);
    }
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


    auto* layout = qobject_cast<QVBoxLayout*>(ui->MultiCore->layout());

    if (!layout) {
        layout = new QVBoxLayout(ui->MultiCore);
        layout->setSpacing(0);
        ui->MultiCore->setLayout(layout);
    }

    selectedNumberOfCores = 2;
    int coreValues[] = {2, 4, 8, 16, 32};
    ui->selectednumberofcores->setText(QString::fromStdString(to_string(2)));
    connect(ui->coreselector, &QSlider::valueChanged, this, [=, this](int value) {
        ui->selectednumberofcores->setText(QString::fromStdString(to_string(coreValues[value])));
        selectedNumberOfCores = coreValues[value];
    });

    connect(ui->next, &QPushButton::clicked, this, [&]() {
        auto* layout = qobject_cast<QVBoxLayout*>(ui->SelectRealTimeAlgorithm->layout());

        if (!layout) {
            layout = new QVBoxLayout(ui->SelectRealTimeAlgorithm);
            layout->setSpacing(0);
            ui->SelectRealTimeAlgorithm->setLayout(layout);
        }

        clearWidgets(layout);
        ui->stackedWidget->setCurrentWidget(ui->SelectRealTimeAlgorithm);

        selectAlgortihmPage(ui->StartPage, ImplementedAlgorithms::getRealTimeAlgortihms(), ui->SelectRealTimeAlgorithm->layout(), true,
                            true);
    });

    connect(ui->back, &QPushButton::clicked, this, [&]() {
        sendInstructionToPipeAndWait("reset", "");
        ui->stackedWidget->setCurrentWidget(ui->StartPage);
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
        sendInstructionToPipeAndWait("reset", "");

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
        if (isRealTime) {
            selectedAlgorithmButtonRealTime(algo, layout);
        } else {
            selectedAlgorithmButton(algo, layout, isMultiCore);
        }
    }
    goBackButton(layout, parent);
}

void MainWindow::selectedAlgorithmButtonRealTime(const std::string& algorithmName, QLayout* layout) {
    auto *button = new QPushButton(QString::fromStdString(algorithmName));
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

    QFont font;
    font.setPointSize(12);
    button->setFont(font);

    connect(button, &QPushButton::clicked, this, [this, algorithmName]() {
        selectedAlgorithm = algorithmName;
        ui->stackedWidget->setCurrentWidget(ui->GenerateRealTimeData);

        ui->horizontalSlider_3->setMinimum(10);
        ui->horizontalSlider_3->setMaximum(selectedNumberOfCores*10);
        connect(ui->horizontalSlider_3, &QSlider::valueChanged, this, [=, this](int value) {
            ui->label_8->setText(QString::fromStdString(to_string((value + 0.0)/10)));
        });

        DES *des = new DES(selectedAlgorithm);
//        des->resetAlgos();
        des->setRealTime(true);

        connect(ui->pushButton_2, &QPushButton::clicked, this, [=, this]() {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

            if (!fileName.isEmpty())
            {
                des->readInputDataFromFile(fileName.toStdString(), true);
                gotoRunning(des, selectedNumberOfCores);
            }
        });

        connect(ui->pushButton, &QPushButton::clicked, this, [=, this]() {
            vector<vector<pair<int,int>>> allGenerated[10];

            vector<int> cntT;
            vector<int> scheduledD;


            double startUtil = ui->label_8->text().toDouble();


            for (int k=0; k<10; k++) {
                    for (int numTasks = int(ui->label_8->text().toDouble())+1; numTasks < int(10 * ui->label_8->text().toDouble()) - 2; numTasks += 2) {

//                        cout << "Experimentul " << k << " numarul de procese " << numTasks << "\n";
                        startUtil = ui->label_8->text().toDouble();
                        int lastcnt = 0;
                        int cnt = 0;
                        int scheduled = 0;
                        int rowZeros = 0;
                        while (cnt < 100) {
//                            cout << k << " " << numTasks << " " << startUtil << " " << cnt[k] << "\n";
//                            cout << cnt << "\n";
                            auto taskSets = DES::generateTaskSet(numTasks, startUtil);

                            for (auto taskSet:taskSets) {
                                double util = 0;
                                for (auto y:taskSet) {
                                    util += (y.first + 0.0) / (y.second + 0.0);
                                }

                                if (!abs(util - ui->label_8->text().toDouble()) < 0.001){
                                    continue;
                                }

                                allGenerated[k].push_back(taskSet);

                                string inputData = "";
                                priority_queue<Event> *events = new priority_queue<Event>();

                                for (auto task : taskSet) {
                                    Process p(task.first, task.second);
                                    inputData += to_string(p.getId()) + " " + to_string(task.first) + " " + to_string(task.second) + "\n";
                                    p.setArrivalTime(0);

                                    Event event = Event(ARRIVAL, 0, p);

                                    events->push(event);
                                }
                                int hyperPeriod = std::lcm(taskSet[0].second, taskSet[1].second);
                                for (int j=2; j<taskSet.size(); j++) {
                                    hyperPeriod = std::lcm(hyperPeriod, taskSet[j].second);
                                }
                                cnt += 1;

                                DES ddes(selectedAlgorithm);
                                ddes.setRealTime(true);
                                ddes.setEvents(events);
                                ddes.setToStop(hyperPeriod);
                                auto m = ddes.startSimulation(selectedNumberOfCores);
                                if (m[m.size() - 1].getContextSwitches() == 0) {
                                    scheduled += 1;
                                }
                            }
                            startUtil += 0.1;
                            if (cnt == lastcnt && lastcnt != 0) break;
                            lastcnt = cnt;
                            if (cnt == 0) rowZeros += 1;

                            if (rowZeros > 2) break;
                        }
                        cout << selectedAlgorithm << ": s-au planificat " << scheduled << " dintr-un total de " << cnt << "\n";
                        scheduledD.push_back(scheduled);
                        cntT.push_back(cnt);
                    }
            }






            ui->stackedWidget->setCurrentWidget(ui->running);
            lastWidget = ui->InputData;

            auto* layout = qobject_cast<QVBoxLayout*>(ui->running->layout());

            double mean = 0;
            double median = 0;
            int len = 0;
            for (int k=0; k<cntT.size(); k++) {
                if (cntT[k] != 0) {
                    mean += (scheduledD[k] + 0.0) / cntT[k] * 100;
                    len += 1;
                }
            }

            mean /= len;

            for (int k=0; k<cntT.size(); k++) {
                if (cntT[k] != 0) {
                    median += ((scheduledD[k] + 0.0) / cntT[k] * 100 - mean) * ((scheduledD[k] + 0.0) / cntT[k] * 100 - mean);
                }
            }
            median = std::sqrt(median / len);

            if (!layout) {
                layout = new QVBoxLayout(ui->running);
                layout->setSpacing(0);
                ui->running->setLayout(layout);
            }

            clearWidgets(layout);



            auto *label = new QLabel(QString::fromStdString("Algoritmul " + selectedAlgorithm + " a planificat in medie " + to_string(mean) + "% din multimi, cu o deviatie standard de " +
                                             to_string(median) + "%."));

            QFont font;
            font.setPointSize(16);
            label->setFont(font);
            layout->addWidget(label);

            goBackButton(ui->running->layout(), ui->StartPage);


        });


        connect(ui->pushButton_3, &QPushButton::clicked, this, [&]() {
           ui->stackedWidget->setCurrentWidget(ui->SelectRealTimeAlgorithm);
        });
    });

    layout->addWidget(button);
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
        sendInstructionToPipeAndWait("reset", "");
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
//    des->resetAlgos();
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

