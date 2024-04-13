#include <iostream>
#include <fstream>

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

int main() {
    // std::string inputFileName = "../HTML/空箱 - 萌娘百科 万物皆可萌的百科全书.html"; // 无罗马音 可直接用
    // std::string inputFileName = "../HTML/INTERNET OVERDOSE - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string inputFileName = "../HTML/千本樱 - 萌娘百科 万物皆可萌的百科全书.html";
    // std::string inputFileName = "../HTML/栞 - 萌娘百科 万物皆可萌的百科全书.html";
    std::string inputFileName = "../HTML/梅菲斯特 - 萌娘百科 万物皆可萌的百科全书.html";

    cut(inputFileName);

    return 0;
}