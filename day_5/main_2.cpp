#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct dataRange{
    long int start;
    long int range;

};

struct dataRangeCollection{
    std::vector<dataRange> data;
};

struct Mapping{
    dataRangeCollection dest;
    dataRangeCollection source;
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

dataRangeCollection parseSeeds(const std::string& line) {
    dataRangeCollection seeds;
    std::istringstream seedStream(line.substr(line.find(":") + 1));
    long int seed;
    long int seed_range;
    while (seedStream >> seed) {
        seedStream>> seed_range;
        dataRange data_range{seed, seed_range};
        seeds.data.push_back(data_range);
    }
    return seeds;
}

Mapping parseMappingSection(std::vector<std::string>& lines, size_t& currentIndex) {
    Mapping mapping;

    for (++currentIndex; currentIndex < lines.size(); ++currentIndex) {
        const std::string& line = lines[currentIndex];

        if (line.empty()) {
            break;
        }

        std::istringstream dataStream(line);
        long int dest{}, source{}, length{};
        dataStream >> dest >> source >> length;

        mapping.dest.data.push_back({dest, length});
        mapping.source.data.push_back({source, length});
    }

    return mapping;
}


long int unfoldChain(std::vector<Mapping>& mapping, dataRangeCollection& initialNumbers) {
    dataRangeCollection current_sources = initialNumbers;
    dataRangeCollection already_used_sources{};
    dataRangeCollection next_sources{};

    for(Mapping& map: mapping){

        next_sources.data.clear();

        std::cout<<"-------------------------------- searching -------------------------------------" << std::endl;

        for(auto& source: current_sources.data){

            already_used_sources.data.clear();
            std::cout<< std::endl;
            std::cout<< "source --> "<<"start: "<< source.start << " end: "<< source.start + source.range<< std::endl;
            std::cout<< std::endl;

            int times_found{0};
            for(unsigned long int i=0; i < map.source.data.size(); i++){
                std::cout<< "checking source --> "<<"start: "<< map.source.data[i].start << " end: "<< map.source.data[i].start + map.source.data[i].range << std::endl;
                std::cout<< "checking dest --> "<<"start: "<< map.dest.data[i].start << " end: "<< map.dest.data[i].start + map.dest.data[i].range << std::endl<<std::endl;

                auto start_start_dist = source.start - map.source.data[i].start;
                auto end_end_dist =  (source.start + source.range) - (map.source.data[i].start + map.source.data[i].range);
                auto end_start_dist =  (source.start + source.range) - map.source.data[i].start;
                auto start_end_dist = source.start - (map.source.data[i].start + map.source.data[i].range);

                // case included inside
                if( start_start_dist >= 0 && end_end_dist <= 0 ){

                    long int start = map.dest.data[i].start + start_start_dist;
                    long int range = source.range;
                    next_sources.data.push_back({start,range});
                    already_used_sources.data.push_back(source);

                    std::cout<< "included inside! "<< std::endl;
                    std::cout<< "added from dest --> "<<"start: "<< start << " end: "<< start + range << std::endl;
                    times_found++;
                }

                // case included outside
                if( start_start_dist <= 0 && end_end_dist >= 0){

                    long int start = map.dest.data[i].start;
                    long int range = map.dest.data[i].range;
                    next_sources.data.push_back({start,range});
                    already_used_sources.data.push_back(map.source.data[i]);

                    times_found++;

                    std::cout<< "included outside! "<< std::endl;
                    std::cout<< "added from dest --> "<<"start: "<< start << " end: "<< start + range << std::endl;

                }

                // case half in left
                if( start_start_dist <= 0 && end_start_dist >= 0 && end_end_dist < 0){

                    long int start = map.dest.data[i].start;
                    long int range = (source.start + source.range) - map.source.data[i].start;
                    next_sources.data.push_back({start,range});
                    already_used_sources.data.push_back({map.source.data[i].start, range});
                    times_found++;

                    std::cout<< "half left! "<< std::endl;
                    std::cout<< "added from dest --> "<<"start: "<< start << " end: "<< start + range << std::endl;

                }

                // case half in right
                if( end_end_dist >= 0 && start_start_dist >= 0 && start_end_dist <= 0){

                    long int start = map.dest.data[i].start + start_start_dist;
                    long int range = (map.source.data[i].start + map.source.data[i].range) - source.start;
                    next_sources.data.push_back({start,range});
                    already_used_sources.data.push_back({map.source.data[i].start + start_start_dist, range});
                    times_found++;

                    std::cout<< "half right " << std::endl;
                    std::cout<< "added from dest --> "<<"start: "<< start << " end: "<< start + range << std::endl;

                }
            }
            if(times_found==0){
                next_sources.data.push_back(source);
                std::cout<< "not found in mapping adding source! "<< std::endl;
            } else{

                std::sort(already_used_sources.data.begin(), already_used_sources.data.end(),
                            [](const dataRange& a, const dataRange& b) {
                                return a.start < b.start;
                            });

                auto curr_start = source.start;

                for(auto& used: already_used_sources.data){
                    if((used.start - curr_start) > 0){
                        next_sources.data.push_back({curr_start, used.start - curr_start});
                        std::cout << "added residual " <<"start: "<< curr_start << " end: "<< used.start - 1 << std::endl;
                    }
                    curr_start = used.start + used.range + 1;
                }

                auto end_end_distance = (source.start + source.range) - (already_used_sources.data.back().start + already_used_sources.data.back().range);
                if(end_end_distance > 0){
                    auto start = already_used_sources.data.back().start + already_used_sources.data.back().range + 1;
                    next_sources.data.push_back({start, end_end_distance-1});
                    std::cout << "added last residual " <<"start: "<< start << " end: "<< start + end_end_distance - 1  << std::endl;
                }
            }
        }
        std::cout<<"-------------------------------- end of search -------------------------------------" << std::endl;

        current_sources = next_sources;
        std::cout << "next sources " << std::endl;
        for(const auto& elm: next_sources.data){
            std::cout << "start: " << elm.start << " end "<< elm.start + elm.range<< " ";
        }
        std::cout << std::endl;
    }

    std::vector<long int> locations;
    for(auto& loc: next_sources.data)
        locations.push_back(loc.start);

    return *std::min_element(locations.begin(),locations.end());
}

int main() {
    const std::string inputFilePath = "day_5/input.txt";
    std::vector<std::string> lines = readInputFile(inputFilePath);

    dataRangeCollection seeds;
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
