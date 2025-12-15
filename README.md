# HeartGuard - Advanced Heart Disease Search Engine ❤️🏥

HeartGuard is a high-performance web application that merges advanced **Data Structures & Algorithms (DSA)** in C++ with a modern web interface using **WebAssembly (WASM)**. It provides users with instant heart disease information, symptom-based predictions, and intelligent hospital routing.

## 🚀 Features & DSA Implementation

This project demonstrates the practical application of core DSA concepts:

| Feature | Description | Data Structure / Algorithm |
|---------|-------------|----------------------------|
| **Disease Encyclopedia** | View detailed info, symptoms, and preventions for heart conditions. | **Linked List** (Dynamic storage of disease nodes) |
| **Symptom AI** | Predicts potential diseases based on user-selected symptoms. | **Max Heap / Frequency Map** (Probabilistic matching) |
| **Intelligent Routing** | Finds the absolute nearest hospital from the user's sector. | **Graph (Adjacency List)** + **Dijkstra’s Algorithm** |
| **Smart Recommendations** | Ranks hospitals based on a hybrid score of distance & rating. | **Min Heap** (Priority Queue based on custom score) |

## 🛠️ Technology Stack

*   **Backend Logic:** C++ (Standard Template Library)
*   **Compilation:** Emscripten (C++ to WebAssembly)
*   **Frontend:** HTML5, CSS3, JavaScript (ES6+)
*   **Build Tools:** PowerShell & Python (for local server)

## 📂 Project Structure

```
HeartGuard/
├── cpp/                    # Core C++ Source Code
│   ├── Disease.h           # Linked List Implementation
│   ├── SymptomChecker.h    # Prediction Logic
│   ├── HospitalGraph.h     # Graph & Dijkstra Implementation
│   ├── HospitalRecommender.h # Heaps & Scoring Logic
│   └── bindings.cpp        # Emscripten Layout / JS Interface
├── frontend/               # Web Interface
│   ├── index.html          # Main UI
│   ├── style.css           # Modern Styling
│   └── script.js           # UI Logic (calls WebAssembly)
├── emsdk/                  # Emscripten SDK (ignored in git)
├── build.ps1               # One-click build script
├── run.ps1                 # Local development server script
└── README.md               # Documentation
```

## ⚙️ How to Set Up & Run

### Prerequisites
- **Git** installed.
- **Python 3.x** (for the local server).
- **Emscripten SDK** (Automatically handled by setup scripts, or pre-installed).

### 1. Build the Project
We have provided a PowerShell script to compile the C++ code into WebAssembly.
```powershell
./build.ps1
```
*Creates `frontend/project.js` and `frontend/project.wasm`.*

### 2. Run the Application
Start the local Python server to view the app in your browser.
```powershell
./run.ps1
```
The app will open automatically at `http://localhost:8000`.

## 🤝 Contributing
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes.
4. Push to the branch and open a Pull Request.

---
*Developed as a Data Structures & Algorithms Semester Project.*
