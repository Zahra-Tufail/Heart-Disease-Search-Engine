#ifndef DISEASE_H
#define DISEASE_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// ==========================================
// FEATURE 1: Linked List for Disease Storage
// ==========================================

struct Disease {
    string name;
    string description;
    vector<string> symptoms;
    vector<string> preventions;
    int severity; // 1-10 scale
    Disease* next;

    Disease(string n, string desc, vector<string> sym, vector<string> prev, int sev) {
        name = n;
        description = desc;
        symptoms = sym;
        preventions = prev;
        severity = sev;
        next = nullptr;
    }
};

class DiseaseList {
private:
    Disease* head;

public:
    DiseaseList() {
        head = nullptr;
    }

    // Add a disease to the end of the Linked List
    void addDisease(string name, string desc, vector<string> sym, vector<string> prev, int sev) {
        Disease* newNode = new Disease(name, desc, sym, prev, sev);
        if (head == nullptr) {
            head = newNode;
        } else {
            Disease* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Search for a disease by name (Simplest Linear Search)
    Disease* getDiseaseDetails(string searchName) {
        Disease* temp = head;
        // Convert search term to lowercase for easier matching behavior if needed, 
        // but for now we do direct string comparison for simplicity as requested.
        while (temp != nullptr) {
            if (temp->name == searchName) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr; // Not found
    }

    // Function to populate the list with required sample data
    void populateSampleData() {
        // 1. Heart Attack
        addDisease("Heart Attack",
            "A blockage of blood flow to the heart muscle.",
            {"Chest Pain", "Shortness of Breath", "Nausea", "Cold Sweat"},
            {"Exercise regularly", "Eat a healthy diet", "Stop smoking"},
            10);

        // 2. Arrhythmia
        addDisease("Arrhythmia",
            "Improper beating of the heart, whether too fast or too slow.",
            {"Fluttering in chest", "Racing heartbeat", "Slow heartbeat", "Dizziness"},
            {"Reduce stress", "Limit alcohol", "Avoid tobacco"},
            6);

        // 3. Angina
        addDisease("Angina",
            "Chest pain caused by reduced blood flow to the heart.",
            {"Squeezing pressure", "Pain in shoulders", "Fatigue", "Nausea"},
            {"Quit smoking", "Manage diabetes", "Control blood pressure"},
            7);

        // 4. Coronary Artery Disease (CAD)
        addDisease("Coronary Artery Disease",
            "a prevalent heart condition characterized by the buildup of atherosclerotic plaque within the arterial lumen",
            {"Chest pain", "Shortness of breath", "Pain in arm"},
            {"Healthy diet", "Regular exercise", "Weight control"},
            9);

        // 5. Heart Failure
        addDisease("Heart Failure",
            "A chronic condition where the heart doesn't pump blood as well as it should.",
            {"Shortness of breath", "Fatigue", "Swollen legs", "Rapid heartbeat"},
            {"Cut back on salt", "Manage stress", "Track fluid intake"},
            8);

        // 6. Congenital Heart Disease
        addDisease("Congenital Heart Disease",
            "An abnormality in the heart that develops before birth.",
            {"Blue skin tint", "Rapid breathing", "Poor weight gain"},
            {"Depends on severity", "Surgery", "Medications"},
            7);

        // 7. Cardiomyopathy
        addDisease("Cardiomyopathy",
            "A disease of the heart muscle that makes it harder to pump blood.",
            {"Breathlessness", "Swelling of legs", "Bloating"},
            {"low-sodium diet", "Exercise", "Avoid Alcohol"},
            7);

        // 8. Atrial Fibrillation
        addDisease("Atrial Fibrillation",
            "An irregular, often rapid heart rate that implies poor blood flow.",
            {"Palpitations", "Weakness", "Confusion"},
            {"Blood thinners", "Healthy weight", "Control cholesterol"},
            6);

         // 9. Pericarditis
        addDisease("Pericarditis",
            "Swelling and irritation of the thin saclike membrane surrounding the heart.",
            {"Sharp chest pain", "Palpitations", "Fever"},
            {"Rest", "Over-the-counter pain relievers"},
            4);
            
         // 10. Valve Disease
        addDisease("Valve Disease",
            "When one or more of the valves in your heart doesn't work properly.",
            {"Whooshing sound (murmur)", "Abdominal swelling", "Fainting"},
            {"Healthy lifestyle", "Valve repair", "Regular checkups"},
            6);
    }
    
    // Helper to get raw pointer for Bindings (optional use)
    Disease* getHead() { return head; }
};

#endif
