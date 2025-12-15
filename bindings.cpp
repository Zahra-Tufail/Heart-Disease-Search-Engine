#include <emscripten/bind.h>
#include <string>
#include <vector>
#include "Disease.h"
#include "SymptomChecker.h"
#include "HospitalGraph.h"
#include "HospitalRecommender.h"

using namespace emscripten;
using namespace emscripten;
// using namespace std; // Removed to avoid conflict with emscripten::function

// ==========================================
// THE BRIDGE: Connects C++ Logic to JavaScript
// ==========================================

// Global Instances (The "State" of our App)
DiseaseList globalDiseaseList;
SymptomChecker* globalSymptomChecker;
AreaGraph globalAreaGraph;
HospitalRecommender globalRecommender;

// Initialization Function (Called when page loads)
void initSystem() {
    // 1. Load Data
    globalDiseaseList.populateSampleData();
    
    // 2. Init Checker
    globalSymptomChecker = new SymptomChecker(&globalDiseaseList);
    
    // 3. Init Map
    globalAreaGraph.setupIslamabadMap();
}

// ------------------------------------------
// WRAPPERS for simplified JS communication
// ------------------------------------------

// Feature 1: Get All Disease Names for Dropdown
val getAllDiseaseNames() {
    val names = val::array();
    Disease* head = globalDiseaseList.getHead();
    while(head != nullptr) {
        names.call<void>("push", head->name);
        head = head->next;
    }
    return names;
}

// Feature 1: Get Details
val getDiseaseByName(std::string name) {
    Disease* d = globalDiseaseList.getDiseaseDetails(name);
    val result = val::object();
    
    if (d != nullptr) {
        result.set("name", d->name);
        result.set("description", d->description);
        result.set("severity", d->severity);
        
        // Convert vectors to JS Arrays
        val syms = val::array();
        for(const auto& s : d->symptoms) syms.call<void>("push", s);
        result.set("symptoms", syms);
        
        val prevs = val::array();
        for(const auto& p : d->preventions) prevs.call<void>("push", p);
        result.set("preventions", prevs);
    } else {
        result.set("error", std::string("Not Found"));
    }
    return result;
}

// Feature 2: Predict Disease
val checkSymptoms(std::string commaSeparatedSymptoms) {
    // Parse simplified string "fever,pain" -> vector
    std::vector<std::string> symptoms;
    std::string buffer = "";
    for(char c : commaSeparatedSymptoms) {
        if(c == ',') {
            if(!buffer.empty()) symptoms.push_back(buffer);
            buffer = "";
        } else if (c != ' ') {
            buffer += c; // skip spaces
        }
    }
    if(!buffer.empty()) symptoms.push_back(buffer);

    std::vector<MatchResult> predictions = globalSymptomChecker->predictDisease(symptoms);
    
    val jsResults = val::array();
    for(const auto& p : predictions) {
        val obj = val::object();
        obj.set("disease", p.diseaseName);
        obj.set("percentage", p.percentage);
        jsResults.call<void>("push", obj);
    }
    return jsResults;
}

// Feature 3: Nearest Hospital
val findNearest(std::string areaName) {
    PathResult res = globalAreaGraph.findNearestHospital(areaName);
    val result = val::object();
    result.set("hospital", res.hospitalName);
    result.set("distance", res.totalDistance);
    
    val pathArr = val::array();
    for(const auto& p : res.path) pathArr.call<void>("push", p);
    result.set("path", pathArr);
    
    return result;
}

val getAreaList() {
    std::vector<std::string> areas = globalAreaGraph.getAreas();
    val jsArr = val::array();
    for(const auto& a : areas) jsArr.call<void>("push", a);
    return jsArr;
}

val getAllSymptoms() {
    std::set<std::string> symptoms = globalSymptomChecker->getUniqueSymptoms();
    val jsArr = val::array();
    for(const auto& s : symptoms) {
        jsArr.call<void>("push", s);
    }
    return jsArr;
}

// Feature 4: Full Recommendations
// Feature 4: Full Recommendations
val getRecommendations(std::string areaName) {
    std::vector<HospitalScoreWrapper> recs = globalRecommender.getRecommendations(areaName, globalAreaGraph);
    val jsArr = val::array();
    
    for(const auto& r : recs) {
        val obj = val::object();
        obj.set("name", r.data.name);
        obj.set("location", r.data.fullLocation); // Add location
        obj.set("distance", r.realDistance); // Use real calculated distance
        obj.set("rating", r.data.rating);
        obj.set("score", r.score);
        jsArr.call<void>("push", obj);
    }
    return jsArr;
}

// BINDING DEFINITIONS
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("initSystem", &initSystem);
    emscripten::function("getAllDiseaseNames", &getAllDiseaseNames);
    emscripten::function("getDiseaseByName", &getDiseaseByName);
    emscripten::function("checkSymptoms", &checkSymptoms);
    emscripten::function("findNearest", &findNearest);
    emscripten::function("getAreaList", &getAreaList);
    emscripten::function("getAllSymptoms", &getAllSymptoms);
    emscripten::function("getRecommendations", &getRecommendations);
}
