/*============================================================================*/
/*                           MAHE Tenne-Qt MainWindow file                   */
/*                            Tenne-Qt use LGPLv3 license                   */
/*                       YOU Need Reserve Licence information              */
/*=========================================================================*/
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "onnx_inference.h"
#include "feedback_dialog.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Tenne-Qt v1.0");
    setFixedSize(800, 600);
    setWindowIcon(QIcon(":/app_icon.ico"));

    createMenuBar();

    connect(ui->detectBtn, &QPushButton::clicked, this, &MainWindow::onDetectClicked);
    initModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenuBar()
{
    QMenu* helpMenu = menuBar()->addMenu("帮助(&H)");

    QAction* feedbackAction = new QAction("提交反馈(&F)", this);
    feedbackAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(feedbackAction, &QAction::triggered, this, &MainWindow::onFeedbackTriggered);
    helpMenu->addAction(feedbackAction);

    helpMenu->addSeparator();

    QAction* aboutAction = new QAction("关于(&A)", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "关于 Tenne-Qt",
                           "Tenne-Qt v1.1.0\n\n"
                           "AI 文本检测器\n"
                           "基于 ONNX Runtime 和 Qt6 开发\n\n"
                           "本地化处理，保护隐私");
    });
    helpMenu->addAction(aboutAction);
}

void MainWindow::initModel()
{
    QString modelPath = QCoreApplication::applicationDirPath() + "/model.onnx";
    QString vocabPath = QCoreApplication::applicationDirPath() + "/vocab.txt";

    if (!ONNXInference::instance().loadModel(modelPath, vocabPath)) {
        QMessageBox::critical(this, "错误", "模型加载失败！\n请确保 model.onnx 和 vocab.txt 文件在程序目录下");
        return;
    }

    ui->statusbar->showMessage("模型加载成功", 3000);
}

void MainWindow::onDetectClicked()
{
    QString text = ui->textEdit->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入文本");
        return;
    }

    m_lastText = text;

    ui->detectBtn->setEnabled(false);
    ui->detectBtn->setText("检测中...");

    auto result = ONNXInference::instance().detect(text);

    m_lastResult = result;

    if (result.isAi) {
        ui->resultLabel->setText("AI 生成文本");
        ui->resultLabel->setStyleSheet("color: #d32f2f; font-weight: bold;");
    } else {
        ui->resultLabel->setText("人类撰写文本");
        ui->resultLabel->setStyleSheet("color: #388e3c; font-weight: bold;");
    }

    ui->aiProgressBar->setValue(static_cast<int>(result.aiScore * 100));
    ui->humanProgressBar->setValue(static_cast<int>(result.humanScore * 100));
    ui->timeLabel->setText(QString("推理耗时: %1 ms").arg(result.inferenceTimeMs));

    ui->detectBtn->setEnabled(true);
    ui->detectBtn->setText("开始检测");
}

void MainWindow::onFeedbackTriggered()
{
    if (m_lastText.isEmpty()) {
        QMessageBox::information(this, "提示",
                                 "请先输入文本并点击「开始检测」，然后再提交反馈。");
        return;
    }

    FeedbackDialog dlg(m_lastText,
                       m_lastResult.isAi,
                       m_lastResult.aiScore,
                       m_lastResult.humanScore,
                       this);
    dlg.exec();
}