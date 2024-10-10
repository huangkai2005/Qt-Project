#include "Battery.h"

Battery::Battery(QWidget *parent)
    : QWidget{parent}
    // 初始化电池控件的各项参数
    , minValue(0)               // 电池最小值
    , maxValue(100)            // 电池最大值
    , value(0)                  // 当前电池值
    , alarmValue(30)            // 报警值

    , animation(true)           // 是否启用动画
    , animationStep(0.5)        // 动画步长

    , borderRadius(8)           // 边框圆角
    , borderWidth(5)            // 边框宽度
    , bgRadius(5)               // 背景圆角
    , headRadius(3)             // 头部圆角

    , borderColorStart(QColor(100, 100, 100)) // 边框渐变开始颜色
    , borderColorEnd(QColor(80, 80, 80))       // 边框渐变结束颜色
    , alarmColorStart(QColor(250, 118, 113))   // 报警颜色开始
    , alarmColorEnd(QColor(204, 38, 38))       // 报警颜色结束
    , normalColorStart(QColor(50, 205, 51))    // 正常状态颜色开始
    , normalColorEnd(QColor(60, 179, 133))      // 正常状态颜色结束

    , isForward(false)           // 动画方向
    , currentValue(0)            // 当前值
    , timer(new QTimer(this))    // 定时器初始化
{
    timer->setInterval(10);      // 设置定时器间隔
    connect(timer, &QTimer::timeout, this, &Battery::updateValue); // 连接定时器信号
}

Battery::~Battery()
{
    // 停止定时器（如果正在运行）
    if (timer->isActive()) {
        timer->stop();
    }
}

void Battery::paintEvent(QPaintEvent *)
{
    // 绘制准备工作, 启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//抗锯齿和文本抗锯齿

    drawBorder(&painter); // 绘制边框
    drawBg(&painter);     // 绘制背景
    drawHead(&painter);   // 绘制头部
}

void Battery::drawBorder(QPainter *painter)
{
    painter->save();

    double headWidth = width() / 15; // 头部宽度
    double batteryWidth = width() - headWidth; // 电池宽度

    // 绘制电池边框
    QPointF topLeft(borderWidth, borderWidth);
    QPointF bottomRight(batteryWidth, height() - borderWidth);
    batteryRect = QRectF(topLeft, bottomRight);

    painter->setPen(QPen(borderColorStart, borderWidth)); // 设置边框颜色和宽度
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(batteryRect, borderRadius, borderRadius); // 绘制带圆角的矩形

    painter->restore();//恢复到保存时的状态
}

void Battery::drawBg(QPainter *painter)
{
    if (value == minValue) { // 如果值为最小值，直接返回
        return;
    }

    painter->save();

    // 根据当前值设置渐变颜色
    QLinearGradient batteryGradient(QPointF(0, 0), QPointF(0, height()));
    if (currentValue <= alarmValue) {
        batteryGradient.setColorAt(0.0, alarmColorStart); // 报警颜色
        batteryGradient.setColorAt(1.0, alarmColorEnd);
    } else {
        batteryGradient.setColorAt(0.0, normalColorStart); // 正常颜色
        batteryGradient.setColorAt(1.0, normalColorEnd);
    }

    int margin = qMin(width(), height()) / 20; // 边距
    double unit = (batteryRect.width() - (margin * 2)) / (maxValue - minValue); // 单位宽度
    double width = currentValue * unit; // 当前值对应的宽度
    QPointF topLeft(batteryRect.topLeft().x() + margin, batteryRect.topLeft().y() + margin);
    QPointF bottomRight(width + margin + borderWidth, batteryRect.bottomRight().y() - margin);
    QRectF rect(topLeft, bottomRight);//获取一个矩阵

    painter->setPen(Qt::NoPen);
    painter->setBrush(batteryGradient); // 设置渐变颜色
    painter->drawRoundedRect(rect, bgRadius, bgRadius); // 绘制电池背景

    painter->restore();
}

void Battery::drawHead(QPainter *painter)
{
    painter->save();

    // 绘制电池头部
    QPointF headRectTopLeft(batteryRect.topRight().x(), height() / 3);
    QPointF headRectBottomRight(width(), height() - height() / 3);
    QRectF headRect(headRectTopLeft, headRectBottomRight);

    QLinearGradient headRectGradient(headRect.topLeft(), headRect.bottomLeft());
    headRectGradient.setColorAt(0.0, borderColorStart); // 头部渐变颜色
    headRectGradient.setColorAt(1.0, borderColorEnd);

    painter->setPen(Qt::NoPen);
    painter->setBrush(headRectGradient);
    painter->drawRoundedRect(headRect, headRadius, headRadius); // 绘制头部

    painter->restore();
}

void Battery::updateValue()
{
    // 更新电池当前值
    if (isForward) {
        currentValue -= animationStep; // 向下调整
        if (currentValue <= value) {
            currentValue = value; // 达到目标值
            timer->stop();
        }
    } else {
        currentValue += animationStep; // 向上调整
        if (currentValue >= value) {
            currentValue = value; // 达到目标值
            timer->stop();
        }
    }

    this->update(); // 触发重绘
}

QSize Battery::sizeHint() const
{
    return QSize(150, 80); // 默认尺寸建议
}

QSize Battery::minimumSizeHint() const
{
    return QSize(30, 10); // 最小尺寸建议
}

void Battery::setRange(double minValue, double maxValue)
{
    // 设置电池的最小和最大值
    if (minValue >= maxValue) {
        return; // 最小值大于等于最大值时不处理
    }

    this->minValue = minValue; // 设置最小值
    this->maxValue = maxValue; // 设置最大值

    // 根据当前值调整
    if (value < minValue) {
        setValue(minValue); // 重新设置目标值
    } else if (value > maxValue) {
        setValue(maxValue);
    }

    this->update(); // 触发重绘
}

void Battery::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue); // 重载方法
}

double Battery::getMinValue() const
{
    return this->minValue; // 获取最小值
}

void Battery::setMinValue(double minValue)
{
    setRange(minValue, maxValue); // 设置最小值
}

double Battery::getMaxValue() const
{
    return this->maxValue; // 获取最大值
}

void Battery::setMaxValue(double maxValue)
{
    setRange(minValue, maxValue); // 设置最大值
}

double Battery::getValue() const
{
    return this->value; // 获取当前值
}

void Battery::setValue(double value)
{
    // 设置电池值
    if (value == this->value) {
        return; // 值未变化
    }

    // 限制值在范围内
    if (value < minValue) {
        value = minValue;
    } else if (value > maxValue) {
        value = maxValue;
    }

    // 根据当前值更新动画方向
    if (value > currentValue) {
        isForward = false; // 向上调整
    } else if (value < currentValue) {
        isForward = true; // 向下调整
    } else {
        this->value = value; // 直接更新
        this->update();
        return;
    }
    this->value = value; // 更新目标值
    Q_EMIT valueChanged(value); // 发送值变化信号
    if (animation) {
        timer->stop(); // 重启定时器
        timer->start();
    } else {
        this->currentValue = value; // 直接更新当前值
        this->update();
    }
}

void Battery::setValue(int value)
{
    setValue((double)value); // 重载方法
}

double Battery::getAlarmValue() const
{
    return this->alarmValue; // 获取报警值
}

void Battery::setAlarmValue(double alarmValue)
{
    if (this->alarmValue != alarmValue) {
        this->alarmValue = alarmValue; // 设置报警值
        this->update(); // 触发重绘
    }
}

void Battery::setAlarmValue(int alarmValue)
{
    setAlarmValue((double)alarmValue); // 重载方法
}

bool Battery::getAnimation() const
{
    return this->animation; // 获取动画状态
}

void Battery::setAnimation(bool animation)
{
    if (this->animation != animation) {
        this->animation = animation; // 设置动画状态
        this->update(); // 触发重绘
    }
}

double Battery::getAnimationStep() const
{
    return this->animationStep; // 获取动画步长
}

void Battery::setAnimationStep(double animationStep)
{
    if (this->animationStep != animationStep) {
        this->animationStep = animationStep; // 设置动画步长
        this->update(); // 触发重绘
    }
}

int Battery::getBorderWidth() const
{
    return this->borderWidth; // 获取边框宽度
}

void Battery::setBorderWidth(int borderWidth)
{
    if (this->borderWidth != borderWidth) {
        this->borderWidth = borderWidth; // 设置边框宽度
        this->update(); // 触发重绘
    }
}

int Battery::getBorderRadius() const
{
    return this->borderRadius; // 获取边框圆角
}

void Battery::setBorderRadius(int borderRadius)
{
    if (this->borderRadius != borderRadius) {
        this->borderRadius = borderRadius; // 设置边框圆角
        this->update(); // 触发重绘
    }
}

int Battery::getBgRadius() const
{
    return this->bgRadius; // 获取背景圆角
}

void Battery::setBgRadius(int bgRadius)
{
    if (this->bgRadius != bgRadius) {
        this->bgRadius = bgRadius; // 设置背景圆角
        this->update(); // 触发重绘
    }
}

int Battery::getHeadRadius() const
{
    return this->headRadius; // 获取头部圆角
}

void Battery::setHeadRadius(int headRadius)
{
    if (this->headRadius != headRadius) {
        this->headRadius = headRadius; // 设置头部圆角
        this->update(); // 触发重绘
    }
}

QColor Battery::getBorderColorStart() const
{
    return this->borderColorStart; // 获取边框渐变开始颜色
}

void Battery::setBorderColorStart(const QColor &borderColorStart)
{
    if (this->borderColorStart != borderColorStart) {
        this->borderColorStart = borderColorStart; // 设置边框渐变开始颜色
        this->update(); // 触发重绘
    }
}

QColor Battery::getBorderColorEnd() const
{
    return this->borderColorEnd; // 获取边框渐变结束颜色
}

void Battery::setBorderColorEnd(const QColor &borderColorEnd)
{
    if (this->borderColorEnd != borderColorEnd) {
        this->borderColorEnd = borderColorEnd; // 设置边框渐变结束颜色
        this->update(); // 触发重绘
    }
}

QColor Battery::getAlarmColorStart() const
{
    return this->alarmColorStart; // 获取报警颜色开始
}

void Battery::setAlarmColorStart(const QColor &alarmColorStart)
{
    if (this->alarmColorStart != alarmColorStart) {
        this->alarmColorStart = alarmColorStart; // 设置报警颜色开始
        this->update(); // 触发重绘
    }
}

QColor Battery::getAlarmColorEnd() const
{
    return this->alarmColorEnd; // 获取报警颜色结束
}

void Battery::setAlarmColorEnd(const QColor &alarmColorEnd)
{
    if (this->alarmColorEnd != alarmColorEnd) {
        this->alarmColorEnd = alarmColorEnd; // 设置报警颜色结束
        this->update(); // 触发重绘
    }
}

QColor Battery::getNormalColorStart() const
{
    return this->normalColorStart; // 获取正常状态颜色开始
}

void Battery::setNormalColorStart(const QColor &normalColorStart)
{
    if (this->normalColorStart != normalColorStart) {
        this->normalColorStart = normalColorStart; // 设置正常状态颜色开始
        this->update(); // 触发重绘
    }
}

QColor Battery::getNormalColorEnd() const
{
    return this->normalColorEnd; // 获取正常状态颜色结束
}

void Battery::setNormalColorEnd(const QColor &normalColorEnd)
{
    if (this->normalColorEnd != normalColorEnd) {
        this->normalColorEnd = normalColorEnd; // 设置正常状态颜色结束
        this->update(); // 触发重绘
    }
}
