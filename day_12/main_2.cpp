#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <future>
#include <map>

using namespace std;

// Function to check if the secret code satisfies the sequence
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

unsigned long int combinations(string& secretCode, const vector<int>& sequenceInfo, string& memo){
    auto pos = secretCode.find("?");
    if(pos != std::string::npos){
        std::string possible_code = secretCode;
        possible_code.at(pos) = '#';
        int comb_has = combinations(possible_code, sequenceInfo, memo);

        possible_code = secretCode;
        possible_code.at(pos) = '.';
        int comb_dot = combinations(possible_code, sequenceInfo, memo);
        return comb_has + comb_dot;
    } else{
        if(satisfiesSequence(secretCode, sequenceInfo)){
            memo = secretCode;
            return 1;
        } else{
            return 0;
        }
    }
}

int main() {
    ifstream inputFile("day_12/input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening the file.\n";
        return 1;
    }

    string line;
    unsigned long totalPossibilities = 0;

    while (getline(inputFile, line)) {
        size_t found = line.find_last_of(' ');
        string secretCode = line.substr(0, found);
        string secretCode_T = line.substr(0, found);
        string numbers = line.substr(found + 1);
        string memo;
        unsigned long int comb1{}, comb2{};
        unsigned long int combForFolded;
        vector<int> sequenceInfo;
        stringstream ss(numbers);
        int num;
        while (ss >> num) {
            sequenceInfo.push_back(num);
            if (ss.peek() == ',') ss.ignore();
        }

        // I think that we dont need to compute the combination of all the unfolded string
        // If I find the multiplier in adding only one repetition, then i should obtain the rest
        // multiplying it 4 more times.

        /* My idea is to
            - first, compute the combination of the initial string like in part1
            - second, consider the case in which the question mark in between is a dot
            - third consider the case in which the case in which the question mark is a has.
            - the combination when repeating once should be the sum of these two cases
        */
        // lets compute the combination of the initial string.
        comb1 = combinations ( secretCode, sequenceInfo, std::ref(memo));

        // case 1: XXXX ° XXXX the question mark is treated as dot, the combination
        // should be comb1^2 since each portion is indipendent.
        comb2 += comb1 * comb1;

        secretCode = secretCode + "#" + secretCode;
        sequenceInfo.insert(sequenceInfo.end(),sequenceInfo.begin(), sequenceInfo.end());
        comb2 += combinations ( secretCode, sequenceInfo, std::ref(memo));

        // case 2: here comes the mess. the code might end with ?,# or . and might finish with ?,# or .
        // my idea is to divide this case into 2 cases:
        // - first case the has is "used" in the combination of the left part, hence "not used" in the right part
        //   XXXX# XXXX
        // - first case the has is "used" in the combination of the right part, hence "not used" in the left part
        //   XXXX #XXXX

        // if(secretCode_T.front()!='#'){

        //     secretCode_T = secretCode + "#";
        //     auto right = combinations ( secretCode_T, sequenceInfo, std::ref(memo));

        //     comb2 += comb1*right;
        // }

        // secretCode_T = secretCode;
        // if(secretCode_T.back()!='#'){

        //     secretCode_T = "#" + secretCode;
        //     auto right = combinations ( secretCode_T, sequenceInfo, std::ref(memo));

        //     comb2 += comb1*right;
        // }

        combForFolded = comb1 * static_cast<unsigned long int>(pow(comb2/comb1, 4));

        cout << "Line: " << secretCode_T << " folded combinations: " << combForFolded << endl;
        cout << "-------------------------" << endl;

        totalPossibilities += combForFolded;
    }

    inputFile.close();
    cout << "Total Possibilities: " << totalPossibilities << endl;

    return 0;
}
