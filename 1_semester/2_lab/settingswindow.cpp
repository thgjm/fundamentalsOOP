#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QDesktopServices>
#include "mainwindow.h"



SettingsWindow::SettingsWindow(QTimeZone timezone, QString TimeFormat, QWidget *parent, MainWindow *mainWindow)
    : QDialog(parent), timeZone(timezone), TimeFormat(TimeFormat), selectedTime(0, 0, 0), Ttype(TimerType::Timer)
    , ui(new Ui::SettingsWindow), mainWindow(mainWindow), Tlists(mainWindow->Tlists)
{
    ui->setupUi(this);

    CheckTimeFormat();

    timer = new Timer(this);
    timer->setTimeZone(&timeZone);

    QDate todayDate = timer->changeTime().date();

    ui->calendarWidget->setMinimumDate(todayDate);

    qDebug() << todayDate.day();

    connect(ui->hourCount, &QSpinBox::valueChanged, this, &SettingsWindow::updateTimeLabel);
    connect(ui->minuteCount, &QSpinBox::valueChanged, this, &SettingsWindow::updateTimeLabel);
    connect(ui->secondCount, &QSpinBox::valueChanged, this, &SettingsWindow::updateTimeLabel);

    updateTimeLabel();

    connect(ui->timerRadioButton, &QRadioButton::toggled, this, &SettingsWindow::CheckTimeFormat);
    connect(ui->alarmRadioButton, &QRadioButton::toggled, this, &SettingsWindow::CheckTimeFormat);
    connect(ui->timerRadioButton, &QRadioButton::toggled, this, &SettingsWindow::updateTimeLabel);
    connect(ui->alarmRadioButton, &QRadioButton::toggled, this, &SettingsWindow::updateTimeLabel);

    ui->startButton->setFixedHeight(50);

    connect(ui->startButton, &QPushButton::clicked, this, &SettingsWindow::openTimerLists);

    setLists();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::openTimerLists()
{
    if (!Tlists) {
        Tlists = new TimerListWindow(this, this); // Create only once
        if (!Tlists) {
            qDebug() << "Failed to allocate memory for TimerListWindow.";
            return;
        }
        Tlists->setWindowTitle("Timers List");
        emit timerListWindowCreated(Tlists);
    }

    QString soundName="", imageName="", appName="", documentName="", title="";
    if(ui->soundCheckBox->isChecked() && ui->soundComboBox->currentIndex()!=-1)
        soundName=ui->soundComboBox->currentData().toString();

    if(ui->imageCheckBox->isChecked() && ui->imageComboBox->currentIndex()!=-1)
        imageName=ui->imageComboBox->currentData().toString();

    if(ui->appCheckBox->isChecked() && ui->appComboBox->currentIndex()!=-1)
        appName=ui->appComboBox->currentData().toString();

    if(ui->documentCheckBox->isChecked() && ui->documentComboBox->currentIndex()!=-1)
        documentName=ui->documentComboBox->currentData().toString();

    timer->setSpinBoxes(ui->hourCount, ui->minuteCount, ui->secondCount);
    selectedTime = timer->getInitialTime();
    QDateTime selectedDateTime = QDateTime(ui->calendarWidget->selectedDate(), selectedTime, timeZone);

    if(ui->timerRadioButton->isChecked()) Ttype=TimerType::Timer;
    else if(ui->alarmRadioButton->isChecked()) Ttype=TimerType::Alarm;

    timer->TimeFormat = TimeFormat;
    timer->AmPm = ui->AmPmBox->currentText();
    if(ui->timerName->text()!="")
        title = ui->timerName->text();
    else if(Ttype == TimerType::Timer)
        title="Timer";
    else
        title="Alarm";

    timer->setTimeZone(&timeZone);

    QDateTime currentDateTime = timer->changeTime();

    qDebug()<<"currentDateTime: "<<currentDateTime.time().hour()<<" "<<currentDateTime.time().minute()<<" "<<currentDateTime.time().second();


    if (Ttype == TimerType::Alarm)
    {
        QDateTime adjustedSelectedDateTime = selectedDateTime;

        qDebug()<<"selectedDateTime: "<<selectedDateTime.time().hour()<<" "<<selectedDateTime.time().minute()<<" "<<selectedDateTime.time().second();

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

        if (secondsToPlay <= 0)
        {
            QMessageBox::about(this, "Error.", "The selected time has already passed. Please choose another one.");
            return;
        }
    }

    Timer *newTimer = timer;

    TimerInfo newTimerInfo(soundName, imageName, appName, documentName, title, selectedDateTime, newTimer, Ttype);

    /*TimerWidget *timerWidget = new TimerWidget(this, timerInfo, this);

    if (!timerWidget) {
        qDebug() << "Failed to create TimerWidget.";
        return;
    }*/

    emit timerCreated(newTimerInfo);
    Tlists->show();
    this->close();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
    ui->timerRadioButton->setChecked(true);
    ui->alarmRadioButton->setChecked(false);

    ui->imageCheckBox->setChecked(false);
    ui->soundCheckBox->setChecked(false);
    ui->appCheckBox->setChecked(false);
    ui->documentCheckBox->setChecked(false);

    ui->soundComboBox->setCurrentIndex(-1);
    ui->imageComboBox->setCurrentIndex(-1);
    ui->appComboBox->setCurrentIndex(-1);
    ui->documentComboBox->setCurrentIndex(-1);

    ui->hourCount->setValue(0);
    ui->minuteCount->setValue(0);
    ui->secondCount->setValue(0);

    ui->calendarWidget->setSelectedDate(timer->changeTime().date());

    ui->timerName->setText("");

    event->accept();
}

void SettingsWindow::updateTimeLabel()
{
    int hours = ui->hourCount->value();
    int minutes = ui->minuteCount->value();
    int seconds = ui->secondCount->value();

    QString timeString;

    connect(ui->AmPmBox, &QComboBox::currentTextChanged, this, &SettingsWindow::updateTimeLabel);

    timeString = QString::number(hours).rightJustified(2, '0') + ":" +
                 QString::number(minutes).rightJustified(2, '0') + ":" +
                 QString::number(seconds).rightJustified(2, '0');

    if(!CheckTimeFormat() && ui->alarmRadioButton->isChecked())
    {
        timeString = timeString + " " + ui->AmPmBox->currentText();
    }

    ui->TimeLabel->setStyleSheet("QLabel { color : orange; }");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Orbitron/static/Orbitron-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        ui->TimeLabel->setFont(QFont(family, 45));
    } else {
        qDebug() << "The font was not uploaded.";
    }

    ui->TimeLabel->setText(timeString);
}


bool SettingsWindow::CheckTimeFormat()
{
    if (ui->timerRadioButton->isChecked()) {
        ui->hourCount->setMaximum(23);
        ui->hourCount->setMinimum(0);
        ui->AmPmBox->hide();
        ui->calendarWidget->hide();
        return true;
    }
    else if (ui->alarmRadioButton->isChecked())
    {
        ui->calendarWidget->show();
        if (TimeFormat == "hh:mm:ss AP") {
            ui->hourCount->setMaximum(12);
            ui->hourCount->setMinimum(1);
            ui->AmPmBox->show();
            return false;
        } else if (TimeFormat == "HH:mm:ss") {
            ui->hourCount->setMaximum(23);
            ui->hourCount->setMinimum(0);
            ui->AmPmBox->hide();
            return true;
        }
    }
    return false;
}


void SettingsWindow::uploadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString imageName = QFileInfo(filePath).fileName();

    QIcon icon(filePath);
    ui->imageComboBox->addItem(icon, imageName);
    ui->imageComboBox->setItemData(ui->imageComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Image Uploaded"), tr("You have successfully uploaded the image: %1").arg(imageName));
}

void SettingsWindow::uploadSound()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Sound File"), QString(), tr("Audio Files (*.mp3 *.wav *.ogg)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString soundName = QFileInfo(filePath).fileName();

    ui->soundComboBox->addItem(soundName);
    ui->soundComboBox->setItemData(ui->soundComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Audio File Uploaded"), tr("You have successfully uploaded the audio file: %1").arg(soundName));
}

void SettingsWindow::uploadApp()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Application"), "", tr("Executable Files (*.exe)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString appName = QFileInfo(filePath).fileName();
    QIcon appIcon(filePath);

    ui->appComboBox->addItem(appIcon, appName);
    ui->appComboBox->setItemData(ui->appComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Application Uploaded"), tr("You have successfully uploaded the application: %1").arg(appName));
}


void SettingsWindow::uploadDocument()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Document"), "", tr("Documents (*.pdf *.txt *.docx *.xlsx)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString docName = QFileInfo(filePath).fileName();
    ui->documentComboBox->addItem(docName);
    ui->documentComboBox->setItemData(ui->documentComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Document Uploaded"), tr("You have successfully uploaded the document: %1").arg(docName));
}



void SettingsWindow::setLists()
{
    //images
    QStringList imageNames = { "Image 1", "Image 2", "Image 3", "Image 4" };
    QStringList imagePaths = { ":/images/resources/images/img1.jpeg",
                              ":/images/resources/images/img2.jpg",
                              ":/images/resources/images/img3.jpg",
                              ":/images/resources/images/img4.jpg" };

    for (int i = 0; i < imageNames.size(); ++i) {
        QIcon icon(imagePaths[i]);
        ui->imageComboBox->addItem(icon, imageNames[i]);
        ui->imageComboBox->setItemData(ui->imageComboBox->count() - 1, imagePaths[i]);
    }

    connect(ui->uploadImageButton, &QPushButton::clicked, this, &SettingsWindow::uploadImage);

    //audio files
    QStringList soundNames = { "Ringtone 1", "Ringtone 2", "Ringtone 3", "Ringtone 4" };
    QStringList soundPaths = { "qrc:/sounds/resources/sounds/Radar.mp3",
                              "qrc:/sounds/resources/sounds/morning_flower.mp3",
                              "qrc:/sounds/resources/sounds/digital-alarm-clock-151920.mp3",
                              "qrc:/sounds/resources/sounds/sweet_wake_up.mp3" };

    for (int i = 0; i < soundNames.size(); ++i) {
        ui->soundComboBox->addItem(soundNames[i]);
        ui->soundComboBox->setItemData(ui->soundComboBox->count() - 1, soundPaths[i]);
    }

    connect(ui->uploadSoundButton, &QPushButton::clicked, this, &SettingsWindow::uploadSound);

    //apps
    QStringList appNames = { "Notepad", "Calculator"};
    QStringList appPaths = { "C:/Windows/notepad.exe",
                            "C:/Windows/System32/calc.exe" };

    for (int i = 0; i < appNames.size(); ++i) {
        ui->appComboBox->addItem(appNames[i]);
        ui->appComboBox->setItemData(ui->appComboBox->count() - 1, appPaths[i]);
    }

    connect(ui->uploadAppButton, &QPushButton::clicked, this, &SettingsWindow::uploadApp);

    //documents
    connect(ui->uploadDocumentButton, &QPushButton::clicked, this, &SettingsWindow::uploadDocument);
}

void SettingsWindow::setTimeZone(const QTimeZone &zone)
{
    timeZone = zone;
    timer->setTimeZone(&timeZone);
    QDate todayDate = timer->changeTime().date();
    ui->calendarWidget->setMinimumDate(todayDate);
}

void SettingsWindow::setTimeFormat(const QString &format)
{
    TimeFormat = format;
}

void SettingsWindow::setupUI()
{

}
