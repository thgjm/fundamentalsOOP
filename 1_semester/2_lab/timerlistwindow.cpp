#include "timerlistwindow.h"
#include "settingswindow.h"

TimerListWindow::TimerListWindow(QWidget *parent, SettingsWindow *settings)
    : QDialog(parent), settings(settings)
{
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint |
                   Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    setWindowModality(Qt::NonModal);

    mainLayout = new QVBoxLayout(this);

    timerSection = new QWidget(this);
    alarmSection = new QWidget(this);

    timerLayout = new QVBoxLayout(timerSection);
    timerSection->setLayout(timerLayout);

    alarmLayout = new QVBoxLayout(alarmSection);
    alarmSection->setLayout(alarmLayout);

    timerScrollArea = new QScrollArea(this);
    timerScrollArea->setWidget(timerSection);
    timerScrollArea->setWidgetResizable(true);

    alarmScrollArea = new QScrollArea(this);
    alarmScrollArea->setWidget(alarmSection);
    alarmScrollArea->setWidgetResizable(true);

    mainLayout->addWidget(timerScrollArea);
    mainLayout->addWidget(alarmScrollArea);

    setLayout(mainLayout);

    connect(settings, &SettingsWindow::timerCreated, this, &TimerListWindow::addTimerWidget);

}

TimerListWindow::~TimerListWindow()
{
    qDeleteAll(timerWidgets);
}

/*void TimerListWindow::addTimerWidget(TimerWidget *timerWidget)
{
    timerWidgets.append(timerWidget);

    if (timerWidget->timerInfo.Ttype == TimerType::Timer)
    {
        timerLayout->addWidget(timerWidget);
    }
    else if (timerWidget->timerInfo.Ttype == TimerType::Alarm)
    {
        alarmLayout->addWidget(timerWidget);
    }
}*/

void TimerListWindow::addTimerWidget(const TimerInfo &timerInfo)
{
    TimerWidget *newTimerWidget = new TimerWidget(this, timerInfo, settings);
    if (newTimerWidget->timerInfo.Ttype == TimerType::Timer)
        timerLayout->addWidget(newTimerWidget);
    else if (newTimerWidget->timerInfo.Ttype == TimerType::Alarm)
        alarmLayout->addWidget(newTimerWidget);
    timerWidgets.append(newTimerWidget);

    connect(newTimerWidget, &TimerWidget::timerDeleted, this, &TimerListWindow::removeTimerWidget);

    //connect(newTimerWidget, &TimerWidget::timerFinished, this, &TimerListWindow::onTimerFinished); // Connect signals if necessary
}

void TimerListWindow::removeTimerWidget(TimerWidget *timerWidget)
{
    // Remove the widget from the layout
    if (timerWidgets.contains(timerWidget)) {
        timerWidgets.removeAll(timerWidget);

        // You should remove it from the layout as well
        if (timerWidget->timerInfo.Ttype == TimerType::Timer) {
            timerLayout->removeWidget(timerWidget);
        } else if (timerWidget->timerInfo.Ttype == TimerType::Alarm) {
            alarmLayout->removeWidget(timerWidget);
        }

        // Now, delete the widget
        timerWidget->deleteLater();
    }
}

void TimerListWindow::clearAllTimers()
{
    for (TimerWidget *timerWidget : qAsConst(timerWidgets))
    {
        timerWidget->deleteTimer();
        if (timerWidget->timerInfo.Ttype == TimerType::Timer) {
            timerLayout->removeWidget(timerWidget);
        } else if (timerWidget->timerInfo.Ttype == TimerType::Alarm) {
            alarmLayout->removeWidget(timerWidget);
        }
    }
    qDeleteAll(timerWidgets);
    timerWidgets.clear();
    qDebug() << "All timers have been cleared.";
}
