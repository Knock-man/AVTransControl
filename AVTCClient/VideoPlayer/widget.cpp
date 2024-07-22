#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QPainter>
#include <QToolTip>
#include "qstatuscheck.h"
#include <QCryptographicHash>
#include <QDesktopWidget>
extern const char* MD5_KEY;
extern const char* HOST;
extern bool LOGIN_STATUS;
//这些线程是用来预防有人逆向绕开登录逻辑 如果没有真正的登录，则检测线程会退出程序
QStatusCheck check_thread[32];
#ifdef WIN32
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "libvlccore.lib")
#pragma comment(lib, "libvlc.lib")
#endif

//TODO:追加信号，使用信号来触发整个流程
//暂停图标
const char* PAUSE_STYLESHEET =
        "QPushButton#playButton{"
        "border-image: url(:/ui/button/white/UI/white/zanting.png);"
        "background-color: transparent;}"
        "QPushButton#playButton:hover{"
        "border-image: url(:/ui/button/blue/UI/blue/zanting.png);}"
        "QPushButton#playButton:pressed{"
        "border-image: url(:/ui/button/gray/UI/gray/zanting.png);}";
//播放图标
const char* PLAY_STYLESHEET =
        "QPushButton#playButton{"
        "border-image: url(:/ui/button/blue/UI/blue/bofang.png);"
        "background-color: transparent;}"
        "QPushButton#playButton:hover{"
        "border-image: url(:/ui/button/gray/UI/gray/bofang.png);}"
        "QPushButton#playButton:pressed{"
        "border-image: url(:/ui/button/white/UI/white/bofang.png);}";

//列表显示的时候，按钮的样式
const char* LIST_SHOW =
        "QPushButton{border-image: url(:/UI/images/arrow-right.png);}"
        "QPushButton:hover{border-image: url(:/UI/images/arrow-right.png);}"
        "QPushButton:pressed{border-image: url(:/UI/images/arrow-right.png);}";
//列表隐藏的时候，按钮的样式
const char* LIST_HIDE =
        "QPushButton{border-image: url(:/UI/images/arrow-left.png);}"
        "QPushButton:hover{border-image: url(:/UI/images/arrow-left.png);}"
        "QPushButton:pressed{border-image: url(:/UI/images/arrow-left.png);}";

//当前播放文件的样式
const char* CURRENT_PLAY_NORMAL =
        "background-color: rgba(255, 255, 255, 0);\nfont: 10pt \"黑体\";"
        "\ncolor: rgb(255, 255, 255);";

const char* CURRENT_PLAY_FULL =
        "background-image: url(:/UI/images/screentop.png);"
        "background-color: transparent;"
        "\nfont: 20pt \"黑体\";\ncolor: rgb(255, 255, 255);";

//恢复按钮
const char* SCREEN_RESTORE_STYLE =
        "QPushButton{border-image: url(:/UI/images/huifu.png);}\n"
        "QPushButton:hover{border-image:url(:/UI/images/huifu-hover.png);}\n"
        "QPushButton:pressed{border-image: url(:/UI/images/huifu.png);}";

//最大化按钮
const char* SCREEN_MAX_STYLE =
        "QPushButton{border-image: url(:/UI/images/fangda.png);}\n"
        "QPushButton:hover{border-image:url(:/UI/images/fangda-hover.png);}\n"
        "QPushButton:pressed{border-image: url(:/UI/images/fangda.png);}";

//悬浮窗口 无头的 工具 窗口顶部
#define FLOAT_TYPE (Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint)

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , helper(this)//尺寸助手
{
    ui->setupUi(this);

    slider_pressed = false;//进度条按下状态
    rate = 1.0f;//播放倍率
    //没有系统边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    //接收文件拖拽
    this->setAcceptDrops(true);
    //当前没有播放任何视频
    current_play = -1;//当前播放序号
    //按钮背景透明
    ui->preButton->setAttribute(Qt::WA_TranslucentBackground, true);//左一集按钮透明

    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection); //列表单选
    ui->horizontalSlider->setRange(0, 1000);//进度条范围设置
    ui->horizontalSlider->setValue(0);//进度条初始值
    //connect(ui->horizontalSlider, SIGNAL(sliderPressed()),
    //        this, SLOT(on_horizontalSlider_sliderPressed()));
    //connect(ui->horizontalSlider, SIGNAL(sliderReleased()),
    //        this, SLOT(on_horizontalSlider_sliderReleased()));
    //timerID = startTimer(500);//用于刷新进度的

    //设置事件过滤器，我们关注音量和窗口的事件
    ui->volumeButton->installEventFilter(this);//音量按钮事件过滤器
    ui->volumeSlider->installEventFilter(this);//音量滑动条事件过滤器
    ui->listWidget->installEventFilter(this);//列表控件事件过滤器
    ui->scaleButton->installEventFilter(this);//倍数按钮事件过滤器
    ui->horizontalSlider->installEventFilter(this);//进度条事件过滤器
    installEventFilter(this);//对话框本身事件过滤

    //音量滑动条控件初始化
    ui->volumeSlider->setVisible(false);//音量滑动条不可见
    ui->volumeSlider->setRange(0, 100);//音量滑动条范围

    //隐藏顶层半透明框
    ui->screentop->setHidden(true);
    //播放倍数不可见
    ui->time0_5->setVisible(false);
    ui->time1->setVisible(false);
    ui->time1_5->setVisible(false);
    ui->time2->setVisible(false);

    //定时相关
    volumeSliderTimerID = -1;//音量滑动条定时器
    timesID = -1;//倍数控件定时器
    timesCount = 0;//倍数按钮计数器
    fullScreenTimerID = -1;//全屏定时器
    volumeCount = 0;//音量滑动条计时器

    setTime(0, 0, 0);//设置当前播放时长值
    setTime2(0, 0, 0);//设置最大播放时长值

    //获取显示器 设置组件最大尺寸适应显示器
    QDesktopWidget* desktop = QApplication::desktop();
    int currentScreenWidth = QApplication::desktop()->width();//显示器宽
    int currentScreenHeight = QApplication::desktop()->height();//显示器高
    if(desktop->screenCount() > 1)//多个显示器
    {
        //使用主显示器的尺寸，如果显示器有多个的情况下
        currentScreenWidth = desktop->screenGeometry(0).width();
        currentScreenHeight = desktop->screenGeometry(0).height();
    }
    //如果分辨率小于800*600则按800*600算  设置当前组件的最大尺寸
    if(currentScreenWidth > 800 && currentScreenHeight > 600)
    {
        setMaximumSize(currentScreenWidth, currentScreenHeight);
    }
    else
    {
        setMaximumSize(800, 600);
    }
    init_media();//media初始化 主要绑定this控件信号和media控件槽函数

    //信息窗口
    info.setWindowFlag(Qt::FramelessWindowHint);//无边框
    info.setWindowModality(Qt::ApplicationModal);//模态
    connect(&info, SIGNAL(button_clicked()),this, SLOT(slot_connect_clicked()));//信息窗口联系助教按钮和槽函数
    QSize sz = size();
    info.move((sz.width() - info.width()) / 2, (sz.height() - info.height()) / 2);//居中

    ui->curplay->setText(u8"");//当前播放的 文字设为空
    //ui->curplay->setAttribute(Qt::WA_TranslucentBackground);
    //ui->curplay->setWindowOpacity(0.5);

    //窗口尺寸控制器
    helper.update(width(), height());//设置八个小区域数组 鼠标到达改变样式 用来调整窗口大小
    setMinimumSize(800, 600);//设置最小窗口尺寸（最小是800*600 最大是屏幕尺寸）
    save_default_rect_info();//保存各个控件默认的尺寸信息 存入控件尺寸映射表sub_item_size中

    //消息窗口(顶部半透明效果)
    full_head = new MessageForm();
    full_head->setWindowFlags(FLOAT_TYPE);//悬浮
    full_head->full_size();//设置尺寸
    full_head->move(0, 0);//移动到(0,0)
    //full_head->show();
    //full_head->setHidden(false);

    screentopTimerID = startTimer(50);//顶部显示刷新定时器
    keep_activity_timerID = startTimer(300000);//定时开启线程检查状态

    net = new QNetworkAccessManager(this);//网络
    connect(net, SIGNAL(finished(QNetworkReply*)),//网络请求处理信号和槽函数
            this, SLOT(slots_login_request_finshed(QNetworkReply*)));

    //拖动进度条 调用media处理槽函数
    connect(this, SIGNAL(update_pos(double)), ui->media, SLOT(seek(double)));
}

Widget::~Widget()
{
    killTimer(keep_activity_timerID);
    delete net;
    delete ui;
    delete full_head;
}

//定时器
void Widget::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == keep_activity_timerID)//保持活跃
    {
        check_thread[GetTickCount()%32].start();//随机开启一个线程
        //TODO:服务器保持活跃
    }
    else if(event->timerId() == timesID) //200ms 倍数按钮定时器
    {
        timesCount++;
        if(timesCount > 5)//超时5次 倍数控件全部隐藏
        {
            timesCount = 0;
            ui->time0_5->setVisible(false);
            ui->time1->setVisible(false);
            ui->time1_5->setVisible(false);
            ui->time2->setVisible(false);
            killTimer(timesID);
            timesID = -1;
            //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        }
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << timesCount;
    }
    else if(event->timerId() == volumeSliderTimerID)//音量滑动条定时器
    {
        volumeCount++;
        if(volumeCount > 5)//超时5次，自动销毁定时器 音量滑动条隐藏
        {
            volumeCount = 0;
            ui->volumeSlider->setVisible(false);
            killTimer(volumeSliderTimerID);
            volumeSliderTimerID = -1;
        }
    }
    else if(event->timerId() == fullScreenTimerID)//全屏定时器 1.5秒
    {
        int currentScreenWidth = QApplication::desktop()->width();
        int currentScreenHeight = QApplication::desktop()->height();
        POINT point;
        GetCursorPos(&point);//获取鼠标位置
        QPoint pt(point.x, point.y);
        QRect bottom(0, currentScreenHeight - 68, currentScreenWidth, 68);//底部控制栏的范围
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << currentScreenWidth << "," << currentScreenHeight;
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << bottom;
        if(full_head->geometry().contains(pt) || bottom.contains(pt))//鼠标位于控制栏或者位于头部半透明框内
        {
            //如果鼠标还在该区域活动，则无需隐藏，顺延一个周期
        }
        else
        {
            helper.auto_hide();//需要自动隐藏的控件全屏时 鼠标离开触发区域一段时间后自动隐藏
            full_head->hide();//隐藏顶部半透明框
            killTimer(fullScreenTimerID);
            fullScreenTimerID = -1;
        }
    }
    else if(event->timerId() == screentopTimerID) //1s 20次
    {
        full_head->update();//半透明框进行重新绘制
    }
}

//保存默认的尺寸信息 控件名 最大化尺寸 全屏尺寸 最大化时的隐藏状态 全屏时的隐藏状态 全屏时是否自动隐藏
void Widget::save_default_rect_info()
{
    QObjectList list = children();//拿到所有的子控件
    QObjectList::iterator it = list.begin();
    QString names[] =//子ui名称
    {//34
            "backgroundLb", "media", "downbkgndLb", "listWidget", //4
            "horizontalSlider", "preButton", "nextButton", "volumeButton",//4
            "timeLb", "volumeSlider", "label", "logoLabel",//4
            "userInfo", "loginstatus", "settingBtn", "miniButton",//4
            "fangdaButton", "closeButton", "showhideList", "playInfo",//4
            "playInfoIco", "curplay", "stopButton", "time2Lb",//4
            "fullscreenBtn", "settingButton", "scaleButton", "autoButton",//4
            "screentop", "time1_5", "time2", "time1", "time0_5", "playButton"//6
    };
    //qDebug().nospace()<<__FILE__<<"("<<__LINE__<<"):"<<__FUNCTION__<<" X,Y = "<<GetSystemMetrics(SM_CXSCREEN)<<GetSystemMetrics(SM_CYSCREEN);

    int currentScreenWidth = QApplication::desktop()->screenGeometry(0).width();//QApplication::desktop()->width();
    int currentScreenHeight = QApplication::desktop()->screenGeometry(0).height() - 50;
    //qDebug().nospace()<<__FILE__<<"("<<__LINE__<<"):"<<__FUNCTION__<<" width="<<currentScreenWidth<<" height"<<currentScreenHeight;
    //qDebug().nospace()<<__FILE__<<"("<<__LINE__<<"):"<<__FUNCTION__<<" screen cout="<<QApplication::desktop()->screenCount();
    QRect max_rect[] =//最大化时的尺寸
    {
            QRect(0, 41, currentScreenWidth, currentScreenHeight - 110), //backgroundLb 视频播放背景黑底板
            QRect(0, 41, currentScreenWidth - 300, currentScreenHeight - 110), //media 视频播放控件
            QRect(0, 0, 0, 0), //downbkgndLb 全屏的时候，下方的背景板
            QRect(currentScreenWidth - 300, 41, 300, currentScreenHeight - 41), //listWidget 播放列表
            QRect(0, currentScreenHeight - 68, currentScreenWidth - 300, 22), //horizontalSlider 播放进度条
            QRect(110, currentScreenHeight - 45, 32, 32),//preButton 上一条按钮
            QRect(160, currentScreenHeight - 45, 32, 32),//nextButton 下一条按钮
            QRect(currentScreenWidth - 400, currentScreenHeight - 45, 32, 32), //volumeButton
            QRect(215, currentScreenHeight - 45, 90, 25), //timeLb
            QRect(currentScreenWidth - 398, currentScreenHeight - 205, 22, 160), //volumeSlider 音量滑动条 10
            QRect(0, 0, 1, 1), //label
            QRect(5, 5, 140, 30), //logoLabel
            QRect(currentScreenWidth - 270, 7, 28, 28), //userInfo
            QRect(currentScreenWidth - 270, 7, 45, 25), //loginstatus
            QRect(currentScreenWidth - 160, 5, 30, 30), //settingBtn
            QRect(currentScreenWidth - 120, 5, 30, 30), //miniButton
            QRect(currentScreenWidth - 80, 5, 30, 30), //fangdaButton
            QRect(currentScreenWidth - 40, 5, 30, 30), //closeButton
            QRect(currentScreenWidth - 360, (currentScreenHeight - 170) / 2, 60, 60), //showhideList
            QRect(currentScreenWidth - 298, 43, 296, 46), //playInfo
            QRect(currentScreenWidth - 300, 41, 32, 50),//playInfoIco
            QRect(currentScreenWidth - 265, 50, 260, 32), //curplay
            QRect(60, currentScreenHeight - 45, 32, 32), //stopButton
            QRect(300, currentScreenHeight - 45, 120, 25), //time2Lb
            QRect(currentScreenWidth - 350, currentScreenHeight - 45, 32, 32), //fullscreenBtn
            QRect(currentScreenWidth - 450, currentScreenHeight - 45, 32, 32), //settingButton
            QRect(currentScreenWidth - 500, currentScreenHeight - 45, 42, 32), //scaleButton
            QRect(currentScreenWidth - 560, currentScreenHeight - 45, 42, 32), //autoButton
            QRect(0, 0, 1, 1),//顶部内容提示
            QRect(currentScreenWidth - 200, currentScreenHeight - 157, 42, 48)/*1.5*/,
            QRect(currentScreenWidth - 200, currentScreenHeight - 185, 42, 48)/*2*/,
            QRect(currentScreenWidth - 200, currentScreenHeight - 129, 42, 48)/*1*/,
            QRect(currentScreenWidth - 200, currentScreenHeight - 101, 42, 48)/*0.5*/,
            QRect(5, currentScreenHeight - 45, 32, 32)//playButton 播放按钮
    };
    currentScreenHeight += 50;
    QRect full_rect[] =//全屏时的尺寸
    {
            QRect(0, 0, currentScreenWidth, currentScreenHeight),//backgroundLb
            QRect(0, 0, currentScreenWidth, currentScreenHeight),//media
            QRect(0, currentScreenHeight - 60, currentScreenWidth, 60), //downbkgndLb 全屏的时候，下方的背景板
            QRect(),//列表框（全屏的时候不显示列表）listWidget
            QRect(0, currentScreenHeight - 68, currentScreenWidth, 22),//播放进度条 5 horizontalSlider
            QRect(110, currentScreenHeight - 45, 32, 32),//preButton
            QRect(160, currentScreenHeight - 45, 32, 32),//nextButton
            QRect(currentScreenWidth - 100, currentScreenHeight - 45, 32, 32),//volumeButton 4
            QRect(215, currentScreenHeight - 45, 90, 25),//timeLb
            QRect(currentScreenWidth - 98, currentScreenHeight - 205, 22, 160),//volumeSlider 音量滑动条
            QRect(), //label
            QRect(), //logoLabel 4
            QRect(), //userInfo
            QRect(), //loginstatus
            QRect(), //settingBtn
            QRect(), //miniButton 4
            QRect(), //fangdaButton
            QRect(), //closeButton
            QRect(),//列表显示按钮
            QRect(),//playInfo
            QRect(),//playInfoIco //4
            QRect(30, 30, 0, 0),//当前播放的内容 curplay
            QRect(60, currentScreenHeight - 45, 32, 32),//停止按钮
            QRect(300, currentScreenHeight - 45, 120, 25),
            QRect(currentScreenWidth - 50, currentScreenHeight - 45, 32, 32),//fullscreenBtn
            QRect(currentScreenWidth - 150, currentScreenHeight - 45, 32, 32),
            QRect(currentScreenWidth - 200, currentScreenHeight - 45, 32, 32),
            QRect(currentScreenWidth - 260, currentScreenHeight - 45, 32, 32),
            QRect(0, 0, 1, 1),
            QRect(currentScreenWidth - 200, currentScreenHeight - 157, 42, 48)/*1.5*/,
            QRect(currentScreenWidth - 200, currentScreenHeight - 185, 42, 48)/*2*/,
            QRect(currentScreenWidth - 200, currentScreenHeight - 129, 42, 48)/*1*/,
            QRect(currentScreenWidth - 200, currentScreenHeight - 101, 42, 48) /*0*/,
            QRect(5, currentScreenHeight - 45, 32, 32),//playButton
    };
    //qDebug() << "max_rect size:" << sizeof(max_rect)/sizeof(QRect);
    //qDebug() << "full_rect size:" << sizeof(full_rect)/sizeof(QRect);
    bool max_hide[] =//最大化时的隐藏状态和初始状态
    {
            false, false, true, false,
            false, false, false, false,
            false, true, false, false,
            true, true, false, false,
            false, false, false, false,
            false, false, false, false,
            false, false, false, false,
            true, true, true, true, true,false,
    };
    bool full_hide[] =//全屏时的隐藏状态和初始状态
    {
            false, false, false, true,
            false, false, false, false,
            false, true, true, true,
            true, true, true, true,
            true, true, true, true,
            true, true, false, false,
            false, false, false, false,
            true, true, true, true, true, false
    };
    int auto_hide_status[] =  //全屏时是否自动隐藏0 不隐藏 1 隐藏 2 不参与
    {
            0, 0, 1, 2,
            1, 1, 1, 1,
            1, 1, 2, 2,
            2, 2, 2, 2,
            2, 2, 2, 2,
            2, 2, 1, 1,
            1, 1, 1, 1,
            1, 2, 2, 2, 2, 1
    };
    for(int i = 0; it != list.end(); it++, i++) //更新默认尺寸
    {
        QWidget* widget = (QWidget*)(*it);
        QString name = widget->objectName();//获取控件名称
        helper.init_size_info(widget);//设置原始尺寸
        helper.set_full_rect(name, full_rect[i]);//设置全屏尺寸
        helper.set_max_rect(name, max_rect[i]);//设置最大化尺寸
        helper.set_full_hide(name, full_hide[i]);//设置全屏隐藏状态
        helper.set_max_hide(name, max_hide[i]);//设置最大化隐藏状态
        helper.set_auto_hide(name, auto_hide_status[i]);//设置全屏自动隐藏状态
        //qDebug() << "name:" << (*it)->objectName() << full_rect[i] << max_rect[i];
    }
}

//设置进度条范围
void Widget::setSlider(QSlider* slider, int nMin, int nMax, int nStep)
{
    slider->setMinimum(nMin);
    slider->setMaximum(nMax);
    slider->setSingleStep(nStep);
}

//设置播放时长(控件)
void Widget::setTime(int hour, int minute, int seconds)
{
    QString s;
    QTextStream out(&s);
    out.setFieldWidth(2);
    out.setPadChar('0');
    out << hour ;
    out.setFieldWidth(1);
    out << ":";
    out.setFieldWidth(2);
    out << minute;
    out.setFieldWidth(1);
    out << ":";
    out.setFieldWidth(2);
    out << seconds;
    //qDebug() << "time:" << s;
    ui->timeLb->setText(s);
}

//设置播放时长(控件)
void Widget::setTime(int64_t tm)
{
    tm /= 1000;//得到秒数
    int seconds = tm % 60;
    int minute = (tm / 60) % 60;
    int hour = tm / 3600;
    setTime(hour, minute, seconds);
}

//设置总播放时长（控件）
void Widget::setTime2(int hour, int minute, int seconds)
{
    QString s;
    QTextStream out(&s);
    out << "/";
    out.setFieldWidth(2);
    out.setPadChar('0');
    out << hour ;
    out.setFieldWidth(1);
    out << ":";
    out.setFieldWidth(2);
    out << minute;
    out.setFieldWidth(1);
    out << ":";
    out.setFieldWidth(2);
    out << seconds;
    //qDebug() << "time:" << s;
    ui->time2Lb->setText(s);
}

//设置总播放时长（控件）
void Widget::setTime2(int64_t tm)
{
    tm /= 1000;//得到秒数
    int seconds = tm % 60;
    int minute = (tm / 60) % 60;
    int hour = tm / 3600;
    setTime2(hour, minute, seconds);
}

//画线
void Widget::paintLine()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 反锯齿
    painter.save();//保存当前绘制环境

    //设置画刷样式
    QLinearGradient linearGradient(0, 40, width(), 40);
    linearGradient.setColorAt(0, QColor(61, 163, 241));
    linearGradient.setColorAt(1, QColor(36, 95, 207));

    //创建画笔
    QPen pen = painter.pen();
    pen.setBrush(linearGradient);
    pen.setWidth(1);
    painter.setPen(pen);

    //画线
    painter.drawLine(0, 40, width(), 40);
    painter.restore();//恢复绘制环境
}

//media初始化 绑定信号和槽
void Widget::init_media()
{
    //打开
    connect(this, SIGNAL(open(const QUrl&)),
            ui->media, SLOT(open(const QUrl&)));
    //播放
    connect(this, SIGNAL(play()),
            ui->media, SLOT(play()));
    //暂停
    connect(this, SIGNAL(pause()),
            ui->media, SLOT(pause()));
    //停止
    connect(this, SIGNAL(stop()),
            ui->media, SLOT(stop()));
    //关闭
    connect(this, SIGNAL(close_media()),
            ui->media, SLOT(close()));
    //跳转
    connect(this, SIGNAL(seek(double)),
            ui->media, SLOT(seek(double)));
    //设置倍率
    connect(this, SIGNAL(set_scale(float)),
            ui->media, SLOT(set_scale(float)));
    //取一个帧
    connect(this, SIGNAL(pick_frame(QImage&, int64_t)),
            ui->media, SLOT(pick_frame(QImage&, int64_t)));
    //设置尺寸
    connect(this, SIGNAL(set_size(const QSize&)),
            ui->media, SLOT(set_size(const QSize&)));
    //音量
    connect(this, SIGNAL(volume(int)),
            ui->media, SLOT(volume(int)));
    //设置静音
    connect(this, SIGNAL(silence()),
            ui->media, SLOT(silence()));
    //TODO:↓↓↓这个逻辑最好在登录之后处理↓↓↓
    ui->media->set_float_text("学员ID：1234567890");//设置悬浮文字 防止盗版
}

void Widget::on_show(const QString& nick, const QByteArray& /*head*/)
{
    show();//弹出当前窗口
    info.show();//弹出信息框
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<u8"打开主窗口";
    ui->loginstatus->setText(u8"已登录");//登录状态框
    ui->media->set_float_text(nick);//登录名称悬浮 防盗版
    //this->nick = nick;
    this->nick = "测试账号";

    //启动检测线程
    if(LOGIN_STATUS && this->isHidden() == false)
    {
        check_thread[0].start();
    }
}

//联系我们 跳转一个网站
void Widget::slot_connect_clicked()
{
    QString strAddress;
    strAddress = QString("tencent://message?v=1&uin=%1&site=qq&menu=yes").arg("2850455193");
    //strAddress = QString("http://wpa.qq.com/msgrd?V=1&Uin=%1&Site=ioshenmue&Menu=yes").arg("2850455193");
    LPCWSTR wcharAddress = reinterpret_cast<const wchar_t*>(strAddress.utf16());
    ShellExecute(0, L"open", wcharAddress, L"", L"", SW_SHOW );
    check_thread[1].start();
}

//点击上一个视频
void Widget::on_preButton_clicked()
{
    if(mediaList.size() <= 0)
    {
        return;    //如果没有视频，则啥也不干
    }
    current_play--;
    if(current_play < 0)
    {
        current_play = mediaList.size() - 1; //如果上一个到头部了，则从列表尾部开始
    }
    emit open(mediaList.at(current_play));//发送打开视频信号
    emit play();
    QString filename = mediaList.at(current_play).fileName();//获取文件名
    if(filename.size() > 12)//名字太长了 砍掉用...代替
    {
        filename.replace(12, filename.size() - 12, u8"…");
    }
    ui->curplay->setText(filename);//显示当前播放视频名
    check_thread[2].start();//检测
}

//点击下一个视频
void Widget::on_nextButton_clicked()
{
    if(mediaList.size() <= 0)
    {
        return;    //如果没有视频，则啥也不干
    }
    current_play++;
    if(current_play >= mediaList.size())
    {
        current_play = 0;//如果下一个到尾部了，则回到列表开头重新开始
    }
    emit open(mediaList.at(current_play));
    emit play();
    QString filename = mediaList.at(current_play).fileName();
    if(filename.size() > 12)
    {
        filename.replace(12, filename.size() - 12, u8"…");
    }
    ui->curplay->setText(filename);
    check_thread[4].start();
}
/*
 * 播放按钮逻辑：
 * 1 没有播放资源 s'm'ye 不做
 * 2 没有视频正在播放 且不是暂停状态 播放选中视频
 * 3 是暂停的状态 直接播放
 * 4 选中资源和正在播放的资源一致 暂停
 * 5 有资源在播放 与选中视频不一致 切换到选中视频播放
*/

//点击播放
void Widget::on_playButton_clicked()
{
    int count = ui->listWidget->count();
    if(count <= 0) //没有资源，则什么也不做
    {
        return;
    }
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
    bool isplaying = ui->media->is_playing();
    int index = 0;//播放视频索引
    if(selectedItems.size() > 0)
     //从选中选中视频开始播放
    {
        index = ui->listWidget->currentRow();
    }

    if(isplaying == false && (ui->media->is_paused() == false)) //没有播放，列表有资源，按index进行播放
    {
        qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<u8"发送播放信号";
        emit open(mediaList.at(index));//打开新视频
        emit play();//播放

        current_play = index;//设置当前播放序号
        ui->playButton->setStyleSheet(PAUSE_STYLESHEET);//改变播放图标样式
        tick = 0;
        QString filename = mediaList.at(current_play).fileName();
        if(filename.size() > 12)
        {
            filename.replace(12, filename.size() - 12, u8"…");
        }
        ui->curplay->setText(filename);
        return;//返回
    }
    if(ui->media->is_paused())//有暂停的视频 直接播放
    {
        //暂停恢复
        emit play();
        return;
    }
    //现在的情况是有资源，正在播放，需要检测逻辑2和3
    if(index == this->current_play)
    {
        emit pause();//资源没有切换，那么执行暂停
        ui->playButton->setStyleSheet(PLAY_STYLESHEET);
        return;
    }

    //现在的情况是有资源，切换了选中资源，需要播放新的内容
    emit stop();//先停止播放
    emit open(mediaList.at(index));//切换打开新视频
    emit play();//播放
    current_play = index;
    QString filename = mediaList.at(current_play).fileName();
    if(filename.size() > 12)
    {
        filename.replace(12, filename.size() - 12, u8"…");
    }
    ui->curplay->setText(filename);
    check_thread[3].start();
}

//点击音量 静音
void Widget::on_volumeButton_clicked()
{
    emit silence();
}

//点击倍数
void Widget::on_scaleButton_clicked()
{
    if(timesID == -1)//启动定时器
    {
        timesID = startTimer(200);
        ui->time0_5->setVisible(true);//几个倍数可见
        ui->time1->setVisible(true);
        ui->time1_5->setVisible(true);
        ui->time2->setVisible(true);
        timesCount = -20;
        return;
    }
    else//点击了第二次 收起倍数 关闭定时器
    {
        killTimer(timesID);
        timesID = -1;
        //播放倍数
        ui->time0_5->setVisible(false);
        ui->time1->setVisible(false);
        ui->time1_5->setVisible(false);
        ui->time2->setVisible(false);
    }
}

//进度条按下
void Widget::on_horizontalSlider_sliderPressed()
{
    slider_pressed = true;
    //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
    //qDebug() << "is_playing:" << ui->media->is_playing();
}

//进度条松开
void Widget::on_horizontalSlider_sliderReleased()
{
    //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
    //qDebug() << "is_playing:" << ui->media->is_playing();
    //else
    {
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
    }
    slider_pressed = false;
}

//进度条改变
void Widget::on_horizontalSlider_rangeChanged(int /*min*/, int /*max*/)
{
    //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
    //qDebug() << "is_playing:" << ui->media->is_playing();
}

//进度条改变
void Widget::on_horizontalSlider_valueChanged(int value)
{
    if(slider_pressed)//鼠标是按下状态
    {
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "): value=" << value;
        // qDebug() << "is_playing:" << ui->media->is_playing();
        if(ui->media->has_media_player())//视频正在播放
        {
            int max = ui->horizontalSlider->maximum();
            int min = ui->horizontalSlider->minimum();
            double cur = (value - min) * 1.0 / (max - min);//根据最大值最小值确定进度条改变后的值
            //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):cur=" << cur << " value=" << value;
            //qDebug() << "min=" << min << " max=" << max;
            emit update_pos(cur);//发送进度条更新信号 media槽函数改变播放位置
            setTime(ui->media->get_duration()*cur);//改变当前播放时间控件
        }
    }
    else
    {
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "): value=" << value;
    }
}

//player不断发出信号 widget不断更新进度条
void Widget::on_media_position(double pos)
{
    if(slider_pressed == false)//没有按下
    {
        int max = ui->horizontalSlider->maximum();
        int min = ui->horizontalSlider->minimum();
        int value = min + pos * (max - min);
        ui->horizontalSlider->setValue(value);//更新进度条
        setTime(ui->media->get_duration()*pos);
        setTime2(ui->media->get_duration());
    }
}

//改变播放状态
void Widget::on_media_media_status(QMediaPlayer::PlayerStatus s)
{
    switch(s)
    {
    case QMediaPlayer::MP_OPEN://打开状态
        ui->playButton->setStyleSheet(PLAY_STYLESHEET);//改变按钮图标
        break;
    case QMediaPlayer::MP_PLAY://播放状态
        ui->playButton->setStyleSheet(PAUSE_STYLESHEET);
        break;
    case QMediaPlayer::MP_PAUSE://暂停状态
        ui->playButton->setStyleSheet(PLAY_STYLESHEET);
        break;
    case QMediaPlayer::MP_CLOSE://关闭状态
        ui->playButton->setStyleSheet(PLAY_STYLESHEET);
        break;
    default:
        ui->playButton->setStyleSheet(PLAY_STYLESHEET);
        break;
    }
}

//功能还未实现 用来观看视频的过程中不停向服务器发送学习时间 跟进学习进度
void Widget::slots_login_request_finshed(QNetworkReply* reply)
{
    this->setEnabled(true);
    bool login_success = false;
    if(reply->error() != QNetworkReply::NoError)
    {
        info.set_text(u8"用户不能为空\r\n" + reply->errorString(), u8"确认").show();
        return;
    }
    QByteArray data = reply->readAll();
    //qDebug() << data;
    QJsonParseError json_error;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &json_error);
    if (json_error.error == QJsonParseError::NoError)
    {
        if (doucment.isObject())
        {
            const QJsonObject obj = doucment.object();
            if (obj.contains("status") && obj.contains("message"))
            {
                QJsonValue status = obj.value("status");
                QJsonValue message = obj.value("message");
            }
        }
    }
    else
    {
        //qDebug() << "json error:" << json_error.errorString();
        info.set_text(u8"登录失败\r\n无法解析服务器应答！", u8"确认").show();
    }
    if(!login_success)
    {
        info.set_text(u8"登录失败\r\n用户名或者密码错误！", u8"确认").show();
    }
    reply->deleteLater();
    check_thread[4].start();
}

void Widget::dragEnterEvent(QDragEnterEvent* event)
{
    static int i = 0;//记录拖放次数
    if(i++ > 20)
    {
        i = 0;
        if(this->isHidden() == false)
        {
            check_thread[5].start();
        }
    }
    event->acceptProposedAction();//接收提议动作
}
void Widget::dropEvent(QDropEvent* event)
{
    auto files = event->mimeData()->urls();//从QDropEvent中获取拖动的文件URL列表
    for (int i = 0; i < files.size() ; i++ )//将这些URL文件添加进列表中
    {
        QUrl url = files.at(i);
        qDebug()<<__FILE__<<__FUNCTION__<<(__LINE__)<<u8"拖拽进去视频地址:"<<url;
        //qDebug() << url;
        //qDebug() << url.fileName();
        //qDebug() << url.path();
        ui->listWidget->addItem(url.fileName());//加入树控件中
        //应该允许存在重复的内容
        mediaList.append(url);//加入url容器中
    }
    check_thread[6].start();
}

void Widget::handleTimeout(int /*nTimerID*/)
{
}

void Widget::mouseMoveEvent(QMouseEvent* event)
{
    //鼠标移动
    if (helper.cur_status() == 0)
    {
        move(event->globalPos() - position);
        helper.modify_mouse_cousor(event->globalPos());//更改鼠标光标
    }
    //qDebug() << __FUNCTION__ << " fullScreenTimerID:" << fullScreenTimerID;
    static int s = 0;
    if(s++ > 50)
    {
        s = 0;
        if(this->isHidden() == false)
        {
            check_thread[7].start();
        }
    }
}

void Widget::mousePressEvent(QMouseEvent* event)
{
    if(helper.type() == 0 && (helper.cur_status() == 0))//只有正常光标，正常窗口 状态才能移动窗口
    {
        //鼠标按下  鼠标相对于控件坐标
        position = event->globalPos() - this->pos();//鼠标在屏幕的位置-控件在屏幕的位置
    }
    //左上，顶，右上，右，右下，底，左下，左
    else if(helper.type()!=0 && (helper.cur_status() == 0))
    {
        helper.press(this->pos());
    }
    //qDebug() << __FUNCTION__ << " pos:" << event->globalPos();
    static int s = 0;
    if(s++ > 10)
    {
        s = 0;
        if(this->isHidden() == false)
        {
            check_thread[8].start();
        }
    }
}

void Widget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    //鼠标释放
    helper.release();
    if(isFullScreen())//全屏
    {
        if((helper.cur_status() == 2) && (fullScreenTimerID == -1))
        {
            fullScreenTimerID = startTimer(1500);//开启全屏定时器
            helper.auto_hide(false);//该隐藏的控件自动隐藏
            full_head->show();//展示半透明框
            //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        }
    }
    static int s = 0;
    if(s++ > 20)
    {
        s = 0;
        if(this->isHidden() == false)
        {
            check_thread[9].start();
        }
    }
}

//事件过滤器
bool Widget::eventFilter(QObject* watched, QEvent* event)//(控件吗，事件)
{
    if(watched == ui->horizontalSlider)//进度条
    {
        if(QEvent::MouseButtonPress == event->type())//按下
        {
            slider_pressed = true;//会关闭进度条定时器
        }
        else if(QEvent::MouseButtonRelease == event->type())//释放
        {
            slider_pressed = false;
        }
    }
    if(watched == this)//控件本身
    {
        if(event->type() == QEvent::Paint)//绘制事件
        {
            bool ret = QWidget::eventFilter(watched, event);//先进行默认绘制
            paintLine();//追加绘制
            return ret;
        }
        if(event->type() == QEvent::MouseButtonDblClick)//双击
        {
            //qDebug() << __FUNCTION__ << "(" << __LINE__ << "):" << event->type();
            if(isMaximized())//是最大化
            {
                //最大化改为恢复为普通
                on_fangdaButton_clicked();
            }
            else if(isFullScreen() == false)//不是全屏
            {
                //不是最大化也不是全屏，那么就是最小化和普通。但是最小化不会有双击消息，只能是普通状态
                on_fangdaButton_clicked();//普通状态变为最大化
            }
        }
    }
    if((event->type() == QEvent::UpdateRequest)//更新事件
            || (event->type() == QEvent::Paint)//重绘事件
            || (event->type() == QEvent::Timer)//定时器事件
            )
    {
        helper.modify_mouse_cousor(QCursor::pos());//更新鼠标光标(鼠标在窗体边沿)
    }
    if(watched == ui->scaleButton)//倍数按钮
    {
        if(event->type() == QEvent::HoverEnter)
        {
        }
    }
    if(watched == ui->volumeButton)//音量按钮
    {
        if(event->type() == QEvent::HoverEnter)//鼠标进入
        {
            //qDebug() << "enter";
            ui->volumeSlider->setVisible(true);//设置音量滑动条可见
            if(volumeSliderTimerID == -1)//显示音量，1秒后自动关闭
            {
                volumeSliderTimerID = startTimer(200);
            }
        }
    }
    if(watched == ui->volumeSlider)//音量滑动条
    {
        //qDebug() << event->type();
        //鼠标进入,鼠标移动,鼠标滚动事件
        if((event->type() == QEvent::HoverMove) ||
                (event->type() == QEvent::MouseMove) ||
                (event->type() == QEvent::Wheel))
        {
            volumeCount = 0;//计数记为0
            //qDebug() << volumeCount;
        }
    }
    //有任何事件发生 开始计数
    static int ss = 0;
    if(ss++ > 100)//超过100个事件发生
    {
        ss = 0;
        if(this->isHidden() == false)
        {
            check_thread[GetTickCount()%32].start();//启动检测线程
        }
    }
    return QWidget::eventFilter(watched, event);
}

//键盘按下
void Widget::keyPressEvent(QKeyEvent* event)
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):" << __FUNCTION__ << " key:" << event->key();
    //qDebug() << __FILE__ << "(" << __LINE__ << "):" << __FUNCTION__ << isFullScreen();
    if(event->key() == Qt::Key_Escape)
    {
        if(isFullScreen())//全屏状态按下ESC 退出全屏
        {
            on_fullscreenBtn_clicked();//切换到正常尺寸
        }
    }
}

//双击列表菜单
void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem*)
{
    if(ui->listWidget->currentRow() < mediaList.size())//在播放范围内
    {
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        //qDebug() << ui->listWidget->currentRow();
        int index = ui->listWidget->currentRow();
        QUrl url = mediaList.at(index);
        //qDebug() << url;
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        emit stop();//停止原来视频
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        emit open(url);//打开新视频
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        emit play();//播放
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        current_play = index;//设置当前播放序号
        QString filename = url.fileName();
        if(filename.size() > 13)
        {
            filename.replace(13, filename.size() - 12, u8"…");
        }
        ui->curplay->setText(filename);//改变当前播放显示
    }
}

//减速
void Widget::on_slowButton_clicked()
{
    rate -= 0.25f;
    emit set_scale(rate);
}

//加速
void Widget::on_fastButton_clicked()
{
    rate += 0.25f;
    emit set_scale(rate);
}

//松开音量滑动条
void Widget::on_volumeSlider_sliderReleased()
{
    emit volume(ui->volumeSlider->value());
}

//音量滑动条改变
void Widget::on_volumeSlider_valueChanged(int value)
{
    //0~100
    emit volume(value % 101);
}

//隐藏列表
void Widget::on_showhideList_pressed()
{
    //隐藏列表和展示列表，同时调整播放区域的宽度和高度
    if(ui->listWidget->isHidden() == false)
    {
        //隐藏
        ui->listWidget->hide();//列表隐藏
        ui->curplay->hide();//隐藏列表上面的信息栏
        ui->playInfoIco->hide();//隐藏列表上面的信息栏图标
        ui->playInfo->hide();//隐藏列表上面的信息栏背景

        //移动隐藏按钮到边边
        ui->showhideList->setStyleSheet(LIST_HIDE);
        QPoint pt = ui->showhideList->pos();
        int w = ui->listWidget->width();
        ui->showhideList->move(pt.x() + w, pt.y());

        //改变media控件位置
        QRect rect = ui->media->frameGeometry();
        ui->media->move(rect.x() + 150, rect.y());

        //改变水平进度条长度
        rect = ui->horizontalSlider->frameGeometry();
        ui->horizontalSlider->setGeometry(rect.left(), rect.top(), rect.width() + 300, rect.height());
    }
    else
    {
        //显示
        ui->listWidget->show();//显示列表
        ui->curplay->show();//显示列表上面的信息栏
        ui->playInfoIco->show();//显示列表上面的信息栏图标
        ui->playInfo->show();//显示列表上面的信息栏背景
        ui->showhideList->setStyleSheet(LIST_SHOW);

        //移动隐藏按钮到左边
        QPoint pt = ui->showhideList->pos();
        int w = ui->listWidget->width();
        ui->showhideList->move(pt.x() - w, pt.y());

        //改变media控件位置
        QRect rect = ui->media->frameGeometry();
        ui->media->move(rect.x() - 150, rect.y());

        //改变水平进度条长度
        rect = ui->horizontalSlider->frameGeometry();
        ui->horizontalSlider->setGeometry(rect.left(), rect.top(), rect.width() - 300, rect.height());
    }
}

Widget::SizeHelper::SizeHelper(Widget* ui)
{
    curent_coursor = 0;
    isabled = true;
    //9种光标
    Qt::CursorShape cursor_type[9] =
    {
        Qt::ArrowCursor, Qt::SizeFDiagCursor, Qt::SizeVerCursor,
        Qt::SizeBDiagCursor, Qt::SizeHorCursor, Qt::SizeFDiagCursor,
        Qt::SizeVerCursor, Qt::SizeBDiagCursor, Qt::SizeHorCursor
    };
    for(int i = 0; i < 9; i++)
    {
        cursors[i] = new QCursor(cursor_type[i]);
    }
    index = 0;
    pressed = false;
    this->ui = ui;
    status = 0;
}

Widget::SizeHelper::~SizeHelper()
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    for(int i = 0; i < 9; i++)
    {//删掉光标
        delete cursors[i];
        cursors[i] = NULL;
    }
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
}

void Widget::SizeHelper::update(int nWidth, int nHeight)
{
    int width = nWidth * 0.01;
    int height = nHeight * 0.01;
    int x_[] =
    {
        //左上，顶，右上，右，右下，底，左下，左
        0/*左上*/, width * 3/*顶上*/, nWidth - 5/*右上*/,
        nWidth - width/*右*/, nWidth - width/*右下*/, width * 3/*底*/,
        0/*左下*/, 0/*左*/
    };
    int y_[] =
    {
        0, 0, 0,//左上，顶，右上
        height * 3, nHeight - 10, nHeight - 10,//右，右下，底
        nHeight - height, height * 3//左下，左
    };
    int w[] =
    {
        width * 2, width * 14, width * 2,
        width * 2, width * 2, width * 14,
        width * 2, width * 2
    };
    int h[] =
    {
        height, height, height,
        height * 14, height, height,
        height, height * 14
    };
    for(int i = 0; i < 8; i++)
    {
        size_rect[i].setX(x_[i]);
        size_rect[i].setY(y_[i]);
        size_rect[i].setWidth(w[i]);
        size_rect[i].setHeight(h[i]);
    }
}

//更新鼠标光标
void Widget::SizeHelper::modify_mouse_cousor(const QPoint& point)
{
    if(pressed == false)
    {
        //ui->pos() 控件在其父控件中的位置
        QPoint pt = point - ui->pos();//相对于主窗口的偏移量
        for(int i = 0; i < 8; i++)//遍历八个区域
        {
            if(size_rect[i].contains(pt))//出现在需要改变光标的区域内(窗体边沿)
            {
                if(i + 1 != index)//保证和当前起效的光标不一样  (+1 因为0给正常光标)
                {
                    ui->setCursor(*cursors[i + 1]);//设置光标
                    index = i + 1;
                }
                return;//设置光标后立马返回
            }
        }
        //如果没有设置光标成功(鼠标没有停留在需要改变光标的区域)
        if(index != 0)
        {
            ui->setCursor(*cursors[0]);//设置为正常光标
            index = 0;
        }
    }
}

void Widget::SizeHelper::set_enable(bool isable)
{
    this->isabled = isable;
}

void Widget::SizeHelper::press(const QPoint& point)
{
    if(pressed == false)
    {
        pressed = true;
        this->point = point;
    }
}

void Widget::SizeHelper::move(const QPoint& /*point*/)
{
    //TODO:绘制调整大小的框，不能小于800×600
}

void Widget::SizeHelper::release()
{
    if(pressed)
    {
        pressed = false;
        point.setX(-1);
        point.setY(-1);
    }
}

void Widget::SizeHelper::init_size_info(QWidget* widget)
{
    SizeInfo info;//控件大小信息结构体
    info.widget = widget;//控件对象储存
    info.org_rect = widget->frameGeometry();//原始尺寸
    info.last_rect = widget->frameGeometry();//上一个尺寸
    //qDebug() << "rect:" << info.org_rect;
    sub_item_size.insert(widget->objectName(), info);
}

//设置控件全屏尺寸
void Widget::SizeHelper::set_full_rect(const QString& name, const QRect& rect)
{
    auto it = sub_item_size.find(name);
    if(it != sub_item_size.end())
    {
        sub_item_size[name].full_rect = rect;
    }
}

//设置控件最大化尺寸
void Widget::SizeHelper::set_max_rect(const QString& name, const QRect& rect)
{
    auto it = sub_item_size.find(name);
    if(it != sub_item_size.end())
    {
        sub_item_size[name].max_rect = rect;
    }
}

//设置控件全屏是否隐藏
void Widget::SizeHelper::set_full_hide(const QString& name, bool is_hide)
{
    auto it = sub_item_size.find(name);
    if(it != sub_item_size.end())
    {
        sub_item_size[name].is_full_hide = is_hide;
    }
}

//设置最大化是否隐藏
void Widget::SizeHelper::set_max_hide(const QString& name, bool is_hide)
{
    auto it = sub_item_size.find(name);
    if(it != sub_item_size.end())
    {
        sub_item_size[name].is_max_hide = is_hide;
    }
}

//设置是否自动隐藏
void Widget::SizeHelper::set_auto_hide(const QString& name, int hide_status)
{
    auto it = sub_item_size.find(name);
    if(it != sub_item_size.end())
    {
        sub_item_size[name].auto_hide_status = hide_status;
    }
}

//全屏显示 控件设置为全屏时该有的大小 该隐藏的隐藏
void Widget::SizeHelper::full_size()
{
    status = 2;
    for(auto it = sub_item_size.begin(); it != sub_item_size.end(); it++)
    {
        //qDebug()<<(*it).widget->objectName()<<(*it).max_rect<<(*it).full_rect;
        if((*it).full_rect.width() > 0)
        {
            (*it).widget->setGeometry((*it).full_rect);
            //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << (*it).full_rect;
        }
        (*it).widget->setHidden((*it).is_full_hide);
    }
}

//恢复正常大小 控件尺寸信息恢复为正常尺寸 隐藏半透明框 音量滑动条 改变图标
void Widget::SizeHelper::org_size()
{
    status = 0;//普通尺寸
    for(auto it = sub_item_size.begin(); it != sub_item_size.end(); it++)
    {
        (*it).widget->setGeometry((*it).org_rect);//所有控件设置成原始大小
        if((*it).widget->objectName() == "screentop")//顶部半透明框隐藏
        {
            (*it).widget->setHidden(true);
        }
        if((*it).widget->objectName() == "volumeSlider")//音量滑动条隐藏
        {
            (*it).widget->setHidden(true);
        }
        if((*it).widget->objectName() == "fangdaButton")
        {
            (*it).widget->setStyleSheet(SCREEN_MAX_STYLE);//改变放大图标为最大化图标
        }
    }
}

//最大化
void Widget::SizeHelper::max_size()
{
    status = 1;
    for(auto it = sub_item_size.begin(); it != sub_item_size.end(); it++)
    {
        //qDebug()<<(*it).widget->objectName()<<(*it).max_rect<<(*it).full_rect;
        if((*it).max_rect.width() > 0)
        {
            (*it).widget->setGeometry((*it).max_rect);//设置控件为最大化尺寸
        }
        (*it).widget->setHidden((*it).is_max_hide);//该隐藏的隐藏
        if((*it).widget->objectName() == "screentop")//隐藏顶层半透明框
        {
            (*it).widget->setHidden(true);
        }
        else if((*it).widget->objectName() == "fangdaButton")//改变放大图标
        {
            (*it).widget->setStyleSheet(SCREEN_RESTORE_STYLE);
        }
        else if((*it).widget->objectName() == "volumeSlider")//隐藏音量滑动条
        {
            (*it).widget->setHidden(true);
        }
    }
}

//返回窗口当前状态 正常 全屏 最大化
int Widget::SizeHelper::cur_status() const
{
    return status;
}

//自动隐藏 检查sub_item_size映射表中设置了自动隐藏的控件，对其进行隐藏
void Widget::SizeHelper::auto_hide(bool hidden)
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):status=" << status;
    //qDebug() << __FILE__ << "(" << __LINE__ << "):hidden=" << hidden;
    if(status == 2)//全屏的时候才起效
    {
        for(auto it = sub_item_size.begin(); it != sub_item_size.end(); it++)
        {
            //qDebug() << __FILE__ << "(" << __LINE__ << "):object=" << (*it).widget->objectName();
            //qDebug() << __FILE__ << "(" << __LINE__ << "):auto_hide_status=" << (*it).auto_hide_status;
            if((*it).widget->objectName() == "volumeSlider")
            {
                (*it).widget->setHidden(true);
            }
            else if((*it).auto_hide_status == 1)
            {
                (*it).widget->setHidden(hidden);
                //qDebug() << __FILE__ << "(" << __LINE__ << "):hidden=" << hidden;
            }
        }
    }
}

//关闭 发送关闭消息给播放器
void Widget::on_closeButton_released()
{
    emit close();
}

//最大化 和 正常尺寸相互切换
void Widget::on_fangdaButton_clicked()
{
    /*TODO:
     * 1 修改图标 *
     * 2 隐藏列表
     * 3 开启定时，1.5秒后隐藏进度等控件
    */
    if(isMaximized() == false)//切为最大化
    {
        //从正常尺寸转移到最大化尺寸
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        showMaximized();//变成最大化
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        helper.max_size();//更新最大化各个控件尺寸信息
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        ui->showhideList->setStyleSheet(LIST_HIDE);//切换全屏的时候，隐藏列表按钮状态设置为初始状态
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        ui->listWidget->setHidden(true);//隐藏列表
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
    }
    else//切为正常大小
    {
        //从最大化尺寸转移到正常尺寸
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        showNormal();//改变为正常尺寸
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        helper.org_size();//更新正常尺寸各个控件尺寸信息
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        ui->listWidget->setHidden(false);//显示列表控件
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        ui->showhideList->setStyleSheet(LIST_SHOW);//切换全屏的时候，隐藏列表按钮状态设置为初始状态
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        ui->userInfo->setHidden(false);//显示头像
    }
}

//全屏
void Widget::on_fullscreenBtn_clicked()
{
    if(isFullScreen() == false)
    {
        QString style = "QPushButton{border-image: url(:/UI/images/tuichuquanping.png);}\n";
        style += "QPushButton:hover{border-image:url(:/UI/images/tuichuquanping-hover.png);}\n";
        style += "QPushButton:pressed{border-image: url(:/UI/images/tuichuquanping.png);}";
        ui->fullscreenBtn->setStyleSheet(style);
        showFullScreen();//全屏
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        helper.full_size();
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        ui->showhideList->setStyleSheet(LIST_SHOW);//切换全屏的时候，隐藏列表按钮状态设置为初始状态
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        full_head->show();
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << current_play;
        QString filename = current_play>=0?mediaList.at(current_play).fileName():"";
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        if(filename.size() > 12)
        {
            filename.replace(12, filename.size() - 12, u8"…");
        }
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        full_head->setText(filename);//显示视频名称
        //QToolTip::showText(QPoint(10, 10), ui->curplay->text(), nullptr, QRect(0, 0, 210, 30), 2950);
        fullScreenTimerID = startTimer(1500);//1.5秒后隐藏顶层和操作框
        screentopTimerID = startTimer(50);//顶层半透明刷新定时器
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
    }
    else
    {
        //从最大化尺寸转移到正常尺寸
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        QString style = "QPushButton{border-image: url(:/UI/images/quanping.png);}\n";
        style += "QPushButton:hover{border-image:url(:/UI/images/quanping-hover.png);}\n";
        style += "QPushButton:pressed{border-image: url(:/UI/images/quanping.png);}";
        ui->fullscreenBtn->setStyleSheet(style);
        showMaximized();
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        helper.max_size();
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):";
        full_head->setHidden(true);
        killTimer(fullScreenTimerID);
        fullScreenTimerID = -1;
        killTimer(screentopTimerID);
        screentopTimerID = -1;
    }
}

//停止
void Widget::on_stopButton_clicked()
{
    emit stop();
    setTime(0, 0, 0);
    ui->horizontalSlider->setValue(0);
}

//2倍数
void Widget::on_time2_clicked()
{
    ui->media->set_scale(2.0);
    if(timesID >= 0)//按完之后隐藏
    {
        killTimer(timesID);
        timesID = -1;
        //播放倍数
        ui->time0_5->setVisible(false);
        ui->time1->setVisible(false);
        ui->time1_5->setVisible(false);
        ui->time2->setVisible(false);
    }
}

//5倍数
void Widget::on_time1_5_clicked()
{
    ui->media->set_scale(1.5);
    if(timesID >= 0)
    {
        killTimer(timesID);
        timesID = -1;
        //播放倍数
        ui->time0_5->setVisible(false);
        ui->time1->setVisible(false);
        ui->time1_5->setVisible(false);
        ui->time2->setVisible(false);
    }
}

//1倍数
void Widget::on_time1_clicked()
{
    ui->media->set_scale(1.0);
    if(timesID >= 0)
    {
        killTimer(timesID);
        timesID = -1;
        //播放倍数
        ui->time0_5->setVisible(false);
        ui->time1->setVisible(false);
        ui->time1_5->setVisible(false);
        ui->time2->setVisible(false);
    }
}

//0.5倍数
void Widget::on_time0_5_clicked()
{
    ui->media->set_scale(0.5);
    if(timesID >= 0)
    {
        killTimer(timesID);
        timesID = -1;
        //播放倍数
        ui->time0_5->setVisible(false);
        ui->time1->setVisible(false);
        ui->time1_5->setVisible(false);
        ui->time2->setVisible(false);
    }
}
QString getTime();
bool Widget::keep_activity()//向服务器发起请求
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    QNetworkRequest request;
    QString url = QString(HOST) + "/keep?";
    QString salt = QString::number(QRandomGenerator::global()->bounded(10000, 99999));
    QString time = getTime();
    md5.addData((time + MD5_KEY + nick + salt).toUtf8());
    QString sign = md5.result().toHex();
    url += "time=" + time + "&";
    url += "salt=" + salt + "&";
    url += "user=" + nick + "&";
    url += "sign=" + sign;
    //qDebug() << url;
    request.setUrl(QUrl(url));
    net->get(request);
    return false;
}

//最小化
void Widget::on_miniButton_clicked()
{
    this->showMinimized();
}
