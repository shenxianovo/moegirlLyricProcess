// @brief   Moegirl lyric extract.
// @version 0.2
// @author  shenxianovo
// @github  https://github.com/shenxianovo/moegirlLyricProcess
// @email   shenxianovo@gmail.com
// last modified: UTC+8 2024/4/15 22:12

#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include <QString>
#include <QSet>
#include <QFile>
#include <QTextStream>

#include "Extractor.h"

Extractor::Extractor() {}

Extractor::Extractor(const QString& filePath) {
    fileInfo.setFile(filePath);
    kanaNotation = true;
    selectionInfo[0] = selectionInfo[1] = selectionInfo[2] = 0;

    cut(fileInfo.filePath());
    lineFormation();
    organize();
}

int Extractor::hierachyInfo() {
    return hierachySet.size();
}

QString Extractor::getFileName() const {
    return fileInfo.fileName();
}

QString Extractor::getFilePath() const {
    return fileInfo.filePath();
}

bool Extractor::getKanaInfo() {
    return kanaNotation;
}

void Extractor::setFileInfo(const QString& filePath) {
    fileInfo.setFile(filePath);
}

void Extractor::extract() {
    select();
}

/// @brief cut the HTML file to get the lyrics part.
///        inputFileName is the path of the HTML file.
///        the cutted part will be saved in temp/cuttedHTML.txt.
/// @param inputFileName
void Extractor::cut(const QString& inputFileName) {
    QFile inputFile(inputFileName);
    QString outputPath = "temp/" + fileInfo.fileName() + "_cutted";
    QFile outputFile(outputPath);

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text) && outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QTextStream out(&outputFile);

        while (!in.atEnd()) {
            QString line = in.readLine();
            int start = line.indexOf("<div class=\"Lyrics Lyrics-");
            if (start != -1) {
                // Remove square brackets and their contents
                int left = line.indexOf("[");
                while (left != -1) {
                    int right = line.indexOf("]", left);
                    line.remove(left, right - left + 1);
                    left = line.indexOf("[", left);
                }
                out << line.mid(start) << "\n";
                break;
            }
        }

        inputFile.close();
        outputFile.close();
    }
}

/// @brief format the HTML file to get the lyrics.
///        input is a huge line of the HTML file.
///        the formated part will be saved in temp/formatedHTML.txt.
/// @param
void Extractor::lineFormation() {
    QString inputPath = "temp/" + fileInfo.fileName() + "_cutted";
    QString outputPath = "temp/" + fileInfo.fileName() + "_formated";
    QFile inputFile(inputPath);
    QFile outputFile(outputPath);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text) && inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QTextStream out(&outputFile);

        QString line = in.readLine();

        int hierachy = 0;

        int right = line.indexOf("<");

        while (right < line.size() && right != -1) {
            QString content;
            int left = right;
            right = line.indexOf(">", left + 1);
            content = line.mid(left + 1, right - left - 1);
            if (content[0] == '/') {
                hierachy--;
                left = right;
                right = line.indexOf("<", left + 1);
            }
            /* HTML 标签 */
            // out << hierachy << " "; // 层级
            // out << QString(hierachy * 4, ' '); // 缩进
            // out << content << "\n"; // HTML标签
            if (content[0] != '/') {
                hierachy++;
                left = right;
                right = line.indexOf("<", left + 1);
            }
            content = line.mid(left + 1, right - left - 1);
            if (!content.isEmpty()) {
                hierachySet.insert(hierachy);
                out << hierachy << " "; // 层级
                // out << QString(hierachy * 4, ' '); // 缩进
                out << content << "\n"; // 内容
            }
        }

        outputFile.close();
    }
}

/// @brief Judge if the first charactor of the string is a kana.
/// @param str
/// @return true-is kana, false-not
bool Extractor::isKana(const QString& line) {
    for (int start = 0; start < line.size(); start++) {
        if (kana.contains(line.mid(start, 1))) {
            return true;
        }
    }

    return false;
}

/// @brief Judge if the first charactor of the string is a punctuation.
/// @param line
/// @return True-is punctuation, False-not.
bool Extractor::isPunctuation(const QString& line) {
    QString firstChar = line.mid(0, 1);

    return punctuation.contains(firstChar);
}


/// @brief organize the scattered lyrics and remove the hiarachy information.
void Extractor::organize() {
    QString inputPath = "temp/" + fileInfo.fileName() + "_formated";
    QString outputPath = "temp/" + fileInfo.fileName() + "_organized";

    QFile inputFile(inputPath);
    QFile outputFile(outputPath);
    bool chinese = false;

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text) && outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QTextStream out(&outputFile);

        if (hierachySet.size() < 3) { // 无假名注音
            kanaNotation = false;
            QString line = in.readLine();
            while (!line.isNull()) {
                out << line.mid(line.indexOf(" ") + 1) << "\n";
                line = in.readLine();
            }
        } else if (hierachySet.size() == 3) { // 有假名注音
            QString line = in.readLine();
            while (!line.isNull()) {
                int hierachy = line.mid(0, line.indexOf(" ")).toInt();
                line = line.mid(line.indexOf(" ") + 1);
                // 中文/假名
                if (hierachy == *hierachySet.begin()) {
                    // 日语歌词
                    if (isKana(line) ||
                        isPunctuation(line) || // 针对日语歌词片段全是标点的情况(INTERNET YAMERO: ！！！)
                        chinese ||
                        line.mid(0, 1) == "　" || // 歌词中间的空格
                        line.mid(0, 1) == " " // 歌词开头的空格(为啥开头会有空格啊喂)
                        ) {
                        out << line;
                        chinese = false;
                    } else { // 中文歌词
                        chinese = true;
                        out << "\n" << line << "\n";
                    }
                } else if (hierachy == *hierachySet.rbegin()) { // 括号
                    if (isKana(line)) { // 针对没有括号的假名标注
                        out << "（" << line << "）";
                    } else { // 括号
                        out << line.mid(0, line.indexOf("\n"));
                    }
                } else { // 日语汉字 + 注音
                    out << line;
                }

                line = in.readLine();
            }
        } else if (hierachySet.size() == 5) { // 中日+假名注音+自带罗马音
                                            // 我的想法是反正有假名，罗马音就不要了。要加的话后面再加个假名转罗马音的功能
            QString line = in.readLine();
            std::set<int>::iterator it = hierachySet.begin();
            it++; it++; // 第三个层级，即罗马音左括号
            static bool breakSign = false; // 分段符 鬼知道为啥要用两行括号分段
            while (!line.isNull()) {
                int hierachy = line.mid(0, line.indexOf(" ")).toInt();
                line = line.mid(line.indexOf(" ") + 1);
                if (hierachy == *hierachySet.begin()) { // 中文
                    out << "\n" << line << "\n";
                    breakSign = true;
                } else if (hierachy == *it) { // 罗马音左括号or罗马音or分段标志左括号
                    if (breakSign) { // 分段标志左括号
                        line = in.readLine(); // 读掉分段标志右括号
                    } else if (line.indexOf("（") != -1) { // 罗马音
                        line = in.readLine();
                        line = in.readLine(); // 再读两次把罗马音和罗马音右括号读掉

                    } else { // 假名
                        out << line;
                    }
                    breakSign = false;
                } else { // 其他
                    out << line;

                    breakSign = false;
                }

                line = in.readLine();
            }
        }
    }
}

/// @brief deltete kana notation.
/// @param line
void Extractor::deleteNotation(QString& line) {
    int left = line.indexOf("（");
    while (left != -1) {
        int right = line.indexOf("）", left);
        line.remove(left, right - left + 1);
        left = line.indexOf("（", left);
    }
}

/// @brief select the language you want and note if you need kana.
/// @param the name of the song
void Extractor::select() {
    QString inputPath = "temp/" + fileInfo.fileName() + "_organized";
    QString songName = fileInfo.fileName().mid(0, fileInfo.fileName().indexOf("-") - 1);
    QString savePath = "lyric/" + songName + ".txt";
    QFile inputFile(inputPath);
    QFile outputFile(savePath);

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text) && outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QTextStream out(&outputFile);

        if (selectionInfo[0] == 1 && selectionInfo[1] == 0 && selectionInfo[2] == 0) { // 中文
            for (QString line = in.readLine(); !line.isNull(); line = in.readLine()) {
                line = in.readLine(); // 去掉日语
                out << line << "\n";
            }
        } else if (selectionInfo[0] == 0 && selectionInfo[1] == 1 && selectionInfo[2] == 0) { // 日语
            for (QString line = in.readLine(); !line.isNull(); line = in.readLine()) {
                deleteNotation(line);
                out << line << "\n";
                line = in.readLine(); // 去掉中文
            }
        } else if (selectionInfo[0] == 1 && selectionInfo[1] == 1 && selectionInfo[2] == 0) { // 中日
            for (QString line = in.readLine(); !line.isNull(); line = in.readLine()) {
                int i = 0;
                if (i % 2 == 0) {
                    deleteNotation(line);
                }
                out << line << "\n";
                i++;
            }
        } else if (selectionInfo[0] == 0 && selectionInfo[1] == 1 && selectionInfo[1] == 1 && hierachySet.size() >= 3) { // 日语+假名
            for (QString line = in.readLine(); !line.isNull(); line = in.readLine()) {
                out << line << "\n";
                line = in.readLine(); // 去掉中文
            }
        } else if (selectionInfo[0] == 1 && selectionInfo[1] == 1 && selectionInfo[2] == 1 && hierachySet.size() >= 3) { // 中日+假名
            for (QString line = in.readLine(); !line.isNull(); line = in.readLine()) {
                out << line << "\n";
            }
        }
    }

    // if (language == 1) { // 中文
    //     for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //         std::getline(inputFile, line, '\n'); // 去掉日语
    //         outputFile << line << "\n";
    //     }
    // } else if (language == 2) { // 日语
    //     if (hierachySet.size() < 3) { // 无假名标注
    //         for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //             outputFile << line << "\n";
    //             std::getline(inputFile, line, '\n'); // 去掉中文
    //         }
    //     } else if (hierachySet.size() >= 3) { // 有假名标注
    //         std::cout << "Need kana?\n" << "1. Yes\n" << "2. No\n";
    //         std::cin >> kana;

    //         if (kana == 1) {
    //             for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //                 outputFile << line << "\n";
    //                 std::getline(inputFile, line, '\n'); // 去掉中文
    //             }
    //         } else { // 不带假名
    //             for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //                 deleteNotation(line);
    //                 outputFile << line << "\n";
    //                 std::getline(inputFile, line, '\n'); // 去掉中文
    //             }
    //         }
    //     }
    // } else { // 中日
    //     if (hierachySet.size() < 3) { // 无假名标注
    //         for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //             outputFile << line << "\n";
    //         }
    //     } else if (hierachySet.size() == 3) { // 有假名标注
    //         std::cout << "Need kana?\n" << "1. Yes\n" << "2. No\n";
    //         std::cin >> kana;

    //         if (kana == 1) { // 带假名
    //             for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //                 outputFile << line << "\n";
    //             }
    //         } else { // 不带假名
    //             for (std::string line; std::getline(inputFile, line, '\n'); ) {
    //                 deleteNotation(line);
    //                 outputFile << line << "\n";
    //             }
    //         }
    //     } else if (hierachySet.size() == 5) { // 假名+罗马音标注

    //     }
    // }
}

QSet<QString> Extractor::kana = {
    "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ", "さ", "し", "す", "せ", "そ", "た", "ち", "つ", "て", "と",
    "な", "に", "ぬ", "ね", "の", "は", "ひ", "ふ", "へ", "ほ", "ま", "み", "む", "め", "も", "や", "ゆ", "よ", "ら", "り",
    "る", "れ", "ろ", "わ", "を", "ん", "が", "ぎ", "ぐ", "げ", "ご", "ざ", "じ", "ず", "ぜ", "ぞ", "だ", "ぢ", "づ", "で",
    "ど", "ば", "び", "ぶ", "べ", "ぼ", "ぱ", "ぴ", "ぷ", "ぺ", "ぽ", "ゃ", "ゅ", "ょ", "っ", "ア", "イ", "ウ", "エ", "オ",
    "カ", "キ", "ク", "ケ", "コ", "サ", "シ", "ス", "セ", "ソ", "タ", "チ", "ツ", "テ", "ト", "ナ", "ニ", "ヌ", "ネ", "ノ",
    "ハ", "ヒ", "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ", "ヤ", "ユ", "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ワ", "ヲ",
    "ン", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ", "ゼ", "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "バ", "ビ", "ブ", "ベ",
    "ボ", "パ", "ピ", "プ", "ペ", "ポ", "ャ", "ュ", "ョ", "ッ"
};

QSet<QString> Extractor::punctuation = {
    "！"
};
