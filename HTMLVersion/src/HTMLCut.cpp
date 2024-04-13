#include <iostream>
#include <fstream>

void cut(const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::in);
    std::ofstream outputFile("../temp/cuttedHTML.txt", std::ios::out);

    for (std::string line; std::getline(inputFile, line, '\n'); ) {
        if (line.find("本段落中所使用的歌词，其著作权属于原著作权人，仅以介绍为目的引用。") != std::string::npos) {
            outputFile << line << "\n";
        }
    }
}

int main() {
    std::string inputFileName = "../HTML/INTERNET OVERDOSE - 萌娘百科 万物皆可萌的百科全书.html";

    cut(inputFileName);

    return 0;
}