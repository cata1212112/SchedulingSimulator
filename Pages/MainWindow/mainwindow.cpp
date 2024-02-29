//
// Created by Cata on 2/27/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <string>
#include <QFileDialog>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->singlecore, &QPushButton::clicked, this, &MainWindow::handleSingleCoreButton);

    ui->stackedWidget->setCurrentWidget(ui->StartPage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleSingleCoreButton() {
    ui->stackedWidget->setCurrentWidget(ui->SingleCore);

    auto layout = new QVBoxLayout();
    layout->setSpacing(0);
    ui->SingleCore->setLayout(layout);

    for (const auto& algorithm : ImplementedAlgorithms::getSingleCoreAlgorithms()) {
        auto *button = new QPushButton(QString::fromStdString(algorithm));
        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        button->setStyleSheet(QString::fromStdString(ButtonStyle::getButtonStyle()));

        QFont font;
        font.setPointSize(12);
        button->setFont(font);

        connect(button, &QPushButton::clicked, this, [=]() {
            selectedAlgorithm = algorithm;
            ui->stackedWidget->setCurrentWidget(ui->RunSimulation);

            connect(ui->maximumTime, &QSlider::valueChanged, this, [=] (int value){
                ui->timpselectat->setText(QString::fromStdString(to_string(value)));
            });

            connect(ui->numarprocese, &QSlider::valueChanged, this, [=](int value) {
                ui->proceseselectate->setText(QString::fromStdString(to_string(value)));
            });

            DES *des = new DES(algorithm);

            connect(ui->generate, &QPushButton::clicked, this, [=]() {
                string inputData = des->generateInputData(ui->proceseselectate->text().toInt(), ui->timpselectat->text().toInt());
                ui->stackedWidget->setCurrentWidget(ui->InputData);

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
                    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text files (*.txt);;All Files (*)"));

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
            });

            connect(ui->usefileasinput, &QPushButton::clicked, this, [=]() {
                QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));

                if (!fileName.isEmpty())
                {
                    des->readInputDataFromFile(fileName.toStdString());
                    gotoRunning(des);
                }
            });

        });

        ui->SingleCore->layout()->addWidget(button);

    }
}

void MainWindow::gotoRunning(DES *des) {
    ui->stackedWidget->setCurrentWidget(ui->running);

    auto layout = new QVBoxLayout();
    layout->setSpacing(0);
    ui->running->setLayout(layout);

    Metrics metrics = des->startSimulation(1);

    std::string source_dir = __FILE__;
    source_dir = source_dir.substr(0, source_dir.find_last_of("\\/") + 1);

    std::string python_executable = "../../Python/plotting/Scripts/python.exe";
    std::string python_script = "../../Python/plotting/gantt_chart.py";

    // Construct the command
    std::string command = source_dir + python_executable + " " + source_dir + python_script + " \"" + metrics.getGanttData() + "\"";
    system(command.c_str());

    QPixmap pixmap("gantt_chart.png");
    auto *imageLabel = new QLabel;
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);
    ui->running->layout()->addWidget(imageLabel);
}
