#ifndef TIMERINFOWINDOW_H
#define TIMERINFOWINDOW_H

#include <QDialog>
#include <QTime>
//#include "settingswindow.h"
#include <QWidget>
#include <QComboBox>
#include <QAbstractButton>
#include <QFileDialog>

class SettingsWindow;

enum class TimerType {Timer, Alarm};

namespace Ui {
class TimerInfoWindow;
}

class TimerInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TimerInfoWindow(QWidget *parent, QDateTime selectedDateTime, QString soundName, QString imageName, QString appName, QString documentName, QString Title, QString TimeFormat, SettingsWindow* settings, TimerType Ttype, bool AmPm);
    ~TimerInfoWindow();
    void copyComboBoxItems(QComboBox* source, QComboBox* destination);

signals:
    void timerInfoUpdated(QDateTime selectedDateTime, QString soundName, QString imageName, QString appName, QString documentName, QString Title, bool AmPm);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void uploadImage();
    void uploadSound();
    void uploadApp();
    void uploadDocument();

private:
    Ui::TimerInfoWindow *ui;
    SettingsWindow* settings;
    QString soundName;
    QString imageName;
    QString appName;
    QString documentName;
    QString Title;
    QString TimeFormat;
    QDateTime selectedDateTime;
    TimerType Ttype;
    bool AmPm; //AM - true, PM - false
};

#endif // TIMERINFOWINDOW_H
