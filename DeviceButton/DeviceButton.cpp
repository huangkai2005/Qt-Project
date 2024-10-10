#include "DeviceButton.h"

DeviceButton::DeviceButton(QWidget *parent)
    : QWidget{parent}
    ,canMove (false)
    ,text ("1")

    ,colorNormal("black")
    ,colorAlarm ( "red")

    ,buttonStyle(ButtonStyle_Police)
    ,buttonColor(ButtonColor_Green)

    ,isPressed (false)
    ,lastPoint(QPoint())

    ,type("police")
    ,imgPath(":/image/devicebutton/devicebutton")
    ,imgName (QString("%1_green_%2.png").arg(imgPath).arg(type))

    ,isDark (false)
    ,timer (new QTimer(this))//计时器
{
    timer->setInterval(500);
    connect(timer,&QTimer::timeout,this,&DeviceButton::checkAlarm);

    this->installEventFilter(this);
}

DeviceButton::~DeviceButton()
{
    //终止定时器
    if (timer->isActive()) {
        timer->stop();
    }

}

//画家
void DeviceButton::paintEvent(QPaintEvent *)
{
    double width = this->width();
    double height = this->height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //绘制背景图
    QImage img(imgName);
    if (!img.isNull()) {
        img = img.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(0, 0, img);
    }

    //计算字体
    QFont font;
    font.setPixelSize(height * 0.37);
    font.setBold(true);

    //自动计算文字绘制区域,绘制防区号
    QRectF rect = this->rect();
    double y = 0.0;

    switch (buttonStyle) {
    case ButtonStyle_Bubble:
        y = (8 * height / 60);
        break;
    case ButtonStyle_Police:
        y = (30 * height / 60);
        break;
    case ButtonStyle_Bubble2:
        y = (13 * height / 60);
        font.setPixelSize(width * 0.33);
        break;
    case ButtonStyle_Msg:
    case ButtonStyle_Msg2:
        y = (17 * height / 60);
        break;
    case ButtonStyle_Circle:
        // 处理 Circle 样式
        break;
    default:
        break;
    }

    // 使用 y 计算 rect
    rect = QRectF(0, (buttonStyle == ButtonStyle_Police ? y : 0), width, height - y);


    //绘制文字标识
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignCenter, text);
}

//鼠标事件过滤器，分别判断是什么鼠标事件
bool DeviceButton::eventFilter(QObject *watched, QEvent *event)
{
    //识别鼠标 按下+移动+松开+双击 等事件
    int type = event->type();
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (type == QEvent::MouseButtonPress) {
        //限定鼠标左键
        if (mouseEvent->button() == Qt::LeftButton) {
            lastPoint = mouseEvent->pos();
            isPressed = true;
            Q_EMIT clicked();   //触发点击信号
            return true;
        }
    } else if (type == QEvent::MouseMove) {
        //允许拖动并且鼠标按下准备拖动
        if (canMove && isPressed) {
            int dx = mouseEvent->pos().x() - lastPoint.x();
            int dy = mouseEvent->pos().y() - lastPoint.y();
            this->move(this->x() + dx, this->y() + dy);
            return true;
        }
    } else if (type == QEvent::MouseButtonRelease) {
        isPressed = false; //按下状态重置
    } else if (type == QEvent::MouseButtonDblClick) {
        Q_EMIT doubleClicked(); //释放双击信号
    }

    return QWidget::eventFilter(watched, event);
}

//返回默认尺寸
QSize DeviceButton::sizeHint() const
{
    return QSize(50, 50);
}

//返回最小尺寸
QSize DeviceButton::minimumSizeHint() const
{
    return QSize(10, 10);
}

//切换报警模式
void DeviceButton::checkAlarm()
{
    if (isDark) {
        imgName = QString("%1_%2_%3.png").arg(imgPath).arg(colorNormal).arg(type);
    } else {
        imgName = QString("%1_%2_%3.png").arg(imgPath).arg(colorAlarm).arg(type);
    }

    isDark = !isDark;
    this->update();
}

//查询是否可动
bool DeviceButton::getCanMove() const
{
    return this->canMove;
}

//设置是否可动
void DeviceButton::setCanMove(bool canMove)
{
    this->canMove = canMove;
}

//获取房屋文本
QString DeviceButton::getText() const
{
    return this->text;
}

//设置文本
void DeviceButton::setText(const QString &text)
{
    if (this->text != text) {
        this->text = text;
        this->update();
    }
}

//得到此时的正常颜色
QString DeviceButton::getColorNormal() const
{
    return this->colorNormal;
}

//设置当前正常颜色
void DeviceButton::setColorNormal(const QString &colorNormal)
{
    if (this->colorNormal != colorNormal) {
        this->colorNormal = colorNormal;
        this->update();
    }
}

//得到当前警戒颜色
QString DeviceButton::getColorAlarm() const
{
    return this->colorAlarm;
}

//设置当前警戒颜色
void DeviceButton::setColorAlarm(const QString &colorAlarm)
{
    if (this->colorAlarm != colorAlarm) {
        this->colorAlarm = colorAlarm;
        this->update();
    }
}

//获取左上角按钮形态
DeviceButton::ButtonStyle DeviceButton::getButtonStyle() const
{
    return this->buttonStyle;
}

//设置左上角按钮形态
void DeviceButton::setButtonStyle(const DeviceButton::ButtonStyle &buttonStyle)
{
    this->buttonStyle = buttonStyle;
    switch (buttonStyle) {
    case ButtonStyle_Bubble:
        type = "bubble";
        break;
    case ButtonStyle_Bubble2:
        type = "bubble2";
        break;
    case ButtonStyle_Msg:
        type = "msg";
        break;
    case ButtonStyle_Msg2:
        type = "msg2";
        break;
    case ButtonStyle_Circle:
        type = "circle";
        break;
    case ButtonStyle_Police:
        type = "police";
        break;
    default:
        break;
    }

    setButtonColor(buttonColor);
}

//获取左上角按钮颜色
DeviceButton::ButtonColor DeviceButton::getButtonColor() const
{
    return this->buttonColor;
}

//设置左上角按钮颜色
void DeviceButton::setButtonColor(const DeviceButton::ButtonColor &buttonColor)
{
    //先停止定时器
    this->buttonColor = buttonColor;
    isDark = false;
    if (timer->isActive()) {
        timer->stop();
    }

    QString color;
    switch (buttonColor) {
    case ButtonColor_Green:
        color = "green";
        break;
    case ButtonColor_Blue:
        color = "blue";
        break;
    case ButtonColor_Gray:
        color = "gray";
        break;
    case ButtonColor_Purple:
        color = "purple";
        break;
    case ButtonColor_Black:
        color = "black";
        break;
    case ButtonColor_Red:
        color = "red";
        break;
    case ButtonColor_Yellow:
        color = "yellow";
        break;
    default:
        break;
    }

    //如果和报警颜色一致则主动启动定时器切换报警颜色
    imgName = QString("%1_%2_%3.png").arg(imgPath).arg(color).arg(type);
    if (color == colorAlarm) {
        checkAlarm();
        if (!timer->isActive()) {
            timer->start();
        }
    }

    this->update();
}

