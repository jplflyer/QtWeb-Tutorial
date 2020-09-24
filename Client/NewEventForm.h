#pragma once

#include <QWidget>

#include "BaseForm.h"
#include "Network.h"

namespace Ui {
class NewEventForm;
}

class NewEventForm : public BaseForm
{
    Q_OBJECT

public:
    explicit NewEventForm(QWidget *parent = nullptr);
    ~NewEventForm();

    void reset();

public slots:
    void gotEventTemplates(EventTemplate::Vector &);
    void saveEventComplete();

private slots:
    void on_cancelBtn_clicked();
    void on_saveBtn_clicked();

private:
    Ui::NewEventForm *ui;

    Network network;
    EventTemplate::Vector * allEventTemplates = nullptr;
};

