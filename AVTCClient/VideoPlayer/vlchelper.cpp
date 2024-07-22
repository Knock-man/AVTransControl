#include "vlchelper.h"
#include <functional>
#include <QDebug>
#include <QTime>
#include <QRandomGenerator>
#include <QImage>
/*
VLC使用教程：
#include <vlc/vlc.h>
libvlc_instance_t *inst = libvlc_new(0, nullptr);   //创建一个VLC实例
libvlc_media_t *media = libvlc_media_new_path(inst,"path_to_your_video_file.mp4") //加载一个媒体文件
libvlc_media_player_t *player = libvlc_media_player_new_from_media(media) //根据媒体文件类型创建一个播放器
libvlc_media_player_play(player)  //播放
libvlc_media_player_stop(player); //暂停

//回收资源
libvlc_release(inst);
libvlc_media_release(media);
libvlc_media_player_release(m_player);
*/


using namespace std;
using namespace std::placeholders;

//vlc日志
void libvlc_log_callback(void* data, int level, const libvlc_log_t* ctx,
                         const char* fmt, va_list args)
{
    //qDebug() << "log level:" << level;
}

//播放器相关
vlchelper::vlchelper(QWidget* widget)
    : m_logo(":/ico/UI/ico/128-128.png")
{
    const char* const args[] =
    {
        "--sub-filter=logo",//显示logo
        "--sub-filter=marq"//显示字幕
    };
    //qDebug() << __FUNCTION__ << ":" << __LINE__;
    m_instance = libvlc_new(2, args);//创建vlc实例
    if(m_instance != NULL)
    {
        //qDebug() << __FUNCTION__ << ":" << __LINE__;
        m_media = new vlcmedia(m_instance);// 创建一个媒体(自己封装的类)
    }
    else
    {
        //qDebug() << __FUNCTION__ << ":" << __LINE__;
        m_media = NULL;
        throw QString("是否没有安装插件？！！");
    }
    libvlc_log_set(m_instance, libvlc_log_callback, this);//设置日志的回调
    m_player = NULL;
    m_hWnd = (HWND)widget->winId();
    winHeight = widget->frameGeometry().height();
    winWidth = widget->frameGeometry().width();
    m_widget = widget;
    //qDebug() << "*winWidth:" << winWidth;
    //qDebug() << "*winHeight:" << winHeight;
    m_isplaying = false;
    m_ispause = false;
    m_volume = 100;
}

vlchelper::~vlchelper()
{
    if(m_player != NULL)
    {
        stop();
        libvlc_media_player_set_hwnd(m_player, NULL);
        libvlc_media_player_release(m_player);
        m_player = NULL;
    }
    if(m_media != NULL)
    {
        m_media->free();
        m_media = NULL;
    }
    if(m_instance != NULL)
    {
        libvlc_release(m_instance);
        m_instance = NULL;
    }
}

//准备播放器
int vlchelper::prepare(const QString& strPath)
{
    //qDebug() << strPath;
    //m_media = libvlc_media_new_location(
    //              m_instance, strPath.toStdString().c_str());
    *m_media = strPath;//加载媒体文件(设置了回调)
    if(m_media == NULL)
    {
        return -1;
    }
    if(m_player != NULL)//存在播放器
    {
        libvlc_media_player_release(m_player);//释放原来的播放器(不同媒体应该用不同的播放器)
    }
    m_player = libvlc_media_player_new_from_media(*m_media);//根据媒体类型创建新的播放器
    if(m_player == NULL)
    {
        return -2;
    }
    m_duration = libvlc_media_get_duration(*m_media);//获取播放时长
    libvlc_media_player_set_hwnd(m_player, m_hWnd);//设置播放器的显示窗口
    libvlc_audio_set_volume(m_player, m_volume);//设置播放器音量
    libvlc_video_set_aspect_ratio(m_player, "16:9");//设置播放器宽高比
    if(text.size() > 0)
    {
        set_float_text();//设置悬浮文本
    }
    m_ispause = false;//初始化暂停状态
    m_isplaying = false;//初始化播放状态
    m_issilence = false;//初始是无静音状态

    //播放器宽高改变
    if(m_widget->frameGeometry().height() != winHeight)
    {
        winHeight = m_widget->frameGeometry().height();
    }
    if(m_widget->frameGeometry().width() != winWidth)
    {
        winWidth = m_widget->frameGeometry().width();
    }
    //qDebug() << "*winWidth:" << winWidth;
    //qDebug() << "*winHeight:" << winHeight;
    return 0;
}

//播放
int vlchelper::play()
{
    if(m_player == NULL)
    {
        return -1;
    }
    if(m_ispause)//如果是暂停，则直接使用play来恢复
    {
        int ret = libvlc_media_player_play(m_player);
        if(ret == 0)//如果执行成功，则改变暂停状态
        {
            m_ispause = false;
            m_isplaying = true;
        }
        return ret;
    }
    if((m_player == NULL) ||//没有设置媒体
            (m_media->path().size() <= 0))
    {
        m_ispause = false;
        m_isplaying = false;
        return -2;
    }
    m_isplaying = true;
    libvlc_video_set_mouse_input(m_player, 0); //使得vlc不处理鼠标交互，方便qt处理
    libvlc_video_set_key_input(m_player, 0); //使得vlc不处理键盘交互，方便qt处理
    libvlc_set_fullscreen(m_player, 1);
    return libvlc_media_player_play(m_player);//开始播放
}

//暂停
int vlchelper::pause()
{
    if(m_player == NULL)
    {
        return -1;
    }
    libvlc_media_player_pause(m_player);
    m_ispause = true;
    m_isplaying = false;
    return 0;
}

//停止
int vlchelper::stop()
{
    if(m_player != NULL)
    {
        libvlc_media_player_stop(m_player);
        m_isplaying = false;
        return 0;
    }
    return -1;
}

//调节音量
int vlchelper::volume(int vol)
{
    if(m_player == NULL)
    {
        return -1;
    }
    if(vol == -1)
    {
        return m_volume;
    }
    int ret = libvlc_audio_set_volume(m_player, vol);
    if(ret == 0)
    {
        m_volume = vol;
        return m_volume;
    }
    return ret;
}

//静音
int vlchelper::silence()
{
    if(m_player == NULL)
    {
        return -1;
    }
    if(m_issilence)
    {
        //恢复
        libvlc_audio_set_mute(m_player, 0);
        m_issilence = false;
    }
    else
    {
        //静音
        m_issilence = true;
        libvlc_audio_set_mute(m_player, 1);
    }
    return m_issilence;
}

//是否正在播放
bool vlchelper::isplaying()
{
    return m_isplaying;
}

//是否静音
bool vlchelper::ismute()
{
    if(m_player && m_isplaying)
    {
        return libvlc_audio_get_mute(m_player) == 1;
    }
    return false;
}

//播放时间获取
libvlc_time_t vlchelper::gettime()
{
    if(m_player == NULL)
    {
        return -1;
    }
    return libvlc_media_player_get_time(m_player);
}

//播放时长获取
libvlc_time_t vlchelper::getduration()
{
    if(m_media == NULL)
    {
        return -1;
    }
    if(m_duration == -1)
    {
        m_duration = libvlc_media_get_duration(*m_media);
    }
    return m_duration;
}

//设置播放时间
int vlchelper::settime(libvlc_time_t time)
{
    if(m_player == NULL)
    {
        return -1;
    }
    libvlc_media_player_set_time(m_player, time);
    return 0;
}

//设置播放速率
int vlchelper::set_play_rate(float rate)
{
    if(m_player == NULL)
    {
        return -1;
    }
    return libvlc_media_player_set_rate(m_player, rate);
}

//获取播放速率
float vlchelper::get_play_rate()
{
    if(m_player == NULL)
    {
        return -1.0;
    }
    return libvlc_media_player_get_rate(m_player);
}

//初始化播放图标
void vlchelper::init_logo()
{
    //libvlc_video_set_logo_int(m_player, libvlc_logo_file, m_logo.handle());
    libvlc_video_set_logo_string(m_player, libvlc_logo_file, "128-128.png"); //Logo 文件名
    libvlc_video_set_logo_int(m_player, libvlc_logo_x, 0); //logo的 X 坐标。
    //libvlc_video_set_logo_int(m_player, libvlc_logo_y, 0); // logo的 Y 坐标。
    libvlc_video_set_logo_int(m_player, libvlc_logo_delay, 100);//标志的间隔图像时间为毫秒,图像显示间隔时间 0 - 60000 毫秒。
    libvlc_video_set_logo_int(m_player, libvlc_logo_repeat, -1); // 标志logo的循环,  标志动画的循环数量。-1 = 继续, 0 = 关闭
    libvlc_video_set_logo_int(m_player, libvlc_logo_opacity, 122);
    // logo 透明度 (数值介于 0(完全透明) 与 255(完全不透明)
    libvlc_video_set_logo_int(m_player, libvlc_logo_position, 5);
    //1 (左), 2 (右), 4 (顶部), 8 (底部), 5 (左上), 6 (右上), 9 (左下), 10 (右下),您也可以混合使用这些值，例如 6=4+2    表示右上)。
    libvlc_video_set_logo_int(m_player, libvlc_logo_enable, 1); //设置允许添加logo
}

//初始化文字(防盗版)
void vlchelper::init_text(const QString& text)
{
    this->text = text;
}

//更新图标
void vlchelper::update_logo()
{
    static int alpha = 0;
    //static int pos[] = {1, 5, 4, 6, 2, 10, 8, 9};
    int height = QRandomGenerator::global()->bounded(20, winHeight - 20);
    libvlc_video_set_logo_int(m_player, libvlc_logo_y, height); // logo的 Y 坐标。
    int width = QRandomGenerator::global()->bounded(20, winWidth - 20);
    libvlc_video_set_logo_int(m_player, libvlc_logo_x, width); //logo的 X 坐标。
    libvlc_video_set_logo_int(m_player, libvlc_logo_opacity, (alpha++) % 80 + 20); //透明度
    //libvlc_video_set_logo_int(m_player, libvlc_logo_position, pos[alpha % 8]);
}

//更新文字
void vlchelper::update_text()
{
    static int alpha = 0;
    if(m_player)
    {
        int color = QRandomGenerator::global()->bounded(0x30, 0x60);//R
        color = color << 8 | QRandomGenerator::global()->bounded(0x30, 0x60);//G
        color = color << 8 | QRandomGenerator::global()->bounded(0x30, 0x60);//B
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Color, color);//颜色
        int x = QRandomGenerator::global()->bounded(20, winHeight - 20);//随机位置
        int y = QRandomGenerator::global()->bounded(20, winWidth - 20);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_X, x);//
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Y, y);
        //随机透明度
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Opacity, (alpha++ % 60) + 10);
    }
}

//文字是否允许
bool vlchelper::is_text_enable()
{
    if(m_player == NULL)
    {
        return false;
    }
    return libvlc_video_get_marquee_int(m_player, libvlc_marquee_Enable) == 1;
}
//图标是否允许
bool vlchelper::is_logo_enable()
{
    if(m_player == NULL)
    {
        return false;
    }
   // qDebug() << __FUNCTION__ << " logo enable:" << libvlc_video_get_logo_int(m_player, libvlc_logo_enable);
    return libvlc_video_get_logo_int(m_player, libvlc_logo_enable) == 1;
}
//是否有播放器在播放媒体
bool vlchelper::has_media_player()
{
    return m_player != NULL && (m_media != NULL);
}

//设置悬浮文字
void vlchelper::set_float_text()
{
    if(m_player)
    {
        libvlc_video_set_marquee_string(m_player, libvlc_marquee_Text, text.toStdString().c_str());
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Color, 0x404040);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_X, 0);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Y, 0);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Opacity, 100);
        //libvlc_video_set_marquee_int(m_player, libvlc_marquee_Timeout, 100);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Position, 5);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Size, 14);
        libvlc_video_set_marquee_int(m_player, libvlc_marquee_Enable, 1);
    }
}


//媒体相关
vlcmedia::vlcmedia(libvlc_instance_t* ins)
    : instance(ins)
{
    media = NULL;
    media_instance = new MediaMP4();//创建一个MP4的媒体
}

vlcmedia::~vlcmedia()
{
    if(media)
    {
        free();
    }
    if(media_instance)
    {
        delete media_instance;
        media_instance = NULL;
    }
}

int vlcmedia::open(void* thiz, void** infile, uint64_t* fsize)
{
    vlcmedia* _this = (vlcmedia*)thiz;
    return _this->open(infile, fsize);
}

ssize_t vlcmedia::read(void* thiz, uint8_t* buffer, size_t length)
{
    vlcmedia* _this = (vlcmedia*)thiz;
    return _this->read(buffer, length);
}

int vlcmedia::seek(void* thiz, uint64_t offset)
{
    vlcmedia* _this = (vlcmedia*)thiz;
    return _this->seek(offset);
}

void vlcmedia::close(void* thiz)
{
    vlcmedia* _this = (vlcmedia*)thiz;
    _this->close();
}

//设置回调
vlcmedia& vlcmedia::operator=(const QString& str)
{
    if(media)//存在媒体 先删除再加载新媒体
    {
        free();
    }
    //libvlc_media_read_cb
    strPath = str;//媒体路径
    media = libvlc_media_new_callbacks(//设置媒体回调
                instance,
                &vlcmedia::open,
                &vlcmedia::read,
                &vlcmedia::seek,
                &vlcmedia::close,
                this);
    return *this;
    /*
    libvlc_media_new_callbacks 是 VLC 媒体播放器库（libvlc）中的一个函数，它允许开发者为媒体对象设置自定义的回调函数，
    以控制媒体数据的打开、读取、偏移和关闭等操作。这个函数在需要自定义媒体数据读取逻辑的场景中非常有用，
    例如当媒体数据来自非标准源（如网络流、加密数据等）时。
    */
}

//释放media
void vlcmedia::free()
{
    if(media != NULL)
    {
        libvlc_media_release(media);
    }
}

//返回文件路径
QString vlcmedia::path()
{
    return strPath;
}

//打开媒体文件
int vlcmedia::open(void** infile, uint64_t* fsize)
{
    //方式1 使用media_instance打开文件(加密文件)
    //"file:///"

    if(media_instance)
    {
        qDebug()<<__FILE__<<__FUNCTION__<<(__LINE__)<<u8"打开媒体文件路径(加密)"<<strPath;
        *infile = this;
        int ret = media_instance->open(strPath, fsize);
        media_size = *fsize;
        return ret;
    }

    //方式2 使用C++流打开文件(普通文件)
    qDebug()<<__FILE__<<__FUNCTION__<<(__LINE__)<<u8"打开媒体文件路径(无加密)"<<strPath;
    this->infile.open(strPath.toStdString().c_str() + 8, ios::binary | ios::in);
    this->infile.seekg(0, ios::end);//移到末尾
    *fsize = (uint64_t)this->infile.tellg();//获取文件指针位置
    media_size = *fsize;
    this->infile.seekg(0);//指针移到开头
    *infile = this;//指向当前对象
    return 0;
}

//读取文件
ssize_t vlcmedia::read(uint8_t* buffer, size_t length)
{
    //方式1 使用media_instance读取文件(加密文件)
    if(media_instance)
    {
        return media_instance->read(buffer, length);
    }

    //方式2 使用C++流来读取文件（普通文件）
    //qDebug() << __FUNCTION__ << " length:" << length;
    uint64_t pos = (uint64_t)infile.tellg();//获取文件指针位置
    //qDebug() << __FUNCTION__ << " positon:" << pos;
    if(media_size - pos < length)//读取长度是否超出
    {
        length = media_size - pos;
    }
    infile.read((char*)buffer, length);//读取文件
    return infile.gcount();//返回读取字节数
}

//设置读取位置
int vlcmedia::seek(uint64_t offset)
{
    //方式1 使用media_instance设置偏移
    if(media_instance)
    {
        return media_instance->seek(offset);
    }
    //qDebug() << __FUNCTION__ << ":" << offset;

    //方式2 使用C++流偏移
    infile.clear();//清除任何先前的流错误状态
    infile.seekg(offset);//偏移到offset位置
    return 0;
}

//关闭文件
void vlcmedia::close()
{
    if(media_instance)
    {
        return media_instance->close();
    }
    //qDebug() << __FUNCTION__;
    infile.close();
}

vlcmedia::operator libvlc_media_t* ()
{
    return media;
}
