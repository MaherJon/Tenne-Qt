/********************************************************************************
** Form generated from reading UI file 'feedback_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FEEDBACK_DIALOG_H
#define UI_FEEDBACK_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FeedbackDialog
{
public:
    QGroupBox *infoGroupBox;
    QGridLayout *gridLayout;
    QLabel *resultLabelTitle;
    QLabel *resultLabel;
    QLabel *aiLabelTitle;
    QLabel *aiScoreLabel;
    QLabel *humanLabelTitle;
    QLabel *humanScoreLabel;
    QGroupBox *textGroupBox;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *textPreview;
    QGroupBox *feedbackGroupBox;
    QFormLayout *formLayout;
    QLabel *expectedLabel;
    QComboBox *expectedCombo;
    QLabel *commentLabel;
    QPlainTextEdit *commentEdit;
    QCheckBox *includeScreenshotCheck;
    QGroupBox *configGroupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *tokenStatusLabel;
    QPushButton *tokenBtn;
    QSpacerItem *horizontalSpacer;
    QWidget *buttonWidget;
    QHBoxLayout *buttonLayout;
    QPushButton *submitBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *FeedbackDialog)
    {
        if (FeedbackDialog->objectName().isEmpty())
            FeedbackDialog->setObjectName("FeedbackDialog");
        FeedbackDialog->resize(615, 521);
        infoGroupBox = new QGroupBox(FeedbackDialog);
        infoGroupBox->setObjectName("infoGroupBox");
        infoGroupBox->setGeometry(QRect(10, 10, 280, 120));
        gridLayout = new QGridLayout(infoGroupBox);
        gridLayout->setObjectName("gridLayout");
        resultLabelTitle = new QLabel(infoGroupBox);
        resultLabelTitle->setObjectName("resultLabelTitle");

        gridLayout->addWidget(resultLabelTitle, 0, 0, 1, 1);

        resultLabel = new QLabel(infoGroupBox);
        resultLabel->setObjectName("resultLabel");
        QFont font;
        font.setBold(true);
        resultLabel->setFont(font);

        gridLayout->addWidget(resultLabel, 0, 1, 1, 1);

        aiLabelTitle = new QLabel(infoGroupBox);
        aiLabelTitle->setObjectName("aiLabelTitle");

        gridLayout->addWidget(aiLabelTitle, 1, 0, 1, 1);

        aiScoreLabel = new QLabel(infoGroupBox);
        aiScoreLabel->setObjectName("aiScoreLabel");

        gridLayout->addWidget(aiScoreLabel, 1, 1, 1, 1);

        humanLabelTitle = new QLabel(infoGroupBox);
        humanLabelTitle->setObjectName("humanLabelTitle");

        gridLayout->addWidget(humanLabelTitle, 2, 0, 1, 1);

        humanScoreLabel = new QLabel(infoGroupBox);
        humanScoreLabel->setObjectName("humanScoreLabel");

        gridLayout->addWidget(humanScoreLabel, 2, 1, 1, 1);

        textGroupBox = new QGroupBox(FeedbackDialog);
        textGroupBox->setObjectName("textGroupBox");
        textGroupBox->setGeometry(QRect(10, 140, 600, 120));
        verticalLayout_2 = new QVBoxLayout(textGroupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        textPreview = new QPlainTextEdit(textGroupBox);
        textPreview->setObjectName("textPreview");
        textPreview->setReadOnly(true);
        textPreview->setMaximumBlockCount(10);

        verticalLayout_2->addWidget(textPreview);

        feedbackGroupBox = new QGroupBox(FeedbackDialog);
        feedbackGroupBox->setObjectName("feedbackGroupBox");
        feedbackGroupBox->setGeometry(QRect(10, 270, 600, 100));
        formLayout = new QFormLayout(feedbackGroupBox);
        formLayout->setObjectName("formLayout");
        expectedLabel = new QLabel(feedbackGroupBox);
        expectedLabel->setObjectName("expectedLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, expectedLabel);

        expectedCombo = new QComboBox(feedbackGroupBox);
        expectedCombo->addItem(QString());
        expectedCombo->addItem(QString());
        expectedCombo->setObjectName("expectedCombo");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, expectedCombo);

        commentLabel = new QLabel(feedbackGroupBox);
        commentLabel->setObjectName("commentLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, commentLabel);

        commentEdit = new QPlainTextEdit(feedbackGroupBox);
        commentEdit->setObjectName("commentEdit");
        commentEdit->setMaximumBlockCount(5);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, commentEdit);

        includeScreenshotCheck = new QCheckBox(FeedbackDialog);
        includeScreenshotCheck->setObjectName("includeScreenshotCheck");
        includeScreenshotCheck->setGeometry(QRect(20, 380, 200, 25));
        includeScreenshotCheck->setChecked(true);
        configGroupBox = new QGroupBox(FeedbackDialog);
        configGroupBox->setObjectName("configGroupBox");
        configGroupBox->setGeometry(QRect(10, 410, 600, 60));
        horizontalLayout = new QHBoxLayout(configGroupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        tokenStatusLabel = new QLabel(configGroupBox);
        tokenStatusLabel->setObjectName("tokenStatusLabel");

        horizontalLayout->addWidget(tokenStatusLabel);

        tokenBtn = new QPushButton(configGroupBox);
        tokenBtn->setObjectName("tokenBtn");

        horizontalLayout->addWidget(tokenBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonWidget = new QWidget(FeedbackDialog);
        buttonWidget->setObjectName("buttonWidget");
        buttonWidget->setGeometry(QRect(420, 480, 190, 35));
        buttonLayout = new QHBoxLayout(buttonWidget);
        buttonLayout->setObjectName("buttonLayout");
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        submitBtn = new QPushButton(buttonWidget);
        submitBtn->setObjectName("submitBtn");

        buttonLayout->addWidget(submitBtn);

        cancelBtn = new QPushButton(buttonWidget);
        cancelBtn->setObjectName("cancelBtn");

        buttonLayout->addWidget(cancelBtn);


        retranslateUi(FeedbackDialog);

        QMetaObject::connectSlotsByName(FeedbackDialog);
    } // setupUi

    void retranslateUi(QDialog *FeedbackDialog)
    {
        FeedbackDialog->setWindowTitle(QCoreApplication::translate("FeedbackDialog", "\346\217\220\344\272\244\351\224\231\350\257\257\345\217\215\351\246\210", nullptr));
        infoGroupBox->setTitle(QCoreApplication::translate("FeedbackDialog", "\346\243\200\346\265\213\347\273\223\346\236\234", nullptr));
        resultLabelTitle->setText(QCoreApplication::translate("FeedbackDialog", "\346\243\200\346\265\213\347\273\223\346\236\234\357\274\232", nullptr));
        resultLabel->setText(QCoreApplication::translate("FeedbackDialog", "-", nullptr));
        aiLabelTitle->setText(QCoreApplication::translate("FeedbackDialog", "AI \345\210\206\346\225\260\357\274\232", nullptr));
        aiScoreLabel->setText(QCoreApplication::translate("FeedbackDialog", "-", nullptr));
        humanLabelTitle->setText(QCoreApplication::translate("FeedbackDialog", "\344\272\272\347\261\273\345\210\206\346\225\260\357\274\232", nullptr));
        humanScoreLabel->setText(QCoreApplication::translate("FeedbackDialog", "-", nullptr));
        textGroupBox->setTitle(QCoreApplication::translate("FeedbackDialog", "\345\216\237\346\226\207\351\242\204\350\247\210", nullptr));
        feedbackGroupBox->setTitle(QCoreApplication::translate("FeedbackDialog", "\346\202\250\347\232\204\345\217\215\351\246\210", nullptr));
        expectedLabel->setText(QCoreApplication::translate("FeedbackDialog", "\345\256\236\351\231\205\345\272\224\350\257\245\346\230\257\357\274\232", nullptr));
        expectedCombo->setItemText(0, QCoreApplication::translate("FeedbackDialog", "\344\272\272\347\261\273\346\222\260\345\206\231\347\232\204", nullptr));
        expectedCombo->setItemText(1, QCoreApplication::translate("FeedbackDialog", "AI \347\224\237\346\210\220\347\232\204", nullptr));

        commentLabel->setText(QCoreApplication::translate("FeedbackDialog", "\345\244\207\346\263\250\350\257\264\346\230\216\357\274\232", nullptr));
        commentEdit->setPlaceholderText(QCoreApplication::translate("FeedbackDialog", "\345\217\257\345\234\250\346\255\244\350\241\245\345\205\205\350\257\264\346\230\216\357\274\210\345\246\202\357\274\232\350\277\231\346\256\265\346\226\207\345\255\227\345\214\205\345\220\253\347\211\271\345\256\232\344\272\272\345\220\215\343\200\201\346\234\257\350\257\255\347\255\211\357\274\211", nullptr));
        includeScreenshotCheck->setText(QCoreApplication::translate("FeedbackDialog", "\360\237\223\270 \345\214\205\345\220\253\346\210\252\345\233\276\357\274\210\345\270\256\345\212\251\346\233\264\345\277\253\345\256\232\344\275\215\351\227\256\351\242\230\357\274\211", nullptr));
        configGroupBox->setTitle(QCoreApplication::translate("FeedbackDialog", "Gitee \351\205\215\347\275\256", nullptr));
        tokenStatusLabel->setStyleSheet(QCoreApplication::translate("FeedbackDialog", "color: orange;", nullptr));
        tokenStatusLabel->setText(QCoreApplication::translate("FeedbackDialog", "\342\232\240\357\270\217 \346\234\252\351\205\215\347\275\256 Token", nullptr));
#if QT_CONFIG(tooltip)
        tokenBtn->setToolTip(QCoreApplication::translate("FeedbackDialog", "\350\256\276\347\275\256\346\202\250\347\232\204 Gitee \347\247\201\344\272\272\350\256\277\351\227\256\344\273\244\347\211\214", nullptr));
#endif // QT_CONFIG(tooltip)
        tokenBtn->setText(QCoreApplication::translate("FeedbackDialog", "\351\205\215\347\275\256 Token", nullptr));
        submitBtn->setStyleSheet(QCoreApplication::translate("FeedbackDialog", "background-color: #4CAF50; color: white; font-weight: bold; padding: 5px 15px;", nullptr));
        submitBtn->setText(QCoreApplication::translate("FeedbackDialog", "\346\217\220\344\272\244\345\217\215\351\246\210", nullptr));
        cancelBtn->setStyleSheet(QCoreApplication::translate("FeedbackDialog", "padding: 5px 15px;", nullptr));
        cancelBtn->setText(QCoreApplication::translate("FeedbackDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FeedbackDialog: public Ui_FeedbackDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FEEDBACK_DIALOG_H
