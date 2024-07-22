#ifndef INFOFORM_H
#define INFOFORM_H

#include <QWidget>

namespace Ui
{
    class InfoForm;
}

class InfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit InfoForm(QWidget* parent = nullptr);
    ~InfoForm();
public:
    //更新文字 每次弹出信息窗口之前，可以通过此接口来更新窗口信息内容
    InfoForm& set_text(const QString& text, const QString& button);
protected:
    //控制窗口移动
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
signals:
    //按钮按下了（如果是关闭的窗口，自带了close信号）
    void button_clicked();
    void closed();
protected slots:
    void on_connectButton_released();
    void on_closeButton_released();
private:
    Ui::InfoForm* ui;
    QPoint position;//按下鼠标的起始位置
};

#endif // INFOFORM_H
