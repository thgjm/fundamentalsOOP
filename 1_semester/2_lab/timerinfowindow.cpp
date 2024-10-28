#include "timerinfowindow.h"
#include "ui_timerinfowindow.h"
#include "settingswindow.h"
#include "ui_settingswindow.h"

TimerInfoWindow::TimerInfoWindow(QWidget *parent, QDateTime selectedDateTime, QString soundName, QString imageName, QString appName, QString documentName, QString Title, QString TimeFormat, SettingsWindow* settings, TimerType Ttype, bool AmPm)
    : QDialog(parent)
    , ui(new Ui::TimerInfoWindow)
{
    ui->setupUi(this);

    if(Ttype == TimerType::Timer)
    {
        ui->hourCount->setMaximum(23);
        ui->hourCount->setMinimum(0);
        ui->AmPmBox->hide();
    }

    if(Ttype == TimerType::Alarm)
    {
        if(TimeFormat == "HH:mm:ss")
        {
            ui->hourCount->setMaximum(23);
            ui->hourCount->setMinimum(0);
            ui->AmPmBox->hide();
        }
        else
        {
            ui->hourCount->setMaximum(12);
            ui->hourCount->setMinimum(1);
            ui->AmPmBox->show();
        }
    }

    if (settings)
    {
        ui->hourCount->setValue(selectedDateTime.time().hour());
        ui->minuteCount->setValue(selectedDateTime.time().minute());
        ui->secondCount->setValue(selectedDateTime.time().second());
        copyComboBoxItems(settings->ui->soundComboBox, ui->soundComboBox);
        copyComboBoxItems(settings->ui->imageComboBox, ui->imageComboBox);
        copyComboBoxItems(settings->ui->appComboBox, ui->appComboBox);
        copyComboBoxItems(settings->ui->documentComboBox, ui->documentComboBox);

        if(soundName!="")
        {
            ui->soundCheckBox->setChecked(true);
            int index = -1;
            for (int i = 0; i < ui->soundComboBox->count(); ++i)
                if (ui->soundComboBox->itemData(i).toString() == soundName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->soundComboBox->setCurrentIndex(index);
        }

        if(imageName!="")
        {
            ui->imageCheckBox->setChecked(true);

            int index = -1;
            for (int i = 0; i < ui->imageComboBox->count(); ++i)
                if (ui->imageComboBox->itemData(i).toString() == imageName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->imageComboBox->setCurrentIndex(index);
        }

        if(appName!="")
        {
            ui->appCheckBox->setChecked(true);
            int index = -1;
            for (int i = 0; i < ui->appComboBox->count(); ++i)
                if (ui->appComboBox->itemData(i).toString() == appName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->appComboBox->setCurrentIndex(index);
        }

        if(documentName!="")
        {
            ui->documentCheckBox->setChecked(true);
            int index = -1;
            for (int i = 0; i < ui->documentComboBox->count(); ++i)
                if (ui->documentComboBox->itemData(i).toString() == documentName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->documentComboBox->setCurrentIndex(index);
        }

        connect(ui->uploadImageButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadImage);
        connect(ui->uploadSoundButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadSound);
        connect(ui->uploadAppButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadApp);
        connect(ui->uploadDocumentButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadDocument);

        ui->timerName->setText(Title);
    }
}

TimerInfoWindow::~TimerInfoWindow()
{
    delete ui;
}

void TimerInfoWindow::copyComboBoxItems(QComboBox* source, QComboBox* destination)
{
    destination->clear();
    for (int i = 0; i < source->count(); ++i)
    {
        destination->addItem(source->itemIcon(i), source->itemText(i));
        destination->setItemData(i, source->itemData(i));
    }
}

void TimerInfoWindow::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        if (ui->soundCheckBox->isChecked())
            soundName = ui->soundComboBox->currentData().toString();
        else
            soundName="";
        if (ui->imageCheckBox->isChecked())
            imageName = ui->imageComboBox->currentData().toString();
        else imageName="";
        if (ui->appCheckBox->isChecked())
            appName = ui->appComboBox->currentData().toString();
        else appName="";
        if (ui->documentCheckBox->isChecked())
            documentName = ui->documentComboBox->currentData().toString();
        else documentName="";
        Title = ui->timerName->text();
        int hours = ui->hourCount ? ui->hourCount->value() : 0;
        int minutes = ui->minuteCount ? ui->minuteCount->value() : 0;
        int seconds = ui->secondCount ? ui->secondCount->value() : 0;
        if(ui->AmPmBox->currentText() == "AM") AmPm=true;
        else AmPm=false;
        selectedDateTime.setTime(QTime(hours, minutes, seconds));
        emit timerInfoUpdated(selectedDateTime, soundName, imageName, appName, documentName, Title, AmPm);
        accept();
    }
    else if (ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
        reject();
}

void TimerInfoWindow::uploadImage()
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

void TimerInfoWindow::uploadSound()
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

void TimerInfoWindow::uploadApp()
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


void TimerInfoWindow::uploadDocument()
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
