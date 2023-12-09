#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Node {
    string left;
    string right;
};

int main() {
    unordered_map<string, Node> graph;

    // Read the instructions
    ifstream file("day_8/input.txt");
    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    string instructions;
    getline(file, instructions);

    // Read the graph nodes and neighbors
    string line;
    while (getline(file, line)) {
        std::cout << line << std::endl;
        if(line.empty())
            continue;

        size_t pos = line.find('=');
        string nodeName = line.substr(0, pos - 1);
        string neighbors = line.substr(pos + 3, line.length() - pos - 4);

        size_t commaPos = neighbors.find(',');
        string leftNeighbor = neighbors.substr(0, commaPos);
        string rightNeighbor = neighbors.substr(commaPos + 2);

        graph[nodeName] = {leftNeighbor, rightNeighbor};
    }

    // Close the file
    file.close();

    // Navigation
    string currentNode = "AAA";
    int steps = 0;
    while(currentNode!="ZZZ"){

        for (char instruction : instructions) {
            Node& currentNodeData = graph[currentNode];
            if (instruction == 'R') {
                currentNode = currentNodeData.right;
            } else if (instruction == 'L') {
                currentNode = currentNodeData.left;
            }
            steps++;
            if (currentNode == "ZZZ") {
                cout << "Steps needed to reach ZZZ: " << steps << endl;
                return 0;
            }
        }
    }

    return 0;
}
