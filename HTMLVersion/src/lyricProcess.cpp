#include <iostream>
#include <fstream>
#include <set>
#include <locale>
#include <codecvt>

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

bool isKana(const std::string& str) {
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

    // Get the first character of the string
    std::string firstChar = str.substr(0, 3);

    // Check.
    return kana.find(firstChar) != kana.end();
}


void organize(const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::in);
    std::ofstream outputFile("../temp/organized.txt", std::ios::out);

    // 中日歌词
    if (hierachySet.size() == 1 || hierachySet.size() == 2) {
        std::cout << "Plaese select the language you want:\n";
        std::cout << "1. Chinese\n" << "2. Japanese\n" << "3. Both\n";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                std::getline(inputFile, line, '\n');
                outputFile << line.substr(line.find(" ") + 1) << "\n";
            }
        } else if (choice == 2) {
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                outputFile << line.substr(line.find(" ") + 1) << "\n";
                std::getline(inputFile, line, '\n');
            }
        } else {
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                outputFile << line.substr(line.find(" ") + 1) << "\n";
            }
        }
    }
    // 中日 + 注音
    if (hierachySet.size() == 3) {
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            int hierachy = std::stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ") + 1);
            // 中文/假名
            if (hierachy == *hierachySet.begin()) {
                // 日语歌词
                if (isKana(line)) {
                    outputFile << line;
                } else { // 中文歌词
                    outputFile << "\n" << line << "\n";
                }
            } else if (hierachy == *hierachySet.rbegin()) { // 括号
                outputFile << line.substr(0, line.find("\n"));
            } else { // 日语汉字 + 注音
                outputFile << line;
            }
        }
    }

}

void select() {
    int language;
    int kana;
    std::ifstream inputFile("../temp/organized.txt", std::ios::in);
    std::ofstream outputFile("../Lyric.txt", std::ios::out);

    std::cout << "Plaese select the language you want:\n";
    std::cout << "1. Chinese\n" << "2. Japanese\n" << "3. Both\n";
    std::cin >> language;

    if (language == 1) { // 中文
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            std::getline(inputFile, line, '\n');
            outputFile << line << "\n";
        }
    } else if (language == 2) {
        std::cout << "Need kana?\n" << "1. Yes\n" << "2. No\n";
        std::cin >> kana;
        
        if (kana == 1) {
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                outputFile << line << "\n";
                std::getline(inputFile, line, '\n');
            }
        } else { // 不带假名
            for (std::string line; std::getline(inputFile, line, '\n'); ) {
                // 删去（）及其内容
                int left = line.find("（");
                while (left != std::string::npos) {
                    int right = line.find("）", left);
                    line.erase(left, right - left + 3);
                    left = line.find("（", left);
                }
                outputFile << line << "\n";
            }
        }
    } else {
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            outputFile << line << "\n";
        }
    }
}

int main() {

    std::string fileName = "../HTML/INTERNET OVERDOSE - 萌娘百科 万物皆可萌的百科全书.html";

    cut(fileName);

    std::ifstream inputFile("../temp/cuttedHTML.txt", std::ios::in);
    std::string line;
    std::getline(inputFile, line, '\n');
    lineFormation(line);

    organize("../temp/formatedHTML.txt");

    if (hierachySet.size() == 1 || hierachySet.size() == 2 ) {
        std::ifstream inputFile("../temp/organized.txt", std::ios::in);
        std::ofstream outputFile("../Lyric.txt", std::ios::out);
        for (std::string line; std::getline(inputFile, line, '\n'); ) {
            outputFile << line << "\n";
        }
    }

    if (hierachySet.size() >= 3) {
        select();
    } 

    std::cout << "Done!\n" << "Lyric saved in ../temp/Lyric.txt\n" << "Please save it to somewhere else.\n";

    return 0;
}