#ifndef HOSPITALRECOMMENDER_H
#define HOSPITALRECOMMENDER_H

#include <vector>
#include <string>
#include <queue>
#include <iostream>

using namespace std;

// =============================================================
// FEATURE 4: Best Hospital Recommendation (Min Heap + Formula)
// =============================================================

struct HospitalData {
    string name;
    string locationNode; // Node name in the AreaGraph
    string fullLocation; // Human readable location
    double rating;       // 1-5 (User provided)

    // Custom Score Calculation
    // Score = Distance + (5.0 - Rating)
    // LOWER score is BETTER.
    double getScore(double distance) const {
        return distance + (5.0 - rating);
    }
};

// Wrapper for Priority Queue
struct HospitalScoreWrapper {
    HospitalData data;
    double score;
    double realDistance;

    // We want Min Heap based on score.
    bool operator>(const HospitalScoreWrapper& other) const {
        return score > other.score; 
    }
};

#include "HospitalGraph.h" // Include to use AreaGraph

class HospitalRecommender {
private:
    vector<HospitalData> db;

public:
    HospitalRecommender() {
        // Initialize with User Provided Data
        // Format: {Name, NodeName, DisplayLocation, Rating}
        db.push_back({"PIMS", "PIMS", "G-8/3 G 8/3 G-8, Islamabad", 3.8});
        db.push_back({"Shifa International", "Shifa", "H 8/4 H-8, Islamabad", 4.2});
        db.push_back({"Kulsum International", "Kulsum", "Block E G 6/2 Blue Area, Islamabad", 3.7});
        db.push_back({"Maroof International", "Maroof", "F-10 Markaz F 10/3 F-10, Islamabad", 3.2}); // Connected to G-10
        db.push_back({"MH Hospital", "MH", "Saddar, Rawalpindi", 4.1});
        db.push_back({"Marya Memorial Hospital", "Marya Memorial Hospital ", "Peshawar Rd , Rawalpindi", 4.5}); // Note space
        db.push_back({"Primax Medical Complex", "Primax Medical Complex", "Murree Rd , Rawalpindi", 4.8});
    }

    vector<HospitalScoreWrapper> getRecommendations(string userArea, AreaGraph& graph) {
        // 1. Get real distances from the graph
        unordered_map<string, double> distances = graph.getShortestPaths(userArea);

        // Min Heap to rank hospitals
        priority_queue<HospitalScoreWrapper, vector<HospitalScoreWrapper>, greater<HospitalScoreWrapper>> minHeap;

        for (const auto& hosp : db) {
            // Check if hospital is reachable
            if (distances.find(hosp.locationNode) != distances.end() && distances[hosp.locationNode] < 1e8) {
                double dist = distances[hosp.locationNode];
                double score = hosp.getScore(dist);
                minHeap.push({hosp, score, dist});
            }
        }

        vector<HospitalScoreWrapper> results;
        while (!minHeap.empty()) {
            results.push_back(minHeap.top());
            minHeap.pop();
        }
        return results;
    }
};

#endif
