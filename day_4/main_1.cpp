#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <regex>

using namespace std;

// Function to calculate the score for a line
int calculateScore(const vector<int>& winningNumbers, const vector<int>& pulledNumbers) {
    int countContained = 0;

    for (const int& num : pulledNumbers) {
        if (find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
            countContained++;
        }
    }

    return pow(2, countContained - 1);
}

int main() {
    ifstream inputFile("day_4/input.txt");
    if (!inputFile) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    string line;
    int overallScore = 0;

    while (getline(inputFile, line)) {

        size_t pos = line.find(':');

        auto line_after_column = line.substr(pos + 2);
        // Use regex to extract winning numbers and pulled numbers
        regex cardRegex(R"((.+) \| (.+))");
        smatch match;

        if (regex_match(line_after_column, match, cardRegex)) {
            // int cardNumber = stoi(match[1]);
            string winningStr = match[1];
            string pulledStr = match[2];

            std::cout<<"winning: "<< winningStr<< " pulled: "<<pulledStr<<std::endl;

            // Parse numbers from the strings
            vector<int> winningNumbers, pulledNumbers;
            stringstream winningStream(winningStr), pulledStream(pulledStr);
            int num;

            while (winningStream >> num) {
                winningNumbers.push_back(num);
            }

            while (pulledStream >> num) {
                pulledNumbers.push_back(num);
            }

            // Calculate and accumulate the score for the line
            overallScore += calculateScore(winningNumbers, pulledNumbers);
        }
    }

    // Output the overall score
    cout << "Overall Score: " << overallScore << endl;

    inputFile.close();

    return 0;
}
