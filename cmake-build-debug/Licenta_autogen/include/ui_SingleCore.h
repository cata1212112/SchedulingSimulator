/********************************************************************************
** Form generated from reading UI file 'singlecore.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SINGLECORE_H
#define UI_SINGLECORE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SingleCore
{
public:

    void setupUi(QWidget *SingleCore)
    {
        if (SingleCore->objectName().isEmpty())
            SingleCore->setObjectName("SingleCore");
        SingleCore->resize(400, 300);

        retranslateUi(SingleCore);

        QMetaObject::connectSlotsByName(SingleCore);
    } // setupUi

    void retranslateUi(QWidget *SingleCore)
    {
        SingleCore->setWindowTitle(QCoreApplication::translate("SingleCore", "SingleCore", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SingleCore: public Ui_SingleCore {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SINGLECORE_H
