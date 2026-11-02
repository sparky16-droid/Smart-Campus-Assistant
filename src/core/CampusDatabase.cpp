#include "CampusDatabase.h"
#include "EntityFactory.h"
#include <sstream>

CampusDatabase::CampusDatabase()
    : m_initialized(false),
      m_adminPasswordHash("adminpassword123")
{
}

CampusDatabase& CampusDatabase::instance() {
    static CampusDatabase db;
    return db;
}

void CampusDatabase::initialize() {
    if (m_initialized) return;
    seedDefaultData();
    m_initialized = true;
}

bool CampusDatabase::verifyAdmin(const std::string& username, const std::string& password) const {
    return (username == "admin" && password == m_adminPasswordHash);
}

void CampusDatabase::seedDefaultData() {
    // 1. Buildings (8 core campus buildings)
    m_buildings.add(EntityFactory::createBuilding(
        "bld-1", "Administrative Block", "ADM", 
        "Central administrative offices, Dean, Registrar, Admissions, and Accounts departments.",
        18.52043, 73.85674, 4, 1998, true, 80, 70, 160, 100, "#1d4ed8"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-2", "Academic Block A - Engineering", "ENG-A", 
        "Houses Computer Science, Information Technology, and AI/DS lecture halls and labs.",
        18.52110, 73.85720, 5, 2005, true, 280, 60, 200, 120, "#2563eb"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-3", "Academic Block B - Sciences & Humanities", "ENG-B", 
        "Mechanical, Electrical, and First Year Engineering lecture halls and workshop bays.",
        18.52150, 73.85800, 4, 2008, true, 520, 60, 190, 120, "#3b82f6"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-4", "Central Library & Knowledge Resource Center", "LIB", 
        "Three-floor modern library with 50,000+ volumes, digital e-library, and 24/7 study cubicles.",
        18.52080, 73.85620, 3, 2002, true, 100, 210, 150, 130, "#0891b2"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-5", "Student Activity Center & Food Court", "SAC", 
        "Multipurpose hall, cafeteria, cooperative bookstore, stationery, and student club offices.",
        18.52010, 73.85790, 2, 2012, true, 300, 220, 180, 110, "#d97706"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-6", "Indoor Sports Arena & Gymnasium", "SPT", 
        "Badminton courts, table tennis, yoga room, modern gymnasium, and outdoor ground access.",
        18.51950, 73.85880, 2, 2015, true, 520, 220, 170, 120, "#059669"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-7", "Boys & Girls Hostels Complex", "HST", 
        "Residential campus hostels with dining mess, high-speed Wi-Fi, and 24/7 security warden.",
        18.52220, 73.85920, 6, 2010, false, 720, 140, 140, 180, "#4f46e5"
    ));
    m_buildings.add(EntityFactory::createBuilding(
        "bld-8", "Health Care Center & Pharmacy", "MED", 
        "Outpatient medical clinic, emergency first aid, doctor consultation, and subsidized pharmacy.",
        18.52060, 73.85550, 1, 2004, true, 40, 360, 120, 80, "#dc2626"
    ));

    // 2. Facilities (13 facilities across campus)
    m_facilities.add(EntityFactory::createFacility(
        "fac-1", "Central Library Reference & Digital Section", "Library",
        "Quiet reference section with IEEE journals, research terminals, and reprographics.",
        "bld-4", "Central Library", "1st Floor", "LIB-102", "Opposite Main Staircase",
        "Mon-Sat 8:00 AM - 10:00 PM", "+91 20 2567 1101", "library@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-2", "Campus Main Cafeteria", "Cafeteria",
        "Hygienic multi-cuisine breakfast, lunch, snacks, and fresh beverage counters.",
        "bld-5", "Student Activity Center", "Ground Floor", "SAC-01", "Near North Courtyard",
        "Mon-Sun 7:30 AM - 9:30 PM", "+91 20 2567 1102", "cafeteria@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-3", "Computer Center & High Performance Computing Lab", "Lab",
        "120 workstations with GPU clusters for AI, Deep Learning, and Distributed Computing.",
        "bld-2", "Academic Block A - Engineering", "3rd Floor", "CS-301", "Adjacent to Server Room",
        "Mon-Fri 8:30 AM - 8:00 PM", "+91 20 2567 1103", "hpc@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-4", "Health Clinic & Emergency Room", "Medical",
        "Doctor consultation, ambulance standby, basic diagnostics, and prescription pharmacy.",
        "bld-8", "Health Care Center", "Ground Floor", "MED-01", "West Campus Gate",
        "24 Hours / 7 Days", "+91 20 2567 1104", "medical@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-5", "Gymnasium & Fitness Center", "Sports",
        "Cardio zone, weight training, certified personal trainers, and locker rooms.",
        "bld-6", "Indoor Sports Arena", "Ground Floor", "SPT-05", "Beside Squash Court",
        "Mon-Sat 6:00 AM - 9:00 PM", "+91 20 2567 1105", "sports@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-6", "Auditorium & Convocation Hall", "Academic",
        "Air-conditioned 1,200 capacity auditorium with line-array acoustics and dual projectors.",
        "bld-1", "Administrative Block", "Ground Floor", "ADM-AUD", "East Wing Entrance",
        "Available on booking: 9:00 AM - 8:00 PM", "+91 20 2567 1106", "events@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-7", "Campus ATM - State Bank", "ATM",
        "24-hour automated teller machine and cash deposit kiosk.",
        "bld-1", "Administrative Block", "Ground Floor", "ATM-01", "Beside Admissions Foyer",
        "24 Hours", "1800 11 2211", "banking@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-8", "Registrar & Academic Student Section", "Administration",
        "Transcripts, bonafide certificates, degree verification, and fee concession desk.",
        "bld-1", "Administrative Block", "1st Floor", "ADM-105", "Central Corridor",
        "Mon-Fri 9:30 AM - 4:30 PM", "+91 20 2567 1108", "studentsection@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-9", "Robotics & Embedded IoT Systems Lab", "Lab",
        "3D printers, laser cutters, oscilloscope benches, and drone development test arena.",
        "bld-2", "Academic Block A - Engineering", "2nd Floor", "CS-204", "Near South Staircase",
        "Mon-Fri 9:00 AM - 6:00 PM", "+91 20 2567 1109", "robotics@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-10", "Training & Placement Cell (T&P)", "Administration",
        "Corporate interview suites, group discussion conference halls, and career counselling.",
        "bld-1", "Administrative Block", "2nd Floor", "ADM-201", "West Wing Executive Floor",
        "Mon-Fri 9:00 AM - 6:00 PM", "+91 20 2567 1110", "placement@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-11", "Campus Cooperative Store & Stationery", "Store",
        "Notebooks, engineering drawing tools, textbooks, project printouts, and stationery.",
        "bld-5", "Student Activity Center", "Ground Floor", "SAC-08", "South Arcade",
        "Mon-Sat 8:30 AM - 7:30 PM", "+91 20 2567 1111", "store@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-12", "Physics & Material Chemistry Laboratory", "Lab",
        "First-year experimental physics benches, spectrometer setups, and chemical safety hoods.",
        "bld-3", "Academic Block B - Sciences", "1st Floor", "SCI-101", "North Hallway",
        "Mon-Fri 8:30 AM - 5:00 PM", "+91 20 2567 1112", "physics@campus.edu", true
    ));
    m_facilities.add(EntityFactory::createFacility(
        "fac-13", "Campus Security Control & Lost and Found", "Security",
        "Campus surveillance hub, visitor pass generation, ID card replacement, and lost property.",
        "bld-1", "Administrative Block", "Ground Floor", "SEC-01", "Main Campus Gate Guardhouse",
        "24 Hours / 7 Days", "+91 20 2567 1113", "security@campus.edu", true
    ));

    // 3. Departments (6 departments)
    m_departments.add(EntityFactory::createDepartment(
        "dept-1", "Computer Engineering & Information Technology", "CE-IT",
        "Pioneering undergraduate and graduate education in software systems, cybersecurity, and algorithms.",
        "bld-2", "Academic Block A - Engineering", "Dr. Rajeshwar Sharma",
        "hod.comp@campus.edu", "+91 20 2567 1201", "https://campus.edu/ce-it",
        {"B.Tech Computer Engineering", "B.Tech Information Technology", "M.Tech Software Engineering", "Ph.D. Computing"},
        720, 28
    ));
    m_departments.add(EntityFactory::createDepartment(
        "dept-2", "Artificial Intelligence & Data Science", "AI-DS",
        "Cutting-edge research in machine learning, deep neural models, vision, and natural language.",
        "bld-2", "Academic Block A - Engineering", "Dr. Sunita Kulkarni",
        "hod.aids@campus.edu", "+91 20 2567 1202", "https://campus.edu/ai-ds",
        {"B.Tech Artificial Intelligence & Data Science", "M.Tech Data Analytics", "Ph.D. AI Systems"},
        360, 16
    ));
    m_departments.add(EntityFactory::createDepartment(
        "dept-3", "Electronics & Telecommunication Engineering", "E&TC",
        "Specializing in embedded VLSI, 5G signal processing, wireless networks, and RF design.",
        "bld-2", "Academic Block A - Engineering", "Prof. Anand Deshmukh",
        "hod.entc@campus.edu", "+91 20 2567 1203", "https://campus.edu/entc",
        {"B.Tech Electronics & Telecommunication", "M.Tech VLSI & Embedded Systems", "Ph.D. Electronics"},
        480, 22
    ));
    m_departments.add(EntityFactory::createDepartment(
        "dept-4", "Mechanical Engineering & Robotics", "MECH",
        "Thermal fluids, additive manufacturing, CAD/CAM robotics, and sustainable automotive engineering.",
        "bld-3", "Academic Block B - Sciences", "Dr. Vikram Joshi",
        "hod.mech@campus.edu", "+91 20 2567 1204", "https://campus.edu/mech",
        {"B.Tech Mechanical Engineering", "M.Tech Thermal Systems & Design", "Ph.D. Mechanical Engineering"},
        420, 20
    ));
    m_departments.add(EntityFactory::createDepartment(
        "dept-5", "Electrical & Renewable Power Engineering", "ELEC",
        "Power electronics, microgrid systems, renewable energy storage, and smart grid automation.",
        "bld-3", "Academic Block B - Sciences", "Dr. Meenakshi Iyer",
        "hod.elec@campus.edu", "+91 20 2567 1205", "https://campus.edu/electrical",
        {"B.Tech Electrical Engineering", "M.Tech Power Systems", "Ph.D. Renewable Systems"},
        360, 18
    ));
    m_departments.add(EntityFactory::createDepartment(
        "dept-6", "Applied Sciences & Humanities", "ASH",
        "Foundational education in Engineering Mathematics, Applied Physics, Chemistry, and Professional Ethics.",
        "bld-3", "Academic Block B - Sciences", "Dr. Arvind Pathak",
        "hod.ash@campus.edu", "+91 20 2567 1206", "https://campus.edu/applied-sciences",
        {"First Year Engineering Curriculum", "Applied Mathematics Minor"},
        600, 24
    ));

    // 4. Faculty (8 faculty members)
    m_faculty.add(EntityFactory::createFaculty(
        "fac-1", "Dr. Rajeshwar Sharma", "Dr.", "Professor & HOD",
        "dept-1", "Computer Engineering", "hod.comp@campus.edu", "+91 20 2567 1201",
        "ENG-A 301", "Mon & Wed 2:00 PM - 4:00 PM",
        {"Object-Oriented Architecture", "Distributed Systems", "Cloud Computing"},
        "Over 22 years of academic leadership and recipient of National Best Teacher Award.", 45
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-2", "Dr. Sunita Kulkarni", "Dr.", "Professor & HOD",
        "dept-2", "AI & Data Science", "hod.aids@campus.edu", "+91 20 2567 1202",
        "ENG-A 402", "Tue & Thu 11:00 AM - 1:00 PM",
        {"Deep Learning", "Natural Language Processing", "Computer Vision"},
        "Senior IEEE Member with 5 patents filed in autonomous perception systems.", 38
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-3", "Prof. Anand Deshmukh", "Prof.", "Associate Professor",
        "dept-3", "Electronics & Telecommunication", "anand.d@campus.edu", "+91 20 2567 1203",
        "ENG-A 210", "Mon & Fri 10:00 AM - 12:00 PM",
        {"VLSI Circuit Design", "Embedded Microcontrollers", "IoT Sensors"},
        "Former ISRO collaborator and advisor to student satellite design initiative.", 27
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-4", "Dr. Vikram Joshi", "Dr.", "Professor & HOD",
        "dept-4", "Mechanical Engineering", "hod.mech@campus.edu", "+91 20 2567 1204",
        "ENG-B 104", "Wed & Fri 3:00 PM - 5:00 PM",
        {"Thermal Engineering", "Computational Fluid Dynamics", "Robotics"},
        "Authored 3 standard university textbooks on Advanced Thermodynamics.", 31
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-5", "Dr. Meenakshi Iyer", "Dr.", "Associate Professor",
        "dept-5", "Electrical Engineering", "meenakshi.i@campus.edu", "+91 20 2567 1205",
        "ENG-B 205", "Tue & Thu 2:00 PM - 4:00 PM",
        {"Smart Grids", "Battery Storage", "Renewable Energy Economics"},
        "Principal Investigator for Ministry of Power smart microgrid testbed grant.", 24
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-6", "Prof. Neha Patil", "Prof.", "Assistant Professor",
        "dept-1", "Computer Engineering", "neha.patil@campus.edu", "+91 20 2567 1207",
        "ENG-A 315", "Mon & Wed 11:30 AM - 1:30 PM",
        {"Object-Oriented C++", "Database Management Systems", "Data Structures"},
        "Dedicated mentor for competitive coding teams and student project hackathons.", 14
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-7", "Dr. Arvind Pathak", "Dr.", "Professor & Dean Academics",
        "dept-6", "Applied Sciences", "dean.academic@campus.edu", "+91 20 2567 1206",
        "ADM 204", "Mon-Fri 4:00 PM - 5:00 PM",
        {"Differential Equations", "Quantum Computing Math", "Engineering Statistics"},
        "Dean of Academic Affairs overseeing curriculum development and accreditation.", 52
    ));
    m_faculty.add(EntityFactory::createFaculty(
        "fac-8", "Prof. Rohan Mehta", "Prof.", "Assistant Professor",
        "dept-2", "AI & Data Science", "rohan.mehta@campus.edu", "+91 20 2567 1208",
        "ENG-A 412", "Tue & Fri 10:00 AM - 12:00 PM",
        {"Reinforcement Learning", "Graph Neural Networks", "Big Data Pipelines"},
        "Active open-source contributor and technical advisor to campus incubation startups.", 11
    ));

    // 5. Notices (5 notices)
    m_notices.add(EntityFactory::createNotice(
        "not-1", "End Semester Theory Examination Schedule Announced",
        "The end semester examination timetable for all undergraduate and postgraduate engineering branches has been officially released. Hall tickets will be issued starting next Monday at the student section.",
        "Exam", NoticePriority::Urgent, true, "Controller of Examinations"
    ));
    m_notices.add(EntityFactory::createNotice(
        "not-2", "Campus Annual TechFest 'TechnoVision 2026' Registrations",
        "Registrations are now open for TechnoVision 2026 featuring 24-Hour Hackathon, Robo-Wars, Paper Presentation, and Coding Sprint. Cash prizes worth Rs 3.5 Lakhs to be won.",
        "Event", NoticePriority::High, true, "Student Council & Tech Club"
    ));
    m_notices.add(EntityFactory::createNotice(
        "not-3", "Microsoft & Google Campus Placement Drive",
        "Final year students eligible for campus recruitment must register on the T&P Portal before 5:00 PM Friday. Pre-placement talk will be held in the Administrative Auditorium.",
        "Placement", NoticePriority::High, true, "Training & Placement Cell"
    ));
    m_notices.add(EntityFactory::createNotice(
        "not-4", "Central Library Book Return & No-Dues Reminder",
        "All outgoing final semester students are required to return borrowed books and clear library dues before the end of the month to receive graduation clearance certificates.",
        "Academic", NoticePriority::Normal, true, "Chief Librarian"
    ));
    m_notices.add(EntityFactory::createNotice(
        "not-5", "Inter-College Sports Tournament Trials This Weekend",
        "Selection trials for Football, Basketball, Badminton, and Cricket teams will take place at the Sports Arena Saturday morning from 7:00 AM onwards.",
        "Sports", NoticePriority::Normal, true, "Physical Education Department"
    ));

    // 6. FAQs (10 FAQs)
    m_faqs.add(EntityFactory::createFaq(
        "faq-1", "What are the Central Library timings and book borrowing limits?",
        "The Central Library is open Monday to Saturday from 8:00 AM to 10:00 PM. During examination periods, the 24/7 reading hall remains open continuously. Undergraduates may borrow up to 4 books for 14 days, renewable once online.",
        "Library", {"library", "books", "timings", "reading", "borrow", "circulation"}, 182, 45
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-2", "Where is the Health Care Center located and who do I call in an emergency?",
        "The Campus Health Center is located in Building 8 (near West Campus Gate). It operates 24/7 with doctor on duty and ambulance standby. Emergency phone number: +91 20 2567 1104 or intercom extension 104.",
        "Medical", {"health", "doctor", "medical", "clinic", "emergency", "ambulance", "pharmacy"}, 145, 39
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-3", "How do I connect to the Campus High-Speed Wi-Fi network?",
        "Connect your device to the SSID 'Campus-Secure-WiFi'. When prompted, log in with your University Roll Number as the username and your Student Portal password. For device registration issues, visit Computer Center room CS-301.",
        "IT & Network", {"wifi", "internet", "network", "password", "ssid", "connect"}, 320, 88
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-4", "What is the procedure to apply for Bonafide or Transcript certificates?",
        "Submit an application at the Registrar Student Section (ADM-105) or apply through the online ERP portal. Standard processing time is 2 working days. Standard fee is Rs 100 for transcripts.",
        "Administration", {"bonafide", "transcript", "certificate", "student section", "registrar"}, 210, 52
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-5", "What sports facilities are available and what are the gym hours?",
        "The Indoor Sports Arena (Building 6) features badminton courts, table tennis, yoga halls, and a modern gym. Gym timings are 6:00 AM - 9:00 AM and 4:30 PM - 9:00 PM Monday through Saturday.",
        "Sports", {"gym", "sports", "badminton", "fitness", "timings", "arena"}, 165, 33
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-6", "Where is the Cafeteria located and what meal plans are served?",
        "The main cafeteria is located in the Student Activity Center (SAC) ground floor. It serves hygienic vegetarian and non-vegetarian breakfast, hot thali lunch, fresh fruit juices, and snacks from 7:30 AM to 9:30 PM daily.",
        "Food", {"cafeteria", "canteen", "food", "lunch", "breakfast", "mess", "sac"}, 275, 71
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-7", "How can I contact my professors during their office hours?",
        "Professors list their cabin locations and office hours on the faculty portal and outside their departmental cabins in Academic Blocks A & B. You may also email them directly through their official @campus.edu email address.",
        "Academic", {"faculty", "professor", "office hours", "cabin", "contact", "appointment"}, 130, 28
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-8", "Where do I report lost or found items on campus?",
        "Report lost property or submit found belongings to the Security Control Hub located at the Main Gate Guardhouse (ADM ground floor extension). You can also contact extension 113 or email security@campus.edu.",
        "Security", {"lost", "found", "security", "id card", "wallet", "keys"}, 95, 21
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-9", "What is the minimum attendance requirement to be eligible for exams?",
        "As per university academic regulations, students must maintain a minimum of 75% attendance in each registered theory and laboratory course to be eligible to appear for the semester end examinations.",
        "Examination", {"attendance", "exam", "criteria", "eligibility", "75 percent", "detention"}, 290, 84
    ));
    m_faqs.add(EntityFactory::createFaq(
        "faq-10", "How do I register for campus placement drives and aptitude training?",
        "Eligible penultimate and final year students must register on the T&P Portal (Training & Placement Cell, ADM-201). Regular aptitude, coding mock tests, and interview workshops are conducted every Saturday.",
        "Placement", {"placement", "jobs", "internship", "t&p", "interviews", "companies"}, 340, 92
    ));
}

std::vector<std::shared_ptr<ICampusEntity>> CampusDatabase::searchAll(const std::string& query, double minRelevance) const {
    std::vector<std::shared_ptr<ICampusEntity>> results;
    if (query.empty()) return results;

    // Polymorphic cross-entity search: query every heterogeneous entity repository
    for (const auto& b : const_cast<CampusRepository<BuildingEntity>&>(m_buildings).getAll()) {
        if (b->calculateRelevance(query) >= minRelevance) {
            results.push_back(b);
        }
    }
    for (const auto& f : const_cast<CampusRepository<FacilityEntity>&>(m_facilities).getAll()) {
        if (f->calculateRelevance(query) >= minRelevance) {
            results.push_back(f);
        }
    }
    for (const auto& d : const_cast<CampusRepository<DepartmentEntity>&>(m_departments).getAll()) {
        if (d->calculateRelevance(query) >= minRelevance) {
            results.push_back(d);
        }
    }
    for (const auto& fac : const_cast<CampusRepository<FacultyEntity>&>(m_faculty).getAll()) {
        if (fac->calculateRelevance(query) >= minRelevance) {
            results.push_back(fac);
        }
    }
    for (const auto& n : const_cast<CampusRepository<NoticeEntity>&>(m_notices).getAll()) {
        if (n->calculateRelevance(query) >= minRelevance) {
            results.push_back(n);
        }
    }
    for (const auto& faq : const_cast<CampusRepository<FaqEntity>&>(m_faqs).getAll()) {
        if (faq->calculateRelevance(query) >= minRelevance) {
            results.push_back(faq);
        }
    }

    // Sort polymorphically by calculated relevance score
    std::sort(results.begin(), results.end(), [&query](const std::shared_ptr<ICampusEntity>& a, const std::shared_ptr<ICampusEntity>& b) {
        return a->calculateRelevance(query) > b->calculateRelevance(query);
    });

    return results;
}

std::string CampusDatabase::getSystemStats() const {
    std::stringstream ss;
    ss << "=== SMART CAMPUS DATABASE STATS ===\n"
       << "Buildings:   " << const_cast<CampusRepository<BuildingEntity>&>(m_buildings).count() << "\n"
       << "Facilities:  " << const_cast<CampusRepository<FacilityEntity>&>(m_facilities).count() << "\n"
       << "Departments: " << const_cast<CampusRepository<DepartmentEntity>&>(m_departments).count() << "\n"
       << "Faculty:     " << const_cast<CampusRepository<FacultyEntity>&>(m_faculty).count() << "\n"
       << "Notices:     " << const_cast<CampusRepository<NoticeEntity>&>(m_notices).count() << "\n"
       << "FAQs:        " << const_cast<CampusRepository<FaqEntity>&>(m_faqs).count() << "\n"
       << "Status:      " << (m_initialized ? "ONLINE & SEEDED" : "PENDING INITIALIZATION") << "\n";
    return ss.str();
}
