// onnx_inference.h
#ifndef ONNX_INFERENCE_H
#define ONNX_INFERENCE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QHash>
#include <onnxruntime_cxx_api.h>

struct TokenResult {
    std::vector<int64_t> input_ids;
    std::vector<int64_t> attention_mask;
};

struct DetectionResult {
    bool isAi;
    float aiScore;
    float humanScore;
    float confidence;
    qint64 inferenceTimeMs;
};

class ONNXInference : public QObject
{
    Q_OBJECT
public:
    static ONNXInference& instance();

    bool loadModel(const QString& modelPath, const QString& vocabPath);
    DetectionResult detect(const QString& text);
    bool isLoaded() const { return m_loaded; }

private:
    ONNXInference() = default;

    TokenResult tokenize(const QString& text, int maxLength = 128);
    QString cleanText(const QString& text);
    QStringList wordpieceTokenize(const QString& word);
    void loadVocabulary(const QString& vocabPath);

    std::unique_ptr<Ort::Session> m_session;
    Ort::Env m_env{ORT_LOGGING_LEVEL_WARNING, "ai_detector"};
    Ort::SessionOptions m_sessionOptions;
    Ort::MemoryInfo m_memoryInfo{Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)};

    QHash<QString, int> m_vocab;
    bool m_loaded = false;

    int m_clsId = 101;
    int m_sepId = 102;
    int m_padId = 0;
    int m_unkId = 100;
};

#endif