/********************************************************************************
** Form generated from reading UI file 'messageform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEFORM_H
#define UI_MESSAGEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageForm
{
public:
    QLabel *label;

    void setupUi(QWidget *MessageForm)
    {
        if (MessageForm->objectName().isEmpty())
            MessageForm->setObjectName(QString::fromUtf8("MessageForm"));
        MessageForm->resize(200, 30);
        MessageForm->setWindowOpacity(0.950000000000000);
        MessageForm->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        label = new QLabel(MessageForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 200, 30));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255, 0);\n"
"color: rgb(255, 255, 255);"));

        retranslateUi(MessageForm);

        QMetaObject::connectSlotsByName(MessageForm);
    } // setupUi

    void retranslateUi(QWidget *MessageForm)
    {
        MessageForm->setWindowTitle(QApplication::translate("MessageForm", "Form", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MessageForm: public Ui_MessageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEFORM_H
