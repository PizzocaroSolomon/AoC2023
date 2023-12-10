#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Define the directions: N, S, E, W
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

struct Point {
    int x, y;
};

int findLoopSteps(vector<string>& grid, Point start) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> distance(n, vector<int>(m, INT_MAX));
    distance[start.x][start.y] = 0;

    queue<Point> q;
    q.push(start);

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        for (int dir = 0; dir < 4; ++dir) {
            int nx = current.x + dx[dir];
            int ny = current.y + dy[dir];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] != '.') {
                int nd = distance[current.x][current.y] + 1;
                // Check for valid pipe connections
                if(grid[current.x][current.y]!='S'){
                    if ((grid[current.x][current.y] == '|' && (dir == 0 || dir == 1)) ||
                        (grid[current.x][current.y] == '-' && (dir == 2 || dir == 3)) ||
                        (grid[current.x][current.y] == 'L' && (dir == 0 || dir == 3)) ||
                        (grid[current.x][current.y] == 'J' && (dir == 0 || dir == 2)) ||
                        (grid[current.x][current.y] == '7' && (dir == 1 || dir == 2)) ||
                        (grid[current.x][current.y] == 'F' && (dir == 1 || dir == 3))) {

                        if (nd < distance[nx][ny]) {
                            distance[nx][ny] = nd;
                            q.push({nx, ny});
                        }
                    }
                }
                else{
                    if ((grid[nx][ny] == '|' && (dir == 0 || dir == 1)) ||
                        (grid[nx][ny] == '-' && (dir == 2 || dir == 3)) ||
                        (grid[nx][ny] == 'L' && (dir == 1 || dir == 2)) ||
                        (grid[nx][ny] == 'J' && (dir == 1 || dir == 3)) ||
                        (grid[nx][ny] == '7' && (dir == 0 || dir == 3)) ||
                        (grid[nx][ny] == 'F' && (dir == 0 || dir == 2))) {

                        if (nd < distance[nx][ny]) {
                            distance[nx][ny] = nd;
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
    }

    int maxDistance = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != '.' && distance[i][j] != INT_MAX) {
                maxDistance = max(maxDistance, distance[i][j]);
            }
        }
    }

    return maxDistance;
}


int main() {
    ifstream inputFile("day_10/input.txt");

    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    vector<string> grid;
    string line;

    while (getline(inputFile, line)) {
        grid.push_back(line);
    }

    inputFile.close();

    int n = grid.size();
    int m = grid[0].size();

    Point start;

    // Find the starting position
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'S') {
                start.x = i;
                start.y = j;
                break;
            }
        }
    }

    int steps = findLoopSteps(grid, start);

    cout << "Steps along the loop to the farthest point from the starting position: " << steps << endl;

    return 0;
}
