//
// Created by Cata on 2/27/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <iostream>
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

            DES *des = new DES();

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


                ui->InputData->layout()->addWidget(button);
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

    auto *movie = new QMovie("../../Assets/loading.gif");
    auto *processLabel = new QLabel(this);

    processLabel->setMovie(movie);
    movie->start();

    ui->running->layout()->addWidget(processLabel);

}
