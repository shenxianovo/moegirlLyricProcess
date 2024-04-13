#include <iostream>
#include <fstream>



void organize(const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::in);
    std::ofstream outputFile("../temp/Lyric.txt", std::ios::out);

}

int main() {

    organize("../temp/formatedHTML.txt");

    return 0;
}