#include <iostream>
#include <fstream>

// 似乎爬虫处理的更好(?

// enum Mode {
//     CHINESE = 0x00, // 中文歌词
//     JAPANESE = 0x02, // 日文歌词
//     ROMAJI = 0x04, // 日语注音
// };

// int mode = CHINESE || JAPANESE || ROMAJI;

/*
    0 中日音
    1 中日
    2 中
    3 日音
    4 日
*/

int mode = 3;

void processEmptyBrackets(std::string& line) {
    auto index = line.find("（）");
    // 只有中文
    if (mode == 2) {
        if (index != std::string::npos) {
            line.replace(0, index + 6, "");
        }
    }


    // 有中文
    if (mode == 0 || mode == 1) {
        if (index != std::string::npos) {
            line.replace(index, 6, "\n");
        }
    }

    // 没中文
    if (mode == 3 || mode == 4) {
        if (index != std::string::npos) {
                line.replace(index, line.size() - index, "");
            }
    }
    return;
}

void deleteBracketContents(std::string& line) {
    size_t start, end;
    // 有注音
    while ((start = line.find("[")) != std::string::npos) {
        end = line.find("]");
        line.erase(start, end - start + 1);
    }

    // 没注音
    if (mode == 1 ||mode == 4) {
        while ((start = line.find("（")) != std::string::npos) {
            end = line.find("）");
            line.erase(start, end - start + 3);
        }
    }
    return;
}

void replaceBracket(std::string& line) {
    int pos = 0;
    while (pos < line.size()) {
        if (line.substr(pos, 3) == "（") {
            line.replace(pos, 3, "(");
        } else if (line.substr(pos, 3) == "）") {
            line.replace(pos, 3, ")");
        } else if (line.substr(pos, 3) == "　") {
            line.replace(pos, 3, " ");
        }
        pos++;
    }
    return;
}

void deleteReadingMode(std::string& line) {
    if (line.find("阅读更多：") != std::string::npos || line.find("本文引自萌娘百科") != std::string::npos) {
        line.clear();
    }
    return;
}

int main() {

    mode = 4;

    std::ifstream lyricInput("yuuhizaka.txt", std::ios::in);
    std::ofstream lyricOutput("output.txt", std::ios::out);

    for (std::string line; std::getline(lyricInput, line, '\n'); ) {
        
        processEmptyBrackets(line);
        deleteBracketContents(line);
        replaceBracket(line);
        deleteReadingMode(line);

        lyricOutput << line << "\n";
    }

    return 0;
}