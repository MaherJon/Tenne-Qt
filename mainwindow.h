#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStatusBar>
#include <QString>
#include "onnx_inference.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initModel();
    void onDetectClicked();

private slots:
    void onFeedbackTriggered();

private:
    Ui::MainWindow *ui;
    DetectionResult m_lastResult;
    QString m_lastText;
    void createMenuBar();
};

#endif // MAINWINDOW_H