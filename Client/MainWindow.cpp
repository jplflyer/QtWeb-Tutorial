#include <QLayout>
#include <QWidget>
#include <QTimer>

#include "MainWindow.h"
#include "ui_MainWindow.h"

/**
 * Constructor.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralForm = ui->welcomeLabel;
    QTimer::singleShot(3000, this, &MainWindow::switchToLoginWindow);
}

/**
 * Destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Switch to contain this form. This is a little magic that makes this work
 * as a Single Page App.
 */
void MainWindow::switchForm(QWidget *widget) {
    if (centralForm != widget) {
        if (centralForm != nullptr) {
            centralForm->hide();
            centralForm = nullptr;
        }
        if (widget != nullptr) {
            centralForm = widget;
            centralForm->show();
            ui->centralwidget->layout()->addWidget(centralForm);
        }
    }
}

/**
 * Set the status bar text for 5 seconds.
 */
void
MainWindow::setStatus(const QString &str) {
    ui->statusbar->showMessage(str, 5000);
}

/**
 * Switch to the Login window, creating it if we don't have
 * one already. Currently this gets called from the 3-second timer
 * created when we're created.
 */
void
MainWindow::switchToLoginWindow() {
    if (loginForm == nullptr) {
        loginForm = new LoginForm();

        QObject::connect(loginForm, &LoginForm::loginSuccess,
                         this, &MainWindow::loginSuccess);
    }
    switchForm(loginForm);
}

void
MainWindow::switchToEventsWindow() {
    if (eventsForm == nullptr) {
        eventsForm = new ListEventsForm();

        QObject::connect(eventsForm, SIGNAL(setStatus(const QString &)),
                         this, SLOT(setStatus(const QString &)) );
        QObject::connect(eventsForm, &ListEventsForm::createNewEvent,
                         this, &MainWindow::createNewEvent);
    }
    switchForm(eventsForm);
}

/**
 * We have login success.
 */
void
MainWindow::loginSuccess() {
    switchToEventsWindow();
}

/**
 * Display the Create New Event window.
 */
void
MainWindow::createNewEvent() {
    if (newEventForm == nullptr) {
        newEventForm = new NewEventForm();
    }
    newEventForm->reset();
    switchForm(newEventForm);
}
