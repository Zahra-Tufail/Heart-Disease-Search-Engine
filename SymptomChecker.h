#ifndef SYMPTOMCHECKER_H
#define SYMPTOMCHECKER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>
#include <iostream>
#include "Disease.h"

using namespace std;

// =========================================================
// FEATURE 2: Symptom-Based Prediction (Priority Queue + Map)
// =========================================================

// Simple struct to hold result for Priority Queue
struct MatchResult {
    string diseaseName;
    double percentage;

    // Overload < operator for Priority Queue (Max Heap behavior)
    bool operator<(const MatchResult& other) const {
        return percentage < other.percentage;
    }
};

class SymptomChecker {
private:
    // We need access to the Disease List to know what symptoms define a disease
    DiseaseList* diseaseListRef;

public:
    SymptomChecker(DiseaseList* list) {
        diseaseListRef = list;
    }

    // Main Logic: Calculate match % and return top results
    vector<MatchResult> predictDisease(const vector<string>& userSymptoms) {
        priority_queue<MatchResult> maxHeap;
        
        // Traverse the Linked List of all diseases
        Disease* current = diseaseListRef->getHead();
        
        while (current != nullptr) {
            int matches = 0;
            int totalDiseaseSymptoms = current->symptoms.size();

            if (totalDiseaseSymptoms > 0) {
                // Check matches
                // Optimization: In a larger app, we might use a Hash Set for symptoms
                // but for ~5 symptoms per disease, nested loop is perfectly fine and simple.
                for (const string& dSym : current->symptoms) {
                    for (const string& uSym : userSymptoms) {
                        // Simple substring find to be more user friendly (e.g. "pain" matches "chest pain")
                        // Or exact match. Let's do simple cleaning logic if needed, but here simple find:
                        if (dSym.find(uSym) != string::npos || uSym.find(dSym) != string::npos) {
                            matches++;
                            break; // Count once per disease symptom
                        }
                    }
                }

                if (matches > 0) {
                    double percent = ((double)matches / totalDiseaseSymptoms) * 100.0;
                    maxHeap.push({current->name, percent});
                }
            }
            current = current->next;
        }

        // Return top results (sorted by the Priority Queue)
        vector<MatchResult> results;
        while (!maxHeap.empty()) {
            results.push_back(maxHeap.top());
            maxHeap.pop();
        }
        return results;
    }

    // Helper: Get all unique symptoms for the frontend dropdown/checkboxes
    set<string> getUniqueSymptoms() {
        set<string> distinctSymptoms;
        Disease* current = diseaseListRef->getHead();
        while (current != nullptr) {
            for (const string& s : current->symptoms) {
                distinctSymptoms.insert(s);
            }
            current = current->next;
        }
        return distinctSymptoms;
    }
};

#endif
