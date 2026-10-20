#include "DatabaseManager.h"
#include <QDebug>
#include <QUuid>
#include <QVariant>
#include <QCryptographicHash>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent) {}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initialize(const QString& dbPath) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qCritical() << "Failed to open SQLite database:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery q;

    // Enable foreign keys
    q.exec("PRAGMA foreign_keys = ON;");

    // Buildings table
    q.exec("CREATE TABLE IF NOT EXISTS buildings ("
           "id TEXT PRIMARY KEY, "
           "name TEXT NOT NULL, "
           "code TEXT UNIQUE NOT NULL, "
           "description TEXT, "
           "latitude REAL, "
           "longitude REAL, "
           "floors INTEGER DEFAULT 1, "
           "yearBuilt INTEGER, "
           "wheelchairAccessible INTEGER DEFAULT 1, "
           "mapX INTEGER, "
           "mapY INTEGER, "
           "mapWidth INTEGER, "
           "mapHeight INTEGER, "
           "color TEXT"
           ");");

    // Facilities table
    q.exec("CREATE TABLE IF NOT EXISTS facilities ("
           "id TEXT PRIMARY KEY, "
           "name TEXT NOT NULL, "
           "category TEXT NOT NULL, "
           "description TEXT, "
           "buildingId TEXT, "
           "floor TEXT, "
           "roomNumber TEXT, "
           "landmark TEXT, "
           "timings TEXT, "
           "contactPhone TEXT, "
           "contactEmail TEXT, "
           "isOperational INTEGER DEFAULT 1, "
           "FOREIGN KEY(buildingId) REFERENCES buildings(id) ON DELETE SET NULL"
           ");");

    // Departments table
    q.exec("CREATE TABLE IF NOT EXISTS departments ("
           "id TEXT PRIMARY KEY, "
           "name TEXT NOT NULL, "
           "code TEXT UNIQUE NOT NULL, "
           "description TEXT, "
           "buildingId TEXT, "
           "headOfDepartment TEXT, "
           "contactEmail TEXT, "
           "contactPhone TEXT, "
           "websiteUrl TEXT, "
           "programsOffered TEXT, "
           "FOREIGN KEY(buildingId) REFERENCES buildings(id) ON DELETE SET NULL"
           ");");

    // Faculty table
    q.exec("CREATE TABLE IF NOT EXISTS faculty ("
           "id TEXT PRIMARY KEY, "
           "name TEXT NOT NULL, "
           "title TEXT DEFAULT 'Prof.', "
           "designation TEXT, "
           "departmentId TEXT, "
           "email TEXT, "
           "phone TEXT, "
           "cabinRoom TEXT, "
           "officeHours TEXT, "
           "specialization TEXT, "
           "bio TEXT, "
           "FOREIGN KEY(departmentId) REFERENCES departments(id) ON DELETE SET NULL"
           ");");

    // Notices table
    q.exec("CREATE TABLE IF NOT EXISTS notices ("
           "id TEXT PRIMARY KEY, "
           "title TEXT NOT NULL, "
           "content TEXT NOT NULL, "
           "category TEXT NOT NULL, "
           "priority TEXT DEFAULT 'Normal', "
           "publishDate TEXT, "
           "isPublished INTEGER DEFAULT 1, "
           "author TEXT DEFAULT 'Administration'"
           ");");

    // FAQs table
    q.exec("CREATE TABLE IF NOT EXISTS faqs ("
           "id TEXT PRIMARY KEY, "
           "question TEXT NOT NULL, "
           "answer TEXT NOT NULL, "
           "category TEXT NOT NULL, "
           "keywords TEXT, "
           "viewCount INTEGER DEFAULT 0"
           ");");

    // Synonyms table
    q.exec("CREATE TABLE IF NOT EXISTS synonyms ("
           "id TEXT PRIMARY KEY, "
           "primaryWord TEXT UNIQUE NOT NULL, "
           "synonyms TEXT NOT NULL"
           ");");

    // Chat messages table
    q.exec("CREATE TABLE IF NOT EXISTS chat_messages ("
           "id TEXT PRIMARY KEY, "
           "sessionId TEXT NOT NULL, "
           "role TEXT NOT NULL, "
           "content TEXT NOT NULL, "
           "helpful INTEGER DEFAULT NULL, "
           "feedbackText TEXT, "
           "referencedEntity TEXT, "
           "createdAt TEXT"
           ");");

    // Settings table
    q.exec("CREATE TABLE IF NOT EXISTS settings ("
           "key TEXT PRIMARY KEY, "
           "value TEXT"
           ");");

    // Admin table
    q.exec("CREATE TABLE IF NOT EXISTS admins ("
           "id TEXT PRIMARY KEY, "
           "username TEXT UNIQUE NOT NULL, "
           "passwordHash TEXT NOT NULL, "
           "fullName TEXT"
           ");");

    seedDatabaseIfEmpty();
    return true;
}

void DatabaseManager::seedDatabaseIfEmpty() {
    QSqlQuery countCheck("SELECT COUNT(*) FROM buildings;");
    if (countCheck.next() && countCheck.value(0).toInt() > 0) {
        return; // Already populated
    }

    qDebug() << "Seeding campus database with demonstration data...";

    // 1. Admin
    QString pHash = QString(QCryptographicHash::hash("adminpassword123", QCryptographicHash::Sha256).toHex());
    QSqlQuery q;
    q.prepare("INSERT INTO admins (id, username, passwordHash, fullName) VALUES ('adm-1', 'admin', :hash, 'Chief Campus Administrator');");
    q.bindValue(":hash", pHash);
    q.exec();

    // 2. Settings
    setSetting("collegeName", "Apex Institute of Technology & Management (Demo Campus)");
    setSetting("shortName", "Apex Tech");
    setSetting("tagline", "Empowering Future Leaders through Innovation and Excellence");
    setSetting("isDemoData", "true");
    setSetting("address", "Knowledge City, Tech Boulevard, Sector 42, Metro Campus - 560001");
    setSetting("contactPhone", "+91 (080) 4123-4567 / 89");
    setSetting("contactEmail", "assistant@smartcampus.edu");
    setSetting("workingHours", "Monday - Saturday: 8:30 AM to 5:30 PM");

    // 3. Buildings with visual schematic layout coordinates
    struct BSeed {
        QString id, name, code, desc;
        double lat, lng;
        int floors, year, access, x, y, w, h;
        QString color;
    };

    QList<BSeed> bSeeds = {
        {"bld-1", "Main Academic Block (Block A)", "AB-1", "Central lecture halls 101-410, Auditorium, and Engineering Labs.", 12.9716, 77.5946, 4, 2012, 1, 180, 140, 160, 110, "#1e40af"},
        {"bld-2", "Science & Technology Tower (Block B)", "ST-2", "High-tech tower with computing centers, AI labs, and Robotics workshops.", 12.9723, 77.5955, 5, 2018, 1, 400, 120, 150, 130, "#0369a1"},
        {"bld-3", "Central Library & Knowledge Center", "LIB-1", "3-tier digital and physical library with 60,000+ volumes and study spaces.", 12.9710, 77.5938, 3, 2014, 1, 140, 290, 130, 95, "#0d9488"},
        {"bld-4", "Student Amenity & Cafeteria (SAC)", "SAC-1", "Multi-cuisine food court, student club offices, and recreational games.", 12.9705, 77.5960, 2, 2016, 1, 330, 290, 140, 90, "#d97706"},
        {"bld-5", "Administrative & Admissions Complex", "ADM-1", "Principal office, Registrar, Admissions Counseling, and Accounts Section.", 12.9730, 77.5940, 3, 2010, 1, 40, 60, 150, 95, "#4338ca"},
        {"bld-6", "Sports Pavilion & Gymnasium", "SP-1", "Gymnasium, indoor badminton, basketball ground, and physical education.", 12.9736, 77.5966, 2, 2017, 0, 570, 80, 140, 100, "#15803d"},
        {"bld-7", "Campus Health Clinic & First Aid", "HC-1", "24/7 primary healthcare, doctor consultation, nurse desk, and pharmacy.", 12.9728, 77.5932, 1, 2015, 1, 40, 190, 110, 75, "#b91c1c"},
        {"bld-8", "Evergreen Hostels & Dining", "HST-1", "Residential quarters with dining mess, study rooms, and high-speed Wi-Fi.", 12.9694, 77.5968, 6, 2019, 1, 520, 260, 150, 120, "#6b21a8"}
    };

    for (const auto& b : bSeeds) {
        QSqlQuery bi;
        bi.prepare("INSERT INTO buildings VALUES (:id, :name, :code, :desc, :lat, :lng, :fl, :yr, :acc, :x, :y, :w, :h, :col);");
        bi.bindValue(":id", b.id);
        bi.bindValue(":name", b.name);
        bi.bindValue(":code", b.code);
        bi.bindValue(":desc", b.desc);
        bi.bindValue(":lat", b.lat);
        bi.bindValue(":lng", b.lng);
        bi.bindValue(":fl", b.floors);
        bi.bindValue(":yr", b.year);
        bi.bindValue(":acc", b.access);
        bi.bindValue(":x", b.x);
        bi.bindValue(":y", b.y);
        bi.bindValue(":w", b.w);
        bi.bindValue(":h", b.h);
        bi.bindValue(":col", b.color);
        bi.exec();
    }

    // 4. Departments
    struct DSeed {
        QString id, name, code, desc, bldId, hod, email, phone, web, progs;
    };
    QList<DSeed> dSeeds = {
        {"dept-1", "Computer Science & Engineering", "CSE", "Focuses on Artificial Intelligence, Software Engineering, Cloud Computing, and Cybersecurity.", "bld-2", "Dr. Rajesh Sharma, Ph.D.", "cse.hod@smartcampus.edu", "+91 (080) 4123-4510", "https://smartcampus.demo/cse", "B.Tech CSE, M.Tech AI & Data Science, Ph.D. in Computing"},
        {"dept-2", "Information Technology & Analytics", "IT", "Specializes in Distributed Systems, Big Data, DevOps, and Enterprise Architecture.", "bld-2", "Dr. Priya Menon, Ph.D.", "it.hod@smartcampus.edu", "+91 (080) 4123-4512", "https://smartcampus.demo/it", "B.Tech IT, M.Tech Big Data"},
        {"dept-3", "Electronics & Communication", "ECE", "Leading research in VLSI, Embedded Systems, IoT, and 5G Wireless Communication.", "bld-1", "Dr. Arvind Swaminathan, Ph.D.", "ece.hod@smartcampus.edu", "+91 (080) 4123-4520", "https://smartcampus.demo/ece", "B.Tech ECE, M.Tech Embedded Systems"},
        {"dept-4", "Mechanical & Mechatronics", "MECH", "Pioneering Robotics, CAD/CAM, Thermal Systems, and Smart Manufacturing.", "bld-1", "Dr. Suresh Kulkarni, Ph.D.", "mech.hod@smartcampus.edu", "+91 (080) 4123-4530", "https://smartcampus.demo/mech", "B.Tech Mechanical Engineering, B.Tech Mechatronics"},
        {"dept-5", "Civil & Environmental Engineering", "CIVIL", "Focus on Sustainable Infrastructure, Structural Engineering, and Smart Cities.", "bld-1", "Dr. Neha Agarwal, Ph.D.", "civil.hod@smartcampus.edu", "+91 (080) 4123-4540", "https://smartcampus.demo/civil", "B.Tech Civil Engineering"},
        {"dept-6", "Department of Management Studies", "MBA", "Excellence in Technology Management, Finance, Analytics, and Corporate Strategy.", "bld-5", "Dr. Vikram Malhotra, Ph.D.", "mba.hod@smartcampus.edu", "+91 (080) 4123-4550", "https://smartcampus.demo/mba", "MBA (Tech & Analytics)"}
    };

    for (const auto& d : dSeeds) {
        QSqlQuery di;
        di.prepare("INSERT INTO departments VALUES (:id, :name, :code, :desc, :bld, :hod, :email, :phone, :web, :progs);");
        di.bindValue(":id", d.id);
        di.bindValue(":name", d.name);
        di.bindValue(":code", d.code);
        di.bindValue(":desc", d.desc);
        di.bindValue(":bld", d.bldId);
        di.bindValue(":hod", d.hod);
        di.bindValue(":email", d.email);
        di.bindValue(":phone", d.phone);
        di.bindValue(":web", d.web);
        di.bindValue(":progs", d.progs);
        di.exec();
    }

    // 5. Facilities
    struct FSeed {
        QString id, name, cat, desc, bldId, floor, room, mark, times, phone, email;
    };
    QList<FSeed> fSeeds = {
        {"fac-1", "Central Library & Reading Halls", "Library", "Three floors of physical books, digital terminals, quiet study cubes, and journals.", "bld-3", "Ground & 1st Floor", "L-101 to L-205", "Directly in front of the Central Lawn", "Mon - Sat: 8:00 AM - 10:00 PM, Sun: 9:00 AM - 5:00 PM", "+91 (080) 4123-4580", "library@smartcampus.edu"},
        {"fac-2", "Campus Cafeteria & Food Court", "Cafeteria", "Multi-cuisine dining area serving vegetarian/non-vegetarian meals, fresh juices, and snacks.", "bld-4", "Ground Floor", "SAC-G01", "Next to Amphitheatre", "Daily: 7:30 AM - 9:30 PM", "+91 (080) 4123-4585", "cafeteria@smartcampus.edu"},
        {"fac-3", "Advanced AI & Deep Learning Research Lab", "Lab", "GPU workstation cluster equipped for neural networks, computer vision, and robotics.", "bld-2", "4th Floor", "Lab B-402", "Opposite Department Server Room", "Mon - Fri: 9:00 AM - 7:00 PM", "+91 (080) 4123-4515", "ailab@smartcampus.edu"},
        {"fac-4", "Admissions Counseling & Helpdesk", "Administration", "Information and counseling counter for prospective students, fee verification, and campus tours.", "bld-5", "Ground Floor", "Room AD-04", "Right beside the Admin Reception", "Mon - Sat: 9:00 AM - 5:00 PM", "+91 (080) 4123-4501", "admissions@smartcampus.edu"},
        {"fac-5", "Registrar & Student Affairs Office", "Administration", "Transcript requests, bonafide certificates, identity cards, and scholarship verification.", "bld-5", "1st Floor", "Room AD-105", "First door right above lobby", "Mon - Fri: 9:30 AM - 4:30 PM", "+91 (080) 4123-4505", "registrar@smartcampus.edu"},
        {"fac-6", "Accounts & Fee Payment Counter", "Administration", "Semester fee collection, fee receipts, challan verification, and account clearances.", "bld-5", "Ground Floor", "Counters 1-4, Room AD-08", "Adjacent to student waiting area", "Mon - Fri: 9:30 AM - 3:30 PM", "+91 (080) 4123-4508", "accounts@smartcampus.edu"},
        {"fac-7", "Campus Health Clinic & Pharmacy", "Medical", "Primary medical care, doctor consultations, emergency triage, and first-aid pharmacy.", "bld-7", "Ground Floor", "HC-01", "Beside North Gate Entrance", "24/7 Emergency / Doctor on Duty 8:00 AM - 8:00 PM", "+91 (080) 4123-4599", "healthcenter@smartcampus.edu"},
        {"fac-8", "Grand University Auditorium", "Academic", "1200-seat acoustic auditorium for convocation, cultural festivals, and technical symposiums.", "bld-1", "Ground Floor", "Main Auditorium Hall", "Central Quadrangle", "As per event schedule", "+91 (080) 4123-4522", "events@smartcampus.edu"},
        {"fac-9", "Gymnasium & Fitness Pavilion", "Sports", "Cardio machines, free weights, yoga studio, and certified instructors.", "bld-6", "1st Floor", "SP-101", "Overlooking the football turf", "Morning: 6:00 AM - 9:00 AM, Evening: 4:30 PM - 8:30 PM", "+91 (080) 4123-4590", "sports@smartcampus.edu"},
        {"fac-10", "Campus ATM Kiosk (SBI & HDFC)", "ATM", "24/7 ATM machines with cash withdrawal, deposit, and passbook printing.", "bld-5", "Ground Floor External", "Kiosk 1 & 2", "Near Gate 1 security post", "24 Hours / 7 Days", "Toll-Free 1800-425-3800", ""},
        {"fac-11", "Main North Parking (Gate 1)", "Transport", "Designated two-wheeler and four-wheeler visitor/student parking.", "bld-5", "Ground Level Lot", "Lot A & B", "Inside Gate 1", "6:30 AM - 10:30 PM", "+91 (080) 4123-4595", ""},
        {"fac-12", "Evergreen Hostel Warden Office", "Hostel", "Hostel admissions, room allocations, leave authorizations, and mess management.", "bld-8", "Ground Floor", "Room H-02", "Opposite residential garden", "Mon - Sat: 8:00 AM - 8:00 PM", "+91 (080) 4123-4570", "hostelwarden@smartcampus.edu"},
        {"fac-13", "Cloud Computing & Linux Lab", "Lab", "120 high-performance dual-boot workstations with gigabit fiber backbone.", "bld-2", "3rd Floor", "Lab B-305", "West Wing corridor", "Mon - Sat: 8:30 AM - 6:00 PM", "+91 (080) 4123-4516", "cslab@smartcampus.edu"}
    };

    for (const auto& f : fSeeds) {
        QSqlQuery fi;
        fi.prepare("INSERT INTO facilities VALUES (:id, :name, :cat, :desc, :bld, :fl, :rm, :lm, :tm, :ph, :em, 1);");
        fi.bindValue(":id", f.id);
        fi.bindValue(":name", f.name);
        fi.bindValue(":cat", f.cat);
        fi.bindValue(":desc", f.desc);
        fi.bindValue(":bld", f.bldId);
        fi.bindValue(":fl", f.floor);
        fi.bindValue(":rm", f.room);
        fi.bindValue(":lm", f.mark);
        fi.bindValue(":tm", f.times);
        fi.bindValue(":ph", f.phone);
        fi.bindValue(":em", f.email);
        fi.exec();
    }

    // 6. Faculty
    struct FacSeed {
        QString id, name, title, desig, deptId, email, phone, cabin, hours, spec, bio;
    };
    QList<FacSeed> facSeeds = {
        {"prof-1", "Rajesh Sharma", "Dr.", "Professor & HOD", "dept-1", "rajesh.sharma@smartcampus.edu", "+91 (080) 4123-4511", "Block B, Room 401", "Tue & Thu: 2:00 PM - 4:00 PM", "Artificial Intelligence, NLP, Deep Learning", "18+ years experience in neural network architectures and smart computing systems."},
        {"prof-2", "Ananya Roy", "Dr.", "Associate Professor", "dept-1", "ananya.roy@smartcampus.edu", "+91 (080) 4123-4514", "Block B, Room 308", "Mon & Wed: 11:00 AM - 1:00 PM", "Cloud Architecture, Microservices, Kubernetes", "Certified Cloud Architect with patents in containerized distributed computing."},
        {"prof-3", "Karthik Ramanathan", "Prof.", "Assistant Professor", "dept-1", "karthik.r@smartcampus.edu", "+91 (080) 4123-4518", "Block B, Room 304", "Fri: 10:00 AM - 12:30 PM", "Full Stack Development, Database Design, DevOps", "Faculty mentor for Google Developer Student Club and hackathons."},
        {"prof-4", "Priya Menon", "Dr.", "Professor & HOD", "dept-2", "priya.menon@smartcampus.edu", "+91 (080) 4123-4513", "Block B, Room 201", "Mon & Fri: 2:30 PM - 4:30 PM", "Big Data Analytics, Information Security", "Lead consultant on university cybersecurity and data science pipelines."},
        {"prof-5", "Arvind Swaminathan", "Dr.", "Professor & HOD", "dept-3", "arvind.s@smartcampus.edu", "+91 (080) 4123-4521", "Block A, Room 215", "Wed & Fri: 1:30 PM - 3:30 PM", "VLSI Chip Design, Embedded IoT, 5G Wireless", "Senior IEEE member with 25+ papers in low-power semiconductor design."},
        {"prof-6", "Suresh Kulkarni", "Dr.", "Professor & HOD", "dept-4", "suresh.kulkarni@smartcampus.edu", "+91 (080) 4123-4531", "Block A, Room 112", "Tue & Thu: 10:00 AM - 12:00 PM", "Robotics & Automation, 3D Printing, CAD", "Supervisor for collegiate Formula Student Racing competition team."},
        {"prof-7", "Neha Agarwal", "Dr.", "Professor & HOD", "dept-5", "neha.agarwal@smartcampus.edu", "+91 (080) 4123-4541", "Block A, Room 104", "Mon & Thu: 3:00 PM - 5:00 PM", "Earthquake Engineering, Green Buildings, Smart Drainage", "Advisory consultant to municipal urban resilience councils."},
        {"prof-8", "Vikram Malhotra", "Dr.", "Director & HOD", "dept-6", "vikram.malhotra@smartcampus.edu", "+91 (080) 4123-4551", "Admin Complex, Room 202", "Mon - Fri: 4:00 PM - 5:30 PM", "Strategic Management, Tech Entrepreneurship, Corporate Finance", "Over 20 years guiding startup incubation and corporate innovation programs."}
    };

    for (const auto& fac : facSeeds) {
        QSqlQuery faci;
        faci.prepare("INSERT INTO faculty VALUES (:id, :name, :title, :desig, :dept, :email, :phone, :cabin, :hours, :spec, :bio);");
        faci.bindValue(":id", fac.id);
        faci.bindValue(":name", fac.name);
        faci.bindValue(":title", fac.title);
        faci.bindValue(":desig", fac.desig);
        faci.bindValue(":dept", fac.deptId);
        faci.bindValue(":email", fac.email);
        faci.bindValue(":phone", fac.phone);
        faci.bindValue(":cabin", fac.cabin);
        faci.bindValue(":hours", fac.hours);
        faci.bindValue(":spec", fac.spec);
        faci.bindValue(":bio", fac.bio);
        faci.exec();
    }

    // 7. Notices
    struct NSeed {
        QString id, title, content, cat, priority, author;
    };
    QList<NSeed> nSeeds = {
        {"not-1", "Mid-Term Semester Examination Schedule - Autumn 2026", "Examinations for B.Tech, M.Tech, and MBA will commence from November 10, 2026. Hall tickets can be downloaded from the student portal after clearing fee dues.", "Exam", "Urgent", "Controller of Examinations"},
        {"not-2", "Annual Technical Fest 'TechVeda 2026' - Call for Project Registrations", "Apex Tech is excited to host TechVeda 2026 on November 25-27, 2026. Events include 24-hr Hackathon, Robowars, Paper Presentation, and AI Pitching. Prizes worth Rs. 3,50,000!", "Event", "High", "Student Affairs Committee"},
        {"not-3", "Tier-1 Campus Placement Drive by Top Tech Firms", "Registrations open for 7th semester B.Tech CSE, IT, and ECE students. Initial coding rounds are scheduled for October 28 in Computer Labs 305 and 306.", "Placement", "High", "Training & Placement Cell"},
        {"not-4", "National Scholarship Portal (NSP) Renewal & Fresh Applications", "Eligible students are requested to complete NSP renewal and submit verified documents at Registrar Office (AD-105) before October 31, 2026.", "Academic", "Normal", "Registrar Office"},
        {"not-5", "Campus Central Library Timings Extended for Exams", "Starting October 20, library reading halls will remain open until 11:30 PM on weekdays with high-speed Wi-Fi and power outlets active.", "General", "Normal", "Chief Librarian"}
    };

    for (const auto& n : nSeeds) {
        QSqlQuery ni;
        ni.prepare("INSERT INTO notices VALUES (:id, :title, :content, :cat, :prio, :date, 1, :auth);");
        ni.bindValue(":id", n.id);
        ni.bindValue(":title", n.title);
        ni.bindValue(":content", n.content);
        ni.bindValue(":cat", n.cat);
        ni.bindValue(":prio", n.priority);
        ni.bindValue(":date", QDateTime::currentDateTime().toString(Qt::ISODate));
        ni.bindValue(":auth", n.author);
        ni.exec();
    }

    // 8. FAQs
    struct QSeed {
        QString id, question, answer, cat, keys;
        int views;
    };
    QList<QSeed> qSeeds = {
        {"faq-1", "Where is the library and what are its timings?", "The Central Library is located in Block LIB-1, opposite the Central Lawn. It is open Monday to Saturday from 8:00 AM to 10:00 PM and Sundays from 9:00 AM to 5:00 PM.", "Facilities", "library, books, reading hall, timings, hours, borrow", 142},
        {"faq-2", "How can I reach the Computer Science and Engineering Department?", "The CSE Department is located in the Science & Technology Tower (Block B) on the 3rd and 4th floors. HOD office (Dr. Rajesh Sharma) is in Room B-401 on the 4th floor.", "Academics", "cse, computer science, block b, st-2, rajesh sharma, computing", 98},
        {"faq-3", "Where is the administrative office and accounts counter?", "The Administrative Complex (ADM-1) is near Gate 1. Accounts & Fee Counter is in Room AD-08 on Ground Floor, and Registrar Office is in Room AD-105 on 1st Floor.", "Administration", "admin, accounts, fees, registrar, admission, bonafide, adm-1", 85},
        {"faq-4", "Where is the college cafeteria and what are the food options?", "The Cafeteria & Food Court is on the Ground Floor of Student Amenity Center (SAC-1). It serves hot meals, snacks, juices, and coffee from 7:30 AM to 9:30 PM daily.", "Facilities", "canteen, cafeteria, food, lunch, breakfast, snacks, sac, eat", 110},
        {"faq-5", "What is the procedure for campus Wi-Fi access?", "Students can register their laptop or mobile MAC address at the IT Helpdesk (Block B, Room 102). WPA2 Enterprise login credentials will be issued within 24 hours.", "Facilities", "wifi, internet, login, network, mac address, it cell", 76},
        {"faq-6", "What medical facilities are available on campus?", "The Campus Health Clinic (HC-1) is near the North Gate. It is open 24/7 for emergencies, has a physician on duty 8:00 AM - 8:00 PM, and provides first aid and ambulance service.", "Facilities", "medical, clinic, doctor, health, hospital, emergency, medicine, hc-1", 54},
        {"faq-7", "How do I contact the hostel warden or apply for accommodation?", "Hostel inquiries are handled at the Evergreen Hostels Office (HST-1, Room H-02, Ground Floor) or via email hostelwarden@smartcampus.edu, phone +91 (080) 4123-4570.", "Hostel", "hostel, room, warden, stay, boys hostel, girls hostel, mess", 63},
        {"faq-8", "Where is the college auditorium located?", "The Grand University Auditorium is on the Ground Floor of Main Academic Block (Block A), accessible from the central quadrangle.", "Facilities", "auditorium, hall, seminar, cultural, stage, block a, ab-1", 47},
        {"faq-9", "Where can I find parking on campus?", "Student two-wheeler and visitor parking is at the North Parking Lot near Gate 1. Faculty parking is reserved behind Main Academic Block A.", "Transport", "parking, vehicle, bike, car, cycle, gate 1", 39},
        {"faq-10", "Are there ATM and banking facilities inside campus?", "Yes, 24/7 SBI and HDFC Bank ATMs are located near the Gate 1 entrance at the external wall of Administrative Complex (ADM-1).", "Facilities", "atm, bank, money, cash, sbi, hdfc", 42}
    };

    for (const auto& qf : qSeeds) {
        QSqlQuery qi;
        qi.prepare("INSERT INTO faqs VALUES (:id, :q, :a, :cat, :keys, :views);");
        qi.bindValue(":id", qf.id);
        qi.bindValue(":q", qf.question);
        qi.bindValue(":a", qf.answer);
        qi.bindValue(":cat", qf.cat);
        qi.bindValue(":keys", qf.keys);
        qi.bindValue(":views", qf.views);
        qi.exec();
    }

    // 9. Synonyms
    struct SSeed {
        QString id, word, syns;
    };
    QList<SSeed> sSeeds = {
        {"syn-1", "library", "books,reading room,reading hall,study hall,journals,central library,lib"},
        {"syn-2", "canteen", "cafeteria,food court,mess,food,lunch,dinner,breakfast,snacks,cafe,coffee shop"},
        {"syn-3", "administration", "admin,principal,registrar,accounts,fees,admission,office,director"},
        {"syn-4", "computer science", "cse,computer,cs,software,coding,programming,ai,ml,it department"},
        {"syn-5", "washroom", "restroom,toilet,latrine,bathroom,water closet,loo"},
        {"syn-6", "laboratory", "lab,practical room,research lab,computing lab,hardware lab"},
        {"syn-7", "hostel", "dorm,dormitory,residence,boys hostel,girls hostel,mess"},
        {"syn-8", "medical", "doctor,clinic,health center,first aid,hospital,nurse,medicine,pharmacy"},
        {"syn-9", "sports", "gym,gymnasium,playground,football ground,badminton,basketball,fitness"},
        {"syn-10", "parking", "vehicle,bike parking,car parking,scooter parking,parking lot"},
        {"syn-11", "exam", "examination,test,mid term,end term,timetable,datesheet,results,hall ticket"},
        {"syn-12", "auditorium", "hall,seminar hall,convocation hall,stage,amphitheatre,events hall"},
        {"syn-13", "contact", "phone,email,number,helpline,reach,call,directory"},
        {"syn-14", "admission", "enrollment,apply,intake,eligibility,counseling,fees structure"}
    };

    for (const auto& s : sSeeds) {
        QSqlQuery si;
        si.prepare("INSERT INTO synonyms VALUES (:id, :word, :syns);");
        si.bindValue(":id", s.id);
        si.bindValue(":word", s.word);
        si.bindValue(":syns", s.syns);
        si.exec();
    }

    qDebug() << "Database seeding completed successfully.";
}

// BUILDINGS
QList<Building> DatabaseManager::getAllBuildings() {
    QList<Building> list;
    QSqlQuery q("SELECT id, name, code, description, latitude, longitude, floors, yearBuilt, wheelchairAccessible, mapX, mapY, mapWidth, mapHeight, color FROM buildings ORDER BY name ASC;");
    while (q.next()) {
        Building b;
        b.id = q.value(0).toString();
        b.name = q.value(1).toString();
        b.code = q.value(2).toString();
        b.description = q.value(3).toString();
        b.latitude = q.value(4).toDouble();
        b.longitude = q.value(5).toDouble();
        b.floors = q.value(6).toInt();
        b.yearBuilt = q.value(7).toInt();
        b.wheelchairAccessible = q.value(8).toBool();
        b.mapX = q.value(9).toInt();
        b.mapY = q.value(10).toInt();
        b.mapWidth = q.value(11).toInt();
        b.mapHeight = q.value(12).toInt();
        b.color = q.value(13).toString();
        list.append(b);
    }
    return list;
}

Building DatabaseManager::getBuildingById(const QString& id) {
    Building b;
    QSqlQuery q;
    q.prepare("SELECT id, name, code, description, latitude, longitude, floors, yearBuilt, wheelchairAccessible, mapX, mapY, mapWidth, mapHeight, color FROM buildings WHERE id = :id;");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        b.id = q.value(0).toString();
        b.name = q.value(1).toString();
        b.code = q.value(2).toString();
        b.description = q.value(3).toString();
        b.latitude = q.value(4).toDouble();
        b.longitude = q.value(5).toDouble();
        b.floors = q.value(6).toInt();
        b.yearBuilt = q.value(7).toInt();
        b.wheelchairAccessible = q.value(8).toBool();
        b.mapX = q.value(9).toInt();
        b.mapY = q.value(10).toInt();
        b.mapWidth = q.value(11).toInt();
        b.mapHeight = q.value(12).toInt();
        b.color = q.value(13).toString();
    }
    return b;
}

bool DatabaseManager::addBuilding(const Building& b) {
    QSqlQuery q;
    q.prepare("INSERT INTO buildings VALUES (:id, :name, :code, :desc, :lat, :lng, :fl, :yr, :acc, :x, :y, :w, :h, :col);");
    QString id = b.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : b.id;
    q.bindValue(":id", id);
    q.bindValue(":name", b.name);
    q.bindValue(":code", b.code);
    q.bindValue(":desc", b.description);
    q.bindValue(":lat", b.latitude);
    q.bindValue(":lng", b.longitude);
    q.bindValue(":fl", b.floors);
    q.bindValue(":yr", b.yearBuilt);
    q.bindValue(":acc", b.wheelchairAccessible ? 1 : 0);
    q.bindValue(":x", b.mapX);
    q.bindValue(":y", b.mapY);
    q.bindValue(":w", b.mapWidth > 0 ? b.mapWidth : 120);
    q.bindValue(":h", b.mapHeight > 0 ? b.mapHeight : 80);
    q.bindValue(":col", b.color.isEmpty() ? "#1e40af" : b.color);
    return q.exec();
}

bool DatabaseManager::updateBuilding(const Building& b) {
    QSqlQuery q;
    q.prepare("UPDATE buildings SET name=:name, code=:code, description=:desc, floors=:fl, wheelchairAccessible=:acc WHERE id=:id;");
    q.bindValue(":id", b.id);
    q.bindValue(":name", b.name);
    q.bindValue(":code", b.code);
    q.bindValue(":desc", b.description);
    q.bindValue(":fl", b.floors);
    q.bindValue(":acc", b.wheelchairAccessible ? 1 : 0);
    return q.exec();
}

bool DatabaseManager::deleteBuilding(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM buildings WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

// FACILITIES
QList<Facility> DatabaseManager::getAllFacilities(const QString& categoryFilter) {
    QList<Facility> list;
    QString sql = "SELECT f.id, f.name, f.category, f.description, f.buildingId, b.name, f.floor, f.roomNumber, f.landmark, f.timings, f.contactPhone, f.contactEmail, f.isOperational "
                  "FROM facilities f LEFT JOIN buildings b ON f.buildingId = b.id ";
    if (!categoryFilter.isEmpty() && categoryFilter != "All") {
        sql += "WHERE f.category = '" + categoryFilter + "' ";
    }
    sql += "ORDER BY f.name ASC;";

    QSqlQuery q(sql);
    while (q.next()) {
        Facility f;
        f.id = q.value(0).toString();
        f.name = q.value(1).toString();
        f.category = q.value(2).toString();
        f.description = q.value(3).toString();
        f.buildingId = q.value(4).toString();
        f.buildingName = q.value(5).toString();
        f.floor = q.value(6).toString();
        f.roomNumber = q.value(7).toString();
        f.landmark = q.value(8).toString();
        f.timings = q.value(9).toString();
        f.contactPhone = q.value(10).toString();
        f.contactEmail = q.value(11).toString();
        f.isOperational = q.value(12).toBool();
        list.append(f);
    }
    return list;
}

Facility DatabaseManager::getFacilityById(const QString& id) {
    Facility f;
    QSqlQuery q;
    q.prepare("SELECT f.id, f.name, f.category, f.description, f.buildingId, b.name, f.floor, f.roomNumber, f.landmark, f.timings, f.contactPhone, f.contactEmail, f.isOperational "
              "FROM facilities f LEFT JOIN buildings b ON f.buildingId = b.id WHERE f.id = :id;");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        f.id = q.value(0).toString();
        f.name = q.value(1).toString();
        f.category = q.value(2).toString();
        f.description = q.value(3).toString();
        f.buildingId = q.value(4).toString();
        f.buildingName = q.value(5).toString();
        f.floor = q.value(6).toString();
        f.roomNumber = q.value(7).toString();
        f.landmark = q.value(8).toString();
        f.timings = q.value(9).toString();
        f.contactPhone = q.value(10).toString();
        f.contactEmail = q.value(11).toString();
        f.isOperational = q.value(12).toBool();
    }
    return f;
}

bool DatabaseManager::addFacility(const Facility& f) {
    QSqlQuery q;
    q.prepare("INSERT INTO facilities VALUES (:id, :name, :cat, :desc, :bld, :fl, :rm, :lm, :tm, :ph, :em, :op);");
    QString id = f.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : f.id;
    q.bindValue(":id", id);
    q.bindValue(":name", f.name);
    q.bindValue(":cat", f.category);
    q.bindValue(":desc", f.description);
    q.bindValue(":bld", f.buildingId.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : f.buildingId);
    q.bindValue(":fl", f.floor);
    q.bindValue(":rm", f.roomNumber);
    q.bindValue(":lm", f.landmark);
    q.bindValue(":tm", f.timings);
    q.bindValue(":ph", f.contactPhone);
    q.bindValue(":em", f.contactEmail);
    q.bindValue(":op", f.isOperational ? 1 : 0);
    return q.exec();
}

bool DatabaseManager::updateFacility(const Facility& f) {
    QSqlQuery q;
    q.prepare("UPDATE facilities SET name=:name, category=:cat, description=:desc, buildingId=:bld, floor=:fl, roomNumber=:rm, landmark=:lm, timings=:tm, contactPhone=:ph, contactEmail=:em, isOperational=:op WHERE id=:id;");
    q.bindValue(":id", f.id);
    q.bindValue(":name", f.name);
    q.bindValue(":cat", f.category);
    q.bindValue(":desc", f.description);
    q.bindValue(":bld", f.buildingId.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : f.buildingId);
    q.bindValue(":fl", f.floor);
    q.bindValue(":rm", f.roomNumber);
    q.bindValue(":lm", f.landmark);
    q.bindValue(":tm", f.timings);
    q.bindValue(":ph", f.contactPhone);
    q.bindValue(":em", f.contactEmail);
    q.bindValue(":op", f.isOperational ? 1 : 0);
    return q.exec();
}

bool DatabaseManager::deleteFacility(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM facilities WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

// DEPARTMENTS
QList<Department> DatabaseManager::getAllDepartments() {
    QList<Department> list;
    QSqlQuery q("SELECT d.id, d.name, d.code, d.description, d.buildingId, b.name, d.headOfDepartment, d.contactEmail, d.contactPhone, d.websiteUrl, d.programsOffered "
                "FROM departments d LEFT JOIN buildings b ON d.buildingId = b.id ORDER BY d.name ASC;");
    while (q.next()) {
        Department d;
        d.id = q.value(0).toString();
        d.name = q.value(1).toString();
        d.code = q.value(2).toString();
        d.description = q.value(3).toString();
        d.buildingId = q.value(4).toString();
        d.buildingName = q.value(5).toString();
        d.headOfDepartment = q.value(6).toString();
        d.contactEmail = q.value(7).toString();
        d.contactPhone = q.value(8).toString();
        d.websiteUrl = q.value(9).toString();
        d.programsOffered = q.value(10).toString();
        list.append(d);
    }
    return list;
}

Department DatabaseManager::getDepartmentByCode(const QString& code) {
    Department d;
    QSqlQuery q;
    q.prepare("SELECT d.id, d.name, d.code, d.description, d.buildingId, b.name, d.headOfDepartment, d.contactEmail, d.contactPhone, d.websiteUrl, d.programsOffered "
              "FROM departments d LEFT JOIN buildings b ON d.buildingId = b.id WHERE d.code = :code;");
    q.bindValue(":code", code.toUpper());
    if (q.exec() && q.next()) {
        d.id = q.value(0).toString();
        d.name = q.value(1).toString();
        d.code = q.value(2).toString();
        d.description = q.value(3).toString();
        d.buildingId = q.value(4).toString();
        d.buildingName = q.value(5).toString();
        d.headOfDepartment = q.value(6).toString();
        d.contactEmail = q.value(7).toString();
        d.contactPhone = q.value(8).toString();
        d.websiteUrl = q.value(9).toString();
        d.programsOffered = q.value(10).toString();
    }
    return d;
}

bool DatabaseManager::addDepartment(const Department& d) {
    QSqlQuery q;
    q.prepare("INSERT INTO departments VALUES (:id, :name, :code, :desc, :bld, :hod, :em, :ph, :web, :prog);");
    QString id = d.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : d.id;
    q.bindValue(":id", id);
    q.bindValue(":name", d.name);
    q.bindValue(":code", d.code);
    q.bindValue(":desc", d.description);
    q.bindValue(":bld", d.buildingId.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : d.buildingId);
    q.bindValue(":hod", d.headOfDepartment);
    q.bindValue(":em", d.contactEmail);
    q.bindValue(":ph", d.contactPhone);
    q.bindValue(":web", d.websiteUrl);
    q.bindValue(":prog", d.programsOffered);
    return q.exec();
}

bool DatabaseManager::updateDepartment(const Department& d) {
    QSqlQuery q;
    q.prepare("UPDATE departments SET name=:name, code=:code, description=:desc, buildingId=:bld, headOfDepartment=:hod, contactEmail=:em, contactPhone=:ph, websiteUrl=:web, programsOffered=:prog WHERE id=:id;");
    q.bindValue(":id", d.id);
    q.bindValue(":name", d.name);
    q.bindValue(":code", d.code);
    q.bindValue(":desc", d.description);
    q.bindValue(":bld", d.buildingId.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : d.buildingId);
    q.bindValue(":hod", d.headOfDepartment);
    q.bindValue(":em", d.contactEmail);
    q.bindValue(":ph", d.contactPhone);
    q.bindValue(":web", d.websiteUrl);
    q.bindValue(":prog", d.programsOffered);
    return q.exec();
}

bool DatabaseManager::deleteDepartment(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM departments WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

// FACULTY
QList<Faculty> DatabaseManager::getAllFaculty(const QString& departmentIdFilter) {
    QList<Faculty> list;
    QString sql = "SELECT f.id, f.name, f.title, f.designation, f.departmentId, d.name, f.email, f.phone, f.cabinRoom, f.officeHours, f.specialization, f.bio "
                  "FROM faculty f LEFT JOIN departments d ON f.departmentId = d.id ";
    if (!departmentIdFilter.isEmpty() && departmentIdFilter != "All") {
        sql += "WHERE f.departmentId = '" + departmentIdFilter + "' ";
    }
    sql += "ORDER BY f.name ASC;";

    QSqlQuery q(sql);
    while (q.next()) {
        Faculty fac;
        fac.id = q.value(0).toString();
        fac.name = q.value(1).toString();
        fac.title = q.value(2).toString();
        fac.designation = q.value(3).toString();
        fac.departmentId = q.value(4).toString();
        fac.departmentName = q.value(5).toString();
        fac.email = q.value(6).toString();
        fac.phone = q.value(7).toString();
        fac.cabinRoom = q.value(8).toString();
        fac.officeHours = q.value(9).toString();
        fac.specialization = q.value(10).toString();
        fac.bio = q.value(11).toString();
        list.append(fac);
    }
    return list;
}

Faculty DatabaseManager::getFacultyById(const QString& id) {
    Faculty fac;
    QSqlQuery q;
    q.prepare("SELECT f.id, f.name, f.title, f.designation, f.departmentId, d.name, f.email, f.phone, f.cabinRoom, f.officeHours, f.specialization, f.bio "
              "FROM faculty f LEFT JOIN departments d ON f.departmentId = d.id WHERE f.id = :id;");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        fac.id = q.value(0).toString();
        fac.name = q.value(1).toString();
        fac.title = q.value(2).toString();
        fac.designation = q.value(3).toString();
        fac.departmentId = q.value(4).toString();
        fac.departmentName = q.value(5).toString();
        fac.email = q.value(6).toString();
        fac.phone = q.value(7).toString();
        fac.cabinRoom = q.value(8).toString();
        fac.officeHours = q.value(9).toString();
        fac.specialization = q.value(10).toString();
        fac.bio = q.value(11).toString();
    }
    return fac;
}

bool DatabaseManager::addFaculty(const Faculty& f) {
    QSqlQuery q;
    q.prepare("INSERT INTO faculty VALUES (:id, :name, :title, :desig, :dept, :email, :phone, :cabin, :hours, :spec, :bio);");
    QString id = f.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : f.id;
    q.bindValue(":id", id);
    q.bindValue(":name", f.name);
    q.bindValue(":title", f.title.isEmpty() ? "Prof." : f.title);
    q.bindValue(":desig", f.designation);
    q.bindValue(":dept", f.departmentId.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : f.departmentId);
    q.bindValue(":email", f.email);
    q.bindValue(":phone", f.phone);
    q.bindValue(":cabin", f.cabinRoom);
    q.bindValue(":hours", f.officeHours);
    q.bindValue(":spec", f.specialization);
    q.bindValue(":bio", f.bio);
    return q.exec();
}

bool DatabaseManager::updateFaculty(const Faculty& f) {
    QSqlQuery q;
    q.prepare("UPDATE faculty SET name=:name, title=:title, designation=:desig, departmentId=:dept, email=:email, phone=:phone, cabinRoom=:cabin, officeHours=:hours, specialization=:spec, bio=:bio WHERE id=:id;");
    q.bindValue(":id", f.id);
    q.bindValue(":name", f.name);
    q.bindValue(":title", f.title);
    q.bindValue(":desig", f.designation);
    q.bindValue(":dept", f.departmentId.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : f.departmentId);
    q.bindValue(":email", f.email);
    q.bindValue(":phone", f.phone);
    q.bindValue(":cabin", f.cabinRoom);
    q.bindValue(":hours", f.officeHours);
    q.bindValue(":spec", f.specialization);
    q.bindValue(":bio", f.bio);
    return q.exec();
}

bool DatabaseManager::deleteFaculty(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM faculty WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

// NOTICES
QList<Notice> DatabaseManager::getAllNotices(const QString& categoryFilter) {
    QList<Notice> list;
    QString sql = "SELECT id, title, content, category, priority, publishDate, isPublished, author FROM notices WHERE isPublished = 1 ";
    if (!categoryFilter.isEmpty() && categoryFilter != "All") {
        sql += "AND category = '" + categoryFilter + "' ";
    }
    sql += "ORDER BY publishDate DESC;";

    QSqlQuery q(sql);
    while (q.next()) {
        Notice n;
        n.id = q.value(0).toString();
        n.title = q.value(1).toString();
        n.content = q.value(2).toString();
        n.category = q.value(3).toString();
        n.priority = q.value(4).toString();
        n.publishDate = QDateTime::fromString(q.value(5).toString(), Qt::ISODate);
        n.isPublished = q.value(6).toBool();
        n.author = q.value(7).toString();
        list.append(n);
    }
    return list;
}

bool DatabaseManager::addNotice(const Notice& n) {
    QSqlQuery q;
    q.prepare("INSERT INTO notices VALUES (:id, :title, :content, :cat, :prio, :date, :pub, :auth);");
    QString id = n.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : n.id;
    q.bindValue(":id", id);
    q.bindValue(":title", n.title);
    q.bindValue(":content", n.content);
    q.bindValue(":cat", n.category);
    q.bindValue(":prio", n.priority);
    q.bindValue(":date", n.publishDate.isValid() ? n.publishDate.toString(Qt::ISODate) : QDateTime::currentDateTime().toString(Qt::ISODate));
    q.bindValue(":pub", n.isPublished ? 1 : 0);
    q.bindValue(":auth", n.author.isEmpty() ? "Administration" : n.author);
    return q.exec();
}

bool DatabaseManager::updateNotice(const Notice& n) {
    QSqlQuery q;
    q.prepare("UPDATE notices SET title=:title, content=:content, category=:cat, priority=:prio, isPublished=:pub, author=:auth WHERE id=:id;");
    q.bindValue(":id", n.id);
    q.bindValue(":title", n.title);
    q.bindValue(":content", n.content);
    q.bindValue(":cat", n.category);
    q.bindValue(":prio", n.priority);
    q.bindValue(":pub", n.isPublished ? 1 : 0);
    q.bindValue(":auth", n.author);
    return q.exec();
}

bool DatabaseManager::deleteNotice(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM notices WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

// FAQS
QList<Faq> DatabaseManager::getAllFaqs(const QString& categoryFilter) {
    QList<Faq> list;
    QString sql = "SELECT id, question, answer, category, keywords, viewCount FROM faqs ";
    if (!categoryFilter.isEmpty() && categoryFilter != "All") {
        sql += "WHERE category = '" + categoryFilter + "' ";
    }
    sql += "ORDER BY viewCount DESC;";

    QSqlQuery q(sql);
    while (q.next()) {
        Faq f;
        f.id = q.value(0).toString();
        f.question = q.value(1).toString();
        f.answer = q.value(2).toString();
        f.category = q.value(3).toString();
        f.keywords = q.value(4).toString();
        f.viewCount = q.value(5).toInt();
        list.append(f);
    }
    return list;
}

bool DatabaseManager::addFaq(const Faq& f) {
    QSqlQuery q;
    q.prepare("INSERT INTO faqs VALUES (:id, :q, :a, :cat, :keys, :views);");
    QString id = f.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : f.id;
    q.bindValue(":id", id);
    q.bindValue(":q", f.question);
    q.bindValue(":a", f.answer);
    q.bindValue(":cat", f.category);
    q.bindValue(":keys", f.keywords);
    q.bindValue(":views", f.viewCount);
    return q.exec();
}

bool DatabaseManager::updateFaq(const Faq& f) {
    QSqlQuery q;
    q.prepare("UPDATE faqs SET question=:q, answer=:a, category=:cat, keywords=:keys WHERE id=:id;");
    q.bindValue(":id", f.id);
    q.bindValue(":q", f.question);
    q.bindValue(":a", f.answer);
    q.bindValue(":cat", f.category);
    q.bindValue(":keys", f.keywords);
    return q.exec();
}

bool DatabaseManager::deleteFaq(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM faqs WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

void DatabaseManager::incrementFaqView(const QString& id) {
    QSqlQuery q;
    q.prepare("UPDATE faqs SET viewCount = viewCount + 1 WHERE id = :id;");
    q.bindValue(":id", id);
    q.exec();
}

// SYNONYMS
QList<Synonym> DatabaseManager::getAllSynonyms() {
    QList<Synonym> list;
    QSqlQuery q("SELECT id, primaryWord, synonyms FROM synonyms;");
    while (q.next()) {
        Synonym s;
        s.id = q.value(0).toString();
        s.primaryWord = q.value(1).toString();
        s.synonyms = q.value(2).toString().split(',', Qt::SkipEmptyParts);
        for (int i = 0; i < s.synonyms.size(); ++i) {
            s.synonyms[i] = s.synonyms[i].trimmed();
        }
        list.append(s);
    }
    return list;
}

bool DatabaseManager::addSynonym(const QString& primaryWord, const QStringList& synonyms) {
    QSqlQuery q;
    q.prepare("INSERT OR REPLACE INTO synonyms VALUES (:id, :word, :syns);");
    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    q.bindValue(":id", id);
    q.bindValue(":word", primaryWord.toLower().trimmed());
    q.bindValue(":syns", synonyms.join(','));
    return q.exec();
}

// CHAT MESSAGES
QString DatabaseManager::saveChatMessage(const QString& sessionId, const QString& role, const QString& content, const QString& refEntityJson) {
    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QSqlQuery q;
    q.prepare("INSERT INTO chat_messages (id, sessionId, role, content, referencedEntity, createdAt) VALUES (:id, :sess, :role, :content, :ref, :date);");
    q.bindValue(":id", id);
    q.bindValue(":sess", sessionId);
    q.bindValue(":role", role);
    q.bindValue(":content", content);
    q.bindValue(":ref", refEntityJson);
    q.bindValue(":date", QDateTime::currentDateTime().toString(Qt::ISODate));
    q.exec();
    return id;
}

bool DatabaseManager::recordFeedback(const QString& messageId, bool helpful, const QString& comment) {
    QSqlQuery q;
    q.prepare("UPDATE chat_messages SET helpful = :h, feedbackText = :c WHERE id = :id;");
    q.bindValue(":id", messageId);
    q.bindValue(":h", helpful ? 1 : 0);
    q.bindValue(":c", comment);
    return q.exec();
}

QList<ChatMessageRecord> DatabaseManager::getRecentChatMessages(const QString& sessionId, int limit) {
    QList<ChatMessageRecord> list;
    QSqlQuery q;
    q.prepare("SELECT id, sessionId, role, content, helpful, feedbackText, referencedEntity, createdAt FROM chat_messages WHERE sessionId = :sess ORDER BY createdAt ASC LIMIT :lim;");
    q.bindValue(":sess", sessionId);
    q.bindValue(":lim", limit);
    if (q.exec()) {
        while (q.next()) {
            ChatMessageRecord r;
            r.id = q.value(0).toString();
            r.sessionId = q.value(1).toString();
            r.role = q.value(2).toString();
            r.content = q.value(3).toString();
            r.hasFeedback = !q.value(4).isNull();
            r.helpful = q.value(4).toBool();
            r.referencedEntity = q.value(6).toString();
            r.createdAt = QDateTime::fromString(q.value(7).toString(), Qt::ISODate);
            list.append(r);
        }
    }
    return list;
}

QMap<QString, int> DatabaseManager::getChatbotAnalytics() {
    QMap<QString, int> stats;
    QSqlQuery q;

    q.exec("SELECT COUNT(*) FROM chat_messages WHERE role = 'user';");
    if (q.next()) stats["total_queries"] = q.value(0).toInt();

    q.exec("SELECT COUNT(*) FROM chat_messages WHERE helpful = 1;");
    if (q.next()) stats["helpful_count"] = q.value(0).toInt();

    q.exec("SELECT COUNT(*) FROM chat_messages WHERE helpful = 0;");
    if (q.next()) stats["unhelpful_count"] = q.value(0).toInt();

    q.exec("SELECT COUNT(*) FROM facilities;");
    if (q.next()) stats["total_facilities"] = q.value(0).toInt();

    q.exec("SELECT COUNT(*) FROM faculty;");
    if (q.next()) stats["total_faculty"] = q.value(0).toInt();

    q.exec("SELECT COUNT(*) FROM notices;");
    if (q.next()) stats["total_notices"] = q.value(0).toInt();

    return stats;
}

// SETTINGS
QString DatabaseManager::getSetting(const QString& key, const QString& defaultValue) {
    QSqlQuery q;
    q.prepare("SELECT value FROM settings WHERE key = :k;");
    q.bindValue(":k", key);
    if (q.exec() && q.next()) {
        return q.value(0).toString();
    }
    return defaultValue;
}

void DatabaseManager::setSetting(const QString& key, const QString& value) {
    QSqlQuery q;
    q.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:k, :v);");
    q.bindValue(":k", key);
    q.bindValue(":v", value);
    q.exec();
}

// ADMIN AUTH
bool DatabaseManager::verifyAdmin(const QString& username, const QString& password) {
    QSqlQuery q;
    q.prepare("SELECT passwordHash FROM admins WHERE username = :u;");
    q.bindValue(":u", username);
    if (q.exec() && q.next()) {
        QString storedHash = q.value(0).toString();
        QString inputHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
        return (storedHash == inputHash);
    }
    return false;
}
