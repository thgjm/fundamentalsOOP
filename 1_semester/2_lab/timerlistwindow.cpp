#include "timerlistwindow.h"
#include "ui_timerlistwindow.h"
#include "settingswindow.h"

TimerListWindow::TimerListWindow(QDialog *parent, TimerInfo timerInfo, SettingsWindow* settings)
    : QDialog(parent)
    , ui(new Ui::TimerListWindow), timerInfo(timerInfo), settings(settings)
{
    ui->setupUi(this);

    SetAlarmDisplay();

    qDebug()<<timerInfo.soundName;
    qDebug()<<timerInfo.imageName;
    qDebug()<<timerInfo.appName;
    qDebug()<<timerInfo.documentName;

    qDebug()<<timerInfo.Title;
    if(timerInfo.Ttype == TimerType::Timer)
    {
        qDebug()<<"Timer";
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(timerInfo.Ttype == TimerType::Alarm)
    {
        qDebug()<<"Alarm";
        ui->stackedWidget->setCurrentIndex(1);
    }

    ui->horizontalLayout_2->setAlignment(Qt::AlignCenter);

    qDebug() << timerInfo.selectedDateTime.time().hour()<<":"<<timerInfo.selectedDateTime.time().minute()<<":"<<timerInfo.selectedDateTime.time().second();

    connect(ui->resetButton, &QPushButton::clicked, this, &TimerListWindow::resetTimer);
    connect(ui->stopButton, &QPushButton::clicked, this, &TimerListWindow::PauseResume);

    TimerStart();

    ui->timerName->setText(timerInfo.Title);

    connect(ui->infoButton, &QPushButton::clicked, this, &TimerListWindow::openInfoWindow);

    connect(ui->turnOnOffButton, &QPushButton::clicked, this, &TimerListWindow::TurnAlarmOnOff);

    connect(ui->alarmInfo, &QPushButton::clicked, this, &TimerListWindow::openInfoWindow);
}

TimerListWindow::~TimerListWindow()
{
    if (timerInfo.timer)
        timerInfo.timer->stop();

    delete ui;
}

//-------------TIMER----------------

void TimerListWindow::PauseResume()
{
    if (timerPaused)
    {
        timerPaused = !timerPaused;
        TimerStart();

        ui->stopButton->setText("Pause");
    }
    else
    {
        timerPaused = !timerPaused;
        remainingTime = timerInfo.timer->getRemainingTime();
        timerInfo.timer->stop();
        countDown->stop();
        ui->stopButton->setText("Resume");
    }
}

void TimerListWindow::resetTimer()
{
    if(timerPaused)
    {
        countDown = new CountDown(this, QTime(0, 0).addSecs(timerInfo.selectedDateTime.time().second() + timerInfo.selectedDateTime.time().minute() * 60 + timerInfo.selectedDateTime.time().hour() * 3600));
        countDown->setDisplay(ui->countdownLabel);
        countDown->start();
        countDown->stop();
        ui->stopButton->setText("Start");
    }
    if (countDown)
    {
        countDown->stop();
        delete countDown;
        countDown = nullptr;
        if(ui->stopButton->text()!="Start") ui->stopButton->setText("Pause");
    }
    remainingTime = 0;
    ui->resetButton->setText("Reset Timer");
    ui->stopButton->show();
    TimerStart();
}


//-------------ALARM----------------

void TimerListWindow::SetAlarmDisplay()
{
    ui->alarmTimeLabel->setStyleSheet("QLabel { color : green; }");

    ui->alarmDateLabel->setStyleSheet("QLabel { color : pink; }");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Orbitron/static/Orbitron-Regular.ttf");
    if (fontId != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        ui->alarmTimeLabel->setFont(QFont(family, 40));
    }
    else
        qDebug() << "The font was not uploaded.";

    fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Oxanium/static/Oxanium-Regular.ttf");
    if (fontId != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        ui->alarmDateLabel->setFont(QFont(family, 20));
    }
    else
        qDebug() << "The font was not uploaded.";

    QString timeText;
    QString DateText = timerInfo.selectedDateTime.date().toString("MMMM d, yyyy");
    ui->alarmDateLabel->setText(DateText);

    if(timerInfo.timer->TimeFormat == "HH:mm:ss")
    {
        timeText = timerInfo.selectedDateTime.time().toString("HH:mm:ss");
        ui->alarmTimeLabel->setText(timeText);
    }
    else
    {
        QTime adjustedSelectedTime = timerInfo.selectedDateTime.time();
        if (timerInfo.timer->AmPm == "PM" && adjustedSelectedTime.hour() != 12)
            adjustedSelectedTime = adjustedSelectedTime.addSecs(12 * 3600);
        else if (timerInfo.timer->AmPm == "AM" && adjustedSelectedTime.hour() == 12)
            adjustedSelectedTime = adjustedSelectedTime.addSecs(-12 * 3600);
        timeText = adjustedSelectedTime.toString("hh:mm:ss AP");
        ui->alarmTimeLabel->setText(timeText);
    }
}

void TimerListWindow::TurnAlarmOnOff()
{
    if(ui->turnOnOffButton->text() == "Turn on again.")
    {
        timerInfo.selectedDateTime = timerInfo.selectedDateTime.addDays(1);
        qDebug() << "next day: "<<timerInfo.selectedDateTime.date();
        SetAlarmDisplay();
        AlarmOn = false;
    }
    if(!AlarmOn)
    {
        QDateTime currentDateTime = timerInfo.timer->changeTime();
        QDateTime adjustedSelectedDateTime = timerInfo.selectedDateTime;
        if (timerInfo.timer->TimeFormat != "HH:mm:ss")
        {
            if (timerInfo.timer->AmPm == "PM" && adjustedSelectedDateTime.time().hour() != 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(12 * 3600);
            else if (timerInfo.timer->AmPm == "AM" && adjustedSelectedDateTime.time().hour() == 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(-12 * 3600);
        }

        int secondsToPlay = currentDateTime.secsTo(adjustedSelectedDateTime);

        if (secondsToPlay > 0)
            TimerStart();
        else
        {
            timerInfo.selectedDateTime = timerInfo.selectedDateTime.addDays(1);
            qDebug() << "next day: "<<timerInfo.selectedDateTime.date();
            SetAlarmDisplay();
        }
        ui->turnOnOffButton->setText("Turn Off");
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : green; }");
    }
    else
    {
        timerInfo.timer->stop();
        ui->turnOnOffButton->setText("Turn On");
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : orange; }");
    }
    AlarmOn = !AlarmOn;
}

//-------------GENERAL----------------

void TimerListWindow::TimerStart()
{
    QDateTime currentDateTime = timerInfo.timer->changeTime();

    if (timerInfo.Ttype == TimerType::Timer)
    {
        if (remainingTime == 0)
            remainingTime = timerInfo.selectedDateTime.time().second() + timerInfo.selectedDateTime.time().minute() * 60 + timerInfo.selectedDateTime.time().hour() * 3600;
        qDebug() << "Timer mode seconds to play:" << remainingTime;
        if (remainingTime > 0)
        {
            countDown = new CountDown(this, QTime(0, 0).addSecs(remainingTime));
            countDown->setDisplay(ui->countdownLabel);
            if(!timerPaused)
            {
                countDown->start();
                timerInfo.timer->start(remainingTime * 1000);
            }
        }
        else
            timerInfo.timer->stop();
    }
    else if (timerInfo.Ttype == TimerType::Alarm)
    {
        QDateTime adjustedSelectedDateTime = timerInfo.selectedDateTime;

        // Handle 12-hour format adjustments
        if (timerInfo.timer->TimeFormat != "HH:mm:ss")
        {
            if (timerInfo.timer->AmPm == "PM" && adjustedSelectedDateTime.time().hour() != 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(12 * 3600);
            else if (timerInfo.timer->AmPm == "AM" && adjustedSelectedDateTime.time().hour() == 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(-12 * 3600);
        }

        // Calculate the seconds until the alarm
        int secondsToPlay = currentDateTime.secsTo(adjustedSelectedDateTime) + 1;

        // Add a debug statement to track the calculated seconds
        qDebug() << "Calculated seconds to alarm:" << secondsToPlay;

        if (secondsToPlay > 0)
            timerInfo.timer->start(secondsToPlay * 1000);
        else
        {
            timerInfo.selectedDateTime = timerInfo.selectedDateTime.addDays(1);
            qDebug() << "next day: "<<timerInfo.selectedDateTime.date();
            SetAlarmDisplay();
        }
    }
    updateTimeoutConnections();
}

void TimerListWindow::updateTimeoutConnections()
{
    disconnect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::PlaySoundtrack);
    disconnect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::ShowImage);
    disconnect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::openDocument);
    disconnect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::openApplication);
    if (timerInfo.soundName!="")
        connect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::PlaySoundtrack);
    if (timerInfo.imageName!="")
        connect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::ShowImage);
    if (timerInfo.documentName!="")
        connect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::openDocument);
    if (timerInfo.appName!="")
        connect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::openApplication);
    connect(timerInfo.timer, &Timer::timeout, this, &TimerListWindow::generalTimeout);
}

void TimerListWindow::openInfoWindow()
{
    info = new TimerInfoWindow(nullptr, timerInfo, settings);
    info->setWindowTitle("Timer Info Page");
    info->show();
    connect(info, &TimerInfoWindow::timerInfoUpdated, this, &TimerListWindow::updateTimerInfo);
}

void TimerListWindow::generalTimeout()
{
    if(timerInfo.Ttype == TimerType::Timer)
    {
        timerPaused = true;
        ui->stopButton->hide();
        ui->resetButton->setText("Restart Timer");
    }
    else
    {
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : red; }");
        ui->turnOnOffButton->setText("Turn on again.");

    }
}

void TimerListWindow::updateTimerInfo(TimerInfo newTimerInfo)
{
    qDebug() << "got info";
    timerInfo.soundName = newTimerInfo.soundName;
    timerInfo.imageName = newTimerInfo.imageName;
    timerInfo.appName = newTimerInfo.appName;
    timerInfo.documentName = newTimerInfo.documentName;
    timerInfo.selectedDateTime = newTimerInfo.selectedDateTime;
    timerInfo.Title= newTimerInfo.Title;
    timerInfo.timer= newTimerInfo.timer;
    if (timerInfo.Ttype == TimerType::Timer)
    {
        ui->timerName->setText(timerInfo.Title);
        remainingTime=0;
        if(countDown)
        {   countDown->stop();
            delete countDown;
            countDown = nullptr; }
    }
    else
        ui->alarmName->setText(timerInfo.Title);
    SetAlarmDisplay();
    TimerStart();
    updateTimeoutConnections();
}

void TimerListWindow::PlaySoundtrack()
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    if (timerInfo.soundName.startsWith("qrc:/"))
        player->setSource(QUrl(timerInfo.soundName));
    else
        player->setSource(QUrl::fromLocalFile(timerInfo.soundName));
    audioOutput->setVolume(50);
    player->play();
    qDebug() << "Soundtrack is playing.";
    qDebug() << "Playing audio from:" << timerInfo.soundName;
    timerInfo.timer->stop();
}

void TimerListWindow::ShowImage()
{
    img = new imageWindow(this, timerInfo.imageName);
    img->setWindowTitle("Image");
    QIcon icon(timerInfo.imageName);
    img->setWindowIcon(icon);
    img->show();
    qDebug() << "Photo is shown:" << timerInfo.imageName;
    timerInfo.timer->stop();
}


void TimerListWindow::openApplication()
{
    QFileInfo appInfo(timerInfo.appName);
    qDebug() << "Checking application path:" << timerInfo.appName;
    if (!appInfo.exists() || !appInfo.isExecutable()) {
        QMessageBox::warning(this, "Error", "The selected application cannot be found or is not executable.");
        qDebug() << "Application not found or not executable:" << timerInfo.appName;
        timerInfo.timer->stop();
        return;
    }
    QProcess::startDetached(timerInfo.appName);
    qDebug() << "Opening application:" << timerInfo.appName;
    timerInfo.timer->stop();
}

void TimerListWindow::openDocument()
{

    qDebug() << "Opening document from path:" << timerInfo.documentName;
    if (QDesktopServices::openUrl(QUrl::fromLocalFile(timerInfo.documentName)))
        qDebug() << "Document opened successfully.";
    else
        qDebug() << "Failed to open document.";
    timerInfo.timer->stop();
}
