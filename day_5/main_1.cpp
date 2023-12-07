#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct Mapping{
    std::vector<long int> start_dest;
    std::vector<long int> start_source;
    std::vector<long int> range;
};

std::vector<std::string> readInputFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::vector<long int> parseSeeds(const std::string& line) {
    std::vector<long int> seeds;
    std::istringstream seedStream(line.substr(line.find(":") + 1));
    long int seed;

    while (seedStream >> seed) {
        seeds.push_back(seed);
    }

    return seeds;
}

Mapping parseMappingSection(std::vector<std::string>& lines, size_t& currentIndex) {
    Mapping mapping;

    for (++currentIndex; currentIndex < lines.size(); ++currentIndex) {
        const std::string& line = lines[currentIndex];
        std::cout << line << std::endl;
        if (line.empty()) {
            break;
        }

        std::istringstream dataStream(line);
        long int dest{}, source{}, length{};
        dataStream >> dest >> source >> length;

        mapping.start_dest.push_back(dest);
        mapping.start_source.push_back(source);
        mapping.range.push_back(length);
    }

    return mapping;
}

long int unfoldChain(std::vector<Mapping>& mapping, const std::vector<long int>& initialNumbers) {
    std::vector<long int> current_sources = initialNumbers;
    std::vector<long int> next_sources{};

    std::cout << "initial sources " << std::endl;
    for(const auto& elm: current_sources){
        std::cout << elm << " ";
    }
    std::cout << std::endl;

    for(Mapping& map: mapping){

        next_sources.clear();
        for(long int& source: current_sources){
            bool found{false};
            for(long int i=0; i < map.start_source.size(); i++){
                if(map.start_source[i]<= source && source <= (map.start_source[i] + map.range[i])){
                    auto source_dist = source - map.start_source[i];
                    next_sources.push_back(map.start_dest[i] + source_dist);
                    found = true;
                    break;
                }
            }
            if(!found){
                next_sources.push_back(source);
            }
        }
        current_sources = next_sources;
        std::cout << "next sources " << next_sources.size() <<std::endl;
        for(const auto& elm: next_sources){
            std::cout << elm << " ";
        }
        std::cout << std::endl;
    }

    return *std::min_element(next_sources.begin(),next_sources.end());
}

int main() {
    const std::string inputFilePath = "day_5/input.txt";
    std::vector<std::string> lines = readInputFile(inputFilePath);

    std::vector<long int> seeds;
    std::vector<Mapping> allMappings;

    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];

        if (line.find("seeds") != std::string::npos) {
            seeds = parseSeeds(line);
        } else if (line.find("map:") != std::string::npos) {
            std::cout << "processing " << line << std::endl;
            allMappings.push_back(parseMappingSection(lines, i));
        }
    }

    std::cout << "created mappings "<< std::endl;

    long int result = unfoldChain(allMappings, seeds);
    std::cout << "result " << result << std::endl;

    return 0;
}
