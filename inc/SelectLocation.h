#ifndef SELECTLOCATION_H
#define SELECTLOCATION_H

#include <QtGui/QWidget>
#include <QGraphicsWidget>
#include "ui_SelectLocation.h"

class SelectLocation : public QWidget
{
    Q_OBJECT

public:
    SelectLocation(QWidget *parent = 0);
    virtual ~SelectLocation();
protected:
	void paintEvent(QPaintEvent *paintevent);
public:
    Ui::selectlocation ui;
    
private:
	QLineEdit* lineEdit;
};

#endif // SELECTLOCATION_H
