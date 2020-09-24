#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "LoginForm.h"
#include "ListEventsForm.h"
#include "NewEventForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setStatus(const QString &);
    void switchToLoginWindow();
    void switchToEventsWindow();
    void loginSuccess();
    void createNewEvent();

private:
    Ui::MainWindow *ui;

    QWidget *centralForm = nullptr;
    LoginForm *loginForm = nullptr;
    ListEventsForm *eventsForm = nullptr;
    NewEventForm *newEventForm = nullptr;

    void setupConnections(BaseForm *);
    void switchForm(QWidget *widget);

};
#endif // MAINWINDOW_H
