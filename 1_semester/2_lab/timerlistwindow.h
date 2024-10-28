#ifndef TIMERLISTWINDOW_H
#define TIMERLISTWINDOW_H

#include <QDialog>
#include "timer.h"
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimeZone>
#include "imagewindow.h"
//#include <QCloseEvent>
#include <QRadioButton>
#include <QLabel>
#include <QFontDatabase>
#include <QMessageBox>
#include <QIcon>
#include <QFileInfo>
#include <QProcess>
#include <QDesktopServices>
#include "countdown.h"
#include "timerinfowindow.h"

class SettingsWindow;

namespace Ui {
class TimerListWindow;
}

class TimerListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TimerListWindow(QDialog *parent, Timer* timer, QString soundName, QString imageName, QString appName, QString documentName, QString Title, TimerType Ttype, QDateTime selectedDateTime, SettingsWindow* settings);
    ~TimerListWindow();

private slots:
    void TimerStart();
    void PlaySoundtrack();
    void ShowImage();
    void openApplication();
    void openDocument();
    void updateTimeoutConnections();
    void restartTimer();
    void PauseResume();
    void openInfoWindow();
    void updateTimerInfo(QDateTime selectedDateTime, QString soundName, QString imageName, QString appName, QString documentName, QString Title, bool AmPm);

    void SetAlarmDisplay();
    void TurnAlarmOnOff();

private:

    Ui::TimerListWindow *ui;

    Timer* timer;
    QDateTime selectedDateTime;
    QString soundName;
    QString imageName;
    QString appName;
    QString documentName;
    QString Title;
    bool timerPaused = false;
    bool AlarmOn = true;
    int remainingTime = 0;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    imageWindow *img;
    TimerType Ttype;
    QString TimeFormat;
    CountDown *countDown;

    TimerInfoWindow *info;
    SettingsWindow *settings;
};

#endif // TIMERLISTWINDOW_H
