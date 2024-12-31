#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <future>

/**
 * @brief Reads a file and stores its contents into a vector of vectors of long integers.
 *
 * This function reads a file specified by the filename and parses its contents line by line.
 * Each line is expected to have a format where a solution (long integer) is followed by a colon
 * and then a series of long integers separated by spaces. The solution and the series of values
 * are stored in a vector, which is then added to the input vector.
 *
 * @param filename The path to the file to be read.
 * @param input A reference to a vector of vectors of long integers where the parsed data will be stored.
 * @return true if the file was successfully opened and read, false otherwise.
 */
bool readFileIntoVector(const std::string& filename, std::vector<std::vector<long>>& input) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string solution_str;
        std::getline(iss, solution_str, ':');
        long solution = std::stol(solution_str);
        std::string values_str;
        std::getline(iss, values_str);
        
        std::istringstream values_stream(values_str);
        std::vector<long> values;
        values.push_back(solution);
        long value;
        while (values_stream >> value) {
            values.push_back(value);
        }
        
        input.push_back(values);
    }
    file.close();
    return true;
}

/**
 * @brief Helper function to recursively check if a series of values can be combined to reach a target value.
 * 
 * This function recursively checks if a series of values can be combined using addition and multiplication
 * to reach a target value. The function uses memoization to store intermediate results and avoid redundant
 * calculations.
 * @param values A reference to a vector of long integers representing the series of values.
 * @param target The target value that the series of values should reach.
 * @param index The current index in the series of values.
 * @param current The current value obtained by combining the series of values.
 * @param memo A reference to an unordered map used for memoization.
 * @return true if the series of values can be combined to reach the target value, false otherwise.
 */
bool canCombineToTargetHelper(std::vector<long>& values, long target, int index, long current, std::unordered_map<std::string, bool>& memo) {
    if (index == values.size()) {
        return current == target;
    }
    if (current > target) {
        return false;
    }
    std::string key = std::to_string(index) + "," + std::to_string(current);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    bool result = canCombineToTargetHelper(values, target, index + 1, current + values[index], memo) ||
                  canCombineToTargetHelper(values, target, index + 1, current * values[index], memo);
    memo[key] = result;
    return result;
}

/**
 * @brief Checks if a series of values can be combined to reach a target value.
 * 
 * This function checks if a series of values can be combined using addition and multiplication
 * to reach a target value. The function calls a helper function that uses memoization to store
 * intermediate results and avoid redundant calculations.
 * @param values A reference to a vector of long integers representing the series of values.
 * @param target The target value that the series of values should reach.
 * @return true if the series of values can be combined to reach the target value, false otherwise.
 */
bool canCombineToTarget(std::vector<long>& values, long target) {
    std::unordered_map<std::string, bool> memo;
    return canCombineToTargetHelper(values, target, 2, values[1], memo);
}

/**
 * @brief Helper function to recursively check if a series of values can be combined to reach a target value using concatenation.
 * 
 * This function recursively checks if a series of values can be combined using addition, multiplication, and concatenation
 * to reach a target value. The function uses memoization to store intermediate results and avoid redundant calculations.
 * @param values A reference to a vector of long integers representing the series of values.
 * @param target The target value that the series of values should reach.
 * @param index The current index in the series of values.
 * @param current The current value obtained by combining the series of values.
 * @param memo A reference to an unordered map used for memoization.
 * @return true if the series of values can be combined to reach the target value, false otherwise.
 */
bool canCombineToTargetWithConcatHelper(std::vector<long>& values, long target, int index, long current, std::unordered_map<std::string, bool>& memo) {
    if (index == values.size()) {
        return current == target;
    } 
    if (current > target) {
        return false;
    }
    std::string key = std::to_string(index) + "," + std::to_string(current);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    bool result = canCombineToTargetWithConcatHelper(values, target, index + 1, current + values[index], memo) ||
                  canCombineToTargetWithConcatHelper(values, target, index + 1, current * values[index], memo) ||
                  canCombineToTargetWithConcatHelper(values, target, index + 1, std::stol(std::to_string(current) + std::to_string(values[index])), memo);
    memo[key] = result;
    return result;
}

/**
 * @brief Checks if a series of values can be combined to reach a target value using concatenation.
 * 
 * This function checks if a series of values can be combined using addition, multiplication, and concatenation
 * to reach a target value. The function calls a helper function that uses memoization to store
 * intermediate results and avoid redundant calculations.
 * @param values A reference to a vector of long integers representing the series of values.
 * @param target The target value that the series of values should reach.
 * @return true if the series of values can be combined to reach the target value, false otherwise.
 */
bool canCombineToTargetWithConcat(std::vector<long>& values, long target) {
    std::unordered_map<std::string, bool> memo;
    return canCombineToTargetWithConcatHelper(values, target, 2, values[1], memo);
}

/**
 * @brief Checks if a series of values can be combined to reach a target value and adds the solution to a total.
 * 
 * This function iterates over a vector of vectors of long integers, where each vector contains a solution
 * followed by a series of values. The function checks if the series of values can be combined to reach the
 * solution and adds the solution to a total if it is possible.
 * @param input A reference to a vector of vectors of long integers.
 * @param total A reference to a long integer where the total of the solutions will be stored.
 * @param useConcat A boolean flag indicating whether concatenation should be used in the calculations.
 */
void checkSolutions(const std::vector<std::vector<long>>& input, long& total, bool useConcat = false) {
    std::vector<std::future<long>> futures;
    for (const auto& values : input) {
        futures.push_back(std::async(std::launch::async, [values, useConcat]() -> long {
            long solution = values[0];
            if (useConcat) {
                if (canCombineToTargetWithConcat(const_cast<std::vector<long>&>(values), solution)) {
                    return solution;
                }
            } else {
                if (canCombineToTarget(const_cast<std::vector<long>&>(values), solution)) {
                    return solution;
                }
            }
            return 0;
        }));
    }

    for (auto& future : futures) {
        total += future.get();
    }
}

/**
 * @brief Main function
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<std::vector<long>> input;
    long total = 0;

    if (readFileIntoVector(filename, input)) {
        if (filename.find("test") != std::string::npos) {
            std::cout << "Input:" << std::endl;
            for (const auto& values : input) {
                for (long i : values) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;
            }
        }
        checkSolutions(input, total);
        std::cout << "Total: " << total << std::endl;
        total = 0;
        checkSolutions(input, total, true);
        std::cout << "Total with concat: " << total << std::endl;
    }
    return 0;
}
