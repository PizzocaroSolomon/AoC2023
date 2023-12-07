#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

int main() {

    std::ifstream inputFile("day_6/input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    // Read and process each race
    std::string line;
    std::vector<int> times;
    std::vector<int> distances;
    std::vector<int> results;

    while (getline(inputFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        if(line.find("Time:") != std::string::npos){
            std::istringstream valStream(line.substr(line.find(":") + 1));
            long int val;
            while (valStream >> val) {
                times.push_back(val);
            }
        }

        if(line.find("Distance:") != std::string::npos){
            std::istringstream valStream(line.substr(line.find(":") + 1));
            long int val;
            while (valStream >> val) {
                distances.push_back(val);
            }
        }
    }

    for(unsigned long int i=0; i < times.size(); i++){
        // Calculate result using the formula
        auto time = times[i];
        auto distance = distances[i];
        auto x1 = (-time + std::sqrt(std::pow(time, 2) - 4 * distance))/(-2);
        auto x2 = (-time - std::sqrt(std::pow(time, 2) - 4 * distance))/(-2);
        std::cout << "roots : " << x1 << " "<< x2 << std::endl;

        auto d1 = x1*(time-x1);
        auto d2 = x2*(time-x2);
        std::cout << "distances : " << d1 << " "<< d2 << " record: "<< distance <<std::endl;

        int result = static_cast<int>(std::round(std::sqrt(std::pow(time, 2) - 4 * distance)));

        if (!(std::abs(x1 - std::round(x1)) > 0)) {
            std::cout << "has no decimals! "<< std::endl;
            result = result - 1;
        }
        else{
            result = static_cast<int>(std::floor(x2)) - static_cast<int>(std::ceil(x1)) + 1;
        }

        results.push_back(result);
    }

    // Output the results
    auto mult{1};
    std::cout << "Results: ";
    for (int result : results) {
        mult*=result;
        std::cout << result << " ";
    }
    std::cout << std::endl;

    std::cout << "The number of ways multiplied is: " << mult <<std::endl;


    return 0;
}
