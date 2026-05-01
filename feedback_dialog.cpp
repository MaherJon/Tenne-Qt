#include "feedback_dialog.h"
#include "ui_feedback_dialog.h"
#include "github_reporter.h"
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QBuffer>
#include <QLineEdit>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>

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

    ui->textPreview->setPlainText(text);

    ui->resultLabel->setText(m_resultIsAi ? "AI生成" : "人类撰写");
    ui->aiScoreLabel->setText(QString::number(m_aiScore * 100, 'f', 1) + "%");
    ui->humanScoreLabel->setText(QString::number(m_humanScore * 100, 'f', 1) + "%");

    updateTokenStatus();

    if (m_resultIsAi) {
        ui->expectedCombo->setCurrentText("人类撰写的");
    } else {
        ui->expectedCombo->setCurrentText("AI生成的");
    }

    connect(ui->submitBtn, &QPushButton::clicked, this, &FeedbackDialog::onSubmitClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->tokenBtn, &QPushButton::clicked, this, &FeedbackDialog::onTokenSetupClicked);

    if (!GitHubReporter::instance().hasToken()) {
        QMessageBox::information(this, "首次使用",
                                 "请先点击「配置 Token」设置您的 GitHub 访问令牌");
    }
}

void FeedbackDialog::onTokenSetupClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("配置 GitHub Token");
    dialog.setMinimumWidth(500);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* infoLabel = new QLabel(
        "请按以下步骤获取 GitHub Personal Access Token：\n\n"
        "1. 访问 https://github.com/settings/tokens\n"
        "2. 点击「Generate new token (classic)」\n"
        "3. 勾选「repo」权限\n"
        "4. 生成并复制 token\n\n"
        "请输入您的 Token："
        );
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    QLineEdit* input = new QLineEdit;
    input->setEchoMode(QLineEdit::Password);
    layout->addWidget(input);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    QPushButton* okBtn = new QPushButton("保存");
    QPushButton* cancelBtn = new QPushButton("取消");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted && !input->text().isEmpty()) {
        GitHubReporter::instance().setToken(input->text());
        updateTokenStatus();
        QMessageBox::information(this, "成功", "Token 已保存");
    }
}

void FeedbackDialog::onSubmitClicked()
{
    QString expected = ui->expectedCombo->currentText();
    QString comment = ui->commentEdit->toPlainText();

    QString title = generateIssueTitle();
    QString body = generateIssueBody() + "\n\n用户期望: " + expected + "\n\n用户备注: " + comment;

    QByteArray screenshot = captureScreenshot();

    ui->submitBtn->setEnabled(false);
    ui->submitBtn->setText("提交中...");

    connect(&GitHubReporter::instance(), &GitHubReporter::submitSuccess,
            this, [this](const QString& url) {
                QMessageBox::information(this, "提交成功",
                                         "反馈已提交，您可以在以下链接查看:\n" + url);
                accept();
            });

    connect(&GitHubReporter::instance(), &GitHubReporter::submitFailed,
            this, [this](const QString& error) {
                QMessageBox::warning(this, "提交失败",
                                     "提交失败: " + error + "\n\n请检查网络连接和 Token 配置");
                ui->submitBtn->setEnabled(true);
                ui->submitBtn->setText("提交反馈");
            });

    connect(&GitHubReporter::instance(), &GitHubReporter::tokenInvalid,
            this, [this]() {
                QMessageBox::warning(this, "Token 无效",
                                     "请先配置有效的 GitHub Token");
                ui->submitBtn->setEnabled(true);
                ui->submitBtn->setText("提交反馈");
                onTokenSetupClicked();
            });

    GitHubReporter::instance().submitFeedback(title, body, screenshot);
}

QString FeedbackDialog::generateIssueTitle() const
{
    return QString("[错误检测] %1 被识别为 %2")
        .arg(m_originalText.left(50))
        .arg(m_resultIsAi ? "AI" : "人类");
}

QString FeedbackDialog::generateIssueBody() const
{
    return QString(
               "## 检测反馈\n\n"
               "**原文文本**:\n```\n%1\n```\n\n"
               "**检测结果**: %2\n"
               "**AI 分数**: %3\n"
               "**人类分数**: %4\n\n"
               "---\n"
               "*提交时间: %5*\n"
               "*Tenne-Qt 反馈工具*\n"
               ).arg(m_originalText)
        .arg(m_resultIsAi ? "AI 生成" : "人类撰写")
        .arg(m_aiScore)
        .arg(m_humanScore)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

QByteArray FeedbackDialog::captureScreenshot()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);

    pixmap = pixmap.scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    return bytes;
}

void FeedbackDialog::updateTokenStatus()
{
    if (GitHubReporter::instance().hasToken()) {
        ui->tokenStatusLabel->setText("✅ Token 已配置");
        ui->tokenStatusLabel->setStyleSheet("color: green;");
    } else {
        ui->tokenStatusLabel->setText("⚠️ 未配置 Token");
        ui->tokenStatusLabel->setStyleSheet("color: orange;");
    }
}

FeedbackDialog::~FeedbackDialog()
{
    delete ui;
}