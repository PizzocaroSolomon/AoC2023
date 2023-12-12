#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Function to calculate the total number of possibilities for a given line
bool satisfiesSequence(const string& secretCode, const vector<int>& sequenceInfo) {
    int groupIndex = 0;
    int consecutiveHashCount = 0;

    for (char c : secretCode) {

        if (c == '#') {
            // If it's '#', increment consecutiveHashCount
            consecutiveHashCount++;
        } else if (c == '.') {
            // If it's '.', check if consecutiveHashCount satisfies the current group size
            if (consecutiveHashCount > 0) {
                if (consecutiveHashCount != sequenceInfo[groupIndex]) {
                    // If the group size is not satisfied, return false
                    return false;
                }
                consecutiveHashCount = 0; // Reset consecutiveHashCount for the next group
                groupIndex++;
            }
        }
    }

    if(groupIndex < sequenceInfo.size()){
        if(consecutiveHashCount==0){
            return false;
        } else if(consecutiveHashCount!= sequenceInfo[groupIndex]) {
            return false;
        } else{
            groupIndex++;
            consecutiveHashCount = 0;
        }
    }

    if(consecutiveHashCount!=0 && groupIndex==sequenceInfo.size()){
        return false;
    }

    if(groupIndex != sequenceInfo.size()){
        return false;
    }

    return true; // If all conditions are satisfied, return true
}

unsigned long int combinations(string& secretCode, const vector<int>& sequenceInfo){
    auto pos = secretCode.find("?");
    if(pos != std::string::npos){
        std::string possible_code = secretCode;
        possible_code.at(pos) = '#';
        int comb_has = combinations(possible_code, sequenceInfo);

        possible_code = secretCode;
        possible_code.at(pos) = '.';
        int comb_dot = combinations(possible_code, sequenceInfo);
        return comb_has + comb_dot;
    } else{
        if(satisfiesSequence(secretCode, sequenceInfo)){
            return 1;
        } else{
            return 0;
        }
    }
}

int main() {
    // Open the input file
    ifstream inputFile("day_12/input.txt");

    // Check if the file is open
    if (!inputFile.is_open()) {
        cerr << "Error opening the file.\n";
        return 1;
    }

    // Process each line in the file
    string line;
    int totalPossibilities = 0;

    while (getline(inputFile, line)) {
        // Extract secret code and sequence information from each line
        size_t found = line.find_last_of(' ');
        string secretCode = line.substr(0, found);
        string numbers = line.substr(found + 1);

        // Split the sequence of numbers
        vector<int> sequenceInfo;
        stringstream ss(numbers);
        int num;
        while (ss >> num) {
            sequenceInfo.push_back(num);

            if (ss.peek() == ',') {
                ss.ignore();
            }
        }

        // Call the function to check if the secret code satisfies the sequence information
        int isSatisfied = combinations(secretCode, sequenceInfo);

        // Output the result for each line
        cout << "Line: " << line << " combinations: " << isSatisfied<<endl;
        cout << "-------------------------" << endl;

        totalPossibilities+=isSatisfied;
    }

    // Close the file
    inputFile.close();

    // Output the total possibilities
    cout << "Total Possibilities: " << totalPossibilities << endl;

    return 0;
}
