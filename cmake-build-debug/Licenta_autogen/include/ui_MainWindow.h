/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *StartPage;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *singlecore;
    QPushButton *multicore;
    QPushButton *realtime;
    QWidget *RunSimulation;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label_5;
    QSlider *horizontalSlider;
    QLabel *label_4;
    QLabel *label_6;
    QSlider *horizontalSlider_2;
    QLabel *label;
    QLabel *timpselectat;
    QSlider *maximumTime;
    QLabel *label_2;
    QLabel *proceseselectate;
    QSlider *numarprocese;
    QPushButton *generate;
    QPushButton *usefileasinput;
    QPushButton *goback;
    QWidget *running;
    QWidget *SelectMultiCoreAlgorithm;
    QWidget *SelectRealTimeAlgorithm;
    QWidget *MultiCore;
    QVBoxLayout *verticalLayout_3;
    QLabel *numberofcores;
    QLabel *selectednumberofcores;
    QSlider *coreselector;
    QPushButton *next;
    QPushButton *back;
    QWidget *GenerateRealTimeData;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_7;
    QLabel *label_8;
    QSlider *horizontalSlider_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *InputData;
    QWidget *SingleCore;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1289, 718);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"	background:rgb(0, 255, 255)\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        QFont font;
        font.setPointSize(24);
        stackedWidget->setFont(font);
        StartPage = new QWidget();
        StartPage->setObjectName("StartPage");
        horizontalLayout_2 = new QHBoxLayout(StartPage);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        singlecore = new QPushButton(StartPage);
        singlecore->setObjectName("singlecore");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(singlecore->sizePolicy().hasHeightForWidth());
        singlecore->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(16);
        singlecore->setFont(font1);
        singlecore->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        horizontalLayout_2->addWidget(singlecore);

        multicore = new QPushButton(StartPage);
        multicore->setObjectName("multicore");
        sizePolicy.setHeightForWidth(multicore->sizePolicy().hasHeightForWidth());
        multicore->setSizePolicy(sizePolicy);
        multicore->setFont(font1);
        multicore->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        horizontalLayout_2->addWidget(multicore);

        realtime = new QPushButton(StartPage);
        realtime->setObjectName("realtime");
        sizePolicy.setHeightForWidth(realtime->sizePolicy().hasHeightForWidth());
        realtime->setSizePolicy(sizePolicy);
        realtime->setFont(font1);
        realtime->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        horizontalLayout_2->addWidget(realtime);

        stackedWidget->addWidget(StartPage);
        RunSimulation = new QWidget();
        RunSimulation->setObjectName("RunSimulation");
        verticalLayout = new QVBoxLayout(RunSimulation);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        label_3 = new QLabel(RunSimulation);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3, 0, Qt::AlignmentFlag::AlignHCenter);

        label_5 = new QLabel(RunSimulation);
        label_5->setObjectName("label_5");
        label_5->setFont(font);

        verticalLayout->addWidget(label_5, 0, Qt::AlignmentFlag::AlignHCenter);

        horizontalSlider = new QSlider(RunSimulation);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(horizontalSlider);

        label_4 = new QLabel(RunSimulation);
        label_4->setObjectName("label_4");

        verticalLayout->addWidget(label_4, 0, Qt::AlignmentFlag::AlignHCenter);

        label_6 = new QLabel(RunSimulation);
        label_6->setObjectName("label_6");
        label_6->setFont(font);

        verticalLayout->addWidget(label_6, 0, Qt::AlignmentFlag::AlignHCenter);

        horizontalSlider_2 = new QSlider(RunSimulation);
        horizontalSlider_2->setObjectName("horizontalSlider_2");
        horizontalSlider_2->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(horizontalSlider_2);

        label = new QLabel(RunSimulation);
        label->setObjectName("label");

        verticalLayout->addWidget(label, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        timpselectat = new QLabel(RunSimulation);
        timpselectat->setObjectName("timpselectat");
        timpselectat->setFont(font);

        verticalLayout->addWidget(timpselectat, 0, Qt::AlignmentFlag::AlignHCenter);

        maximumTime = new QSlider(RunSimulation);
        maximumTime->setObjectName("maximumTime");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(maximumTime->sizePolicy().hasHeightForWidth());
        maximumTime->setSizePolicy(sizePolicy1);
        maximumTime->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    border: 1px solid #4682B4;\n"
"    height: 8px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4682B4, stop:1 #87CEEB);\n"
"    margin: 2px 0;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #4682B4);\n"
"    border: 1px solid #4682B4;\n"
"    width: 20px;\n"
"    margin: -2px 0;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #1E90FF);\n"
"}\n"
""));
        maximumTime->setMinimum(1);
        maximumTime->setMaximum(100);
        maximumTime->setTracking(true);
        maximumTime->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(maximumTime);

        label_2 = new QLabel(RunSimulation);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2, 0, Qt::AlignmentFlag::AlignHCenter);

        proceseselectate = new QLabel(RunSimulation);
        proceseselectate->setObjectName("proceseselectate");
        proceseselectate->setFont(font);

        verticalLayout->addWidget(proceseselectate, 0, Qt::AlignmentFlag::AlignHCenter);

        numarprocese = new QSlider(RunSimulation);
        numarprocese->setObjectName("numarprocese");
        QFont font2;
        font2.setPointSize(9);
        numarprocese->setFont(font2);
        numarprocese->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    border: 1px solid #4682B4;\n"
"    height: 8px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4682B4, stop:1 #87CEEB);\n"
"    margin: 2px 0;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #4682B4);\n"
"    border: 1px solid #4682B4;\n"
"    width: 20px;\n"
"    margin: -2px 0;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #1E90FF);\n"
"}\n"
""));
        numarprocese->setMinimum(1);
        numarprocese->setMaximum(50);
        numarprocese->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(numarprocese);

        generate = new QPushButton(RunSimulation);
        generate->setObjectName("generate");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(generate->sizePolicy().hasHeightForWidth());
        generate->setSizePolicy(sizePolicy2);
        generate->setMaximumSize(QSize(16777215, 16));
        generate->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        verticalLayout->addWidget(generate, 0, Qt::AlignmentFlag::AlignVCenter);

        usefileasinput = new QPushButton(RunSimulation);
        usefileasinput->setObjectName("usefileasinput");
        usefileasinput->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        verticalLayout->addWidget(usefileasinput);

        goback = new QPushButton(RunSimulation);
        goback->setObjectName("goback");
        goback->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}\n"
"\n"
""));

        verticalLayout->addWidget(goback);

        stackedWidget->addWidget(RunSimulation);
        running = new QWidget();
        running->setObjectName("running");
        stackedWidget->addWidget(running);
        SelectMultiCoreAlgorithm = new QWidget();
        SelectMultiCoreAlgorithm->setObjectName("SelectMultiCoreAlgorithm");
        stackedWidget->addWidget(SelectMultiCoreAlgorithm);
        SelectRealTimeAlgorithm = new QWidget();
        SelectRealTimeAlgorithm->setObjectName("SelectRealTimeAlgorithm");
        stackedWidget->addWidget(SelectRealTimeAlgorithm);
        MultiCore = new QWidget();
        MultiCore->setObjectName("MultiCore");
        verticalLayout_3 = new QVBoxLayout(MultiCore);
        verticalLayout_3->setObjectName("verticalLayout_3");
        numberofcores = new QLabel(MultiCore);
        numberofcores->setObjectName("numberofcores");

        verticalLayout_3->addWidget(numberofcores, 0, Qt::AlignmentFlag::AlignHCenter);

        selectednumberofcores = new QLabel(MultiCore);
        selectednumberofcores->setObjectName("selectednumberofcores");
        selectednumberofcores->setFont(font);

        verticalLayout_3->addWidget(selectednumberofcores, 0, Qt::AlignmentFlag::AlignHCenter);

        coreselector = new QSlider(MultiCore);
        coreselector->setObjectName("coreselector");
        coreselector->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    border: 1px solid #4682B4;\n"
"    height: 8px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4682B4, stop:1 #87CEEB);\n"
"    margin: 2px 0;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #4682B4);\n"
"    border: 1px solid #4682B4;\n"
"    width: 20px;\n"
"    margin: -2px 0;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #1E90FF);\n"
"}\n"
""));
        coreselector->setMaximum(4);
        coreselector->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout_3->addWidget(coreselector);

        next = new QPushButton(MultiCore);
        next->setObjectName("next");
        next->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        verticalLayout_3->addWidget(next);

        back = new QPushButton(MultiCore);
        back->setObjectName("back");
        back->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        verticalLayout_3->addWidget(back);

        stackedWidget->addWidget(MultiCore);
        GenerateRealTimeData = new QWidget();
        GenerateRealTimeData->setObjectName("GenerateRealTimeData");
        verticalLayout_4 = new QVBoxLayout(GenerateRealTimeData);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_7 = new QLabel(GenerateRealTimeData);
        label_7->setObjectName("label_7");

        verticalLayout_4->addWidget(label_7, 0, Qt::AlignmentFlag::AlignHCenter);

        label_8 = new QLabel(GenerateRealTimeData);
        label_8->setObjectName("label_8");
        label_8->setFont(font);

        verticalLayout_4->addWidget(label_8, 0, Qt::AlignmentFlag::AlignHCenter);

        horizontalSlider_3 = new QSlider(GenerateRealTimeData);
        horizontalSlider_3->setObjectName("horizontalSlider_3");
        horizontalSlider_3->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    border: 1px solid #4682B4;\n"
"    height: 8px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4682B4, stop:1 #87CEEB);\n"
"    margin: 2px 0;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #4682B4);\n"
"    border: 1px solid #4682B4;\n"
"    width: 20px;\n"
"    margin: -2px 0;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #87CEEB, stop:1 #1E90FF);\n"
"}\n"
""));
        horizontalSlider_3->setMinimum(2);
        horizontalSlider_3->setMaximum(100);
        horizontalSlider_3->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout_4->addWidget(horizontalSlider_3);

        pushButton = new QPushButton(GenerateRealTimeData);
        pushButton->setObjectName("pushButton");
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        verticalLayout_4->addWidget(pushButton);

        pushButton_2 = new QPushButton(GenerateRealTimeData);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background: rgb(0, 170, 255);\n"
"	border-radius: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgb(0, 145, 217)\n"
"}"));

        verticalLayout_4->addWidget(pushButton_2);

        stackedWidget->addWidget(GenerateRealTimeData);
        InputData = new QWidget();
        InputData->setObjectName("InputData");
        stackedWidget->addWidget(InputData);
        SingleCore = new QWidget();
        SingleCore->setObjectName("SingleCore");
        stackedWidget->addWidget(SingleCore);

        verticalLayout_2->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        singlecore->setText(QCoreApplication::translate("MainWindow", "Single Core", nullptr));
        multicore->setText(QCoreApplication::translate("MainWindow", "Multi Core", nullptr));
        realtime->setText(QCoreApplication::translate("MainWindow", "Real Time", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "MEAN", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "STANDARD DEVIATION", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "MAXIMUM PROCESS ARRIVAL TIME", nullptr));
        timpselectat->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "NUMBER OF PROCESSES", nullptr));
        proceseselectate->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        generate->setText(QCoreApplication::translate("MainWindow", "Generate New Data", nullptr));
        usefileasinput->setText(QCoreApplication::translate("MainWindow", "Use File as Input", nullptr));
        goback->setText(QCoreApplication::translate("MainWindow", "Go back", nullptr));
        numberofcores->setText(QCoreApplication::translate("MainWindow", "NUMBER OF CORES", nullptr));
        selectednumberofcores->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        next->setText(QCoreApplication::translate("MainWindow", "Next", nullptr));
        back->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "NUMBER OF TASKS", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "GENERATE", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Use file as input", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
