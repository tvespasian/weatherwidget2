/********************************************************************************
** Form generated from reading ui file 'SelectLocation.ui'
**
** Created: Sun Oct 4 23:13:28 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SELECTLOCATION_H
#define UI_SELECTLOCATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectLocationClass
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *SelectLocationClass)
    {
        if (SelectLocationClass->objectName().isEmpty())
            SelectLocationClass->setObjectName(QString::fromUtf8("SelectLocationClass"));
        SelectLocationClass->resize(233, 165);
        verticalLayoutWidget = new QWidget(SelectLocationClass);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(40, 30, 158, 71));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        lineEdit = new QLineEdit(verticalLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        buttonBox = new QDialogButtonBox(verticalLayoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SelectLocationClass);

        QMetaObject::connectSlotsByName(SelectLocationClass);
    } // setupUi

    void retranslateUi(QWidget *SelectLocationClass)
    {
        SelectLocationClass->setWindowTitle(QApplication::translate("SelectLocationClass", "SelectLocation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SelectLocationClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SelectLocationClass);
    } // retranslateUi

};

namespace Ui {
    class SelectLocationClass: public Ui_SelectLocationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTLOCATION_H
