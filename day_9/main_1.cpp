#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sstream>

// Recursive function to compute differences and estimate the next value
 long int recursiveDifferenceAndEstimate(std::vector< long int>& sequence) {

    if (std::all_of(sequence.begin(), sequence.end(), []( long int diff) { return diff == 0; })) {
        // All differences are zero, stop recursion
        return sequence.back();  // Return the last element as the estimated next value
    }

    // Compute differences
    std::vector< long int> differences;
    for (size_t i = 1; i < sequence.size(); ++i) {
        differences.push_back(sequence[i] - sequence[i - 1]);
    }

    // Continue recursion with the differences
    for(const auto& num: sequence)
        std::cout << num << " " ;
    std::cout << std::endl;

    return sequence.back() + recursiveDifferenceAndEstimate(differences);
}

int main() {

    // Open the file
    std::ifstream inputFile("day_9/input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

     long int totalSum = 0;

    // Process each line in the file
    std::string line;
    while (std::getline(inputFile, line)) {
        // Parse the sequence from the line
        std::vector< long int> sequence;
        std::istringstream iss(line);
         long int num;
        while (iss >> num) {
            sequence.push_back(num);
        }
        std::cout << "------------------------------------------------" << std::endl;

        // Call the recursive function to compute differences and estimate the next value
         long int nextValue = recursiveDifferenceAndEstimate(sequence);
        std::cout << "next value: " << nextValue << std::endl;

        // Add the next value to the total sum
        totalSum += nextValue;
    }

    // Close the file
    inputFile.close();

    // Output the total sum of estimated next values
    std::cout << "Total sum of estimated next values: " << totalSum << std::endl;

    return 0;
}
