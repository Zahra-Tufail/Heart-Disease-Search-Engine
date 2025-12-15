#ifndef HOSPITALGRAPH_H
#define HOSPITALGRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// ============================================
// FEATURE 3: Nearest Hospital (Graph + Dijkstra)
// ============================================

// A simple structure to represent a neighbor node
struct Edge {
    string targetNode;
    double distance; // Changed to double for precision
};

// Return structure for the Frontend
struct PathResult {
    string hospitalName;
    double totalDistance;
    vector<string> path; // The sequence of areas: Start -> Node -> Hospital
};

class AreaGraph {
private:
    // Adjacency List: Area Name -> List of (Neighbor, Distance)
    unordered_map<string, vector<Edge>> adjList;
    
    // List of known hospitals to check against
    vector<string> hospitalLocations;

public:
    void addArea(string areaName) {
        // Just ensures the key exists
        if (adjList.find(areaName) == adjList.end()) {
            adjList[areaName] = {};
        }
    }

    void addRoad(string u, string v, double dist) {
        adjList[u].push_back({v, dist});
        adjList[v].push_back({u, dist}); // Undirected graph (road goes both ways)
    }

    void addHospitalLocation(string areaName) {
        hospitalLocations.push_back(areaName);
    }
    
    void setupIslamabadMap() {
        // Creating a simplified map of Islamabad Sectors
        vector<string> areas = {"G-10", "G-11", "G-9", "F-10", "F-11", "F-8", "Blue Area", "H-8", "Saddar"};
        for(const auto& area : areas) addArea(area);
        
        // Adding Roads (Edges) with approximate distances in KM
        addRoad("G-11", "G-10", 1.9);
        addRoad("G-10", "G-9", 3.0);
        addRoad("G-9", "F-8", 5.0);
        addRoad("F-10", "F-11", 1.8);
        addRoad("F-10", "G-10", 3.3);
        addRoad("F-8", "Blue Area", 5.5);
        addRoad("G-9", "H-8",4.0); // Shifa is near H-8
        addRoad("Blue Area", "Saddar", 17.9);
        addRoad("F-8", "F-7", 3.2);
        
        // PIMS is in G-8 (connected to G-9) -> We'll simplify and put PIMS near G-9
        addArea("PIMS");
        addRoad("G-9", "G-8", 2.0); // Connect G-8 to G-9
        addRoad("G-8", "PIMS", 1.6);
        addHospitalLocation("PIMS");
        
        // Shifa International in H-8
        addArea("Shifa");
        addRoad("H-8", "Shifa", 1.1);
        addHospitalLocation("Shifa");
        
        // Kulsum is in Blue Area
        addArea("Kulsum");
        addRoad("Blue Area", "Kulsum", 1.0);
        addHospitalLocation("Kulsum");

        // Maroof International in G-10
        addArea("Maroof");
        addRoad("G-10", "Maroof", 1.4);
        addHospitalLocation("Maroof");

        //MH hospital in Saddar
        addArea("MH");
        addRoad("Saddar", "MH", 1.1);
        addHospitalLocation("MH");

        //Marya Memorial Hospital 
        addArea("Marya Memorial Hospital ");
        addRoad("Saddar", "Marya Memorial Hospital ", 3.1);
        addHospitalLocation("Marya Memorial Hospital ");

        //Primax Medical Complex
        addArea("Primax Medical Complex");
        addRoad("Saddar", "Primax Medical Complex", 1.0);
        addHospitalLocation("Primax Medical Complex");
    }

    // DIJKSTRA'S ALGORITHM
    PathResult findNearestHospital(string startNode) {
        if (adjList.find(startNode) == adjList.end()) {
            return {"Unknown Area", -1, {}};
        }

        // Priority Queue for Dijkstra: {Distance, NodeName}
        // Min Heap uses greater by default logic for pairs? No, typically we need custom.
        // Let's use standard min heap with pairs.
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        
        unordered_map<string, double> dist;
        unordered_map<string, string> parent; // To reconstruct path

        for (auto& pair : adjList) {
            dist[pair.first] = 1e9; // 1e9 as INF equivalent for double
        }

        dist[startNode] = 0.0;
        pq.push({0.0, startNode});

        string nearestHospital = "";
        double minHospitalDist = 1e9;

        while (!pq.empty()) {
            double d = pq.top().first;
            string u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            // Check if this node is a hospital
            for (const string& hosp : hospitalLocations) {
                if (u == hosp) {
                    // Start logic: found a hospital. 
                    // Since Dijkstra guarantees shortest path to 'u', and 'u' is a hospital,
                    // we can stop if we only want the *absolute* nearest.
                    // But maybe there's a closer one in queue? 
                    // Dijkstra explores by distance, so the FIRST hospital we pop from PQ is guaranteed to be the nearest.
                    nearestHospital = u;
                    minHospitalDist = d;
                    goto Found; // Break out of both loops
                }
            }

            for (auto& edge : adjList[u]) {
                if (dist[u] + edge.distance < dist[edge.targetNode]) {
                    dist[edge.targetNode] = dist[u] + edge.distance;
                    parent[edge.targetNode] = u;
                    pq.push({dist[edge.targetNode], edge.targetNode});
                }
            }
        }

    Found:
        if (nearestHospital == "") {
            return {"No Hospital Found", -1, {}};
        }

        // Reconstruct Path
        vector<string> path;
        string curr = nearestHospital;
        while (curr != startNode) {
            path.push_back(curr);
            curr = parent[curr];
        }
        path.push_back(startNode);
        reverse(path.begin(), path.end());

        return {nearestHospital, minHospitalDist, path};
    }

    // New method for Feature 4: Get all distances from startNode
    unordered_map<string, double> getShortestPaths(string startNode) {
        unordered_map<string, double> dist;
        if (adjList.find(startNode) == adjList.end()) {
            return dist; // Empty map if invalid start
        }

        for (auto& pair : adjList) {
            dist[pair.first] = 1e9;
        }

        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        dist[startNode] = 0.0;
        pq.push({0.0, startNode});

        while (!pq.empty()) {
            double d = pq.top().first;
            string u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto& edge : adjList[u]) {
                if (dist[u] + edge.distance < dist[edge.targetNode]) {
                    dist[edge.targetNode] = dist[u] + edge.distance;
                    pq.push({dist[edge.targetNode], edge.targetNode});
                }
            }
        }
        return dist;
    }
    
    vector<string> getAreas() {
        vector<string> areas;
        for(auto const& [key, val] : adjList) {
            // Filter out internal hospital nodes usually, but for simple dropdowns return all non-hospital areas
            bool isHosp = false;
            for(const auto& h : hospitalLocations) if(h == key) isHosp = true;
            if(!isHosp) areas.push_back(key);
        }
        return areas;
    }
};

#endif
