#pragma once

#include <QWidget>


class BaseForm: public QWidget
{
Q_OBJECT

public:
    BaseForm(QWidget *parent = nullptr);

signals:
    void setStatus(const QString &);
};

