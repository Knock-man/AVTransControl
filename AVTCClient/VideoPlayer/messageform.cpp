#include "messageform.h"
#include "ui_messageform.h"
#include <QDebug>
#include <QtWin>
#include <QPainter>
#include <QDesktopWidget>

RECT rect;

MessageForm::MessageForm(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MessageForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
    top = new QImage(":/UI/images/screentop.png");//半透明图片
    screen_width = QApplication::desktop()->width();
}


MessageForm::~MessageForm()
{
    delete ui;
    delete top;
}

void MessageForm::setText(const QString& text)
{
    this->text = text;
}

//画图事件 画截图 渐变色图 写字
void MessageForm::paintEvent(QPaintEvent*)
{
    QPainter painter(this);//画家
    QRect r = frameGeometry();//获取半透明框范围
    POINT pt = {r.right() / 2, r.bottom() + 5};//找到中间点(后台窗口)
    HWND hWndBack = WindowFromPoint(pt);//中间点对应的后台窗口

    QImage image = CopyDCToBitmap(hWndBack);//截图
    QFont font = painter.font();//(u8"黑体", 12, QFont::Bold, true);
    font.setPixelSize(18);//设置大小
    painter.setFont(font);
    painter.setCompositionMode(QPainter::CompositionMode_Source);//新绘制的图像或颜色完全替换绘图设备上的现有内容
    painter.drawImage(0, 0, image, 0, 0);//绘制截图
    painter.drawImage(0, 0, *top, 0, 0);//绘制半透明图片

    //写字
    QPen pen = painter.pen();
    pen.setColor(QColor(255, 255, 255));
    painter.setPen(pen);
    r.setLeft(r.left() + 50);
    r.setTop(r.top() + 50);
    painter.drawText(r, text);
}

//截图(将DC转换为位图)
QImage MessageForm::CopyDCToBitmap(HWND hWnd)
{
    //获取整个窗口的DC
    HDC hDC = GetWindowDC(hWnd);
    if(!hDC)
    {
        return QImage();
    }

    //获取窗口的范围
    RECT wndRect;
    GetWindowRect(hWnd, &wndRect);
    int nWidth = wndRect.right - wndRect.left;
    int nHeight = wndRect.bottom - wndRect.top;

    HDC hMemDC = CreateCompatibleDC(hDC);//创建一个内存设备上下文
    HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);//创建位图
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);//选择新创建的位图到设备上下文
    //::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY | PW_RENDERFULLCONTENT);//TODO:新更改的
    ::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY);//将窗口内容绘制到位图

    //释放资源
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);//恢复原来的位图对象到hMemDC
    DeleteDC(hMemDC);
    DeleteObject(hOldBitmap);

    //转为QImage
    QImage ret = QtWin::imageFromHBITMAP(hBitmap);
    DeleteObject(hBitmap);
    return ret;
}
