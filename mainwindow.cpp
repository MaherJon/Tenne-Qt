#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "onnx_inference.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Tenne-Qt v1.0");
    setFixedSize(800, 600);
    setWindowIcon(QIcon(":/app_icon.ico"));
    connect(ui->detectBtn, &QPushButton::clicked, this, &MainWindow::onDetectClicked);
    initModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initModel()
{
    QString modelPath = QCoreApplication::applicationDirPath() + "/model.onnx";
    QString vocabPath = QCoreApplication::applicationDirPath() + "/vocab.txt";

    if (!ONNXInference::instance().loadModel(modelPath, vocabPath)) {
        QMessageBox::critical(this, "错误", "模型加载失败！\n请确保 model.onnx 和 vocab.txt 文件在程序目录下");
        return;
    }

    ui->statusbar->showMessage("✅ 模型加载成功", 3000);
}

void MainWindow::onDetectClicked()
{
    QString text = ui->textEdit->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入文本");
        return;
    }

    ui->detectBtn->setEnabled(false);
    ui->detectBtn->setText("检测中...");

    auto result = ONNXInference::instance().detect(text);

    if (result.isAi) {
        ui->resultLabel->setText("🤖 AI 生成文本");
        ui->resultLabel->setStyleSheet("color: #d32f2f; font-weight: bold;");
    } else {
        ui->resultLabel->setText("👤 人类撰写文本");
        ui->resultLabel->setStyleSheet("color: #388e3c; font-weight: bold;");
    }

    ui->aiProgressBar->setValue(static_cast<int>(result.aiScore * 100));
    ui->humanProgressBar->setValue(static_cast<int>(result.humanScore * 100));
    ui->timeLabel->setText(QString("推理耗时: %1 ms").arg(result.inferenceTimeMs));

    ui->detectBtn->setEnabled(true);
    ui->detectBtn->setText("开始检测");
}