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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *StartPage;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *singlecore;
    QPushButton *multicore;
    QPushButton *realtime;
    QWidget *SingleCore;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1302, 744);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"	background:rgb(0, 255, 255)\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        StartPage = new QWidget();
        StartPage->setObjectName("StartPage");
        horizontalLayout_2 = new QHBoxLayout(StartPage);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        singlecore = new QPushButton(StartPage);
        singlecore->setObjectName("singlecore");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(singlecore->sizePolicy().hasHeightForWidth());
        singlecore->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(16);
        singlecore->setFont(font);
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
        multicore->setFont(font);
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
        realtime->setFont(font);
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
        SingleCore = new QWidget();
        SingleCore->setObjectName("SingleCore");
        stackedWidget->addWidget(SingleCore);

        horizontalLayout->addWidget(stackedWidget);

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
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
