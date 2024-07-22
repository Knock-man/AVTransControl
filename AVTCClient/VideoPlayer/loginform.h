#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDesktopServices>

#include "infoform.h"
#include "recordfile.h"


namespace Ui
{
    class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT
    friend class testVideoPlayer;
public:
    explicit LoginForm(QWidget* parent = nullptr);
    ~LoginForm();

    //时间过滤器
    virtual bool eventFilter(QObject* watched, QEvent* event);
    //定时器超时函数 自动登录
    virtual void timerEvent(QTimerEvent* event);

protected slots://槽函数
    void on_logoButton_released();//点击登录
    void on_remberPwd_stateChanged(int state);//记住密码
    void slots_autoLoginCheck_stateChange(int state);//自动登录
    void slots_login_request_finshed(QNetworkReply* reply);//收到服务器响应消息

signals://信号
    //登录信号 主窗口槽函数应答
    void login(const QString& nick, const QByteArray& head);

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);//移动窗口
    virtual void mousePressEvent(QMouseEvent* event);//更新鼠标位置
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    //加密 向服务器发送Get请求
    bool check_login(const QString& user, const QString& pwd);
    void load_config();//加载配置文件,基本信息显示到界面上

private:
    Ui::LoginForm* ui;
    QNetworkAccessManager* net;//网络
    InfoForm info;//信息窗口
    RecordFile* record;//配置文件
    int auto_login_id;//自动登录定时器id
    QPoint position;//鼠标位置向量(相对于控件左上角)
};

#endif // LOGINFORM_H
