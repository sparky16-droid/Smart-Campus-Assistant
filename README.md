# 🏛️ Smart Campus Assistant — AI-Assisted Campus Navigator & Portal

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6.12.0-green.svg)](https://www.qt.io/)
[![Database](https://img.shields.io/badge/Database-SQLite3-lightgrey.svg)](https://sqlite.org/)
[![Build](https://img.shields.io/badge/Build-CMake%20%7C%20qmake-orange.svg)](https://cmake.org/)
[![PBL Project](https://img.shields.io/badge/Project-PBL%20College%20Submission-teal.svg)](#)

A full-stack, GUI-based college digital assistant built in **C++** using **Qt 6 (Qt Creator)** and **SQLite**. Designed for students, parents, faculty members, and campus visitors to seamlessly locate facilities, explore academic departments, access verified campus records, and converse with an intelligent local knowledge-retrieval AI Chatbot.

---

## 🌟 Key Features

| Feature | Description |
| :--- | :--- |
| **🤖 AI Campus Chatbot** | Database-grounded intelligent assistant using keyword normalization, 14+ synonym dictionaries, intent scoring, pronoun context resolution (*"Where is it?", "What are its timings?"*), and anti-hallucination guardrails. |
| **🗺️ Interactive Campus Map** | Custom 2D schematic canvas displaying 8 campus blocks with hover effects, pan/zoom controls, and clickable buildings that open full floor & facility drawers. |
| **🎓 Academic Departments** | Directory of departments (CSE, IT, ECE, Mechanical, Civil, MBA) detailing HODs, programs offered, research hubs, and direct map links. |
| **👨‍🏫 Faculty Directory** | Searchable faculty directory with department filters, cabin/room numbers, weekly office hours, emails, and research specializations. |
| **🏢 Facilities & Services** | Categorized directory for Central Library, Food Court/Cafeteria, Health Clinic, GPU AI Labs, Sports Pavilion, ATMs, Hostels, and Parking. |
| **📢 Notices & Announcements**| Live campus circulars with urgency badges (`Urgent`, `High`, `Normal`), category filtering, and publication dates. |
| **❓ Searchable FAQs** | Interactive accordion of frequently asked campus questions with view counts and one-click *"Ask Assistant"* buttons. |
| **🔐 Admin Control Portal** | Password-protected administrator console (`admin` / `adminpassword123`) with live query metrics, satisfaction percentages, and full CRUD operations. |
| **🌐 Embedded C++ Web Portal** | Built-in lightweight HTTP web server listening on `http://localhost:8080`, allowing the project to be accessed from any browser while running natively in C++! |

---

## 🏗️ System Architecture

```
                               ┌─────────────────────────────────┐
                               │   Qt Creator / C++ Desktop GUI  │
                               │           (MainWindow)          │
                               └──────────────┬──────────────────┘
                                              │
                     ┌────────────────────────┼────────────────────────┐
                     │                        │                        │
                     ▼                        ▼                        ▼
           ┌──────────────────┐     ┌──────────────────┐     ┌──────────────────┐
           │  Chatbot Engine  │     │ Campus Map Canvas│     │  Directory Views │
           │ (Synonym & NLP)  │     │  (2D Schematic)  │     │(Depts, Fac, Serv)│
           └─────────┬────────┘     └─────────┬────────┘     └─────────┬────────┘
                     │                        │                        │
                     └────────────────────────┼────────────────────────┘
                                              │
                                              ▼
                               ┌─────────────────────────────────┐
                               │   DatabaseManager (SQLite 3)    │
                               │  smart_campus.db (Auto-Seeded)  │
                               └──────────────┬──────────────────┘
                                              │
                                              ▼
                               ┌─────────────────────────────────┐
                               │    Embedded C++ HTTP Server     │
                               │    http://localhost:8080        │
                               │     (Web Portal & REST)         │
                               └─────────────────────────────────┘
```

---

## 🚀 How to Run in Qt Creator

### Prerequisites
- **Qt 6.x** (MinGW 64-bit kit)
- **Qt Creator IDE**

### Steps
1. **Launch Qt Creator**.
2. Click **File** ➔ **Open File or Project...** (or press `Ctrl+O`).
3. Select either:
   - `CMakeLists.txt` (Modern CMake standard) **OR**
   - `SmartCampusAssistant.pro` (Standard qmake file)
4. Choose the **Desktop Qt 6.x.x MinGW 64-bit** build kit and click **Configure Project**.
5. Click the green **Run (▶️)** button in the bottom-left corner (or press `Ctrl+R`).
6. The GUI window will open with all campus buildings, interactive map, and the AI chatbot ready!

---

## 💻 How to Build & Run from Command Line (Terminal)

Open PowerShell in the project directory:

```powershell
# 1. Set environment variables to Qt tools
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\CMake_64\bin;" + $env:PATH

# 2. Configure CMake with Ninja
cmake.exe -B build -S . -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64"

# 3. Build the application
cmake.exe --build build

# 4. Run the executable
.\build\SmartCampusAssistant.exe
```

---

## 🧪 Running Automated Tests

Run the built-in C++ automated test suite verifying all 7 test categories:

```powershell
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\CMake_64\bin;" + $env:PATH
cmake.exe --build build --target test_campus
.\build\test_campus.exe
```

**Test Output:**
```
==================================================
   Smart Campus Assistant Automated Test Suite    
==================================================
[PASS] Database connection initialized
[PASS] Buildings count verified: 8
[PASS] Facilities count verified: 13
[PASS] Departments count verified: 6
[PASS] Faculty count verified: 8
[PASS] FAQs count verified: 10
[PASS] Admin password verification verified
[PASS] Greeting intent recognized
[PASS] Library query grounded successfully
[PASS] Contextual follow-up resolved successfully
[PASS] Synonym expansion (canteen -> cafeteria) verified
[PASS] Anti-hallucination guardrail verified
==================================================
 🎉 ALL 7 TEST SUITES PASSED CLEANLY (100% SUCCESS)!
==================================================
```

---

## 🌐 Embedded Web Portal

While the desktop application is running, open your web browser and navigate to:
```
http://localhost:8080
```
The embedded C++ server serves a responsive web dashboard with live AI chat assistant connected to the C++ database!

---

## 🐙 Step-by-Step GitHub Deployment Guide

Follow these exact steps to push this project to your GitHub account:

### Step 1: Install Git (If not already installed)
Open Windows PowerShell as Administrator and run:
```powershell
winget install --id Git.Git -e --source winget
```
*(Or download Git for Windows from [https://git-scm.com](https://git-scm.com) or use GitHub Desktop).*

### Step 2: Create a New Repository on GitHub
1. Go to [https://github.com/new](https://github.com/new).
2. Repository name: `Smart-Campus-Assistant`
3. Description: `AI-Assisted Campus Information and Navigation System built in C++ with Qt 6 for college PBL`
4. Choose **Public** (or Private).
5. **Do not** initialize with a README, .gitignore, or license (we already created them).
6. Click **Create repository**.

### Step 3: Initialize and Push from Your Workspace
Open PowerShell in `c:\Users\HP\OneDrive\Desktop\pbl2`:

```powershell
# 1. Initialize Git repository
git init

# 2. Add all source files (build artifacts are automatically ignored by .gitignore)
git add .

# 3. Create your first commit
git commit -m "feat: complete Smart Campus Assistant C++ Qt6 application with AI chatbot and interactive map"

# 4. Set default branch to main
git branch -M main

# 5. Link your GitHub repository (replace YOUR-USERNAME with your actual GitHub username)
git remote add origin https://github.com/YOUR-USERNAME/Smart-Campus-Assistant.git

# 6. Push code to GitHub
git push -u origin main
```

---

## 👨‍💼 Administrator Credentials

- **Username:** `admin`
- **Password:** `adminpassword123`
- Access via the **🔐 Administrator Portal** button on the bottom of the sidebar.

---

## 🎓 College Viva / PBL Presentation Guide

When demonstrating this project to college examiners:
1. **Showcase Contextual AI:**
   - Type `"Where is the library?"` ➔ The assistant answers with building LIB-1, floor, timings, and map button.
   - Immediately follow up with `"What are its timings?"` ➔ The assistant identifies *"its"* refers to the library and retrieves operating hours!
2. **Demonstrate Synonym Expansion:**
   - Ask for `"food court"` or `"canteen"` ➔ Assistant correctly routes to the Student Amenity Center (SAC) Cafeteria.
3. **Interactive 2D Canvas:**
   - Click on Science Tower (ST-2) to show the interactive detail drawer and list of GPU AI Labs.
4. **Demonstrate Anti-Hallucination:**
   - Ask an unverified question (e.g., *"What is the moon rocket launch time?"*) ➔ The engine responds gracefully that verified data is not found rather than fabricating fake answers.
5. **Showcase Web Server:**
   - Click **"Open in Web Browser"** to show examiners the embedded C++ web server running at `http://localhost:8080`.

---

## 📜 License
Developed as part of the College Project-Based Learning (PBL) curriculum.
Free to use and adapt for academic purposes.
