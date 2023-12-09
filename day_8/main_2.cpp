#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

struct Node {
    string left;
    string right;
};

void searchPath(unordered_map<string, Node>& graph, const string& instructions, std::map<std::thread::id, unsigned long int >& steps,
                const string& start_node, std::map<std::thread::id, int >& threads_state_done,
                mutex& mtx) {

    string currentNode = start_node;
    auto thread_id = this_thread::get_id();

    {
        unique_lock<mutex> lock{mtx};
        threads_state_done[thread_id] = 0;
        steps[thread_id] = 0;
    }

    while ( true ) {
        char instruction{};
        // wait for steps to be updated by 1
        {
            unique_lock<mutex> lock(mtx);

            instruction = instructions[steps[thread_id] % instructions.size()];

        }

        Node& currentNodeData = graph[currentNode];

        if (instruction == 'R') {
            currentNode = currentNodeData.right;
        } else if (instruction == 'L') {
            currentNode = currentNodeData.left;
        }

        {
            unique_lock<mutex> lock{mtx};

            // Check if the current path reaches a node ending with 'Z'
            if (currentNode.back() == 'Z') {
                threads_state_done[thread_id] = 2;
                std::cout << "[" << thread_id << "] " << "| node: " << currentNode << " | steps: " << steps[thread_id] << " end node: "<< currentNode <<std::endl;
            } else{
                threads_state_done[thread_id] = 1;
            }

            steps[thread_id]++;
            if(threads_state_done[thread_id] == 2){
                break;
            }
        }

    }
}

unsigned long int calculateLCM(const std::map<std::thread::id, unsigned long int>& values) {
    if (values.empty()) {
        std::cerr << "Map is empty." << std::endl;
        return 0; // Return an appropriate value for an empty map
    }

    // Use accumulate and gcd to calculate the LCM
    return std::accumulate(values.begin(), values.end(), 1ull,
                           [](unsigned long int a, const std::pair<std::thread::id, unsigned long int>& b) {
                               return std::lcm(a, b.second);
                           });
}

int main() {
    unordered_map<string, Node> graph;
    string instructions;
    vector<string> start_nodes;

    // fill graph, instructions and start_nodes
    {
        // Read the graph nodes and neighbors
        ifstream file("day_8/input.txt");
        if (!file.is_open()) {
            cerr << "Error opening the file." << endl;
            return 1;
        }

        getline(file, instructions);

        string line;

        // Read the remaining lines to populate the graph
        while (getline(file, line)) {
            if (line.empty())
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

        // Find all nodes ending with 'A' to start navigation
        for (const auto& entry : graph) {
            if (entry.first.back() == 'A') {
                start_nodes.push_back(entry.first);
            }
        }

    }

    // create threads
    vector<thread> threads;
    mutex mtx;

    std::map< std::thread::id, int > threads_state_done{};
    std::map< std::thread::id, unsigned long int > steps{};

    // threads for path searching starting from start_node
    for (const string& start_node : start_nodes) {
        threads.emplace_back(searchPath, ref(graph), ref(instructions), ref(steps), ref(start_node), ref(threads_state_done), ref(mtx));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    unsigned long int total_steps = calculateLCM(steps);

    std::cout << "Steps needed to reach the end: " << total_steps << endl;

    return 0;
}
