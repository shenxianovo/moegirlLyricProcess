#include <iostream>
#include <fstream>
#include <set>


std::set<int> hierachySet; // 被迫开个全局变量

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

int main() {
    std::ifstream inputFile("../temp/cuttedHTML.txt", std::ios::in);

    std::string line;

    std::getline(inputFile, line, '\n');

    lineFormation(line);

    std::cout << "层级数: " << hierachySet.size() << std::endl;

    return 0;
}
