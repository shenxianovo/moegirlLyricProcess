// @brief   Moegirl lyric process.
// @version 0.2
// @author  shenxianovo
// @github  https://github.com/shenxianovo/moegirlLyricProcess
// @email   shenxianovo@gmail.com
// last modified: UTC+8 2024/4/15 22:12

#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>

/// @brief cut the HTML file to get the lyrics part.
///        inputFileName is the path of the HTML file.
///        the cutted part will be saved in ../temp/cuttedHTML.txt.
/// @param inputFileName 
void cut(const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::in);
    std::ofstream outputFile("../temp/cuttedHTML.txt", std::ios::out);

    if (!inputFile) {
        std::cerr << "Failed to open input file" << std::endl;
        return;
    }

    for (std::string line; std::getline(inputFile, line, '\n'); ) {
        int start = line.find("<div class=\"Lyrics Lyrics-");
        if (start != std::string::npos) {
            // 去掉中括号及其中内容
            int left = line.find("[");
            while (left != std::string::npos) {
                int right = line.find("]", left);
                line.erase(left, right - left + 1);
                left = line.find("[", left);
            }
            outputFile << line.substr(start) << "\n";
            break;
        }
    }
}


std::set<int> hierachySet;
/// @brief format the HTML file to get the lyrics.
///        input is a huge line of the HTML file.
///        the formated part will be saved in ../temp/formatedHTML.txt.
/// @param line 
void lineFormation(const std::string& line) {
    std::ofstream outputFile("../temp/formatedHTML.txt", std::ios::out);

    int hierachy = 0;

    int right = line.find("<");

    while (right < line.size() && right != std::string::npos) {
        std::string content;
        int left = right;
        right = line.find(">", left + 1);
        content = line.substr(left + 1, right - left - 1);
        if (content[0] == '/') {
            hierachy--;
            left = right;
            right = line.find("<", left + 1);
        }
        /* HTML 标签 */
        // outputFile << hierachy << " "; // 层级
        // outputFile << std::string(hierachy * 4, ' '); // 缩进
        // outputFile << content << "\n"; // HTML标签
        if (content[0] != '/') {
            hierachy++;
            left = right;
            right = line.find("<", left + 1);
        }
        content = line.substr(left + 1, right - left - 1);
        if (content != "") {
            hierachySet.insert(hierachy);
            outputFile << hierachy << " "; // 层级
            // outputFile << std::string(hierachy * 4, ' '); // 缩进
            outputFile << content << "\n"; // 内容
        }
    }
}

/// @brief Judge if the first charactor of the string is a kana.
/// @param str 
/// @return true-is kana, false-not
bool isKana(const std::string& line) {
    std::set<std::string> kana = {
        "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ", "さ", "し", "す", "せ", "そ", "た", "ち", "つ", "て", "と",
        "な", "に", "ぬ", "ね", "の", "は", "ひ", "ふ", "へ", "ほ", "ま", "み", "む", "め", "も", "や", "ゆ", "よ", "ら", "り",
        "る", "れ", "ろ", "わ", "を", "ん", "が", "ぎ", "ぐ", "げ", "ご", "ざ", "じ", "ず", "ぜ", "ぞ", "だ", "ぢ", "づ", "で",
        "ど", "ば", "び", "ぶ", "べ", "ぼ", "ぱ", "ぴ", "ぷ", "ぺ", "ぽ", "ゃ", "ゅ", "ょ", "っ", "ア", "イ", "ウ", "エ", "オ",
        "カ", "キ", "ク", "ケ", "コ", "サ", "シ", "ス", "セ", "ソ", "タ", "チ", "ツ", "テ", "ト", "ナ", "ニ", "ヌ", "ネ", "ノ",
        "ハ", "ヒ", "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ", "ヤ", "ユ", "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ワ", "ヲ",
        "ン", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ", "ゼ", "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "バ", "ビ", "ブ", "ベ",
        "ボ", "パ", "ピ", "プ", "ペ", "ポ", "ャ", "ュ", "ョ", "ッ"
    };

    int start = 0;
    for (start; start < line.size(); start+=3) {
        if (kana.find(line.substr(start, 3)) != kana.end()) {
            return true;
        }
    }

    return false;
}

/// @brief Judge if the first charactor of the string is a punctuation.
/// @param line 
/// @return True-is punctuation, False-not.
bool isPunctuation(const std::string& line) {
    std::set<std::string> punctuation = {
        "！"
    };

    std::string firstChar = line.substr(0, 3);

    return punctuation.find(firstChar) != punctuation.end();
}


/// @brief organize the scattered lyrics and remove the hiarachy information.
void organize() {
    std::ifstream inputFile("../temp/formatedHTML.txt", std::ios::in);
    std::ofstream outputFile("../temp/organized.txt", std::ios::out);
    bool chinese = false;

    if (hierachySet.size() < 3) { // 无假名注音
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
                outputFile << line.substr(line.find(" ") + 1) << "\n";
        }
    } else if (hierachySet.size() == 3) { // 有假名注音
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            int hierachy = std::stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ") + 1);
            // 中文/假名
            if (hierachy == *hierachySet.begin()) {
                // 日语歌词
                if (isKana(line) ||
                    isPunctuation(line) || // 针对日语歌词片段全是标点的情况(INTERNET YAMERO: ！！！)
                    chinese || 
                    line.substr(0, 3) == "　" || // 歌词中间的空格 
                    line.substr(0, 1) == " " // 歌词开头的空格(为啥开头会有空格啊喂) 
                    ) {
                    outputFile << line;
                    chinese = false;
                } else { // 中文歌词
                    chinese = true;
                    outputFile << "\n" << line << "\n";
                }
            } else if (hierachy == *hierachySet.rbegin()) { // 括号
                if (isKana(line)) { // 针对没有括号的假名标注
                    outputFile << "（" << line << "）";
                } else { // 括号
                    outputFile << line.substr(0, line.find("\n"));
                }
            } else { // 日语汉字 + 注音
                outputFile << line;
            }
        }
    } else if (hierachySet.size() == 5) { // 中日+假名注音+自带罗马音 
                                          // 我的想法是反正有假名，罗马音就不要了。要加的话后面再加个假名转罗马音的功能
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            std::set<int>::iterator it = hierachySet.begin();
            it++; it++; // 第三个层级，即罗马音左括号
            static bool breakSign = false; // 分段符 鬼知道为啥要用两行括号分段
            int hierachy = std::stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ") + 1);
            if (hierachy == *hierachySet.begin()) { // 中文
                outputFile << "\n" << line << "\n";
                breakSign = true;
            } else if (hierachy == *it) { // 罗马音左括号or罗马音or分段标志左括号
                if (breakSign) { // 分段标志左括号
                    std::getline(inputFile, line, '\n'); // 读掉分段标志右括号
                } else if (line.find("（") != std::string::npos) { // 罗马音
                    std::getline(inputFile, line, '\n');
                    std::getline(inputFile, line, '\n'); // 再读两次把罗马音和罗马音右括号读掉

                } else { // 假名
                    outputFile << line;
                }
                breakSign = false;
            } else { // 其他
                outputFile << line;

                breakSign = false;
            }
        }
    }
}

/// @brief deltete kana notation.
/// @param line 
void deleteNotation(std::string& line) {
    int left = line.find("（");
    while (left != std::string::npos) {
        int right = line.find("）", left);
        line.erase(left, right - left + 3);
        left = line.find("（", left);
    }
}

/// @brief select the language you want and note if you need kana.
/// @param the name of the song
void select(std::string songName) {
    int language;
    int kana;
    int romaji;
    std::string savePath = "../lyric/" + songName + ".txt";
    std::ifstream inputFile("../temp/organized.txt", std::ios::in);
    std::ofstream outputFile(savePath, std::ios::out);

    std::cout << "Plaese select the language you want:\n";
    std::cout << "1. Chinese\n" << "2. Japanese\n" << "3. Both\n";
    std::cin >> language;

    if (language == 1) { // 中文
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            std::getline(inputFile, line, '\n'); // 去掉日语
            outputFile << line << "\n";
        }
    } else if (language == 2) { // 日语
        if (hierachySet.size() < 3) { // 无假名标注
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                outputFile << line << "\n";
                std::getline(inputFile, line, '\n'); // 去掉中文
            }
        } else if (hierachySet.size() >= 3) { // 有假名标注
            std::cout << "Need kana?\n" << "1. Yes\n" << "2. No\n";
            std::cin >> kana;
            
            if (kana == 1) {
                for (std::string line; std::getline(inputFile, line, '\n'); ) {
                    outputFile << line << "\n";
                    std::getline(inputFile, line, '\n'); // 去掉中文
                }
            } else { // 不带假名
                for (std::string line; std::getline(inputFile, line, '\n'); ) {
                    deleteNotation(line);
                    outputFile << line << "\n";
                    std::getline(inputFile, line, '\n'); // 去掉中文
                }
            }
        }
    } else { // 中日
        if (hierachySet.size() < 3) { // 无假名标注
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                outputFile << line << "\n";
            }
        } else if (hierachySet.size() == 3) { // 有假名标注
            std::cout << "Need kana?\n" << "1. Yes\n" << "2. No\n";
            std::cin >> kana;
            
            if (kana == 1) { // 带假名
                for (std::string line; std::getline(inputFile, line, '\n'); ) {
                    outputFile << line << "\n";
                }
            } else { // 不带假名
                for (std::string line; std::getline(inputFile, line, '\n'); ) {
                    deleteNotation(line);
                    outputFile << line << "\n";
                }
            }
        } else if (hierachySet.size() == 5) { // 假名+罗马音标注
            
        }
    }
}

int main() {

    // 创建必要目录
    std::filesystem::create_directory("../temp");
    std::filesystem::create_directory("../lyric");

    std::string fileName = "../HTML/千本樱 - 萌娘百科 万物皆可萌的百科全书.html";

    // std::string fileName = "../HTML/梅菲斯特 - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string fileName = "../HTML/空箱 - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string fileName = "../HTML/栞 - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string fileName = "../HTML/INTERNET OVERDOSE - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string fileName = "../HTML/INTERNET YAMERO - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string fileName = "../HTML/Melt - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string fileName = "../HTML/吉原哀歌 - 萌娘百科 万物皆可萌的百科全书.html";

    int nameLeft = fileName.find_last_of("\\/");
    int nameRight = fileName.find("-");
    std::string name = fileName.substr(nameLeft + 1, nameRight - nameLeft - 2);
    std::cout << "Name: " << name << "\n";

    cut(fileName);

    std::ifstream inputFile("../temp/cuttedHTML.txt", std::ios::in);
    std::string line;
    std::getline(inputFile, line, '\n');
    lineFormation(line);

    organize();

    select(name);

    std::cout << "Done!\n" << "Lyric saved in ../temp/Lyric.txt\n" << "Please save it to somewhere else.\n";

    return 0;
}