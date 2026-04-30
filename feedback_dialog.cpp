#include "feedback_dialog.h"
#include "ui_feedback_dialog.h"
#include "gitee_reporter.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QBuffer>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QCheckBox>

FeedbackDialog::FeedbackDialog(const QString& text,
                               bool isAiResult,
                               float aiScore,
                               float humanScore,
                               QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FeedbackDialog)
    , m_originalText(text)
    , m_resultIsAi(isAiResult)
    , m_aiScore(aiScore)
    , m_humanScore(humanScore)
{
    ui->setupUi(this);

    setWindowTitle("提交错误反馈");

    ui->resultLabel->setText(m_resultIsAi ? "AI 生成" : "人类撰写");
    ui->aiScoreLabel->setText(QString::number(m_aiScore * 100, 'f', 1) + "%");
    ui->humanScoreLabel->setText(QString::number(m_humanScore * 100, 'f', 1) + "%");

    QString shortText = text;
    if (shortText.length() > 200) {
        shortText = shortText.left(200) + "...";
    }
    ui->textPreview->setPlainText(shortText);

    connect(ui->submitBtn, &QPushButton::clicked, this, &FeedbackDialog::onSubmitClicked);
    connect(ui->tokenBtn, &QPushButton::clicked, this, &FeedbackDialog::onTokenSetupClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    if (!GiteeReporter::instance().hasToken()) {
        ui->tokenStatusLabel->setText("⚠️ 未配置 Token");
        ui->tokenStatusLabel->setStyleSheet("color: orange;");
    } else {
        ui->tokenStatusLabel->setText("✅ Token 已配置");
        ui->tokenStatusLabel->setStyleSheet("color: green;");
    }
}

FeedbackDialog::~FeedbackDialog()
{
    delete ui;
}

void FeedbackDialog::onTokenSetupClicked()
{
    showTokenDialog();
}

void FeedbackDialog::showTokenDialog()
{
    bool ok;
    QString token = QInputDialog::getText(
        this,
        "配置 Gitee Token",
        "请输入您的 Gitee 私人令牌：\n\n"
        "获取步骤：\n"
        "1. 访问 https://gitee.com/profile/personal_tokens\n"
        "2. 点击「生成新令牌」\n"
        "3. 勾选 issues 权限\n"
        "4. 生成并复制令牌\n\n"
        "令牌：",
        QLineEdit::Password,
        QString(),
        &ok
        );

    if (ok && !token.isEmpty()) {
        GiteeReporter::instance().setToken(token);
        GiteeReporter::instance().setRepo("maheos", "Tenne-Qt");

        ui->tokenStatusLabel->setText("✅ Token 已配置");
        ui->tokenStatusLabel->setStyleSheet("color: green;");

        QMessageBox::information(this, "成功", "Token 配置成功！");
    }
}

void FeedbackDialog::onSubmitClicked()
{
    // 检查 Token
    if (!GiteeReporter::instance().hasToken()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "未配置 Token",
            "尚未配置 Gitee Token，是否立即配置？",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            showTokenDialog();
        }
        return;
    }

    QString expectedLabel = ui->expectedCombo->currentText();
    QString comment = ui->commentEdit->toPlainText();

    QString title = generateIssueTitle();
    QString body = generateIssueBody() +
                   "\n\n## 用户反馈\n" +
                   "**期望标签**: " + expectedLabel + "\n\n" +
                   "**备注**: " + (comment.isEmpty() ? "无" : comment);

    QByteArray screenshot;
    if (ui->includeScreenshotCheck->isChecked()) {
        screenshot = captureScreenshot();
    }

    ui->submitBtn->setEnabled(false);
    ui->submitBtn->setText("提交中...");

    connect(&GiteeReporter::instance(), &GiteeReporter::submitSuccess,
            this, [this](const QString& url) {
                QMessageBox::information(this, "提交成功",
                                         QString("反馈已提交！\n\n您可以在以下链接查看：\n%1").arg(url));
                accept();
            });

    connect(&GiteeReporter::instance(), &GiteeReporter::submitFailed,
            this, [this](const QString& error) {
                QMessageBox::warning(this, "提交失败",
                                     QString("反馈提交失败：\n%1\n\n请检查网络连接和 Token 配置").arg(error));
                ui->submitBtn->setEnabled(true);
                ui->submitBtn->setText("提交反馈");
            });

    connect(&GiteeReporter::instance(), &GiteeReporter::tokenInvalid,
            this, [this]() {
                QMessageBox::warning(this, "Token 无效",
                                     "Gitee Token 无效或已过期，请重新配置。");
                showTokenDialog();
                ui->submitBtn->setEnabled(true);
                ui->submitBtn->setText("提交反馈");
            });

    GiteeReporter::instance().submitFeedback(title, body, screenshot);
}

QString FeedbackDialog::generateIssueTitle() const
{
    QString prefix = m_resultIsAi ? "[误报] AI被识别为人类" : "[漏报] 人类被识别为AI";
    QString preview = m_originalText.left(50);
    return QString("%1: %2").arg(prefix, preview);
}

QString FeedbackDialog::generateIssueBody() const
{
    return QString(
               "## 📝 检测信息\n\n"
               "| 项目 | 值 |\n"
               "|------|-----|\n"
               "| 检测结果 | **%1** |\n"
               "| AI 分数 | %2 |\n"
               "| 人类分数 | %3 |\n\n"
               "## 📄 原文内容\n\n"
               "```\n%4\n```\n\n"
               "---\n"
               "*提交时间: %5*\n"
               "*由 Tenne-Qt 客户端自动提交*"
               ).arg(m_resultIsAi ? "AI 生成" : "人类撰写")
        .arg(m_aiScore)
        .arg(m_humanScore)
        .arg(m_originalText)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

QByteArray FeedbackDialog::captureScreenshot()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) return QByteArray();

    QPixmap pixmap = screen->grabWindow(0);
    pixmap = pixmap.scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    return bytes;
}