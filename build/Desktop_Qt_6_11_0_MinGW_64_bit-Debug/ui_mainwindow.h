/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextEdit *textEdit;
    QLabel *InputPrompt;
    QPushButton *detectBtn;
    QWidget *widget;
    QGridLayout *gridLayout;
    QProgressBar *aiProgressBar;
    QLabel *timeLabel;
    QLabel *timePrompt;
    QProgressBar *humanProgressBar;
    QLabel *POAT;
    QLabel *POHT;
    QWidget *widget1;
    QGridLayout *gridLayout_2;
    QLabel *ResultPrompt;
    QLabel *resultLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(260, 10, 521, 311));
        InputPrompt = new QLabel(centralwidget);
        InputPrompt->setObjectName("InputPrompt");
        InputPrompt->setGeometry(QRect(70, 20, 161, 61));
        detectBtn = new QPushButton(centralwidget);
        detectBtn->setObjectName("detectBtn");
        detectBtn->setGeometry(QRect(70, 140, 131, 51));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(260, 400, 183, 96));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        aiProgressBar = new QProgressBar(widget);
        aiProgressBar->setObjectName("aiProgressBar");
        aiProgressBar->setValue(0);

        gridLayout->addWidget(aiProgressBar, 1, 1, 1, 1);

        timeLabel = new QLabel(widget);
        timeLabel->setObjectName("timeLabel");

        gridLayout->addWidget(timeLabel, 2, 1, 1, 1);

        timePrompt = new QLabel(widget);
        timePrompt->setObjectName("timePrompt");

        gridLayout->addWidget(timePrompt, 2, 0, 1, 1);

        humanProgressBar = new QProgressBar(widget);
        humanProgressBar->setObjectName("humanProgressBar");
        humanProgressBar->setValue(0);

        gridLayout->addWidget(humanProgressBar, 0, 1, 1, 1);

        POAT = new QLabel(widget);
        POAT->setObjectName("POAT");

        gridLayout->addWidget(POAT, 1, 0, 1, 1);

        POHT = new QLabel(widget);
        POHT->setObjectName("POHT");

        gridLayout->addWidget(POHT, 0, 0, 1, 1);

        widget1 = new QWidget(centralwidget);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(260, 340, 351, 51));
        gridLayout_2 = new QGridLayout(widget1);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        ResultPrompt = new QLabel(widget1);
        ResultPrompt->setObjectName("ResultPrompt");

        gridLayout_2->addWidget(ResultPrompt, 0, 0, 1, 1);

        resultLabel = new QLabel(widget1);
        resultLabel->setObjectName("resultLabel");

        gridLayout_2->addWidget(resultLabel, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        InputPrompt->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:16pt; font-weight:700;\">\345\205\266\350\276\223\345\205\245\346\243\200\346\265\213\346\226\207\346\234\254\357\274\232</span></p></body></html>", nullptr));
#if QT_CONFIG(tooltip)
        detectBtn->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">\345\274\200\345\247\213\346\243\200\346\265\213</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        detectBtn->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\243\200\346\265\213", nullptr));
        timeLabel->setText(QString());
        timePrompt->setText(QCoreApplication::translate("MainWindow", "\346\216\250\347\220\206\350\200\227\346\227\266\357\274\232", nullptr));
        POAT->setText(QCoreApplication::translate("MainWindow", "ai\347\227\225\350\277\271\345\215\240\346\257\224\357\274\232", nullptr));
        POHT->setText(QCoreApplication::translate("MainWindow", "\344\272\272\347\261\273\347\227\225\350\277\271\345\215\240\346\257\224\357\274\232", nullptr));
        ResultPrompt->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:16pt; font-weight:700;\">\346\243\200\346\265\213\347\273\223\346\236\234\357\274\232</span></p></body></html>", nullptr));
        resultLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><br/></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
