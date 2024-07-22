#ifndef WIDGET_H
#define WIDGET_H
#include <windows.h>
#include <QWidget>
#include <QSlider>
#include <QDropEvent>
#include <QList>
#include <QListWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRandomGenerator>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include "vlchelper.h"
#include "ssltool.h"
#include "qmediaplayer.h"
#include "infoform.h"
#include "messageform.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();
    virtual void timerEvent(QTimerEvent* event);//定时器事件函数
protected:
    void save_default_rect_info();//保存所有控件的默认尺寸信息
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void handleTimeout(int nTimerID);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual bool eventFilter(QObject* watched, QEvent* event);//事件过滤器
    virtual void keyPressEvent(QKeyEvent* event);
private:
    void setSlider(QSlider* slider, int nMin, int nMax, int nStep);
    //设置当前播放的时间
    void setTime(int hour, int minute, int seconds);
    void setTime(int64_t tm);
    //设置媒体总的时长
    void setTime2(int hour, int minute, int seconds);
    void setTime2(int64_t tm);

    void paintLine();//画线
    void init_media();//media初始化 绑定this信号和media的槽函数

signals:
    void update_pos(double pos);//拖动进度条位置
    void open(const QUrl& url);
    void play();
    void pause();//暂停
    void stop();//停止播放
    void close_media();//关闭媒体
    void seek(double pos);
    void set_position(int64_t pos);//设置播放位置 单位毫秒
    void set_position(//设置播放位置，时分秒毫秒
        int hour, int minute, int second, int millisecond
    );
    void set_scale(float scale);//设置播放倍率
    //取指定位置(单位毫秒)的一帧图片（用于预览）
    void pick_frame(QImage& frame, int64_t pos);
    //改变播放窗口大小
    void set_size(const QSize& sz);
    void volume(int vol);
    void silence();

public slots:
    void on_show(const QString& nick, const QByteArray& head);//显示主窗口
    void slot_connect_clicked();//跳转联系售后地址
protected slots:
    void on_preButton_clicked();
    void on_playButton_clicked();
    void on_nextButton_clicked();
    void on_volumeButton_clicked();
    void on_scaleButton_clicked();
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_rangeChanged(int min, int max);
    void on_horizontalSlider_valueChanged(int value);
    void on_media_position(double pos);
    void on_media_media_status(QMediaPlayer::PlayerStatus s);
    void slots_login_request_finshed(QNetworkReply* reply);
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_volumeSlider_sliderReleased();
    void on_volumeSlider_valueChanged(int value);
    void on_showhideList_pressed();
    void on_closeButton_released();
    void on_fangdaButton_clicked();
    void on_fullscreenBtn_clicked();
    void on_stopButton_clicked();
    void on_time2_clicked();
    void on_time1_5_clicked();
    void on_time1_clicked();
    void on_time0_5_clicked();

    void on_miniButton_clicked();

private:
    void on_fastButton_clicked();
    void on_slowButton_clicked();
    bool keep_activity();
private:
    //尺寸助手
    class SizeHelper
    {
    public:
        SizeHelper(Widget* ui);
        ~SizeHelper();
        //当调整完大小的时候更新 鼠标光标变化区域数组
        void update(int nWidth, int nHeight);
        //改变鼠标光标类型 0 正常 1 左上 2 顶 3 右上 4 右 5 右下 6 底 7 左下 8 左
        void modify_mouse_cousor(const QPoint& point);
        //设置false禁用本功能（全屏时禁用）true开启功能，默认为true
        void set_enable(bool isable = true);
        //返回光标编号
        int type()
        {
            return index;
        }
        void press(const QPoint& point);
        void move(const QPoint& point);
        void release();

        //设置控件尺寸信息 存入尺寸映射表sub_item_size
        void init_size_info(QWidget* widget);
        void set_full_rect(const QString& name, const QRect& rect);
        void set_max_rect(const QString& name, const QRect& rect);
        void set_org_rect(const QString& name, const QRect& rect);
        void set_last_rect(const QString& name, const QRect& rect);
        void set_full_hide(const QString& name, bool is_hide = true);
        void set_max_hide(const QString& name, bool is_hide = true);
        void set_auto_hide(const QString& name, int hide_status = 1);

        void full_size();//设置全屏显示
        void org_size();//设置正常显示
        void max_size();//设置最大化显示
        int  cur_status() const;//当前屏幕状态 0正常  1最大化  2全屏
        void auto_hide(bool hidden = true);//全屏时 设置了自动隐藏的控件 自动隐藏
    protected:
        struct SizeInfo//尺寸信息
        {
            bool is_full_hide;//全屏隐藏
            bool is_max_hide;//最大化隐藏
            int auto_hide_status;//全屏时自动隐藏 0 不隐藏 1 隐藏 2 不参与（该控件在全屏模式下不存在)
            QRect org_rect;//原始尺寸
            QRect max_rect;//最大化尺寸
            QRect full_rect;//全屏尺寸
            QRect last_rect;//上一个尺寸
            QWidget* widget;//控件
            SizeInfo()
            {
                widget = NULL;
                is_full_hide = false;
                is_max_hide = false;
            }
        };
    protected:
        //窗口调整属性
        QRect size_rect[8];//8个区域，这八个区域应该触发鼠标变化，用于拖拽改变ui整体窗口大小
        bool isabled;//默认为true
        int curent_coursor;//当前光标类型，默认为0，表示普通光标
        QCursor* cursors[9];//光标
        int index;//当前起效的光标编号
        QPoint point;//按下的起点
        bool pressed;//按下状态
        Widget* ui;
        QMap<QString, SizeInfo> sub_item_size;//控件尺寸信息映射表
        int status;//当前屏幕状态0 普通 1 最大化 2 全屏
    };
private:
    Ui::Widget* ui;
    QList<QUrl> mediaList;//菜单视频列表
    //vlchelper* vlc;
    SslTool ssl_tool;
    int current_play;//当前播放序号
    int timerID;
    int timesCount;//倍数控件超时次数 五次=5s 隐藏倍数选择(0.5 1 1.5 2)
    int timesID;//倍速按钮定时器id
    int volumeSliderTimerID;//音量滑动条定时器id
    int fullScreenTimerID;//全屏定时器id
    int keep_activity_timerID;//保持活跃状态定时器id
    int volumeCount;//音量滑动条计时器
    QPoint position;//鼠标相对于控件的位置
    float rate;//播放倍率
    bool slider_pressed;//播放进度条按下的状态
    qint16 tick;
    SizeHelper helper;
    InfoForm info;//浮动信息栏
    MessageForm* full_head;//全屏时的头部半透明框
    int screentopTimerID;//顶部半透明框定时器id
    QNetworkAccessManager* net;//网络
    QString nick;//登录昵称
};
#endif // WIDGET_H
