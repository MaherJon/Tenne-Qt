#ifndef FEEDBACK_DIALOG_H
#define FEEDBACK_DIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui {
class FeedbackDialog;
}
QT_END_NAMESPACE

class FeedbackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeedbackDialog(const QString& text,
                            bool isAiResult,
                            float aiScore,
                            float humanScore,
                            QWidget* parent = nullptr);
    ~FeedbackDialog();

private slots:
    void onSubmitClicked();
    void onTokenSetupClicked();

private:
    Ui::FeedbackDialog* ui;

    QString m_originalText;
    bool m_resultIsAi;
    float m_aiScore;
    float m_humanScore;

    QByteArray captureScreenshot();
    QString generateIssueTitle() const;
    QString generateIssueBody() const;
    void updateTokenStatus();
};

#endif // FEEDBACK_DIALOG_H