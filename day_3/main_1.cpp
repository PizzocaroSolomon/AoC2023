#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct numberIndices{
    long unsigned int left_index{};
    long unsigned int right_index{};
    int number{};
};

// Function to check if a character is a symbol
bool isSymbol(char c) {
    return !isdigit(c)&&c!='.' ;
}

// Function to check if a position is valid in the grid
bool isValid(int i, int j, int rows, int cols) {
    return i >= 0 && i < rows && j >= 0 && j < cols;
}

// Function to check if a number is a part number
bool isPartNumber(vector<string>& grid, long unsigned int i, numberIndices& number) {
    // Check in the 8 adjacent squares
    for ( int x = static_cast<int>(i) - 1; x <= static_cast<int>(i) + 1; ++x) {
        for (int y = static_cast<int>(number.left_index) - 1; y <= static_cast<int>(number.right_index) + 1; ++y) {
            if (isValid(x, y, grid.size(), grid[0].size()) && isSymbol(grid[x][y])) {
                std::cout << "is part! symbol " << grid[x][y] << std::endl;
                return true;
            }
        }
    }
    return false;
}

// Function to merge consecutive digits into a single number
numberIndices mergeDigits(string& line, long unsigned int& index) {
    numberIndices result{};
    result.left_index = index;

    while (isdigit(line[index])) {
        result.number = result.number * 10 + (line[index] - '0');
        result.right_index = index;
        index++;
    }
    return result;
}


int main() {
    ifstream inputFile("day_3/input.txt");
    if (!inputFile) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    vector<string> grid;
    string line;

    // Read the input file into a grid
    while (getline(inputFile, line)) {
        grid.push_back(line);
    }

    inputFile.close();

    int sum = 0;

    // Iterate through the grid to find and sum up part numbers
    for (long unsigned int i = 0; i < grid.size(); ++i) {
        for (long unsigned int j = 0; j < grid[i].size();) {
            if (isdigit(grid[i][j]) ) {
                numberIndices merged_number = mergeDigits(grid[i], j);
                std::cout << "the merged number is "<< merged_number.number<< " indices "<< merged_number.left_index<< ","<< merged_number.right_index<<std::endl;
                j = merged_number.right_index+1;
                if(isPartNumber(grid, i, merged_number)){
                    sum += merged_number.number;
                }
            }
            j++;
        }
    }

    // Output the sum of part numbers
    cout << "Sum of part numbers: " << sum << endl;

    return 0;
}
