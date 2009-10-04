#ifndef SELECTLOCATION_H
#define SELECTLOCATION_H

#include <QtGui/QWidget>
#include "ui_SelectLocation.h"

class SelectLocation : public QWidget
{
    Q_OBJECT

public:
    SelectLocation(QWidget *parent = 0);
    ~SelectLocation();

private:
    Ui::SelectLocationClass ui;
};

#endif // SELECTLOCATION_H
