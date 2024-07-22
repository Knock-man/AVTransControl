
#ifndef VLCHELPER_H
#define VLCHELPER_H
#ifndef SSIZE_T
    #define SSIZE_T
    typedef long long ssize_t;
#endif
#include "vlc/vlc.h"
#include <QString>
#include <QWidget>
#include <fstream>
#include <mediamp4.h>

//主要控制对媒体(MP4)的操作
class vlcmedia
{
public:
    vlcmedia(libvlc_instance_t* instance);
    ~vlcmedia();
    static int open(void* thiz, void** infile, uint64_t* fsize);//打开媒体文件
    static ssize_t read(void* thiz, uint8_t* buffer, size_t length);//读取媒体文件
    static int seek(void* thiz, uint64_t offset);//移动指针
    static void close(void* thiz);//关闭媒体文件
    vlcmedia& operator=(const QString& str);//给媒体设置回调
    operator libvlc_media_t* ();//返回媒体media
    void free();//释放媒体文件
    QString path();//获取媒体路径
private:
    int open(void** infile, uint64_t* fsize);
    ssize_t read(uint8_t* buffer, size_t length);
    int seek(uint64_t offset);
    void close();
private:
    QString strPath;//媒体文件路径
    std::ifstream infile;//文件流
    uint64_t media_size;//文件大小
    MediaBase* media_instance;//MP4媒体 需要解密

    libvlc_instance_t* instance;//VLC 媒体播放器的实例
    libvlc_media_t* media;//VLC媒体
};



//主要控制播放器
class vlchelper
{
public:
    vlchelper(QWidget* widget);
    ~vlchelper();
    int prepare(const QString& strPath = "");//准备播放器
    int play();//播放
    int pause();//暂停
    int stop();//停止
    int volume(int vol = -1);//音量
    int silence();//静音
    bool isplaying();//是否在播放
    bool ispause() const//是否暂停
    {
        return m_ispause;
    }
    bool ismute();//是否静音
    libvlc_time_t gettime();//返回播放的毫秒数
    libvlc_time_t getduration();//获取总时长
    int settime(libvlc_time_t time);//设置播放时间
    int set_play_rate(float rate);//设置播放速率
    float get_play_rate();//获取播放速率
    void init_logo();//初始化logo
    void update_logo();//更新logo
    bool is_logo_enable();//是启用logo
    void init_text(const QString& text);//初始化文字
    void update_text();//更新文字
    bool is_text_enable();//是否启用文字
    bool has_media_player();//是否有媒体正在播放
private:
    void set_float_text();//设置悬浮文字
private:
    libvlc_instance_t* m_instance;//vlc实例
    libvlc_media_player_t* m_player;//播放器
    vlcmedia* m_media;//播放媒体(MP4)

    HWND m_hWnd;//主窗口句柄
    int winHeight;//主窗口高
    int winWidth;//主窗口宽
    bool m_isplaying;//是否正在播放
    bool m_ispause;//是否暂停
    bool m_issilence;//是否静音
    int m_volume;//音量
    libvlc_time_t m_duration;//当前媒体播放总时长（毫秒）

    QFile m_logo;//log
    QString text;//悬浮文字
    QWidget* m_widget;//主窗口
};

#endif // VLCHELPER_H
