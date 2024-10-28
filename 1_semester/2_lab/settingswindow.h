#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QFile>
#include <QTime>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimeZone>
#include "timer.h"
#include "imagewindow.h"
#include <QCloseEvent>
#include <QRadioButton>
#include <QLabel>
#include <QFontDatabase>
#include <QMessageBox>
#include <QIcon>
#include "timerlistwindow.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QTimeZone *timezone, QString TimeFormat, QWidget *parent = nullptr);
    ~SettingsWindow();

    bool CheckTimeFormat();

    void setLists();

    Ui::SettingsWindow *ui;

    Timer *timer;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateTimeLabel();
    void openTimerLists();
    void uploadImage();
    void uploadSound();
    void uploadApp();
    void uploadDocument();

private:


    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QFile *file;
    QVBoxLayout *layout;
    QTimeZone *timeZone;
    QTime selectedTime;
    imageWindow *img;
    QString TimeFormat;

    TimerListWindow *Tlists = nullptr;
    TimerType Ttype;
};

#endif // SETTINGSWINDOW_H
