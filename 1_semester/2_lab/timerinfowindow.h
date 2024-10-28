#ifndef TIMERINFOWINDOW_H
#define TIMERINFOWINDOW_H

#include <QDialog>
#include <QTime>
//#include "settingswindow.h"
#include <QWidget>
#include <QComboBox>
#include <QAbstractButton>
#include <QFileDialog>
#include "timerinfo.h"

class SettingsWindow;

namespace Ui {
class TimerInfoWindow;
}

class TimerInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TimerInfoWindow(QWidget *parent, TimerInfo timerInfo, SettingsWindow* settings);
    ~TimerInfoWindow();
    void copyComboBoxItems(QComboBox* source, QComboBox* destination);

signals:
    void timerInfoUpdated(TimerInfo timerInfo);

private slots:
    void uploadImage();
    void uploadSound();
    void uploadApp();
    void uploadDocument();
    void OKPushed();
    void CancelPushed();

private:
    Ui::TimerInfoWindow *ui;
    SettingsWindow* settings;
    TimerInfo timerInfo;
};

#endif // TIMERINFOWINDOW_H
