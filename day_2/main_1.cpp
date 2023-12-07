#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

// Function to check if a round is feasible
bool isFeasible(const std::unordered_map<std::string, int>& cubeCounts, const std::string& roundData) {
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
                    return false;
                }
            }
        }
    }

    return true;
}

int main() {

    std::ifstream inputFile("day_2/input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> cubeCounts = {{"red", 12}, {"green", 13}, {"blue", 14}};
    std::string line;
    int feasibleRoundsSum = 0;

    // Process each line in the file
    while (std::getline(inputFile, line)) {
        if(line.empty())
            continue;

        // Find the game ID
        size_t pos = line.find(':');
        int gameID = std::stoi(line.substr(5, pos - 5));

        // Check if the round is feasible
        if (isFeasible(cubeCounts, line.substr(pos + 2))) {
            feasibleRoundsSum += gameID;
        }
    }

    // Close the file
    inputFile.close();

    // Print the sum of feasible round IDs
    std::cout << "Sum of feasible round IDs: " << feasibleRoundsSum << std::endl;

    return 0;
}
