/********************************************************************************
** Form generated from reading ui file 'weatherwidget2.ui'
**
** Created: Sat Oct 17 21:42:03 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WEATHERWIDGET2_H
#define UI_WEATHERWIDGET2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WeatherWidget2Class
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WeatherWidget2Class)
    {
        if (WeatherWidget2Class->objectName().isEmpty())
            WeatherWidget2Class->setObjectName(QString::fromUtf8("WeatherWidget2Class"));
        WeatherWidget2Class->resize(800, 600);
        centralwidget = new QWidget(WeatherWidget2Class);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(80, 10, 651, 431));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        WeatherWidget2Class->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WeatherWidget2Class);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        WeatherWidget2Class->setMenuBar(menubar);
        statusbar = new QStatusBar(WeatherWidget2Class);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WeatherWidget2Class->setStatusBar(statusbar);

        retranslateUi(WeatherWidget2Class);

        QMetaObject::connectSlotsByName(WeatherWidget2Class);
    } // setupUi

    void retranslateUi(QMainWindow *WeatherWidget2Class)
    {
        WeatherWidget2Class->setWindowTitle(QApplication::translate("WeatherWidget2Class", "MainWindow", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(WeatherWidget2Class);
    } // retranslateUi

};

namespace Ui {
    class WeatherWidget2Class: public Ui_WeatherWidget2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEATHERWIDGET2_H
