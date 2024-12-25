#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

/**
 * Reads the input file into a single string
 * 
 * @param filename The name of the file to read.
 * @param input A reference to the string to store the input.
 */
bool readFileIntoString(const std::string& filename, std::string& input) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    // Read the file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    input = buffer.str();
    // Close the file
    file.close();
    return true;
}

/**
 * Helper to conduct the regex match
 * We are looking for sequences in the string that match mul(<int>, <int>)
 * Where each int as a 1-3 digit number
 * We are also looking for do() and don't() matches
 * @param input The input string to search.
 * @param matches A reference to the vector to store the matches.
 * @param includeDo Flag to include do() matches.
 */
void findMatches(const std::string& input, std::vector<std::string>& matches, bool includeDo = false) {
    // Regular expression to match the patterns
    std::regex pattern; 
    pattern = "mul\\([0-9]{1,3},[0-9]{1,3}\\)"; 
    if (includeDo) {
        pattern = "mul\\([0-9]{1,3},[0-9]{1,3}\\)|do\\(\\)|don't\\(\\)";
    }
    // Iterator to search the input string
    std::sregex_iterator it(input.begin(), input.end(), pattern);
    std::sregex_iterator end;
    // Iterate over the matches and store them in the vector
    while (it != end) {
        matches.push_back(it->str());
        ++it;
    }
}

/**
 * Helper to execute a multiplication match
 * 
 * @param match The string that contains the match.
 * @return The result of the multiplication.
 */
int executeMatch(const std::string& match) {
    // Extract the two numbers from the match
    int num1StartIdx = match.find("(") + 1;
    int num1EndIdx = match.find(",");
    int num2EndIdx = match.find(")");
    int num1 = std::stoi(match.substr(num1StartIdx, num1EndIdx - num1StartIdx));
    int num2 = std::stoi(match.substr(num1EndIdx + 1, num2EndIdx - num1EndIdx - 1));
    // Multiply the numbers and return the result
    return num1 * num2;
}

/**
 * Helper to check if a match is a do() match
 * 
 * @param match The string that contains the match.
 * @return true if the match is a do() match, false otherwise.
 */
bool isDoMatch(const std::string& match) {
    return match.find("do(") != std::string::npos;
}

/**
 * Helper to check if a match is a don't() match
 * 
 * @param match The string that contains the match.
 * @return true if the match is a don't() match, false otherwise.
 */
bool isDontMatch(const std::string& match) {
    return match.find("don't(") != std::string::npos;
}

/** 
 * Helper to executes the list of matched multiplications and sum them up
 * 
 * @param matches The vector of matches to execute.
 * @param result A reference to the integer to store the result.
 */
void executeMatches(const std::vector<std::string>& matches, int& result) {
    // Iterate over the matches
    bool doFlag = true;
    for (const std::string& match : matches) {
        if (isDoMatch(match)) {
            doFlag = true;
            continue;
        } else if (isDontMatch(match)) {
            doFlag = false;
            continue;
        }
        if (doFlag) {
            result += executeMatch(match);
        }
    }
}

/**
 * Main block. Receives a filename as an argument and reads the file into a single string.
 * 
 */
int main(int argc, char* argv[]) {
    // Check if the file name was provided as an argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    // Get the file name from the command-line argument
    std::string filename = argv[1];
    // String to store the input
    std::string input;
    // list of strings to store the matches
    std::vector<std::string> matches;
    // Result of the summed multiplications
    int result = 0;
    // matches include do() and dont() matches
    std::vector<std::string> doMatches;
    // Result of the summed multiplications with do() and dont() matches
    int doResult = 0;
    // Read the file into input
    if (readFileIntoString(filename, input)) {
        // Print the first few characters of the input
        std::cout << "First few characters of the input:" << std::endl;
        for (int i = 0; i < 50 && i < input.size(); i++) {
            std::cout << input[i];
        }
        std::cout << std::endl;
        // Find the matches in the input
        findMatches(input, matches);
        // Print the first few matches
        std::cout << "First few matches:" << std::endl;
        for (int i = 0; i < 5 && i < matches.size(); i++) {
            std::cout << matches[i] << std::endl;
        }
        // Execute the matches and sum the multiplications
        executeMatches(matches, result);
        // Print the result
        std::cout << "Result: " << result << std::endl;
        // Find the matches in the input including do() and dont()
        findMatches(input, doMatches, true);
        // Print the first few matches with do() and dont()
        std::cout << "First few matches with do() and dont():" << std::endl;
        for (int i = 0; i < 5 && i < doMatches.size(); i++) {
            std::cout << doMatches[i] << std::endl;
        }
        // Execute the matches with do() and dont() and sum the multiplications
        executeMatches(doMatches, doResult);
        // Print the result with do() and dont()
        std::cout << "Result with do() and dont(): " << doResult << std::endl;
    }
    return 0;
}