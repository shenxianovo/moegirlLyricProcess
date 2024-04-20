#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QString>
#include <QSet>
#include <set>
#include <QFileInfo>


class Extractor{
public:
    Extractor();
    Extractor(const QString& filePath);

    void extract();

    int hierachyInfo();
    void setFileInfo(const QString& file);
    void setFileName(const QString& file);

    QString getFileName() const;
    QString getFilePath() const;
    bool getKanaInfo();

    int selectionInfo[3];


private:
    std::set<int> hierachySet;
    QFileInfo fileInfo;
    bool kanaNotation;

    static QSet<QString> kana;
    static QSet<QString> punctuation;

    void cut(const QString& inputFileName);
    void lineFormation();
    bool isKana(const QString& line);
    bool isPunctuation(const QString& line);
    void organize();
    void deleteNotation(QString& line);
    void select();


};

#endif // EXTRACTOR_H
