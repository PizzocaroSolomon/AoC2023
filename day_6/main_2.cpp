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
    std::string times;
    std::string distances;
    unsigned long int time{};
    unsigned long int distance{};

    while (getline(inputFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        if(line.find("Time:") != std::string::npos){
            std::istringstream valStream(line.substr(line.find(":") + 1));
            char val;
            while (valStream >> val) {
                if (isdigit(val)) {
                    times.push_back(val);
                }
            }
        }

        if(line.find("Distance:") != std::string::npos){
            std::istringstream valStream(line.substr(line.find(":") + 1));
            char val;
            while (valStream >> val) {
                if (isdigit(val)) {
                    distances.push_back(val);
                }
            }
        }
    }

    std::cout << "times " << times << " distances " << distances << std::endl;
    std::cout << "time " << std::stoul(times) << " distance " << std::stoul(distances) << std::endl;

    time = std::stoul(times);
    distance = std::stoul(distances);

    // Calculate result using the formula
    auto x1 = (-time + std::sqrt(std::pow(time, 2) - 4 * distance))/(-2);
    auto x2 = (-time - std::sqrt(std::pow(time, 2) - 4 * distance))/(-2);
    std::cout << "roots : " << x1 << " "<< x2 << std::endl;

    auto d1 = x1*(time-x1);
    auto d2 = x2*(time-x2);
    std::cout << "distances : " << d1 << " "<< d2 << " record: "<< distance <<std::endl;

    auto result = static_cast<unsigned long int>(std::round(std::sqrt(std::pow(time, 2) - 4 * distance)));

    if (!(std::abs(x1 - std::round(x1)) > 0)) {
        std::cout << "has no decimals! "<< std::endl;
        result = result - 1;
    }
    else{
        result = static_cast<unsigned long int>(std::floor(x2)) - static_cast<unsigned long int>(std::ceil(x1)) + 1;
    }



    // Output the results
    std::cout << "The number of ways multiplied is: " << result <<std::endl;


    return 0;
}
