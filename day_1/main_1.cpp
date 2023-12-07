#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

int main() {
    std::ifstream inputFile("day_1/input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int sum = 0;
    std::string line;

    // Process each line in the calibration document
    while (std::getline(inputFile, line)) {
        int lineSum = 0;

        // Check if the line is not empty
        if (!line.empty()) {
            // Find the first digit
            char firstDigit = '\0';
            for (char ch : line) {
                if (std::isdigit(ch)) {
                    firstDigit = ch;
                    break;
                }
            }

            // Find the last digit
            char lastDigit = '\0';
            for (auto it = line.rbegin(); it != line.rend(); ++it) {
                if (std::isdigit(*it)) {
                    lastDigit = *it;
                    break;
                }
            }

            // Check if digits are found
            if (std::isdigit(firstDigit) && std::isdigit(lastDigit)) {
                // Convert characters to integers and form a two-digit number
                int calibrationValue = (firstDigit - '0') * 10 + (lastDigit - '0');

                // Add the calibration value to the lineSum
                lineSum = calibrationValue;
            }
        }

        // Add the lineSum to the total sum
        sum += lineSum;
        std::cout <<"line sum " << lineSum<<std::endl;
    }

    // Close the file
    inputFile.close();

    // Print the sum of calibration values
    std::cout << "Sum of calibration values: " << sum << std::endl;

    return 0;
}
