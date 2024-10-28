#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QLCDNumber>
#include <QSpinBox>
#include <QLabel>

class Timer : public QObject {
    Q_OBJECT

public:
    explicit Timer(QObject *parent = nullptr);
    void start(int msec);
    void stop();
    void setSpinBoxes(QSpinBox *hours, QSpinBox *minutes, QSpinBox *seconds);
    QTime getInitialTime();
    void setTimeZone(QTimeZone *zone);
    QDateTime changeTime();
    QString TimeFormat;
    QString AmPm;
    int getRemainingTime();

signals:
    void timeout();

private:
    QTimer *timer;
    QTimeZone *timeZone;

    QSpinBox *hourCount;
    QSpinBox *minuteCount;
    QSpinBox *secondCount;
};

#endif // TIMER_H
