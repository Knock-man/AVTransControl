#ifndef RECORDFILE_H
#define RECORDFILE_H
#include <QString>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include "ssltool.h"

//配置和信息记录文件类
class RecordFile
{
public:
    RecordFile(const QString& path);//读取文件
    ~RecordFile();
    QJsonObject& config();//返回m_config
    bool save();//写入文件
private:
    QJsonObject m_config;//json 对象
    QString m_path;//文件路径
    SslTool tool;//SSL工具类
};

#endif // RECORDFILE_H
