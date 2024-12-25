#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/**
 * Reads a file with two tab-separated columns into two lists.
 *
 * @param filename The name of the file to read.
 * @param list1 A reference to the first list to store the data.
 * @param list2 A reference to the second list to store the data.
 * @return True if the file was read successfully, false otherwise.
 */
bool readFileIntoLists(const std::string& filename, std::vector<std::string>& list1, std::vector<std::string>& list2) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string item1, item2;

        // Read two tab-separated items
        if (std::getline(iss, item1, ' ') && std::getline(iss, item2)) {
            item2.erase(0, item2.find_first_not_of(' ')); // Strip leading spaces from item2
            list1.push_back(item1);
            list2.push_back(item2);
        } else {
            std::cerr << "Invalid line format in file " << filename << std::endl;
            file.close();
            return false;
        }
    }

    // Close the file
    file.close();

    return true;
}

/**
 * Sorts the contents of a list in ascending order.
 * 
 * @param list The list to sort.
 */
void bubbleSort(std::vector<std::string>& list) {
    for (int i = 0; i < list.size() - 1; i++) {
        for (int j = 0; j < list.size() - i - 1; j++) {
            if (list[j] > list[j + 1]) {
                std::swap(list[j], list[j + 1]);
            }
        }
    }
}

/** 
 * Gets the absolute difference between the item at each index of two lists.
 * 
 * @param list1 The first list.
 * @param list2 The second list.
 * @return The sum of the absolute differences between the items at each index of the two lists.
 */
int getAbsoluteDifferences(const std::vector<std::string>& list1, const std::vector<std::string>& list2) {
    int differences = 0;
    // verify the two lists are the same size
    if (list1.size() != list2.size()) {
        std::cerr << "The two lists are not the same size." << std::endl;
        return 0;
    }
    for (int i = 0; i < list1.size(); i++) {
        int item1 = std::stoi(list1[i]);
        int item2 = std::stoi(list2[i]);
        differences += std::abs(item1 - item2);
    }
    return differences;
}

/**
 * Gets the similarity by adding up each number in the left list after multiplying it by the number of times that number appears in the right list.
 * 
 * @param list1 The left list, sorted in ascending order.
 * @param list2 The right list, sorted in ascending order.
 * @return The similarity.
 */
int getSimilarity(const std::vector<std::string>& list1, const std::vector<std::string>& list2) {
    int similarity = 0;
    // interate through the left list
    for (int i = 0; i < list1.size(); i++) {
        int item1 = std::stoi(list1[i]);
        int count = 0;
        // iterate through the right list
        for (int j = 0; j < list2.size(); j++) {
            int item2 = std::stoi(list2[j]);
            if (item1 == item2) {
                count++;
            }
            // break if the item in the right list is greater than the item in the left list
            if (item2 > item1) {
                break;
            }
        }
        similarity += item1 * count;
    }

    return similarity;
}


/**
 * Prints the contents of two lists.
 *
 * @param list1 The first list to print.
 * @param list2 The second list to print.
 */
void printLists(const std::vector<std::string>& list1, const std::vector<std::string>& list2) {
    std::cout << "List 1:" << std::endl;
    int i = 0;
    for (const auto& item : list1) {
        if (i < 5) {
            std::cout << item << std::endl;
        } else if ( i >= 5 && i < list1.size() - 6) {
            i++;
            continue;
        } else if (i == list1.size() - 6) {
            for (int j = 0; j < 3; j++) {
                std::cout << '.' << std::endl;
            }
        } else {
            std::cout << item << std::endl;
        }
        i++;
    }

    i = 0;
    std::cout << "List 2:" << std::endl;
    for (const auto& item : list2) {
        if (i < 5) {
            std::cout << item << std::endl;
        } else if ( i >= 5 && i < list2.size() - 6) {
            i++;
            continue;
        } else if (i == list2.size() - 6) {
            for (int j = 0; j < 3; j++) {
                std::cout << '.' << std::endl;
            }
        } else {
            std::cout << item << std::endl;
        }
        i++;
    }
}

int main(int argc, char* argv[]) {
    // Check if the file name was provided as an argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Get the file name from the command-line argument
    std::string filename = argv[1];

    // Vectors to store the lists
    std::vector<std::string> list1;
    std::vector<std::string> list2;

    // Read the file into lists
    if (readFileIntoLists(filename, list1, list2)) {
        // Print the lists

        // Sort the lists
        bubbleSort(list1);
        bubbleSort(list2);

        printLists(list1, list2);

        // Get the sum of the absolute differences between the items at each index of the two lists
        int differences = getAbsoluteDifferences(list1, list2);
        std::cout << "Sum of absolute differences: " << differences << std::endl;

        // Get the similarity
        int similarity = getSimilarity(list1, list2);
        std::cout << "Similarity: " << similarity << std::endl;
    }

    return 0;
}