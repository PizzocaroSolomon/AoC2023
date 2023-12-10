#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Define the directions: N, S, E, W
const int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
const int dy[] = {0, 0, -1, 1, -1, 1, 1, -1};

struct Point {
    int x, y;
};

void floodFill(vector<vector<int>>& grid, Point start, vector<vector<bool>>& visited, int& number_of_tiles, bool& is_inside) {
    int n = grid.size();
    int m = grid[0].size();

    queue<Point> q;
    q.push(start);
    visited[start.x][start.y] = true;

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        if(grid[current.x][current.y] == INT_MAX)
            number_of_tiles++;

        for (int dir = 0; dir < 8; ++dir) {
            int nx = current.x + dx[dir];
            int ny = current.y + dy[dir];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny] && (grid[nx][ny]==-2 || grid[nx][ny]==INT_MAX)) {
                visited[nx][ny] = true;
                if((nx == 0 || nx == n-1 || ny == 0 || ny == m-1) )
                    is_inside = false;
                q.push({nx, ny});
            }

        }
    }
}

int findIncludedNumbers(vector<string>& grid, Point start) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> distance(n, vector<int>(m, INT_MAX));
    vector<vector<bool>> valid_grid(n, vector<bool>(m, false));
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

    vector<vector<int>> augmented(2*n, vector<int>(2*m, -2));

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            augmented[i*2][j*2] = distance[i][j];
        }
    }

    for(int i = 0; i < 2*n; ++i){
        for(int j = 0; j < 2*m; ++j){
            if(augmented[i][j]== -2){
                // check left right
                int lx = i - 1, rx = i + 1;
                // check up down
                int uy = j - 1, dy = j + 1;

                bool left_right_condition = lx>=0 && rx < 2*n && abs(augmented[lx][j] - augmented[rx][j])==1;
                bool up_down_condition = uy>=0 && dy < 2*m && abs(augmented[i][uy] - augmented[i][dy])==1;

                if(left_right_condition)
                    augmented[i][j] = augmented[lx][j];

                if(up_down_condition)
                    augmented[i][j] = augmented[i][uy];
            }
        }
    }

    // Initialize visited matrix for flood-fill
    vector<vector<bool>> visited(2*n, vector<bool>(2*m, false));

    // Perform flood-fill to mark the tiles within the loop
    int number_of_tiles {0};
    bool is_inside{false};
    Point start_flood{};

    while(!is_inside){
        for(int i = 0; i < 2*n; ++i){
            for(int j = 0; j < 2*m; ++j){
                if(augmented[i][j]==-2 && !visited[i][j]){
                    start_flood.x = i;
                    start_flood.y = j;
                }
            }
        }

        is_inside = true;
        number_of_tiles = 0;
        floodFill(augmented, start_flood, visited, number_of_tiles, is_inside);

    }

    return number_of_tiles;
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

    int steps = findIncludedNumbers(grid, start);

    cout << "number of tiles included is : " << steps << endl;

    return 0;
}
