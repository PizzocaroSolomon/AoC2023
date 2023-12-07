#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

// Function to check if a round is feasible
int gamePower(const std::string& roundData) {

    std::unordered_map<std::string, int> cubeCounts = {{"red", 0}, {"green", 0}, {"blue", 0}};

    std::stringstream ss_game(roundData);
    std::string round;
    while(std::getline(ss_game, round, ';'))
    {
        std::stringstream ss_round(round);
        std::string cubes;
        while(std::getline(ss_round, cubes, ',')){
            for(auto& expected: cubeCounts){
                std::size_t pos = cubes.find(expected.first);
                if(pos==std::string::npos)
                    continue;

                int cube_count = std::stoi(cubes.substr(0, pos - 1));
                std::cout<< "number fo " << cubes << " is " << cube_count<<std::endl;

                if (cube_count > expected.second) {
                    expected.second = cube_count;
                }
            }
        }
    }

    int power = 1;
    for(auto& expected: cubeCounts){
        power *= expected.second;
    }
    return power;
}

int main() {

    std::ifstream inputFile("day_2/input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    int gamePowerSum = 0;

    // Process each line in the file
    while (std::getline(inputFile, line)) {
        if(line.empty())
            continue;

        // Find the game ID
        size_t pos = line.find(':');

        // Check if the round is feasible
        gamePowerSum += gamePower(line.substr(pos + 2));

    }

    // Close the file
    inputFile.close();

    // Print the sum of feasible round IDs
    std::cout << "Sum of games power is: " << gamePowerSum << std::endl;

    return 0;
}
