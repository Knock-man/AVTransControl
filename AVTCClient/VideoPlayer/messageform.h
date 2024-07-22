#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H
#include <Windows.h>
#include <QtWin>
#include <QWidget>
#include <QImage>

namespace Ui
{
    class MessageForm;
}

class MessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit MessageForm(QWidget* parent = nullptr);
    ~MessageForm();
    void set_widget(QWidget* bkgnd)
    {
        backgnd = bkgnd;
    }
    void full_size()//设置半透明窗体大小
    {
        setGeometry(0, 0, screen_width, 200);
    }
    void setText(const QString& text);//设置显示文字
protected:
    virtual void paintEvent(QPaintEvent* event);//画图事件 画截图 渐变色图 写字
    QImage CopyDCToBitmap(HWND hWnd);//截图
private:
    Ui::MessageForm* ui;
    QWidget* backgnd;
    QImage* top;//半透明图片
    int screen_width;
    QString text;//待写的文字
};

#endif // MESSAGEFORM_H
