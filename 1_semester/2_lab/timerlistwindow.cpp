#include "timerlistwindow.h"
#include "ui_timerlistwindow.h"
#include "settingswindow.h"

TimerListWindow::TimerListWindow(QDialog *parent, Timer* timer, QString soundName, QString imageName, QString appName, QString documentName, QString Title, TimerType Ttype, QDateTime selectedDateTime, SettingsWindow* settings)
    : QDialog(parent)
    , ui(new Ui::TimerListWindow), timer(timer), soundName(soundName), imageName(imageName), appName(appName), documentName(documentName), Title(Title), Ttype(Ttype), player(), selectedDateTime(selectedDateTime), settings(settings)
{
    ui->setupUi(this);

    SetAlarmDisplay();

    qDebug()<<soundName;
    qDebug()<<imageName;
    qDebug()<<appName;
    qDebug()<<documentName;

    qDebug()<<Title;
    if(Ttype == TimerType::Timer)
    {
        qDebug()<<"Timer";
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(Ttype == TimerType::Alarm)
    {
        qDebug()<<"Alarm";
        ui->stackedWidget->setCurrentIndex(1);
    }

    ui->horizontalLayout_2->setAlignment(Qt::AlignCenter);

    qDebug() << selectedDateTime.time().hour()<<":"<<selectedDateTime.time().minute()<<":"<<selectedDateTime.time().second();

    connect(ui->restartButton, &QPushButton::clicked, this, &TimerListWindow::restartTimer);
    connect(ui->stopButton, &QPushButton::clicked, this, &TimerListWindow::PauseResume);

    TimerStart();

    ui->timerName->setText(Title);

    connect(ui->infoButton, &QPushButton::clicked, this, &TimerListWindow::openInfoWindow);

    connect(ui->turnOnOffButton, &QPushButton::clicked, this, &TimerListWindow::TurnAlarmOnOff);

    connect(ui->alarmInfo, &QPushButton::clicked, this, &TimerListWindow::openInfoWindow);
}

TimerListWindow::~TimerListWindow()
{
    if (timer)
        timer->stop();

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
        remainingTime = timer->getRemainingTime();
        timer->stop();
        countDown->stop();
        ui->stopButton->setText("Resume");
    }
}

void TimerListWindow::restartTimer()
{
    if(timerPaused)
    {
        countDown = new CountDown(this, QTime(0, 0).addSecs(selectedDateTime.time().second() + selectedDateTime.time().minute() * 60 + selectedDateTime.time().hour() * 3600));
        countDown->setDisplay(ui->countdownLabel);
        countDown->start();
        countDown->stop();
    }
    if (countDown)
    {
        countDown->stop();
        delete countDown;
        countDown = nullptr;
    }
    remainingTime = 0;
    ui->stopButton->setText("Start");
    TimerStart();
}


//-------------ALARM----------------

void TimerListWindow::SetAlarmDisplay()
{
    ui->alarmTimeLabel->setStyleSheet("QLabel { color : green; }");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Orbitron/static/Orbitron-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        ui->alarmTimeLabel->setFont(QFont(family, 40));
    } else {
        qDebug() << "The font was not uploaded.";
    }

    QString timeText;

    if(timer->TimeFormat == "HH:mm:ss")
    {
        timeText = selectedDateTime.time().toString("HH:mm:ss");
        ui->alarmTimeLabel->setText(timeText);
    }
    else
    {
        QTime adjustedSelectedTime = selectedDateTime.time();
        if (timer->AmPm == "PM" && adjustedSelectedTime.hour() != 12)
            adjustedSelectedTime = adjustedSelectedTime.addSecs(12 * 3600);
        else if (timer->AmPm == "AM" && adjustedSelectedTime.hour() == 12)
            adjustedSelectedTime = adjustedSelectedTime.addSecs(-12 * 3600);
        timeText = adjustedSelectedTime.toString("hh:mm:ss AP");
        ui->alarmTimeLabel->setText(timeText);
    }
}

void TimerListWindow::TurnAlarmOnOff()
{
    if(AlarmOn)
    {
        timer->stop();
        ui->turnOnOffButton->setText("Turn On");
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : orange; }");
    }
    else
    {
        TimerStart();
        ui->turnOnOffButton->setText("Turn Off");
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : green; }");
    }
    AlarmOn = !AlarmOn;
}

//-------------GENERAL----------------

void TimerListWindow::TimerStart()
{
    QDateTime currentDateTime = timer->changeTime();

    if (Ttype == TimerType::Timer)
    {
        if (remainingTime == 0)
            remainingTime = selectedDateTime.time().second() + selectedDateTime.time().minute() * 60 + selectedDateTime.time().hour() * 3600;
        qDebug() << "Timer mode seconds to play:" << remainingTime;
        if (remainingTime > 0)
        {
            countDown = new CountDown(this, QTime(0, 0).addSecs(remainingTime));
            countDown->setDisplay(ui->countdownLabel);
            if(!timerPaused)
            {
                countDown->start();
                timer->start(remainingTime * 1000);
            }
        }
        else
            timer->stop();
    }
    else if (Ttype == TimerType::Alarm)
    {
        QDateTime adjustedSelectedDateTime = selectedDateTime;

        // Handle 12-hour format adjustments
        if (timer->TimeFormat != "HH:mm:ss")
        {
            if (timer->AmPm == "PM" && adjustedSelectedDateTime.time().hour() != 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(12 * 3600);
            else if (timer->AmPm == "AM" && adjustedSelectedDateTime.time().hour() == 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(-12 * 3600);
        }

        int secondsToPlay = currentDateTime.secsTo(adjustedSelectedDateTime);

        qDebug() << "Alarm mode seconds to play:" << secondsToPlay;

        if (secondsToPlay > 0)
            timer->start(secondsToPlay * 1000);
        else
            timer->stop();
    }
    updateTimeoutConnections();
}


void TimerListWindow::PlaySoundtrack()
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    if (soundName.startsWith("qrc:/"))
        player->setSource(QUrl(soundName));
    else
        player->setSource(QUrl::fromLocalFile(soundName));
    audioOutput->setVolume(50);
    player->play();
    qDebug() << "Soundtrack is playing.";
    qDebug() << "Playing audio from:" << soundName;
    timer->stop();
}

void TimerListWindow::ShowImage()
{
    img = new imageWindow(this, imageName);
    img->setWindowTitle("Image");
    QIcon icon(imageName);
    img->setWindowIcon(icon);
    img->show();
    qDebug() << "Photo is shown:" << imageName;
    timer->stop();
}

void TimerListWindow::updateTimeoutConnections()
{
    disconnect(timer, &Timer::timeout, this, &TimerListWindow::PlaySoundtrack);
    disconnect(timer, &Timer::timeout, this, &TimerListWindow::ShowImage);
    disconnect(timer, &Timer::timeout, this, &TimerListWindow::openDocument);
    disconnect(timer, &Timer::timeout, this, &TimerListWindow::openApplication);
    if (soundName!="") {
        connect(timer, &Timer::timeout, this, &TimerListWindow::PlaySoundtrack);
    }
    if (imageName!="") {
        connect(timer, &Timer::timeout, this, &TimerListWindow::ShowImage);
    }
    if (documentName!="") {
        connect(timer, &Timer::timeout, this, &TimerListWindow::openDocument);
    }
    if (appName!="") {
        connect(timer, &Timer::timeout, this, &TimerListWindow::openApplication);
    }
}

void TimerListWindow::openApplication()
{
    QFileInfo appInfo(appName);
    qDebug() << "Checking application path:" << appName;
    if (!appInfo.exists() || !appInfo.isExecutable()) {
        QMessageBox::warning(this, "Error", "The selected application cannot be found or is not executable.");
        qDebug() << "Application not found or not executable:" << appName;
        timer->stop();
        return;
    }
    QProcess::startDetached(appName);
    qDebug() << "Opening application:" << appName;
    timer->stop();
}

void TimerListWindow::openDocument()
{

    qDebug() << "Opening document from path:" << documentName;
    if (QDesktopServices::openUrl(QUrl::fromLocalFile(documentName)))
        qDebug() << "Document opened successfully.";
    else
        qDebug() << "Failed to open document.";
    timer->stop();
}

void TimerListWindow::openInfoWindow()
{
    bool AmPm;
    if(timer->AmPm == "AM") AmPm = true;
    else AmPm = false;
    info = new TimerInfoWindow(this, selectedDateTime, soundName, imageName, appName, documentName, Title, timer->TimeFormat, settings, Ttype, AmPm);
    info->setWindowTitle("Timer Info Page");
    info->setModal(true);
    info->show();
    connect(info, &TimerInfoWindow::timerInfoUpdated, this, &TimerListWindow::updateTimerInfo);
}

void TimerListWindow::updateTimerInfo(QDateTime selectedDateTime, QString soundName, QString imageName, QString appName, QString documentName, QString Title, bool AmPm)
{
    this->soundName = soundName;
    this->imageName = imageName;
    this->appName = appName;
    this->documentName = documentName;
    this->selectedDateTime = selectedDateTime;
    this->Title=Title;
    if(AmPm) timer->AmPm = "AM";
    else timer->AmPm = "PM";
    if (Ttype == TimerType::Timer)
    {
        ui->timerName->setText(Title);
        remainingTime=0;
        if(countDown)
        {   countDown->stop();
            delete countDown;
            countDown = nullptr; }
    }
    else
    {
        ui->alarmName->setText(Title);
    }
    SetAlarmDisplay();
    TimerStart();
    updateTimeoutConnections();
}
