#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

/**
 * @brief Reads a file and stores its contents into a vector of vectors of characters.
 * 
 * @param filename The path to the file to be read.
 * @param input A reference to a vector of vectors of characters where the parsed data will be stored.
 */
bool readFileIntoVector(const std::string& filename, std::vector<std::vector<char>>& input) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> values(line.begin(), line.end());
        input.push_back(values);
    }
    file.close();
    return true;
}

/**
 * @brief Determine the pairs of non-zero entries that are equal in the matrix.
 * Finds all combinations of pairs of non-zero entries that are equal in the matrix.
 * Stores them as a vector of pairs of pairs of integers (coordinates).
 * 
 * @param input A reference to a vector of vectors of characters representing the matrix.
 * @param pairs A reference to a vector of pairs of pairs of integers where the pairs of non-zero entries that are equal will be stored.
 */
void findEqualPairs(const std::vector<std::vector<char>>& input, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& pairs) {
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] != '.') {
                for (int k = i; k < input.size(); k++) {
                    int l_start = (k == i) ? j + 1 : 0;
                    for (int l = l_start; l < input[k].size(); l++) {
                        if (input[k][l] == input[i][j]) {
                            pairs.push_back(std::make_pair(std::make_pair(i, j), std::make_pair(k, l)));
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Determine the number of antinodes a pair of equal non-zero entries has in the matrix.
 * An antinode is a point in the matrix that falls along the axis that connects two non-zero entries of the same value.
 * The antinode must be twice as far from one of the non-zero entries as it is from the other.
 * If the antinode falls on a non-zero entry, it is not counted.
 * 
 * @param input A reference to a vector of vectors of characters representing the matrix.
 * @param pair A pair of pairs of integers representing the coordinates of the two non-zero entries.
 * @param antinodes vector of pairs of integers where the antinodes will be stored.
 */
void findAntinodes(const std::vector<std::vector<char>>& input, const std::pair<std::pair<int, int>, std::pair<int, int>>& pair, std::vector<std::pair<int, int>>& antinodes) {
    int x1 = pair.first.first;
    int y1 = pair.first.second;
    int x2 = pair.second.first;
    int y2 = pair.second.second;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int x = x1 + dx;
    int y = y1 + dy;
    if (x2 + dx < input.size() && x2 + dx >= 0 && y2 + dy < input[x2 + dx].size() && y2 + dy >= 0) {
        if (find(antinodes.begin(), antinodes.end(), std::make_pair(x2 + dx, y2 + dy)) == antinodes.end()) {
            antinodes.push_back(std::make_pair(x2 + dx, y2 + dy));
        }
    }
    if (x1 - dx >= 0 && x1 - dx < input.size() && y1 - dy >= 0 && y1 - dy < input[x1 - dx].size()) {
        if (find(antinodes.begin(), antinodes.end(), std::make_pair(x1 - dx, y1 - dy)) == antinodes.end()) {
            antinodes.push_back(std::make_pair(x1 - dx, y1 - dy));
        }
    }
}

/**
 * @brief locate the antinodes
 * 
 * Antinodes fall along a line that is drawn through two nonzero matrix entries of the same value. 
 * The antinode is located along this line such that one of the two equal non-zero entries
 * is twice as far from the antinode as the other.
 * 
 * @param input A reference to a vector of vectors of characters representing the matrix.
 * @param total A reference to an integer where the total number of antinodes will be stored.
 * 
 * @return a copy of the input matrix with the antinodes marked with a '9' value.
 */
std::vector<std::vector<char>> locateAntinodes(const std::vector<std::vector<char>>& input, int& total) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>> > pairs;
    std::vector<std::pair<int, int>> uniqueAntinodes;
    findEqualPairs(input, pairs);
    std::cout << "Pairs: " << pairs.size() << std::endl;
    for (const auto& pair : pairs) {
        findAntinodes(input, pair, uniqueAntinodes);
    }
    total = uniqueAntinodes.size();
    std::vector<std::vector<char>> output = input;
    for (const auto& antinode : uniqueAntinodes) {
        output[antinode.first][antinode.second] = '#';
    }
    return output;
}

/**
 * @brief main function
 * 
 * Ingests the filename from the command line arguments, reads the file into a vector of vectors of characters,
 * and if test is in the filename, prints the input. 
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<std::vector<char>> input;
    int total = 0;

    if (readFileIntoVector(filename, input)) {
        if (filename.find("test") != std::string::npos) {
            std::cout << "Input:" << std::endl;
            for (const auto& values : input) {
                for (char c : values) {
                    std::cout << c << " ";
                }
                std::cout << std::endl;
            }
        }
        // Locate the antinodes in the matrix
        std::vector<std::vector<char>> output = locateAntinodes(input, total);
        // emit the total number of antinodes
        std::cout << "Total: " << total << std::endl;
        // if test is in the filename, print the output
        if (filename.find("test") != std::string::npos) {
            std::cout << "Output:" << std::endl;
            for (const auto& values : output) {
                for (char c : values) {
                    std::cout << c << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    return 0;
}
