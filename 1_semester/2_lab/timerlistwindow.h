#ifndef TIMERLISTWINDOW_H
#define TIMERLISTWINDOW_H

#include <QDialog>
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
    explicit TimerListWindow(QDialog *parent, TimerInfo timerInfo, SettingsWindow* settings);
    ~TimerListWindow();

private slots:
    void TimerStart();
    void PlaySoundtrack();
    void ShowImage();
    void openApplication();
    void openDocument();
    void updateTimeoutConnections();
    void resetTimer();
    void PauseResume();
    void generalTimeout();
    void openInfoWindow();
    void updateTimerInfo(TimerInfo timerInfo);

    void SetAlarmDisplay();
    void TurnAlarmOnOff();

private:

    Ui::TimerListWindow *ui;

    TimerInfo timerInfo;

    bool timerPaused = false;
    bool AlarmOn = true;
    int remainingTime = 0;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    imageWindow *img;
    CountDown *countDown;
    TimerInfoWindow *info;
    SettingsWindow *settings;
};

#endif // TIMERLISTWINDOW_H
