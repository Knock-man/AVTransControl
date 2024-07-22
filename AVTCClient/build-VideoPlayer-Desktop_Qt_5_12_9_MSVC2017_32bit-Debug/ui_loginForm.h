/********************************************************************************
** Form generated from reading UI file 'loginForm.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFORM_H
#define UI_LOGINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QLabel *logo;
    QLabel *head;
    QLabel *nameLb;
    QFrame *lineup;
    QLineEdit *nameEdit;
    QLineEdit *pwdEdit;
    QFrame *linedown;
    QLabel *passwdLb;
    QCheckBox *autoLoginCheck;
    QCheckBox *remberPwd;
    QLabel *forget;
    QPushButton *logoButton;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName(QString::fromUtf8("LoginForm"));
        LoginForm->resize(340, 510);
        LoginForm->setWindowOpacity(1.000000000000000);
        LoginForm->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(39, 40, 47), stop:1 rgb(63, 65, 76));\n"
"color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgb(63, 65, 76));"));
        logo = new QLabel(LoginForm);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(100, 36, 141, 32));
        logo->setStyleSheet(QString::fromUtf8("image: url(:/UI/images/logo.png);"));
        head = new QLabel(LoginForm);
        head->setObjectName(QString::fromUtf8("head"));
        head->setGeometry(QRect(140, 105, 72, 72));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(32);
        sizePolicy.setVerticalStretch(32);
        sizePolicy.setHeightForWidth(head->sizePolicy().hasHeightForWidth());
        head->setSizePolicy(sizePolicy);
        head->setSizeIncrement(QSize(0, 0));
        head->setAutoFillBackground(false);
        head->setStyleSheet(QString::fromUtf8("image: url(:/UI/images/headimg-login.png);\n"
"background-color: transparent;"));
        nameLb = new QLabel(LoginForm);
        nameLb->setObjectName(QString::fromUtf8("nameLb"));
        nameLb->setGeometry(QRect(45, 215, 40, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setBold(true);
        font.setWeight(75);
        font.setStyleStrategy(QFont::PreferAntialias);
        nameLb->setFont(font);
        nameLb->setStyleSheet(QString::fromUtf8("color: rgb(251, 251, 251);\n"
"background-color: transparent;"));
        nameLb->setTextFormat(Qt::PlainText);
        nameLb->setTextInteractionFlags(Qt::NoTextInteraction);
        lineup = new QFrame(LoginForm);
        lineup->setObjectName(QString::fromUtf8("lineup"));
        lineup->setGeometry(QRect(45, 272, 265, 1));
        lineup->setStyleSheet(QString::fromUtf8("color: rgb(48, 50, 60);\n"
"background-color: rgb(48, 50, 60);"));
        lineup->setFrameShape(QFrame::HLine);
        lineup->setFrameShadow(QFrame::Sunken);
        nameEdit = new QLineEdit(LoginForm);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        nameEdit->setGeometry(QRect(45, 245, 265, 23));
        nameEdit->setStyleSheet(QString::fromUtf8("color: rgb(71, 75, 94);\n"
"background-color: transparent;"));
        pwdEdit = new QLineEdit(LoginForm);
        pwdEdit->setObjectName(QString::fromUtf8("pwdEdit"));
        pwdEdit->setGeometry(QRect(45, 330, 265, 23));
        pwdEdit->setStyleSheet(QString::fromUtf8("color: rgb(71, 75, 94);\n"
"background-color: transparent;"));
        linedown = new QFrame(LoginForm);
        linedown->setObjectName(QString::fromUtf8("linedown"));
        linedown->setGeometry(QRect(45, 360, 265, 1));
        linedown->setStyleSheet(QString::fromUtf8("color: rgb(48, 50, 60);\n"
"background-color: rgb(48, 50, 60);"));
        linedown->setFrameShape(QFrame::HLine);
        linedown->setFrameShadow(QFrame::Sunken);
        passwdLb = new QLabel(LoginForm);
        passwdLb->setObjectName(QString::fromUtf8("passwdLb"));
        passwdLb->setGeometry(QRect(45, 300, 40, 20));
        passwdLb->setFont(font);
        passwdLb->setStyleSheet(QString::fromUtf8("color: rgb(251, 251, 251);\n"
"background-color: transparent;"));
        passwdLb->setTextFormat(Qt::PlainText);
        passwdLb->setTextInteractionFlags(Qt::NoTextInteraction);
        autoLoginCheck = new QCheckBox(LoginForm);
        autoLoginCheck->setObjectName(QString::fromUtf8("autoLoginCheck"));
        autoLoginCheck->setGeometry(QRect(45, 390, 85, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(7);
        autoLoginCheck->setFont(font1);
        autoLoginCheck->setStyleSheet(QString::fromUtf8("color: rgb(146, 148, 168);\n"
"background-color: transparent;"));
        remberPwd = new QCheckBox(LoginForm);
        remberPwd->setObjectName(QString::fromUtf8("remberPwd"));
        remberPwd->setGeometry(QRect(135, 390, 85, 20));
        remberPwd->setFont(font1);
        remberPwd->setStyleSheet(QString::fromUtf8("color: rgb(146, 148, 168);\n"
"background-color: transparent;"));
        forget = new QLabel(LoginForm);
        forget->setObjectName(QString::fromUtf8("forget"));
        forget->setGeometry(QRect(230, 390, 90, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(8);
        forget->setFont(font2);
        forget->setStyleSheet(QString::fromUtf8("color: rgb(92, 180, 20);\n"
"background-color: transparent;"));
        forget->setWordWrap(false);
        forget->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);
        logoButton = new QPushButton(LoginForm);
        logoButton->setObjectName(QString::fromUtf8("logoButton"));
        logoButton->setGeometry(QRect(50, 430, 265, 28));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        logoButton->setFont(font3);
        logoButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	color: rgb(251, 251, 251);\n"
"	border:0px groove gray;\n"
"    border-radius:5px;\n"
"    padding:2px 4px;\n"
"    border-style: outset;\n"
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
        logoButton->setFlat(false);

        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QWidget *LoginForm)
    {
        logo->setText(QString());
        head->setText(QString());
        nameLb->setText(QApplication::translate("LoginForm", "\350\264\246\345\217\267", nullptr));
        pwdEdit->setInputMask(QString());
        passwdLb->setText(QApplication::translate("LoginForm", "\345\257\206\347\240\201", nullptr));
        autoLoginCheck->setText(QApplication::translate("LoginForm", "\350\207\252\345\212\250\347\231\273\345\275\225", nullptr));
        remberPwd->setText(QApplication::translate("LoginForm", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        forget->setText(QApplication::translate("LoginForm", "\345\277\230\350\256\260\346\202\250\347\232\204\345\257\206\347\240\201\357\274\237", nullptr));
        logoButton->setText(QApplication::translate("LoginForm", "\347\231\273\345\275\225", nullptr));
        Q_UNUSED(LoginForm);
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
