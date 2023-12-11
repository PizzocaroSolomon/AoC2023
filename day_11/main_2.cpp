#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <climits>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

struct Cell {
    int row, col;

    bool operator==(const Cell& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Cell& other) const {
        return !(*this == other);
    }

    bool operator<(const Cell& other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
};

void galaxyToGalaxyDist(const vector<vector<char>> grid, const Cell start, const Cell end, map<Cell, map<Cell, int>>& memo) {

    if (start == end) {
        memo[start][end] = 0;
        return; // Same cell, no movement needed
    }

    if (memo[start].find(end) != memo[start].end()) {
        return;
    }

    auto dist = abs(end.row - start.row) + abs(end.col - start.col);
    memo[start][end] = dist;
    memo[end][start] = dist;
}

int main() {
    ifstream inputFile("day_11/input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening the file.\n";
        return 1;
    }

    vector<vector<char>> grid;
    string line;
    while (getline(inputFile, line)) {
        grid.push_back(vector<char>(line.begin(), line.end()));
    }

    inputFile.close();

    int numRows = grid.size();
    int numCols = grid[0].size();

    map<Cell, map<Cell, int>> memo;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (grid[i][j] == '#') {
                for (int x = 0; x < numRows; ++x) {
                    for (int y = 0; y < numCols; ++y) {
                        if (grid[x][y] == '#' && (i != x || j != y)) {
                            Cell start = {i, j};
                            Cell end = {x, y};

                            galaxyToGalaxyDist(grid, start, end, memo);

                        }
                    }
                }
            }
        }
    }

    long int increment = 999999;

    // Expand rows and columns
    for (int i = 0; i < numRows; ++i) {
        if (find(grid[i].begin(), grid[i].end(), '#') == grid[i].end()) {

            for (auto& outerPair : memo) {
                if (outerPair.first.row < i) {
                    for (auto& innerPair : outerPair.second) {
                        if(innerPair.first.row > i){
                            memo[innerPair.first][outerPair.first]+=increment;
                            memo[outerPair.first][innerPair.first]+=increment;
                        }
                    }
                }
            }

        }
    }

    for (int j = 0; j < numCols; ++j) {
        bool hasGalaxy = false;
        for (int i = 0; i < numRows; ++i) {
            if (grid[i][j] == '#') {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy) {
            for (auto& outerPair : memo) {
                if (outerPair.first.col < j) {
                    for (auto& innerPair : outerPair.second) {
                        if(innerPair.first.col > j){
                            memo[innerPair.first][outerPair.first]+=increment;
                            memo[outerPair.first][innerPair.first]+=increment;
                        }
                    }
                }
            }
        }
    }


    long int totalShortestPaths = 0;

    for (auto& outerPair : memo) {
        for (auto& innerPair : outerPair.second) {
            totalShortestPaths += memo[innerPair.first][outerPair.first];
        }

    }
    cout << "Sum of shortest paths between pairs of galaxies: " << totalShortestPaths/2 << endl;

    return 0;
}
