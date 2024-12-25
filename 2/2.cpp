#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/**
 * Reads a file of reports into a nested vector. 
 * Each report is in a new line, and each report field is an int separated by a space.
 * 
 * @param filename The name of the file to read.
 * @param reports A reference to the vector to store the reports.
 */

bool readFileIntoLists(const std::string& filename, std::vector<std::vector<int>>& reports) {
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
        std::vector<int> report;
        int item;
        while (iss >> item) {
            report.push_back(item);
        }
        reports.push_back(report);
    }

    // Close the file
    file.close();

    return true;
}

/** 
 * Helper to check the safety of a single report.
 * 
 * @param report The report to check.
 * @return true if the report is safe, false otherwise.
 */
bool isSafe(const std::vector<int>& report) {
    // Check if the levels are increasing or decreasing
    bool increasing = true;
    bool decreasing = true;
    for (int i = 1; i < report.size(); i++) {
        if (report[i] > report[i - 1]) {
            decreasing = false;
        } else if (report[i] < report[i - 1]) {
            increasing = false;
        }
    }

    // Check if the levels differ by at least one and at most three
    bool diff = true;
    for (int i = 1; i < report.size(); i++) {
        if (std::abs(report[i] - report[i - 1]) < 1 || std::abs(report[i] - report[i - 1]) > 3) {
            diff = false;
        }
    }

    return (increasing || decreasing) && diff;
}

/** 
 * Determines if a report is to be considered safe. A report is considered safe if both of the following 
 * conditions are met:
 * - The levels are either all increasing or all decreasing.
 * - Any two adjacent levels differ by at least one and at most three.
 * 
 * @param report The report to check.
 * @param numSafe The number of safe reports.
 * @return unsafeReports - the nested vector of reports that are not safe
 */
std::vector<std::vector<int>> numSafeReports(const std::vector<std::vector<int>>& reports, int& numSafe) {
    std::vector<std::vector<int>> unsafeReports;
    // interate through the reports
    for (int i = 0; i < reports.size(); i++) {
        const std::vector<int>& report = reports[i];
        if (isSafe(report)) {
            numSafe++;
        } else {
            unsafeReports.push_back(report);
        } 
    }
    return unsafeReports;
}

/**
 * Number of safer reports with a report level dampener
 * Report safety follows the same criteria as numSafeReports, but this time we can tolerate a single level that falls outside of the rules
 * 
 * @param unsafeReports The nested vector of reports that are not safe.
 * @param numSafeDampened The number of safe reports with a dampened level.
 */
void numSafeReportsDampened(const std::vector<std::vector<int>>& unsafeReports, int& numSafeDampened) {
    // iterate through the unsafe reports
    for (int i = 0; i < unsafeReports.size(); i++) {
        const std::vector<int>& report = unsafeReports[i];
        // iterate through the levels of the report
        for (int j = 0; j < report.size(); j++) {
            // check the safety of the report with each level removed
            std::vector<int> reportDampened = report;
            reportDampened.erase(reportDampened.begin() + j);
            if (isSafe(reportDampened)) {
                numSafeDampened++;
                break;
            }
        }
    }
}

/** 
 * Main block. Receives a filename as an argument and reads the file into a nested vector.
 * 
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

    // Vector to store the reports
    std::vector<std::vector<int>> reports;

    // Integers to store the number of safe reports
    int numSafe = 0;

    // vector of unsafe reports without dampening
    std::vector<std::vector<int>> unsafeReports;

    // Int to store the number of safe reports with dampening
    int numSafeDampened = 0;

    // Read the file into reports
    if (readFileIntoLists(filename, reports)) {
        // Print the first few reports
        std::cout << "First few reports:" << std::endl;
        for (int i = 0; i < 5 && i < reports.size(); i++) {
            for (int j = 0; j < reports[i].size(); j++) {
                std::cout << reports[i][j] << " ";
            }
            std::cout << std::endl;
        }

        // Determine the number of safe reports
        unsafeReports = numSafeReports(reports, numSafe);
        std::cout << "Number of safe reports: " << numSafe << std::endl;

        // Determine the number of safe reports with a dampened level
        numSafeReportsDampened(unsafeReports, numSafeDampened);
        std::cout << "Number of safe reports with a dampened level: " << numSafeDampened + numSafe << std::endl;
    }

    return 0;
}
