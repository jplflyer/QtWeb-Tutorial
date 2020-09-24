#pragma once

#include <QWidget>

#include "Network.h"

namespace Ui {
class NewEventForm;
}

class NewEventForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewEventForm(QWidget *parent = nullptr);
    ~NewEventForm();

    void reset();

public slots:
    void gotEventTemplates(EventTemplate::Vector &);

    void on_cancelBtn_clicked();
    void on_saveTbn_clicked();

private:
    Ui::NewEventForm *ui;

    Network network;
    EventTemplate::Vector * allEventTemplates = nullptr;
};

