#include "loginform.h"
#include "ui_loginform.h"
#include "widget.h"
#include <time.h>
#include <QPixmap>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QCryptographicHash>
//y%dcunyd6x202lmfm9=-y7#bd-w(ro4*(9u$0i-3#$txwbkzg$
const char* MD5_KEY = "*&^%$#@b.v+h-b*g/h@n!h#n$d^ssx,.kl<kl";
//const char* HOST = "http://192.168.0.152:8000";
//const char* HOST = "http://code.edoyun.com:9530";
const char* HOST = "http://127.0.0.1:19527";//服务器地址
bool LOGIN_STATUS = false;

LoginForm::LoginForm(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::LoginForm),//ui 转为代码文件
    auto_login_id(-1)//定时器id初始化为-1
{
    record = new RecordFile("edoyun.dat");//加载配置文件
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);//设置无头窗口(无标题，最大化，最小化等按钮)
    //头像进行缩放

    //设置账号编辑栏属性
    ui->nameEdit->setPlaceholderText(u8"用户名/手机号/邮箱");//底部灰色提示
    ui->nameEdit->setFrame(false);//平台样式指南指定的框架关闭
    ui->nameEdit->installEventFilter(this);//安装事件过滤
    //设置密码编辑栏属性
    ui->pwdEdit->setPlaceholderText(u8"填写密码");
    ui->pwdEdit->setFrame(false);
    ui->pwdEdit->setEchoMode(QLineEdit::Password);//设置为密码输入(替换为占位符)
    ui->pwdEdit->installEventFilter(this);
    ui->forget->installEventFilter(this);

    //初始化网络
    net = new QNetworkAccessManager(this);//发送网络请求（如 HTTP 请求）并接收响应。
    //完成一个网络请求时发送信号给槽函数 QNetworkReply包含响应信息
    connect(net, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slots_login_request_finshed(QNetworkReply*)));


    info.setWindowFlag(Qt::FramelessWindowHint);//信息窗口为无边框窗口
    info.setWindowModality(Qt::ApplicationModal);//模态窗口
    //信息窗口调整大小 居中显示
    QSize sz = size();//获取当前窗口的大小
    info.move((sz.width() - info.width()) / 2, (sz.height() - info.height()) / 2);
    load_config();//加载配置信息到界面
}

LoginForm::~LoginForm()
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    delete record;
    delete ui;
    delete net;
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
}

bool LoginForm::eventFilter(QObject* watched, QEvent* event)
{
    if(ui->pwdEdit == watched)//密码编辑框的事件
    {
        if(event->type() == QEvent::FocusIn)//有焦点
        {
            ui->pwdEdit->setStyleSheet("color: rgb(251, 251, 251);background-color: transparent;");//白色
        }
        else if(event->type() == QEvent::FocusOut)//没焦点
        {
            if(ui->pwdEdit->text().size() == 0)//有文本
            {
                ui->pwdEdit->setStyleSheet("color: rgb(71, 75, 94);background-color: transparent;");//灰色
            }
        }
    }
    else if(ui->nameEdit == watched)//账号编辑框事件
    {
        if(event->type() == QEvent::FocusIn)//有焦点
        {
            ui->nameEdit->setStyleSheet("color: rgb(251, 251, 251);background-color: transparent;");//白色
        }
        else if(event->type() == QEvent::FocusOut)//无焦点
        {
            if(ui->nameEdit->text().size() == 0)//有文本
            {
                ui->nameEdit->setStyleSheet("color: rgb(71, 75, 94);background-color: transparent;");//灰色
            }
        }
    }
    if((ui->forget == watched) && (event->type() == QEvent::MouseButtonPress))//忘记密码按下事件
    {
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        QDesktopServices::openUrl(QUrl(QString(HOST) + "/forget"));//打开一个网页
    }
    return QWidget::eventFilter(watched, event);//其他事件正常处理
}

//定时器超时 自动登录
void LoginForm::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == auto_login_id)
    {
        killTimer(auto_login_id);//取消定时器
        QJsonObject& root = record->config();
        QString user = root["user"].toString();
        QString pwd = root["password"].toString();
        check_login(user, pwd);
    }
}

//点击登录按钮
void LoginForm::on_logoButton_released()
{
    if(ui->logoButton->text() == u8"取消自动登录")
    {
        killTimer(auto_login_id);//如果时取消自动登录 关闭定时器
        auto_login_id = -1;
        ui->logoButton->setText(u8"登录");
    }
    else//登录
    {
        QString user = ui->nameEdit->text();
        //检查用户名的有效性
        if(user.size() == 0 || user == u8"用户名/手机号/邮箱")
        {
            info.set_text(u8"用户不能为空\r\n请输入用户名", u8"确认").show();
            ui->nameEdit->setFocus();//设置焦点到账号编辑框
            return;
        }
        //检查密码的有效性
        QString pwd = ui->pwdEdit->text();
        if(pwd.size() == 0 || pwd == u8"填写密码")
        {
            info.set_text(u8"密码不能为空\r\n请输入密码", u8"确认").show();
            ui->pwdEdit->setFocus();//设置焦点到密码编辑框
            return;
        }
        check_login(user, pwd);//加密 发送登录请求
    }
}

//记住密码复选框发生变化
void LoginForm::on_remberPwd_stateChanged(int state)
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    record->config()["remember"] = state == Qt::Checked;
    if(state == Qt::Checked)
    {
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    }
    else
    {
        //ui->autoLoginCheck->setChecked(false);//关闭记住密码，则取消自动登录
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    }
}

//自动登录复选框发生变化
void LoginForm::slots_autoLoginCheck_stateChange(int state)
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    record->config()["auto"] = state == Qt::Checked;
    if(state == Qt::Checked)
    {
        record->config()["remember"] = true;//记住密码也要跟着变化
        ui->remberPwd->setChecked(true);//自动登录会开启记住密码
        //ui->remberPwd->setCheckable(false);//禁止修改状态
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    }
    else
    {
        ui->remberPwd->setCheckable(true);//启动修改状态
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    }
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
}

//收到服务器响应消息
void LoginForm::slots_login_request_finshed(QNetworkReply* reply)
{
    this->setEnabled(true);//设置窗口可以用
    bool login_success = false;
    if(reply->error() != QNetworkReply::NoError)
    {
        info.set_text(u8"登录发生错误\r\n" + reply->errorString(), u8"确认").show();
        return;
    }
    //json解析
    QByteArray data = reply->readAll();
    //qDebug() << data;
    QJsonParseError json_error;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &json_error);
    //qDebug() << "json error = "<<json_error.error;
    if (json_error.error == QJsonParseError::NoError)
    {
        if (doucment.isObject())//解析有数据
        {
            const QJsonObject obj = doucment.object();
            if (obj.contains("status") && obj.contains("message"))//存在status和message字段
            {
                QJsonValue status = obj.value("status");
                QJsonValue message = obj.value("message");
                if(status.toInt(-1) == 0) //登录成功
                {
                    //TODO:token 要保存并传递widget 用于保持在线状态
                    LOGIN_STATUS = status.toInt(-1) == 0;//设置登录状态
                    emit login(record->config()["user"].toString(), QByteArray());
                    hide();//隐藏登录对话框
                    login_success = true;
                    char tm[64] = "";
                    //更新登录时间
                    time_t t;
                    time(&t);
                    strftime(tm, sizeof(tm), "%Y-%m-%d %H:%M:%S", localtime(&t));
                    record->config()["date"] = QString(tm);
                    record->save();
                }
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
}

QString getTime()
{
    time_t t = 0;
    time (&t);
    return QString::number(t);
}

//加密 发送Get请求
bool LoginForm::check_login(const QString& user, const QString& pwd)
{
    /*
    QCryptographicHash md5(QCryptographicHash::Md5);
    QNetworkRequest request;
    QString url = QString(HOST) + "/login?";
    QString salt = QString::number(QRandomGenerator::global()->bounded(10000, 99999));//盐
    QString time = getTime();//时间
    qDebug().nospace()<< __FILE__ << "(" << __LINE__ << "):" <<time + MD5_KEY + pwd + salt;
    md5.addData((time + MD5_KEY + pwd + salt).toUtf8());//md5加密
    QString sign = md5.result().toHex();
    url += "time=" + time + "&";
    url += "salt=" + salt + "&";
    url += "user=" + user + "&";
    url += "sign=" + sign;
    //qDebug() << url;

    record->config()["password"] = ui->pwdEdit->text();
    record->config()["user"] = ui->nameEdit->text();
    this->setEnabled(false);//登录过程中设置窗口禁用

    request.setUrl(QUrl(url));//设置url
    net->get(request);//发送Get请求
    return true;
    */

    LOGIN_STATUS = true;
    emit login(record->config()["user"].toString(), QByteArray());
    hide();
    char tm[64] = "";
    time_t t;
    ::time(&t);
    strftime(tm, sizeof(tm), "%Y-%m-%d %H:%M:%S", localtime(&t));
    record->config()["date"] = QString(tm);//更新登录时间
    record->save();
    return false;
}

//加载配置到界面
void LoginForm::load_config()
{
    //自动登录QCheckbox状态变化信号→slots_autoLoginCheck_stateChange（）槽函数
    connect(ui->autoLoginCheck, SIGNAL(stateChanged(int)),
            this, SLOT(slots_autoLoginCheck_stateChange(int)));

    //利用Json加载界面基本信息
    QJsonObject& root = record->config();
    ui->remberPwd->setChecked(root["remember"].toBool());//是否记住密码
    ui->autoLoginCheck->setChecked(root["auto"].toBool());//是否自动登录
    QString user = root["user"].toString();//设置用户名密码
    QString pwd = root["password"].toString();
    ui->nameEdit->setText(user);
    ui->pwdEdit->setText(pwd);
    //qDebug() << "auto:" << root["auto"].toBool();
    //qDebug() << "remember:" << root["remember"].toBool();
    if(root["auto"].toBool()) //如果开启了自动登录，则检查用户名和密码是否ok
    {
        //qDebug() << __FILE__ << "(" << __LINE__ << "):user=" << user;
        //qDebug() << __FILE__ << "(" << __LINE__ << "):pwd=" << pwd;
        if(user.size() > 0 && pwd.size() > 0)
        {
            ui->logoButton->setText(u8"取消自动登录");//登录按钮显示取消自动登录
            auto_login_id = startTimer(3000);//开启定时器，给3秒的时间，方便用户终止登录过程
        }
    }
}

//鼠标移动
void LoginForm::mouseMoveEvent(QMouseEvent* event)
{
    move(event->globalPos() - position);//新鼠标点击位置对应控件应该对应的左上角位置
}

//鼠标按下 记录鼠标位置(相对于控件左上角的位置)  == 向量
void LoginForm::mousePressEvent(QMouseEvent* event)
{
    position = event->globalPos() - this->pos();
}

void LoginForm::mouseReleaseEvent(QMouseEvent* /*event*/)
{
}
