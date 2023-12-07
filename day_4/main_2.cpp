#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <regex>

using namespace std;

// Function to calculate the score for a line
void addConsecutiveCards(map<int, int>& deck, int card_number, const vector<int>& winningNumbers, const vector<int>& pulledNumbers) {
    int countContained = 0;
    bool found{false};
    int istances = deck[card_number];
    if(istances==0){
        istances = 1;
        deck[card_number]=istances;
    }
    else{
        std::cout << "found "<< card_number<< " " << deck[card_number]<<std::endl;
        deck[card_number]++;
        istances++;
    }

    for(auto& elm: deck){
        std::cout<<"Card "<<elm.first<<": "<< elm.second<<std::endl;
    }

    for (const int& num : pulledNumbers) {
        if (find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
            countContained+= 1;
        }
    }

    if(countContained>0){
        for(int i=card_number+1; i<= card_number+countContained; i++){
            deck[i]+=istances;
        }
    }

    std::cout << std::endl<<std::endl;

}


int main() {
    ifstream inputFile("day_4/input.txt");
    if (!inputFile) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    string line;
    map<int, int> deck{};

    while (getline(inputFile, line)) {

        size_t pos = line.find(':');

        auto line_after_column = line.substr(pos + 2);
        // Use regex to extract winning numbers and pulled numbers
        regex cardRegex(R"((.+) \| (.+))");
        smatch match;

        if (regex_match(line_after_column, match, cardRegex)) {
            string winningStr = match[1];
            string pulledStr = match[2];

            // Parse numbers from the strings
            vector<int> winningNumbers, pulledNumbers;
            stringstream winningStream(winningStr), pulledStream(pulledStr);
            int num;

            while (winningStream >> num) {
                winningNumbers.push_back(num);
            }

            while (pulledStream >> num) {
                pulledNumbers.push_back(num);
            }

            int cardNumber = stoi(line.substr(4, pos - 4)); // Extract the card number

            // Calculate and accumulate the score for the line
            addConsecutiveCards(deck, cardNumber, winningNumbers, pulledNumbers);

        }
    }

    // Count the number of cards in the deck
    int totalCards = 0;

    for(auto& elm: deck){
        totalCards+=elm.second;
    }

    // Output the overall score and total number of cards
    cout << "Total Number of Cards: " << totalCards << endl;

    inputFile.close();

    return 0;
}
