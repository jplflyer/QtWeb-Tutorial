#include "LoginForm.h"
#include "ui_LoginForm.h"

/**
 * Constructor.
 */
LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

/**
 * Destructor.
 */
LoginForm::~LoginForm()
{
    delete ui;
}

/**
 * They clicked Login.
 */
void LoginForm::on_loginBtn_clicked()
{
    emit loginSuccess();
}
