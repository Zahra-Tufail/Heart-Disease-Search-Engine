// Global Module variable from Emscripten
var Module = {
    onRuntimeInitialized: function () {
        console.log("C++ Engine Loaded!");
        // Update Status Indicator
        const statusEl = document.getElementById('system-status');
        if (statusEl) {
            statusEl.innerText = "● System Online";
            statusEl.classList.remove('offline');
            statusEl.classList.add('online');
            statusEl.style.textShadow = "0 0 10px #22c55e";
        }

        // Initialize C++ System
        Module.initSystem();

        // Pre-load Dropdowns & Checkboxes
        populateDiseaseDropdown();
        populateAreaDropdown();
        populateSymptomsAndCheckboxes();
    }
};

// =======================
// NAVIGATION LOGIC
// =======================
function showSection(sectionId) {
    // 1. Hide all sections
    const sections = ['home', 'search', 'checker', 'nearest', 'recommend'];
    sections.forEach(id => {
        const el = document.getElementById(id);
        if (el) el.style.display = 'none';
    });

    // 2. Show target section
    const target = document.getElementById(sectionId);
    if (target) target.style.display = 'block';

    // 3. Update Sidebar Active State
    const navItems = document.querySelectorAll('.sidebar li');
    navItems.forEach(item => item.classList.remove('active'));

    const activeLink = document.querySelector(`.sidebar li[onclick="showSection('${sectionId}')"]`);
    if (activeLink) activeLink.classList.add('active');
}

// =======================
// FEATURE 1: DISEASE SEARCH
// =======================
function populateDiseaseDropdown() {
    try {
        if (Module.getAllDiseaseNames) {
            const names = Module.getAllDiseaseNames();
            const select = document.getElementById('disease-dropdown');
            if (select) {
                select.innerHTML = '<option value="" disabled selected>Select a Heart Disease...</option>';
                for (let i = 0; i < names.length; i++) {
                    let opt = document.createElement('option');
                    opt.value = names[i];
                    opt.innerText = names[i];
                    select.appendChild(opt);
                }
            }
        }
    } catch (e) {
        console.error("Error populating diseases:", e);
    }
}

function loadDiseaseDetails() {
    const dropdown = document.getElementById('disease-dropdown');
    if (!dropdown) return;
    const name = dropdown.value;
    if (!name) return;

    if (Module.getDiseaseByName) {
        const details = Module.getDiseaseByName(name);

        if (details.error) {
            alert("Details not found!");
            return;
        }

        // Update UI
        document.getElementById('d-name').innerText = details.name;
        document.getElementById('d-severity').innerText = "Severity: " + details.severity;
        document.getElementById('d-desc').innerText = details.description;

        // Lists
        const symList = document.getElementById('d-symptoms');
        symList.innerHTML = '';
        if (details.symptoms) {
            for (let i = 0; i < details.symptoms.length; i++) {
                let li = document.createElement('li');
                li.innerText = details.symptoms[i];
                symList.appendChild(li);
            }
        }

        const prevList = document.getElementById('d-preventions');
        prevList.innerHTML = '';
        if (details.preventions) {
            for (let i = 0; i < details.preventions.length; i++) {
                let li = document.createElement('li');
                li.innerText = details.preventions[i];
                prevList.appendChild(li);
            }
        }

        // Show result
        document.getElementById('disease-result').classList.remove('hidden');
    }
}

// =======================
// FEATURE 2: SYMPTOM AI
// =======================
function populateSymptomsAndCheckboxes() {
    try {
        const symptoms = Module.getAllSymptoms();
        const container = document.getElementById('symptoms-container');
        container.innerHTML = '';

        if (symptoms.length === 0) {
            container.innerHTML = '<p>No symptoms loaded.</p>';
            return;
        }

        symptoms.forEach(sym => {
            const div = document.createElement('div');
            div.className = 'checkbox-item';

            const checkbox = document.createElement('input');
            checkbox.type = 'checkbox';
            checkbox.id = 'sym-' + sym.replace(/\s+/g, '-').toLowerCase();
            checkbox.value = sym;

            const label = document.createElement('label');
            label.htmlFor = checkbox.id;
            label.innerText = sym;

            // Allow clicking the div to toggle
            div.onclick = (e) => {
                if (e.target !== checkbox && e.target !== label) {
                    checkbox.checked = !checkbox.checked;
                }
            };

            div.appendChild(checkbox);
            div.appendChild(label);
            container.appendChild(div);
        });
    } catch (e) {
        console.error("Error loading symptoms:", e);
    }
}

function runPrediction() {
    // Gather selected checkboxes
    const container = document.getElementById('symptoms-container');
    const checkboxes = container.querySelectorAll('input[type="checkbox"]:checked');

    // Create comma separated string for C++
    let input = "";
    if (checkboxes.length > 0) {
        let syms = [];
        checkboxes.forEach(cb => syms.push(cb.value));
        input = syms.join(",");
    } else {
        // Fallback for empty selection? Or just return
        // Optional: User might still type if we left the input... 
        // but we removed the text input.
        alert("Please select at least one symptom.");
        return;
    }

    const results = Module.checkSymptoms(input);
    const resultContainer = document.getElementById('prediction-results');
    resultContainer.innerHTML = '';

    if (results.length === 0) {
        resultContainer.innerHTML = '<p style="color:var(--text-muted)">No matches found based on your selection.</p>';
        return;
    }

    for (let i = 0; i < results.length; i++) {
        let r = results[i];
        // Create Card
        let card = document.createElement('div');
        card.className = 'card';
        card.style.width = '100%';
        card.style.flexDirection = 'row';
        card.style.justifyContent = 'space-between';

        let color = r.percentage > 70 ? 'var(--accent)' : 'var(--primary)';

        card.innerHTML = `
            <div>
                <h3>${r.disease}</h3>
                <div style="width: 100%; background: #333; height: 6px; border-radius: 3px; margin-top: 10px; width: 200px;">
                    <div style="width: ${r.percentage}%; background: ${color}; height: 100%; border-radius: 3px;"></div>
                </div>
            </div>
            <h2 style="color:${color}">${r.percentage.toFixed(0)}% Match</h2>
        `;
        resultContainer.appendChild(card);
    }
}

// =======================
// FEATURE 3: NEAREST HOSPITAL
// =======================
function populateAreaDropdown() {
    const areas = Module.getAreaList();
    const select = document.getElementById('area-dropdown');
    for (let i = 0; i < areas.length; i++) {
        let opt = document.createElement('option');
        opt.value = areas[i];
        opt.innerText = areas[i];
        select.appendChild(opt);
    }
}

function findNearestHospital() {
    const area = document.getElementById('area-dropdown').value;
    if (!area) return; // or default

    const result = Module.findNearest(area);

    document.getElementById('n-hospital').innerText = result.hospital;
    document.getElementById('n-dist').innerText = result.distance;

    const pathDiv = document.getElementById('n-path');
    pathDiv.innerHTML = '';

    for (let i = 0; i < result.path.length; i++) {
        let span = document.createElement('span');
        span.className = 'step';
        span.innerText = result.path[i];
        pathDiv.appendChild(span);

        if (i < result.path.length - 1) {
            let arrow = document.createElement('i');
            arrow.className = 'fa-solid fa-arrow-right arrow';
            pathDiv.appendChild(arrow);
        }
    }

    document.getElementById('route-result').classList.remove('hidden');
}

// =======================
// FEATURE 4: RECOMMENDATIONS
// =======================
function getTopHospitals() {
    // 1. Get the area selected in the previous section (Feature 3)
    const area = document.getElementById('area-dropdown').value;

    if (!area) {
        alert("Please select your current location in the 'Nearest Hospital' section first!");
        // Optional: switch to that section
        showSection('nearest');
        return;
    }

    // 2. Call C++ with the real area
    const recs = Module.getRecommendations(area);

    const tbody = document.getElementById('rec-table-body');
    tbody.innerHTML = '';

    // Recs are sorted by Min Heap (Best score first)
    for (let i = 0; i < recs.length; i++) {
        let r = recs[i];
        let row = `
            <tr>
                <td>#${i + 1}</td>
                <td style="font-weight:bold; color: var(--primary)">${r.name}</td>
                <td style="font-size: 0.9em; color: var(--text-muted);">${r.location}</td>
                <td>${r.rating}/5</td>
                <td>${r.distance.toFixed(1)} km</td>
                <td>${r.score.toFixed(1)}</td>
            </tr>
        `;
        tbody.innerHTML += row;
    }
}
