/********************************************************************************
** Form generated from reading UI file 'infoform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOFORM_H
#define UI_INFOFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InfoForm
{
public:
    QPushButton *connectButton;
    QPushButton *closeButton;
    QLabel *label;
    QLabel *textLb;

    void setupUi(QWidget *InfoForm)
    {
        if (InfoForm->objectName().isEmpty())
            InfoForm->setObjectName(QString::fromUtf8("InfoForm"));
        InfoForm->resize(420, 240);
        InfoForm->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        connectButton = new QPushButton(InfoForm);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(130, 150, 160, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(10);
        connectButton->setFont(font);
        connectButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	color: rgb(251, 251, 251);\n"
"	border:0px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(61,163,241, 255), stop:1 rgba(36,95,207, 255));\n"
"}\n"
"QPushButton:hover{\n"
"	color: rgb(251, 251, 251);\n"
"	border:0px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(36, 95, 207, 255), stop:1 rgba(61, 163, 241, 255));\n"
"}\n"
"QPushButton:pressed{\n"
"	color: rgb(251, 251, 251);\n"
"	border:0px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(61,163,241, 255), stop:1 rgba(36,95,207, 255));\n"
"}"));
        closeButton = new QPushButton(InfoForm);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(380, 10, 25, 25));
        closeButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"image: url(:/UI/images/close-tanchuang.png);\n"
"background-color: transparent;\n"
"border:0px;\n"
"}\n"
"QPushButton:hover{\n"
"image: url(:/UI/images/close-tanchuang.png);\n"
"background-color: transparent;\n"
"border:0px;\n"
"}\n"
"QPushButton:pressed{\n"
"image: url(:/UI/images/close-tanchuang.png);\n"
"background-color: transparent;\n"
"border:0px;\n"
"}"));
        label = new QLabel(InfoForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 420, 72));
        label->setStyleSheet(QString::fromUtf8("background-image: url(:/UI/images/tanchuangbg.jpg);\n"
"background-color: transparent;"));
        textLb = new QLabel(InfoForm);
        textLb->setObjectName(QString::fromUtf8("textLb"));
        textLb->setGeometry(QRect(60, 90, 300, 40));
        textLb->setFont(font);
        textLb->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        textLb->setWordWrap(true);
        connectButton->raise();
        label->raise();
        closeButton->raise();
        textLb->raise();

        retranslateUi(InfoForm);

        QMetaObject::connectSlotsByName(InfoForm);
    } // setupUi

    void retranslateUi(QWidget *InfoForm)
    {
        InfoForm->setWindowTitle(QApplication::translate("InfoForm", "Form", nullptr));
        connectButton->setText(QApplication::translate("InfoForm", "\347\202\271\345\207\273\350\264\255\344\271\260VIP", nullptr));
        closeButton->setText(QString());
        label->setText(QString());
        textLb->setText(QApplication::translate("InfoForm", "\345\215\263\345\260\206\350\277\233\345\205\245VIP\346\222\255\346\224\276\351\203\250\345\210\206\n"
"\350\257\267\350\264\255\344\271\260vip", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InfoForm: public Ui_InfoForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOFORM_H
