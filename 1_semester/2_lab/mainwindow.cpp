#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString userTimeZoneId = FillTimeZones();
    qDebug() << "The user is in this timezone: " << userTimeZoneId;

    timeZone = QTimeZone(userTimeZoneId.toUtf8());

    TimeFormat = "HH:mm:ss";

    clock = new DigitalClock(this);
    clock->setDisplay(ui->labelTime);

    sett = new SettingsWindow(new QTimeZone(timeZone), TimeFormat, this);

    Clock(timeZone);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(openSettingsWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSettingsWindow()
{
    sett->setModal(true);
    sett->setWindowTitle("Settings");
    sett->show();
    sett->exec();
}

void MainWindow::Clock(const QTimeZone &TimeZone)
{
    clock->setTimeZone(new QTimeZone(TimeZone));
    clock->start();
}


QString MainWindow::FillTimeZones()
{
    QList<QByteArray> timeZoneIdsByteArray = QTimeZone::availableTimeZoneIds();

    QStringList timeZoneIds;
    for (const QByteArray &id : timeZoneIdsByteArray) {
        timeZoneIds.append(QString::fromUtf8(id));
    }

    for (const QString &id : timeZoneIds) {
        ui->comboBox->addItem(id);
    }

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onTimeZoneChanged);

    QTimeZone userTimeZone = QTimeZone::systemTimeZone();
    QString userTimeZoneId = userTimeZone.id();

    return userTimeZoneId;
}


void MainWindow::onTimeZoneChanged(int index)
{
    QString selectedTimeZoneId = ui->comboBox->currentText();
    qDebug() << "Selected timezone:" << selectedTimeZoneId;

    timeZone = QTimeZone(selectedTimeZoneId.toUtf8());

    QDateTime currentTime = QDateTime::currentDateTime().toTimeZone(timeZone);
    QTime currenttTime = currentTime.time();
    qDebug() << "Current time in selected timezone: " << currenttTime.toString("HH:mm:ss");
    sett = new SettingsWindow(new QTimeZone(timeZone), TimeFormat, this);
    Clock(timeZone);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if (index == 0)
        TimeFormat = "HH:mm:ss";
    else
        TimeFormat = "hh:mm:ss AP";
    clock->changeFormat(TimeFormat);
    sett = new SettingsWindow(new QTimeZone(timeZone), TimeFormat, this);
}
