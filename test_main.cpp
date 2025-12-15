#include <iostream>
#include "Disease.h"
#include "SymptomChecker.h"
#include "HospitalGraph.h"
#include "HospitalRecommender.h"

using namespace std;

// ==========================================
// CONSOLE TESTER (For verifying Logic)
// ==========================================

int main() {
    cout << "=== HEART DISEASE SEARCH ENGINE (CONSOLE TEST) ===" << endl;

    // 1. TEST LINKED LIST
    cout << "\n[Testing Feature 1: Disease Linked List]" << endl;
    DiseaseList dList;
    dList.populateSampleData();
    Disease* d = dList.getDiseaseDetails("Heart Attack");
    if(d) {
        cout << "Found: " << d->name << " | Severity: " << d->severity << endl;
        cout << "Symptoms: " << d->symptoms[0] << ", " << d->symptoms[1] << "..." << endl;
    } else {
        cout << "Failed to find Heart Attack" << endl;
    }

    // 2. TEST SYMPTOM CHECKER
    cout << "\n[Testing Feature 2: Symptom Prediction]" << endl;
    SymptomChecker checker(&dList);
    vector<string> mySymptoms = {"chest", "pain", "nausea"};
    vector<MatchResult> results = checker.predictDisease(mySymptoms);
    
    cout << "Prediction Results for 'chest, pain, nausea':" << endl;
    for(const auto& r : results) {
        cout << "- " << r.diseaseName << ": " << r.percentage << "% match" << endl;
    }

    // 3. TEST GRAPH
    cout << "\n[Testing Feature 3: Nearest Hospital]" << endl;
    AreaGraph graph;
    graph.setupIslamabadMap();
    
    // Test from G-10
    PathResult res = graph.findNearestHospital("G-10");
    cout << "User at G-10. Nearest Hospital: " << res.hospitalName << endl;
    cout << "Distance: " << res.totalDistance << " km" << endl;
    cout << "Path: ";
    for(const auto& node : res.path) cout << node << " -> ";
    cout << "END" << endl;

    // 4. TEST RECOMMENDATIONS
    cout << "\n[Testing Feature 4: Hospital Rankings]" << endl;
    HospitalRecommender recommender;
    vector<HospitalScoreWrapper> recs = recommender.getRecommendations("G-10", graph);
    
    cout << "Top Recommendations (Lowest Score is Best):" << endl;
    for(const auto& r : recs) {
        cout << r.data.name << " (" << r.data.fullLocation << ") | Score: " << r.score << " | Dist: " << r.realDistance << "km | Rating: " << r.data.rating << endl;
    }

    return 0;
}
