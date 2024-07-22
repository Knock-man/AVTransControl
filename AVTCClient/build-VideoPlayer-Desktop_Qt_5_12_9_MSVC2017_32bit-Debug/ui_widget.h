/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "qmediaplayer.h"
#include "qscreentop.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QMediaPlayer *media;
    QListWidget *listWidget;
    QSlider *horizontalSlider;
    QPushButton *playButton;
    QPushButton *preButton;
    QPushButton *nextButton;
    QPushButton *volumeButton;
    QLabel *timeLb;
    QSlider *volumeSlider;
    QPushButton *showhideList;
    QLabel *backgroundLb;
    QLabel *label;
    QLabel *logoLabel;
    QLabel *userInfo;
    QLabel *loginstatus;
    QPushButton *settingBtn;
    QPushButton *miniButton;
    QPushButton *fangdaButton;
    QPushButton *closeButton;
    QLabel *playInfo;
    QLabel *playInfoIco;
    QLabel *curplay;
    QPushButton *stopButton;
    QLabel *time2Lb;
    QPushButton *fullscreenBtn;
    QPushButton *settingButton;
    QPushButton *scaleButton;
    QPushButton *autoButton;
    QLabel *downbkgndLb;
    QScreenTop *screentop;
    QPushButton *time2;
    QPushButton *time1_5;
    QPushButton *time1;
    QPushButton *time0_5;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1500, 800);
        Widget->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));\n"
"color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));"));
        media = new QMediaPlayer(Widget);
        media->setObjectName(QString::fromUtf8("media"));
        media->setGeometry(QRect(0, 45, 1200, 690));
        media->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));\n"
"color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));"));
        media->setAlignment(Qt::AlignCenter);
        listWidget = new QListWidget(Widget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(1200, 90, 300, 710));
        listWidget->setMouseTracking(true);
        listWidget->setTabletTracking(true);
        listWidget->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(72, 71, 76); \n"
"}\n"
"QListWidget::Item{ \n"
"	padding-bottom:4px; padding-bottom:4px; background-color:rgb(72, 71, 76);\n"
"	color: rgb(218, 218, 229);\n"
"}\n"
"QListWidget::Item:hover{\n"
"	padding-top:4px; padding-bottom:4px; background-color: rgb(85, 255, 255); \n"
"}\n"
"QListWidget::item:selected{\n"
"	padding-top:4px; padding-bottom:4px; background-color: rgb(55, 56, 61); \n"
"	color:rgb(61,163,241); \n"
"}\n"
"QListWidget::item:selected:!active{\n"
"	padding-top:4px; padding-bottom:4px; background-color:rgb(72, 71, 76);\n"
"	color:rgb(122,122,128); \n"
"}"));
        listWidget->setFrameShape(QFrame::NoFrame);
        listWidget->setFrameShadow(QFrame::Plain);
        listWidget->setLineWidth(1);
        listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        horizontalSlider = new QSlider(Widget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(0, 732, 1200, 22));
        horizontalSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"	background: rgb(121,121,128);\n"
"	height: 4px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"	background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, stop: 0 rgb(61,163,241), stop: 1 rgb(36,95,207));\n"
"	height: 4px;\n"
"}\n"
"QSlider::groove:horizontal:hover {\n"
"	background: rgb(121,121,128);\n"
"	height: 8px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal:hover {\n"
"	background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, stop: 0 rgb(61,163,241), stop: 1 rgb(36,95,207));\n"
"	height: 8px;\n"
"}\n"
"QSlider::add-page:horizontal {\n"
"	background: #575757;\n"
"	border: 0px solid #777;\n"
"	height: 10px;\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal \n"
"{\n"
"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, \n"
"stop:0.6 rgb(61,163,241), stop:0.778409 rgb(255, 255, 255));\n"
"    width: 11px;\n"
"    margin-top: -3px;\n"
"    margin-bottom: -3px;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:hor"
                        "izontal:hover {\n"
"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA, \n"
"    stop:0.778409 rgba(255, 255, 255, 255));\n"
"\n"
"    width: 11px;\n"
"    margin-top: -3px;\n"
"    margin-bottom: -3px;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal:disabled {\n"
"	background: #00009C;\n"
"	border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal:disabled {\n"
"	background: #eee;\n"
"	border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:disabled {\n"
"	background: #eee;\n"
"	border: 1px solid #aaa;\n"
"	border-radius: 4px;\n"
"}"));
        horizontalSlider->setValue(50);
        horizontalSlider->setOrientation(Qt::Horizontal);
        playButton = new QPushButton(Widget);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setGeometry(QRect(5, 755, 32, 32));
        playButton->setAutoFillBackground(false);
        playButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/ui/button/blue/UI/blue/bofang.png);\n"
"}\n"
"QPushButton:hover{\n"
"	border-image: url(:/ui/button/gray/UI/grey/bofang.png);\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image: url(:/ui/button/white/UI/white/bofang.png);\n"
"}"));
        playButton->setFlat(true);
        preButton = new QPushButton(Widget);
        preButton->setObjectName(QString::fromUtf8("preButton"));
        preButton->setGeometry(QRect(110, 755, 32, 32));
        preButton->setAutoFillBackground(false);
        preButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/UI/images/shangyige.png);\n"
"	background-color: transparent;\n"
"}\n"
"QPushButton:hover{\n"
"	border-image: url(:/UI/images/shangyige-hover.png);\n"
"	background-color: transparent;\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image: url(:/UI/images/shangyige.png);\n"
"	background-color: transparent;\n"
"}"));
        preButton->setFlat(true);
        nextButton = new QPushButton(Widget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(160, 755, 32, 32));
        nextButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/UI/images/xiayige.png);\n"
"	background-color: transparent;\n"
"}\n"
"QPushButton:hover{\n"
"	border-image: url(:/UI/images/xiayige-hover.png);\n"
"	background-color: transparent;\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image: url(:/UI/images/xiayige.png);\n"
"	background-color: transparent;\n"
"}"));
        nextButton->setFlat(true);
        volumeButton = new QPushButton(Widget);
        volumeButton->setObjectName(QString::fromUtf8("volumeButton"));
        volumeButton->setGeometry(QRect(1100, 755, 32, 32));
        volumeButton->setStyleSheet(QString::fromUtf8("QPushButton#volumeButton{\n"
"border-image: url(:/UI/images/voice.png);\n"
"background-color: transparent;\n"
"}\n"
"QPushButton#volumeButton:hover{border-image: url(:/UI/images/voice-hover.png);background-color: transparent;}\n"
"QPushButton#volumeButton:pressed{border-image: url(:/UI/images/voice.png);background-color: transparent;}"));
        volumeButton->setFlat(true);
        timeLb = new QLabel(Widget);
        timeLb->setObjectName(QString::fromUtf8("timeLb"));
        timeLb->setGeometry(QRect(205, 762, 91, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(12);
        timeLb->setFont(font);
        timeLb->setStyleSheet(QString::fromUtf8("color: rgb(194, 194, 204);\n"
"background-color: rgba(255, 255, 255, 0);"));
        timeLb->setAlignment(Qt::AlignCenter);
        volumeSlider = new QSlider(Widget);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setGeometry(QRect(1102, 595, 22, 160));
        volumeSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:vertical {\n"
"	border: 1px solid #4A708B;\n"
"	background: #C0C0C0;\n"
"	width: 5px;\n"
"	border-radius: 1px;\n"
"	padding-left:-1px;\n"
"	padding-right:-1px;\n"
"	padding-top:-1px;\n"
"	padding-bottom:-1px;\n"
"}\n"
"\n"
"QSlider::sub-page:vertical {\n"
"	background: #575757;\n"
"	border: 1px solid #4A708B;\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::add-page:vertical {\n"
"	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \n"
"    stop:0 #c4c4c4, stop:1 #B1B1B1);\n"
"	background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\n"
"    stop: 0 #5DCCFF, stop: 1 #1874CD);\n"
"	border: 0px solid #777;\n"
"	width: 10px;\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::handle:vertical \n"
"{\n"
"	background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #45ADED, \n"
"	stop:0.778409 rgba(255, 255, 255, 255));\n"
"    height: 11px;\n"
"    margin-left: -3px;\n"
"    margin-right: -3px;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QSlider::sub-page:vertical:disa"
                        "bled {\n"
"	background: #00009C;\n"
"	border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:vertical:disabled {\n"
"	background: #eee;\n"
"	border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:vertical:disabled {\n"
"	background: #eee;\n"
"	border: 1px solid #aaa;\n"
"	border-radius: 4px;\n"
"}"));
        volumeSlider->setMaximum(100);
        volumeSlider->setValue(100);
        volumeSlider->setOrientation(Qt::Vertical);
        showhideList = new QPushButton(Widget);
        showhideList->setObjectName(QString::fromUtf8("showhideList"));
        showhideList->setGeometry(QRect(1140, 320, 60, 60));
        showhideList->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/arrow-right.png);background-color: transparent;}\n"
"QPushButton:hover{border-image: url(:/UI/images/arrow-right.png);background-color: transparent;}\n"
"QPushButton:pressed{border-image: url(:/UI/images/arrow-right.png);background-color: transparent;}\n"
""));
        showhideList->setFlat(true);
        backgroundLb = new QLabel(Widget);
        backgroundLb->setObjectName(QString::fromUtf8("backgroundLb"));
        backgroundLb->setGeometry(QRect(0, 41, 1500, 700));
        backgroundLb->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));\n"
"color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 1, 1));
        logoLabel = new QLabel(Widget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setGeometry(QRect(5, 5, 140, 30));
        logoLabel->setStyleSheet(QString::fromUtf8("image: url(:/UI/images/logo.png);"));
        userInfo = new QLabel(Widget);
        userInfo->setObjectName(QString::fromUtf8("userInfo"));
        userInfo->setGeometry(QRect(1230, 7, 28, 28));
        userInfo->setStyleSheet(QString::fromUtf8("image: url(:/UI/images/headimg-default.png);"));
        loginstatus = new QLabel(Widget);
        loginstatus->setObjectName(QString::fromUtf8("loginstatus"));
        loginstatus->setGeometry(QRect(1270, 7, 45, 25));
        loginstatus->setStyleSheet(QString::fromUtf8("color: rgb(194, 194, 204);"));
        settingBtn = new QPushButton(Widget);
        settingBtn->setObjectName(QString::fromUtf8("settingBtn"));
        settingBtn->setGeometry(QRect(1340, 5, 30, 30));
        settingBtn->setStyleSheet(QString::fromUtf8("QPushButton#settingBtn{border-image: url(:/UI/images/shezhi.png);}\n"
"QPushButton#settingBtn:hover{border-image: url(:/UI/images/shezhi-hover.png);} \n"
"QPushButton#settingBtn:pressed{border-image: url(:/UI/images/shezhi.png);}"));
        settingBtn->setFlat(true);
        miniButton = new QPushButton(Widget);
        miniButton->setObjectName(QString::fromUtf8("miniButton"));
        miniButton->setGeometry(QRect(1380, 5, 30, 30));
        miniButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/suoxiao.png);}\n"
"QPushButton:hover{border-image: url(:/UI/images/suoxiao-hover.png);} \n"
"QPushButton:pressed{border-image: url(:/UI/images/suoxiao.png);}"));
        miniButton->setFlat(true);
        fangdaButton = new QPushButton(Widget);
        fangdaButton->setObjectName(QString::fromUtf8("fangdaButton"));
        fangdaButton->setGeometry(QRect(1420, 5, 32, 32));
        fangdaButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/fangda.png);}\n"
"QPushButton:hover{border-image: url(:/UI/images/fangda-hover.png);} \n"
"QPushButton:pressed{border-image: url(:/UI/images/fangda.png);}"));
        fangdaButton->setFlat(true);
        closeButton = new QPushButton(Widget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(1460, 5, 30, 30));
        closeButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/close.png);}\n"
"QPushButton:hover{border-image: url(:/UI/images/close-hover.png);} \n"
"QPushButton:pressed{border-image: url(:/UI/images/close.png);}"));
        closeButton->setFlat(true);
        playInfo = new QLabel(Widget);
        playInfo->setObjectName(QString::fromUtf8("playInfo"));
        playInfo->setGeometry(QRect(1202, 43, 296, 46));
        playInfo->setStyleSheet(QString::fromUtf8("background-color: rgb(94, 94, 97);"));
        playInfoIco = new QLabel(Widget);
        playInfoIco->setObjectName(QString::fromUtf8("playInfoIco"));
        playInfoIco->setGeometry(QRect(1200, 41, 32, 50));
        playInfoIco->setStyleSheet(QString::fromUtf8("image: url(:/UI/images/shezhi.png);\n"
"background-color: rgba(255, 255, 255, 0);"));
        curplay = new QLabel(Widget);
        curplay->setObjectName(QString::fromUtf8("curplay"));
        curplay->setGeometry(QRect(1235, 50, 260, 32));
        curplay->setAutoFillBackground(false);
        curplay->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"font: 10pt \"\351\273\221\344\275\223\";\n"
"color: rgb(255, 255, 255);\n"
"\n"
""));
        stopButton = new QPushButton(Widget);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setGeometry(QRect(60, 755, 32, 32));
        stopButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/UI/images/stop.png);\n"
"	background-color: transparent;\n"
"}\n"
"QPushButton:hover{\n"
"	border-image: url(:/UI/images/stop-hover.png);\n"
"	background-color: transparent;\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image: url(:/UI/images/stop.png);\n"
"	background-color: transparent;\n"
"}"));
        stopButton->setFlat(true);
        time2Lb = new QLabel(Widget);
        time2Lb->setObjectName(QString::fromUtf8("time2Lb"));
        time2Lb->setGeometry(QRect(300, 762, 105, 15));
        time2Lb->setFont(font);
        time2Lb->setStyleSheet(QString::fromUtf8("color: rgb(121, 121, 128);\n"
"background-color: rgba(255, 255, 255, 0);"));
        fullscreenBtn = new QPushButton(Widget);
        fullscreenBtn->setObjectName(QString::fromUtf8("fullscreenBtn"));
        fullscreenBtn->setGeometry(QRect(1150, 755, 32, 32));
        fullscreenBtn->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/quanping.png);background-color: transparent;}\n"
"QPushButton:hover{border-image: url(:/UI/images/quanping-hover.png);background-color: transparent;} \n"
"QPushButton:pressed{border-image: url(:/UI/images/quanping.png);background-color: transparent;}\n"
""));
        fullscreenBtn->setFlat(true);
        settingButton = new QPushButton(Widget);
        settingButton->setObjectName(QString::fromUtf8("settingButton"));
        settingButton->setGeometry(QRect(1050, 755, 32, 32));
        settingButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/bofangshezhi.png);background-color: transparent;}\n"
"QPushButton:hover{border-image: url(:/UI/images/bofangshezhi-hover.png);background-color: transparent;} \n"
"QPushButton:pressed{border-image: url(:/UI/images/bofangshezhi.png);background-color: transparent;}\n"
""));
        settingButton->setIconSize(QSize(32, 32));
        settingButton->setFlat(true);
        scaleButton = new QPushButton(Widget);
        scaleButton->setObjectName(QString::fromUtf8("scaleButton"));
        scaleButton->setGeometry(QRect(1000, 755, 42, 32));
        scaleButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/beisu.png);background-color: transparent;}\n"
"QPushButton:hover{border-image: url(:/UI/images/beisu-hover.png);background-color: transparent;} \n"
"QPushButton:pressed{border-image: url(:/UI/images/beisu.png);background-color: transparent;}"));
        scaleButton->setFlat(true);
        autoButton = new QPushButton(Widget);
        autoButton->setObjectName(QString::fromUtf8("autoButton"));
        autoButton->setGeometry(QRect(940, 755, 42, 32));
        autoButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/UI/images/zidong.png);background-color: transparent;}\n"
"QPushButton:hover{border-image: url(:/UI/images/zidong-hover.png);background-color: transparent;} \n"
"QPushButton:pressed{border-image: url(:/UI/images/zidong.png);background-color: transparent;}"));
        autoButton->setFlat(true);
        downbkgndLb = new QLabel(Widget);
        downbkgndLb->setObjectName(QString::fromUtf8("downbkgndLb"));
        downbkgndLb->setGeometry(QRect(700, 800, 1, 1));
        downbkgndLb->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));\n"
"color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(39, 40, 47, 255), stop:1 rgba(40, 42, 53, 255));"));
        screentop = new QScreenTop(Widget);
        screentop->setObjectName(QString::fromUtf8("screentop"));
        screentop->setGeometry(QRect(0, 0, 1, 1));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(20);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        screentop->setFont(font1);
        screentop->setStyleSheet(QString::fromUtf8(""));
        time2 = new QPushButton(Widget);
        time2->setObjectName(QString::fromUtf8("time2"));
        time2->setGeometry(QRect(1000, 643, 42, 28));
        time2->setAutoFillBackground(false);
        time2->setStyleSheet(QString::fromUtf8("QPushButton{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}\n"
"QPushButton:hover{color:rgb(61,143,241);background-color: rgba(40,40,48,255);} \n"
"QPushButton:pressed{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}"));
        time2->setFlat(true);
        time1_5 = new QPushButton(Widget);
        time1_5->setObjectName(QString::fromUtf8("time1_5"));
        time1_5->setGeometry(QRect(1000, 671, 42, 28));
        time1_5->setAutoFillBackground(false);
        time1_5->setStyleSheet(QString::fromUtf8("QPushButton{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}\n"
"QPushButton:hover{color:rgb(61,143,241);background-color: rgba(40,40,48,255);} \n"
"QPushButton:pressed{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}"));
        time1_5->setFlat(true);
        time1 = new QPushButton(Widget);
        time1->setObjectName(QString::fromUtf8("time1"));
        time1->setGeometry(QRect(1000, 699, 42, 28));
        time1->setAutoFillBackground(false);
        time1->setStyleSheet(QString::fromUtf8("QPushButton{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}\n"
"QPushButton:hover{color:rgb(61,143,241);background-color: rgba(40,40,48,255);} \n"
"QPushButton:pressed{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}"));
        time1->setFlat(true);
        time0_5 = new QPushButton(Widget);
        time0_5->setObjectName(QString::fromUtf8("time0_5"));
        time0_5->setGeometry(QRect(1000, 727, 42, 28));
        time0_5->setAutoFillBackground(false);
        time0_5->setStyleSheet(QString::fromUtf8("QPushButton{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}\n"
"QPushButton:hover{color:rgb(61,143,241);background-color: rgba(40,40,48,255);} \n"
"QPushButton:pressed{color:rgb(255,255,255);background-color: rgba(40,40,48,217);}"));
        time0_5->setFlat(true);
        backgroundLb->raise();
        media->raise();
        downbkgndLb->raise();
        listWidget->raise();
        horizontalSlider->raise();
        preButton->raise();
        nextButton->raise();
        volumeButton->raise();
        timeLb->raise();
        volumeSlider->raise();
        label->raise();
        logoLabel->raise();
        userInfo->raise();
        loginstatus->raise();
        settingBtn->raise();
        miniButton->raise();
        fangdaButton->raise();
        closeButton->raise();
        showhideList->raise();
        playInfo->raise();
        playInfoIco->raise();
        curplay->raise();
        stopButton->raise();
        time2Lb->raise();
        fullscreenBtn->raise();
        settingButton->raise();
        scaleButton->raise();
        autoButton->raise();
        screentop->raise();
        time1_5->raise();
        time2->raise();
        time1->raise();
        time0_5->raise();
        playButton->raise();

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        media->setText(QString());
        playButton->setText(QString());
        preButton->setText(QString());
        nextButton->setText(QString());
        volumeButton->setText(QString());
        timeLb->setText(QApplication::translate("Widget", "959:59:59", nullptr));
        showhideList->setText(QString());
        backgroundLb->setText(QApplication::translate("Widget", "TextLabel", nullptr));
        label->setText(QString());
        logoLabel->setText(QString());
        userInfo->setText(QString());
        loginstatus->setText(QApplication::translate("Widget", "\346\234\252\347\231\273\345\275\225", nullptr));
        settingBtn->setText(QString());
        miniButton->setText(QString());
        fangdaButton->setText(QString());
        closeButton->setText(QString());
        playInfo->setText(QString());
        playInfoIco->setText(QString());
        curplay->setText(QApplication::translate("Widget", "C\350\257\255\350\250\200/C++\351\233\266\345\237\272\347\241\200\345\210\260\345\244\247\347\245\236\345\205\250\346\240\210\350\257\276\345\255\246\344\271\240\345\244\247\345\205\250", nullptr));
        stopButton->setText(QString());
        time2Lb->setText(QApplication::translate("Widget", "/999:59:59", nullptr));
        fullscreenBtn->setText(QString());
        settingButton->setText(QString());
        scaleButton->setText(QString());
        autoButton->setText(QString());
        downbkgndLb->setText(QString());
        screentop->setText(QString());
        time2->setText(QApplication::translate("Widget", "2x", nullptr));
        time1_5->setText(QApplication::translate("Widget", "1.5x", nullptr));
        time1->setText(QApplication::translate("Widget", "1x", nullptr));
        time0_5->setText(QApplication::translate("Widget", "0.5x", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
