#ifndef QMEDIAPLAYER_H
#define QMEDIAPLAYER_H
#include <QTimer>
#include <QLabel>
#include "vlchelper.h"


class QMediaPlayer : public QLabel
{
    Q_OBJECT
public:
    //播放状态
    enum PlayerStatus
    {
        MP_NONE,//MediaPlayer 空 初始状态
        MP_OPEN,
        MP_PLAY,
        MP_PAUSE,
        MP_STOP,
        MP_CLOSE,
        MP_DESTORY,
        MP_MEDIA_INIT_FAILED = -1,
        MP_OPERATOR_FAILED = -2,
        MP_TIMER_INIT_FAILED = -3,
        MP_MEDIA_LOAD_FAILED = -4,
    };
public:
    QMediaPlayer(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit QMediaPlayer(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~QMediaPlayer();
    PlayerStatus status()
    {
        return stat;
    }
    bool is_playing();
    bool is_paused();
    bool has_media_player();
    bool is_mute();//返回true表示静音了，否则表示没有
    int64_t get_duration();
    int volume();
    //设置悬浮文字
    void set_float_text(const QString& text);
    void set_title_text(const QString& title);
protected:
    //virtual void paintEvent(QPaintEvent* event);
public slots://槽函数 用于接收外面的控制信号
    void open(const QUrl& path);//打开(本地或者网络)的媒体
    void play();//播放或者继续
    void pause();//暂停
    void stop();//停止播放
    void close();//关闭媒体
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
private slots:
    void handleTimer();
    void volume(int vol);
    void silence();//在静音和恢复之间切换

signals://信号函数，用于对外发出信号
    //大约500ms发出一次，用于通知外部播放进度
    void position(double pos);
    //通知状态发生改变：
    //0 空 1 打开媒体 2 播放 3 暂停 4 停止 5 关闭 <0 错误
    void media_status(QMediaPlayer::PlayerStatus s);

private:
    //成员初始化。警告：仅能在构造函数使用一次！！！！
    void init_member();
private:
    QTimer* timer;
    vlchelper* vlc;
    //-1 视频模块初始化失败
    QMediaPlayer::PlayerStatus stat;
    QMediaPlayer::PlayerStatus last;
};

#endif // QMEDIAPLAYER_H
