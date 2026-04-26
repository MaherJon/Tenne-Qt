// onnx_inference.cpp
#include "onnx_inference.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <chrono>

ONNXInference& ONNXInference::instance()
{
    static ONNXInference instance;
    return instance;
}

void ONNXInference::loadVocabulary(const QString& vocabPath)
{
    QFile file(vocabPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "❌ 无法打开词表文件:" << vocabPath;
        return;
    }

    QTextStream stream(&file);
    int idx = 0;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (!line.isEmpty()) {
            m_vocab[line] = idx;
            idx++;
        }
    }

    m_clsId = m_vocab.value("[CLS]", 101);
    m_sepId = m_vocab.value("[SEP]", 102);
    m_padId = m_vocab.value("[PAD]", 0);
    m_unkId = m_vocab.value("[UNK]", 100);

    qDebug() << "✅ 词表加载完成，大小:" << m_vocab.size();
    qDebug() << "   [CLS]:" << m_clsId << "[SEP]:" << m_sepId << "[PAD]:" << m_padId;
}

bool ONNXInference::loadModel(const QString& modelPath, const QString& vocabPath)
{
    try {
        loadVocabulary(vocabPath);

        m_sessionOptions.SetIntraOpNumThreads(1);
        m_sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

#ifdef _WIN32
        std::wstring wModelPath = modelPath.toStdWString();
        m_session = std::make_unique<Ort::Session>(
            m_env,
            wModelPath.c_str(),
            m_sessionOptions
            );
#else
        m_session = std::make_unique<Ort::Session>(
            m_env,
            modelPath.toStdString().c_str(),
            m_sessionOptions
            );
#endif

        m_loaded = true;
        qDebug() << "✅ ONNX 模型加载成功";
        return true;

    } catch (const Ort::Exception& e) {
        qDebug() << "❌ ONNX 加载失败:" << e.what();
        return false;
    }
}

QString ONNXInference::cleanText(const QString& text)
{
    QString result = text.toLower();
    result = result.simplified();

    QRegularExpression punctRegex("[，。！？；：\"\"''【】（）《》]");
    result.replace(punctRegex, " $0 ");
    result = result.simplified();

    return result;
}

QStringList ONNXInference::wordpieceTokenize(const QString& word)
{
    QStringList tokens;

    if (m_vocab.contains(word)) {
        tokens.append(word);
        return tokens;
    }

    int start = 0;
    while (start < word.length()) {
        int end = word.length();
        bool found = false;

        while (start < end) {
            QString subWord = word.mid(start, end - start);
            if (start > 0) {
                subWord = "##" + subWord;
            }
            if (m_vocab.contains(subWord)) {
                tokens.append(subWord);
                start = end;
                found = true;
                break;
            }
            end--;
        }

        if (!found) {
            tokens.append("[UNK]");
            break;
        }
    }

    return tokens;
}

TokenResult ONNXInference::tokenize(const QString& text, int maxLength)
{
    TokenResult result;

    result.input_ids.push_back(m_clsId);
    result.attention_mask.push_back(1);

    QString cleaned = cleanText(text);
    QStringList words = cleaned.split(" ", Qt::SkipEmptyParts);

    for (const QString& word : words) {
        QStringList wordTokens = wordpieceTokenize(word);

        if (result.input_ids.size() + wordTokens.size() >= maxLength) {
            break;
        }

        for (const QString& token : wordTokens) {
            int tokenId = m_vocab.value(token, m_unkId);
            result.input_ids.push_back(tokenId);
            result.attention_mask.push_back(1);
        }
    }

    result.input_ids.push_back(m_sepId);
    result.attention_mask.push_back(1);

    while (result.input_ids.size() < maxLength) {
        result.input_ids.push_back(m_padId);
        result.attention_mask.push_back(0);
    }

    return result;
}

DetectionResult ONNXInference::detect(const QString& text)
{
    DetectionResult result;
    result.isAi = false;
    result.aiScore = 0.0f;
    result.humanScore = 0.0f;
    result.confidence = 0.0f;
    result.inferenceTimeMs = 0;

    if (!m_loaded || text.isEmpty()) {
        return result;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    try {
        auto tokens = tokenize(text, 128);

        std::vector<int64_t> inputShape = {1, static_cast<int64_t>(tokens.input_ids.size())};
        std::vector<int64_t> maskShape = {1, static_cast<int64_t>(tokens.attention_mask.size())};

        Ort::Value inputIds = Ort::Value::CreateTensor<int64_t>(
            m_memoryInfo,
            tokens.input_ids.data(),
            tokens.input_ids.size(),
            inputShape.data(),
            inputShape.size()
            );

        Ort::Value attentionMask = Ort::Value::CreateTensor<int64_t>(
            m_memoryInfo,
            tokens.attention_mask.data(),
            tokens.attention_mask.size(),
            maskShape.data(),
            maskShape.size()
            );

        const char* inputNames[] = {"input_ids", "attention_mask"};
        const char* outputNames[] = {"logits"};

        std::vector<Ort::Value> inputs;
        inputs.push_back(std::move(inputIds));
        inputs.push_back(std::move(attentionMask));

        auto outputs = m_session->Run(Ort::RunOptions{nullptr}, inputNames, inputs.data(), 2, outputNames, 1);
        float* logits = outputs[0].GetTensorMutableData<float>();

        // Softmax
        float exp0 = std::exp(logits[0]);
        float exp1 = std::exp(logits[1]);
        float sum = exp0 + exp1;

        result.humanScore = exp0 / sum;
        result.aiScore = exp1 / sum;
        result.isAi = result.aiScore > 0.5;
        result.confidence = result.isAi ? result.aiScore : result.humanScore;

    } catch (const Ort::Exception& e) {
        qDebug() << "❌ 推理失败:" << e.what();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.inferenceTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return result;
}