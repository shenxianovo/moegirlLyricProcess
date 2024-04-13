#include <iostream>
#include <fstream>

void lineFormation(const std::string& line) {
    int hierachy = 0;
    int right = line.find("<");
    std::ofstream outputFile("../temp/formatedHTML.txt", std::ios::out);

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
        outputFile << hierachy << " " << std::string(hierachy * 4, ' ') << content << "\n";
        if (content[0] != '/') {
            hierachy++;
            left = right;
            right = line.find("<", left + 1);
        }
        content = line.substr(left + 1, right - left - 1);
        if (content != "") {
            outputFile << hierachy << " " << std::string(hierachy * 4, ' ') << content << "\n";
        }
    }
}

int main() {
    std::ifstream inputFile("../temp/cuttedHTML.txt", std::ios::in);

    std::string line;

    std::getline(inputFile, line, '\n');

    lineFormation(line);

    return 0;
}
