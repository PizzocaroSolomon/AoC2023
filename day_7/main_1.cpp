#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

class CardHand {
private:
    int bid;
    std::unordered_map<char, int> rankCount;
    std::string hand;
    int type;

public:

    CardHand(int _bid, const std::string& _hand) : bid(_bid), hand(_hand) {
        for (char card : hand) {
            if (card != ' ') {
                rankCount[card]++;
            }
        }

        setType();
    }

    void setType() {

        if (isFiveOfAKind()) {
            type = 7;
        } else if (isFourOfAKind()) {
            type = 6;
        } else if (isFullHouse()) {
            type = 5;
        } else if (isThreeOfAKind()) {
            type = 4;
        } else if (isTwoPair()) {
            type = 3;
        } else if (isOnePair()) {
            type = 2;
        } else {
            type = 1;
        }
    }

    bool isFiveOfAKind() {
        for (const auto& pair : rankCount) {
            if (pair.second == 5) {
                return true;
            }
        }
        return false;
    }

    bool isFourOfAKind() {
        for (const auto& pair : rankCount) {
            if (pair.second == 4) {
                return true;
            }
        }
        return false;
    }

    bool isFullHouse() {
        bool hasThree = false;
        bool hasPair = false;

        for (const auto& pair : rankCount) {
            if (pair.second == 3) {
                hasThree = true;
            } else if (pair.second == 2) {
                hasPair = true;
            }
        }

        return hasThree && hasPair;
    }

    bool isThreeOfAKind() {
        for (const auto& pair : rankCount) {
            if (pair.second == 3) {
                return true;
            }
        }
        return false;
    }

    bool isTwoPair() {
        int pairCount = 0;

        for (const auto& pair : rankCount) {
            if (pair.second == 2) {
                pairCount++;
            }
        }

        return pairCount == 2;
    }

    bool isOnePair() {
        int pairCount = 0;

        for (const auto& pair : rankCount) {
            if (pair.second == 2) {
                pairCount++;
            }
        }

        return pairCount == 1;
    }

    int getBid(){return bid;};

    int getType(){return type;};

    std::string getHand(){return hand;};

    // Comparison operators
    bool operator<(const CardHand& other) const {
        if (type != other.type) {
            return type < other.type;
        }

        // Compare hands character by character
        return std::lexicographical_compare(hand.begin(), hand.end(), other.hand.begin(), other.hand.end(), compareChars);
    }

    bool operator>(const CardHand& other) const {
        return other < *this;
    }

    // Helper function for character comparison
    static bool compareChars(char c1, char c2) {
        // Define the order of characters
        const std::string order = "AKQJT98765432";
        return order.find(c1) > order.find(c2);
    }
};

int main() {

    std::vector<CardHand> hands;

    std::ifstream inputFile("day_7/input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file.\n";
        return 1;
    }

    // Parse each line and create CardHand objects
    std::string line;
    while (std::getline(inputFile, line)) {
        int bid;
        std::string hand;
        std::istringstream valStream(line);
        if (valStream >> hand >> bid) {
            hands.emplace_back(bid, hand);
        } else {
            std::cerr << "Error parsing line: " << line << "\n";
        }
    }

    // Sort the vector in ascending order
    std::sort(hands.begin(), hands.end());

    // Calculate the final result
    int finalResult = 0;
    for (size_t i = 0; i < hands.size(); ++i) {
        std::cout << "pos: " << i << " " << hands[i].getHand() << " "<< hands[i].getBid() << " " << hands[i].getType()<< std::endl;
        finalResult += hands[i].getBid() * (i + 1);
    }

    // Output the final result
    std::cout << "Final Result: " << finalResult << std::endl;

    return 0;
}
