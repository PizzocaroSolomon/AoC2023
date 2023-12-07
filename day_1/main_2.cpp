#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>

// Function to convert the spelled-out numbers to digits
int convertSpelledNumber(const std::string& spelledNumber) {
    static const std::map<std::string, int> spelledToDigit = {
        {"one", 1}, {"two", 2}, {"three", 3},
        {"four", 4}, {"five", 5}, {"six", 6},
        {"seven", 7}, {"eight", 8}, {"nine", 9}
    };

    auto it = spelledToDigit.find(spelledNumber);
    return (it != spelledToDigit.end()) ? it->second : 0;
}

int main() {

    std::ifstream inputFile("day_1/input.txt");

    std::vector<std::string> strings_to_find{
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9"
    };

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int sum = 0;
    std::string line;
    int lines_covered{};

    // Process each line in the calibration document
    while (std::getline(inputFile, line)) {
        int lineSum = 0;

        std::size_t min_index = line.size();
        int firstDigit{};
        int lastDigit{};

        // Check if the line is not empty
        if (!line.empty()) {
            lines_covered++;
            long unsigned int not_found {};
            for (auto& str: strings_to_find){
                std::size_t index = line.find(str);
                if(index==std::string::npos){
                    not_found++;
                    continue;
                }

                //first digit
                if(index<min_index){
                    min_index = index;
                    if(convertSpelledNumber(str)!=0){
                        firstDigit = convertSpelledNumber(str);
                    }
                    else{
                        firstDigit = std::stoi(str);
                    }
                }
            }

            std::reverse(line.begin(), line.end());
            min_index = line.size();

            for (auto& str: strings_to_find){
                // line is reversed so lets look for reversed strings
                std::reverse(str.begin(), str.end());
                std::size_t index = line.find(str);
                std::reverse(str.begin(), str.end());

                if(index==std::string::npos){
                    not_found++;
                    continue;
                }

                //last digit
                if(index<min_index){
                    min_index = index;
                    if(convertSpelledNumber(str)!=0){
                        lastDigit = convertSpelledNumber(str);
                    }
                    else{
                        lastDigit = std::stoi(str);
                    }
                }
            }
            if(not_found==strings_to_find.size())
                std::cout<<"error in line "<<lines_covered<<std::endl;
        }

        lineSum = firstDigit*10 + lastDigit;


        // Add the lineSum to the total sum
        sum += lineSum;
        std::reverse(line.begin(), line.end());
        std::cout <<"line sum " << lineSum<< " line  "<< line << std::endl;
    }

    // Close the file
    inputFile.close();

    // Print the sum of calibration values
    std::cout << "Sum of calibration values: " << sum << std::endl;

    return 0;
}
