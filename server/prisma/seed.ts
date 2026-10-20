import { PrismaClient } from '@prisma/client';
import bcrypt from 'bcryptjs';

const prisma = new PrismaClient();

async function main() {
  console.log('🌱 Starting database seed...');

  // Clear existing data in reverse dependency order
  await prisma.chatMessage.deleteMany();
  await prisma.faculty.deleteMany();
  await prisma.department.deleteMany();
  await prisma.facility.deleteMany();
  await prisma.building.deleteMany();
  await prisma.notice.deleteMany();
  await prisma.faq.deleteMany();
  await prisma.synonym.deleteMany();
  await prisma.campusSetting.deleteMany();
  await prisma.adminUser.deleteMany();

  console.log('🧹 Cleaned existing database tables.');

  // 1. Admin User
  const passwordHash = await bcrypt.hash('adminpassword123', 10);
  await prisma.adminUser.create({
    data: {
      username: 'admin',
      passwordHash,
      fullName: 'Chief Campus Administrator',
      role: 'superadmin',
    },
  });
  console.log('👤 Admin user created (username: admin, password: adminpassword123)');

  // 2. Campus Settings (clearly identified as Demo Data)
  const settings = [
    { key: 'collegeName', value: 'Apex Institute of Technology & Management (Demo Campus)' },
    { key: 'shortName', value: 'Apex Tech' },
    { key: 'tagline', value: 'Empowering Future Leaders through Innovation and Excellence' },
    { key: 'isDemoData', value: 'true' },
    { key: 'address', value: 'Knowledge City, Tech Boulevard, Sector 42, Metro Campus - 560001' },
    { key: 'contactPhone', value: '+91 (080) 4123-4567 / 89' },
    { key: 'contactEmail', value: 'assistant@smartcampus.edu' },
    { key: 'admissionsEmail', value: 'admissions@smartcampus.edu' },
    { key: 'website', value: 'https://smartcampus.demo' },
    { key: 'workingHours', value: 'Monday - Saturday: 8:30 AM to 5:30 PM (Administrative Offices)' },
    { key: 'campusCenterLat', value: '12.9716' },
    { key: 'campusCenterLng', value: '77.5946' },
    { key: 'mapZoom', value: '17' },
    { key: 'externalAiEnabled', value: 'false' },
    { key: 'externalAiProvider', value: 'openai' },
  ];

  for (const s of settings) {
    await prisma.campusSetting.create({ data: s });
  }

  // 3. Buildings
  const buildingsData = [
    {
      name: 'Main Academic Block (Block A)',
      code: 'AB-1',
      description: 'Central academic complex housing Lecture Halls 101-410, Auditorium, and Engineering Departments.',
      latitude: 12.9716,
      longitude: 77.5946,
      floors: 4,
      yearBuilt: 2012,
      wheelchairAccessible: true,
    },
    {
      name: 'Science & Technology Tower (Block B)',
      code: 'ST-2',
      description: 'High-tech tower with computing centers, AI labs, Robotics lab, and Electronics workshops.',
      latitude: 12.9723,
      longitude: 77.5955,
      floors: 5,
      yearBuilt: 2018,
      wheelchairAccessible: true,
    },
    {
      name: 'Central Library & Knowledge Resource Center',
      code: 'LIB-1',
      description: '3-tier digital & physical library with over 60,000 volumes, e-journals, and silent study zones.',
      latitude: 12.9710,
      longitude: 77.5938,
      floors: 3,
      yearBuilt: 2014,
      wheelchairAccessible: true,
    },
    {
      name: 'Student Amenity & Cafeteria Complex (SAC)',
      code: 'SAC-1',
      description: 'Multi-cuisine food court, student club offices, indoor sports games, and stationery store.',
      latitude: 12.9705,
      longitude: 77.5960,
      floors: 2,
      yearBuilt: 2016,
      wheelchairAccessible: true,
    },
    {
      name: 'Administrative & Admissions Complex',
      code: 'ADM-1',
      description: 'Headquarters for Principal, Registrar, Admissions Counseling, Accounts, and Examination Control.',
      latitude: 12.9730,
      longitude: 77.5940,
      floors: 3,
      yearBuilt: 2010,
      wheelchairAccessible: true,
    },
    {
      name: 'Sports Complex & Pavilion',
      code: 'SP-1',
      description: 'Gymnasium, indoor badminton courts, basketball ground, and physical education department.',
      latitude: 12.9736,
      longitude: 77.5966,
      floors: 2,
      yearBuilt: 2017,
      wheelchairAccessible: false,
    },
    {
      name: 'Campus Health & Wellness Center',
      code: 'HC-1',
      description: '24/7 primary healthcare clinic, emergency ambulance bay, nurse station, and campus counselor.',
      latitude: 12.9728,
      longitude: 77.5932,
      floors: 1,
      yearBuilt: 2015,
      wheelchairAccessible: true,
    },
    {
      name: 'Evergreen Hostels (Boys & Girls Blocks)',
      code: 'HST-1',
      description: 'Residential quarters with dining facilities, high-speed Wi-Fi, laundry, and recreation halls.',
      latitude: 12.9694,
      longitude: 77.5968,
      floors: 6,
      yearBuilt: 2019,
      wheelchairAccessible: true,
    },
  ];

  const createdBuildings: Record<string, string> = {};
  for (const b of buildingsData) {
    const created = await prisma.building.create({ data: b });
    createdBuildings[b.code] = created.id;
  }
  console.log(`🏛️ Created ${Object.keys(createdBuildings).length} campus buildings.`);

  // 4. Departments
  const deptData = [
    {
      name: 'Computer Science & Engineering',
      code: 'CSE',
      description: 'Focuses on Artificial Intelligence, Software Engineering, Cloud Computing, and Cybersecurity.',
      buildingId: createdBuildings['ST-2'],
      headOfDepartment: 'Dr. Rajesh Sharma, Ph.D.',
      contactEmail: 'cse.hod@smartcampus.edu',
      contactPhone: '+91 (080) 4123-4510',
      websiteUrl: 'https://smartcampus.demo/cse',
      programsOffered: 'B.Tech CSE, M.Tech AI & Data Science, Ph.D. in Computing',
    },
    {
      name: 'Information Technology & Data Analytics',
      code: 'IT',
      description: 'Specializes in Distributed Systems, Data Science, Full-Stack Development, and DevOps.',
      buildingId: createdBuildings['ST-2'],
      headOfDepartment: 'Dr. Priya Menon, Ph.D.',
      contactEmail: 'it.hod@smartcampus.edu',
      contactPhone: '+91 (080) 4123-4512',
      websiteUrl: 'https://smartcampus.demo/it',
      programsOffered: 'B.Tech Information Technology, M.Tech Big Data',
    },
    {
      name: 'Electronics & Communication Engineering',
      code: 'ECE',
      description: 'Leading research in VLSI, Embedded Systems, IoT, and 5G Wireless Communication.',
      buildingId: createdBuildings['AB-1'],
      headOfDepartment: 'Dr. Arvind Swaminathan, Ph.D.',
      contactEmail: 'ece.hod@smartcampus.edu',
      contactPhone: '+91 (080) 4123-4520',
      websiteUrl: 'https://smartcampus.demo/ece',
      programsOffered: 'B.Tech ECE, M.Tech Embedded Systems',
    },
    {
      name: 'Mechanical & Mechatronics Engineering',
      code: 'MECH',
      description: 'Pioneering Robotics, CAD/CAM, Thermal Systems, and Smart Manufacturing technologies.',
      buildingId: createdBuildings['AB-1'],
      headOfDepartment: 'Dr. Suresh Kulkarni, Ph.D.',
      contactEmail: 'mech.hod@smartcampus.edu',
      contactPhone: '+91 (080) 4123-4530',
      websiteUrl: 'https://smartcampus.demo/mech',
      programsOffered: 'B.Tech Mechanical Engineering, B.Tech Mechatronics',
    },
    {
      name: 'Civil & Environmental Engineering',
      code: 'CIVIL',
      description: 'Focus on Sustainable Infrastructure, Structural Engineering, and Smart City Planning.',
      buildingId: createdBuildings['AB-1'],
      headOfDepartment: 'Dr. Neha Agarwal, Ph.D.',
      contactEmail: 'civil.hod@smartcampus.edu',
      contactPhone: '+91 (080) 4123-4540',
      websiteUrl: 'https://smartcampus.demo/civil',
      programsOffered: 'B.Tech Civil Engineering, M.Tech Structural Engineering',
    },
    {
      name: 'Department of Management Studies (MBA)',
      code: 'MBA',
      description: 'Excellence in Technology Management, Finance, Human Resources, and Business Analytics.',
      buildingId: createdBuildings['ADM-1'],
      headOfDepartment: 'Dr. Vikram Malhotra, Ph.D.',
      contactEmail: 'mba.hod@smartcampus.edu',
      contactPhone: '+91 (080) 4123-4550',
      websiteUrl: 'https://smartcampus.demo/mba',
      programsOffered: 'MBA (Tech & Analytics), Executive MBA',
    },
  ];

  const createdDepts: Record<string, string> = {};
  for (const d of deptData) {
    const created = await prisma.department.create({ data: d });
    createdDepts[d.code] = created.id;
  }
  console.log(`🎓 Created ${Object.keys(createdDepts).length} academic departments.`);

  // 5. Facilities
  const facilitiesData = [
    {
      name: 'Central Library & Reading Halls',
      category: 'Library',
      description: 'Three floors containing physical books, digital terminals, journals, and private study rooms.',
      buildingId: createdBuildings['LIB-1'],
      floor: 'Ground & 1st Floor',
      roomNumber: 'L-101 to L-205',
      landmark: 'Directly in front of the Central Lawn',
      timings: 'Monday - Saturday: 8:00 AM - 10:00 PM, Sunday: 9:00 AM - 5:00 PM',
      contactPhone: '+91 (080) 4123-4580',
      contactEmail: 'library@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Campus Food Court & Cafeteria',
      category: 'Cafeteria',
      description: 'Multi-cuisine cafeteria serving hygienic breakfast, lunch, fresh juices, and snacks.',
      buildingId: createdBuildings['SAC-1'],
      floor: 'Ground Floor',
      roomNumber: 'SAC-G01',
      landmark: 'Next to the Amphitheatre and Fountain',
      timings: 'Monday - Sunday: 7:30 AM - 9:30 PM',
      contactPhone: '+91 (080) 4123-4585',
      contactEmail: 'cafeteria@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Advanced AI & Machine Learning Research Lab',
      category: 'Lab',
      description: 'NVIDIA GPU clusters equipped for Deep Learning, Computer Vision, and Generative AI research.',
      buildingId: createdBuildings['ST-2'],
      floor: '4th Floor',
      roomNumber: 'Lab 402',
      landmark: 'Opposite to Department Server Room',
      timings: 'Monday - Friday: 9:00 AM - 7:00 PM',
      contactPhone: '+91 (080) 4123-4515',
      contactEmail: 'ailab@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Computer Programming & Cloud Computing Lab',
      category: 'Lab',
      description: '120 high-performance workstations with Linux/Windows dual boot and gigabit internet.',
      buildingId: createdBuildings['ST-2'],
      floor: '3rd Floor',
      roomNumber: 'Lab 305 & 306',
      landmark: 'Near the West Wing Staircase',
      timings: 'Monday - Saturday: 8:30 AM - 6:00 PM',
      contactPhone: '+91 (080) 4123-4516',
      contactEmail: 'cslab@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Admissions Counseling & Information Cell',
      category: 'Administration',
      description: 'Helpdesk for prospective candidates, application assistance, campus tours, and document verification.',
      buildingId: createdBuildings['ADM-1'],
      floor: 'Ground Floor',
      roomNumber: 'Room AD-04',
      landmark: 'Right beside the Admin Reception Desk',
      timings: 'Monday - Saturday: 9:00 AM - 5:00 PM',
      contactPhone: '+91 (080) 4123-4501',
      contactEmail: 'admissions@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Registrar & Student Affairs Office',
      category: 'Administration',
      description: 'Student verification, bonafide certificates, scholarship disbursement, and transcript issuance.',
      buildingId: createdBuildings['ADM-1'],
      floor: '1st Floor',
      roomNumber: 'Room AD-105',
      landmark: 'First door on the right above main lobby',
      timings: 'Monday - Friday: 9:30 AM - 4:30 PM',
      contactPhone: '+91 (080) 4123-4505',
      contactEmail: 'registrar@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Finance & Accounts Fee Counter',
      category: 'Administration',
      description: 'Semester tuition fee payment, receipts, bank challan submission, and no-dues clearance.',
      buildingId: createdBuildings['ADM-1'],
      floor: 'Ground Floor',
      roomNumber: 'Counter 1 to 4, Room AD-08',
      landmark: 'Adjacent to Student Lobby',
      timings: 'Monday - Friday: 9:30 AM - 3:30 PM',
      contactPhone: '+91 (080) 4123-4508',
      contactEmail: 'accounts@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Campus Health Clinic & Pharmacy',
      category: 'Medical',
      description: 'Free basic medical checkup, emergency first aid, doctor consultation, and stocked pharmacy.',
      buildingId: createdBuildings['HC-1'],
      floor: 'Ground Floor',
      roomNumber: 'HC-01',
      landmark: 'Beside North Gate Entrance',
      timings: '24 Hours Emergency / Doctor on Duty 8:00 AM - 8:00 PM',
      contactPhone: '+91 (080) 4123-4599',
      contactEmail: 'healthcenter@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Grand University Auditorium',
      category: 'Academic',
      description: 'State-of-the-art 1200-seat acoustic auditorium for convocation, seminars, and cultural festivals.',
      buildingId: createdBuildings['AB-1'],
      floor: 'Ground Floor',
      roomNumber: 'Auditorium Hall',
      landmark: 'Main Quadrangle entrance',
      timings: 'As per event schedule',
      contactPhone: '+91 (080) 4123-4522',
      contactEmail: 'events@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Gymnasium & Fitness Pavilion',
      category: 'Sports',
      description: 'Modern cardiovascular machines, weights, yoga studio, and certified fitness coaches.',
      buildingId: createdBuildings['SP-1'],
      floor: '1st Floor',
      roomNumber: 'SP-101',
      landmark: 'Overlooking the Football Ground',
      timings: 'Morning: 6:00 AM - 9:00 AM, Evening: 4:30 PM - 8:30 PM',
      contactPhone: '+91 (080) 4123-4590',
      contactEmail: 'sports@smartcampus.edu',
      isOperational: true,
    },
    {
      name: 'Campus ATM & Banking Kiosk (SBI & HDFC)',
      category: 'ATM',
      description: 'Dual ATM machines with cash deposit facility and passbook printer.',
      buildingId: createdBuildings['ADM-1'],
      floor: 'Ground Floor (External Entry)',
      roomNumber: 'Kiosk 1 & 2',
      landmark: 'Gate 1 main perimeter wall',
      timings: '24 Hours / 7 Days',
      contactPhone: 'Toll-Free 1800-425-3800',
      isOperational: true,
    },
    {
      name: 'Main Campus Parking (Gate 1)',
      category: 'Transport',
      description: 'Designated secured parking zones for student two-wheelers and visitor four-wheelers.',
      buildingId: createdBuildings['ADM-1'],
      floor: 'Ground Level Open Lot',
      landmark: 'North entrance near Gate 1 security cabin',
      timings: '6:30 AM - 10:30 PM',
      contactPhone: '+91 (080) 4123-4595',
      isOperational: true,
    },
    {
      name: 'Evergreen Hostels Office & Warden Desk',
      category: 'Hostel',
      description: 'Hostel admissions, room allotment, leave pass sanction, and student mess coordination.',
      buildingId: createdBuildings['HST-1'],
      floor: 'Ground Floor',
      roomNumber: 'Hostel Admin H-02',
      landmark: 'Opposite the Residential Garden',
      timings: 'Monday - Saturday: 8:00 AM - 8:00 PM',
      contactPhone: '+91 (080) 4123-4570',
      contactEmail: 'hostelwarden@smartcampus.edu',
      isOperational: true,
    },
  ];

  for (const f of facilitiesData) {
    await prisma.facility.create({ data: f });
  }
  console.log(`🏢 Created ${facilitiesData.length} campus facilities and services.`);

  // 6. Faculty Members
  const facultyData = [
    {
      name: 'Dr. Rajesh Sharma',
      title: 'Dr.',
      designation: 'Professor & Head of Department',
      departmentId: createdDepts['CSE'],
      email: 'rajesh.sharma@smartcampus.edu',
      phone: '+91 (080) 4123-4511',
      cabinRoom: 'Block B, Room 401',
      officeHours: 'Tuesday & Thursday: 2:00 PM - 4:00 PM',
      specialization: 'Artificial Intelligence, Deep Learning, Natural Language Processing',
      bio: 'Over 18 years of academic and industrial research experience in machine learning algorithms and intelligent systems.',
    },
    {
      name: 'Dr. Ananya Roy',
      title: 'Dr.',
      designation: 'Associate Professor',
      departmentId: createdDepts['CSE'],
      email: 'ananya.roy@smartcampus.edu',
      phone: '+91 (080) 4123-4514',
      cabinRoom: 'Block B, Room 308',
      officeHours: 'Monday & Wednesday: 11:00 AM - 1:00 PM',
      specialization: 'Cloud Computing, Distributed Architecture, Kubernetes, Microservices',
      bio: 'Certified Cloud Architect with multiple patents in automated microservice resilience and edge computing.',
    },
    {
      name: 'Prof. Karthik Ramanathan',
      title: 'Prof.',
      designation: 'Assistant Professor',
      departmentId: createdDepts['CSE'],
      email: 'karthik.r@smartcampus.edu',
      phone: '+91 (080) 4123-4518',
      cabinRoom: 'Block B, Room 304',
      officeHours: 'Friday: 10:00 AM - 12:30 PM',
      specialization: 'Full Stack Web Development, Database Engineering, DevOps',
      bio: 'Open-source contributor, passionate educator, and faculty coordinator for the Google Developer Student Club.',
    },
    {
      name: 'Dr. Priya Menon',
      title: 'Dr.',
      designation: 'Professor & Head of Department',
      departmentId: createdDepts['IT'],
      email: 'priya.menon@smartcampus.edu',
      phone: '+91 (080) 4123-4513',
      cabinRoom: 'Block B, Room 201',
      officeHours: 'Monday & Friday: 2:30 PM - 4:30 PM',
      specialization: 'Big Data Systems, Information Security, Predictive Analytics',
      bio: 'Former consultant for multinational enterprises and lead investigator on government cyber intelligence projects.',
    },
    {
      name: 'Dr. Arvind Swaminathan',
      title: 'Dr.',
      designation: 'Professor & Head of Department',
      departmentId: createdDepts['ECE'],
      email: 'arvind.s@smartcampus.edu',
      phone: '+91 (080) 4123-4521',
      cabinRoom: 'Block A, Room 215',
      officeHours: 'Wednesday & Friday: 1:30 PM - 3:30 PM',
      specialization: 'VLSI Chip Design, Digital Signal Processing, Embedded IoT',
      bio: 'Senior IEEE Member with 25+ published research papers in low-power semiconductor architectures.',
    },
    {
      name: 'Dr. Suresh Kulkarni',
      title: 'Dr.',
      designation: 'Professor & Head of Department',
      departmentId: createdDepts['MECH'],
      email: 'suresh.kulkarni@smartcampus.edu',
      phone: '+91 (080) 4123-4531',
      cabinRoom: 'Block A, Room 112',
      officeHours: 'Tuesday & Thursday: 10:00 AM - 12:00 PM',
      specialization: 'Robotics & Automation, Finite Element Analysis, Additive 3D Printing',
      bio: 'Leads the Formula Student Racing team and has supervised over 40 industrial robotics design projects.',
    },
    {
      name: 'Dr. Neha Agarwal',
      title: 'Dr.',
      designation: 'Professor & Head of Department',
      departmentId: createdDepts['CIVIL'],
      email: 'neha.agarwal@smartcampus.edu',
      phone: '+91 (080) 4123-4541',
      cabinRoom: 'Block A, Room 104',
      officeHours: 'Monday & Thursday: 3:00 PM - 5:00 PM',
      specialization: 'Green Building Design, Earthquake Engineering, Smart City Drainage',
      bio: 'Consultant to urban municipal corporations for disaster-resilient building design.',
    },
    {
      name: 'Dr. Vikram Malhotra',
      title: 'Dr.',
      designation: 'Director & Head of Department',
      departmentId: createdDepts['MBA'],
      email: 'vikram.malhotra@smartcampus.edu',
      phone: '+91 (080) 4123-4551',
      cabinRoom: 'Admin Complex, Room 202',
      officeHours: 'Monday to Friday: 4:00 PM - 5:30 PM',
      specialization: 'Strategic Management, Technology Entrepreneurship, Corporate Finance',
      bio: '20+ years in corporate strategy consulting and startup incubation guidance.',
    },
  ];

  for (const fac of facultyData) {
    await prisma.faculty.create({ data: fac });
  }
  console.log(`👨‍🏫 Created ${facultyData.length} faculty profiles.`);

  // 7. Notices & Announcements
  const noticesData = [
    {
      title: 'Mid-Term Semester Examination Schedule - Autumn 2026',
      content: 'The mid-term examinations for B.Tech, M.Tech, and MBA programs will commence from November 10, 2026. Hall tickets can be downloaded from the student ERP portal after clearing fee dues.',
      category: 'Exam',
      priority: 'Urgent',
      publishDate: new Date('2026-10-15T09:00:00Z'),
      isPublished: true,
      author: 'Controller of Examinations',
    },
    {
      title: 'Annual Technical Fest "TechVeda 2026" - Call for Project Registrations',
      content: 'Apex Tech is excited to host TechVeda 2026 on November 25-27, 2026. Events include 24-hr Hackathon, Robowars, Paper Presentation, and AI Pitching. Cash prizes worth Rs. 3,50,000! Register at the SAC Desk or online.',
      category: 'Event',
      priority: 'High',
      publishDate: new Date('2026-10-12T10:00:00Z'),
      isPublished: true,
      author: 'Student Affairs & Cultural Committee',
    },
    {
      title: 'Tier-1 Campus Placement Drive by Top Tech Firms',
      content: 'Registrations are open for 7th semester B.Tech CSE, IT, and ECE students. Initial coding rounds are scheduled for October 28 in Computer Labs 305 and 306. Eligible students must update their resumes on the placement portal.',
      category: 'Placement',
      priority: 'High',
      publishDate: new Date('2026-10-10T11:30:00Z'),
      isPublished: true,
      author: 'Training & Placement Cell',
    },
    {
      title: 'National Scholarship Portal (NSP) Renewal & Fresh Applications',
      content: 'Eligible SC/ST/OBC/Minority students are requested to complete their NSP renewal and submit verified physical documents at the Registrar Office (AD-105) before October 31, 2026.',
      category: 'Academic',
      priority: 'Normal',
      publishDate: new Date('2026-10-05T08:00:00Z'),
      isPublished: true,
      author: 'Registrar Office',
    },
    {
      title: 'Campus Central Library Timings Extended for Exam Preparations',
      content: 'Starting October 20, the Central Library reading halls will remain open until 11:30 PM on weekdays with high-speed Wi-Fi and power outlets active for group study.',
      category: 'General',
      priority: 'Normal',
      publishDate: new Date('2026-10-02T14:00:00Z'),
      isPublished: true,
      author: 'Chief Librarian',
    },
  ];

  for (const n of noticesData) {
    await prisma.notice.create({ data: n });
  }
  console.log(`📢 Created ${noticesData.length} campus announcements.`);

  // 8. Frequently Asked Questions (FAQs)
  const faqsData = [
    {
      question: 'Where is the library and what are its timings?',
      answer: 'The Central Library is located in the Central Library Block (LIB-1), directly opposite the Central Lawn. It is open Monday to Saturday from 8:00 AM to 10:00 PM and Sundays from 9:00 AM to 5:00 PM. During exam periods, reading hours are extended to 11:30 PM.',
      category: 'Facilities',
      keywords: 'library, books, reading hall, timings, hours, borrow, lib-1',
      viewCount: 142,
    },
    {
      question: 'How can I reach the Computer Science and Engineering Department?',
      answer: 'The Computer Science & Engineering (CSE) Department is situated in the Science & Technology Tower (Block B) on the 3rd and 4th floors. The HOD office (Dr. Rajesh Sharma) is in Room 401 on the 4th floor.',
      category: 'Academics',
      keywords: 'cse, computer science, block b, st-2, rajesh sharma, computing, it',
      viewCount: 98,
    },
    {
      question: 'Where is the administrative office and accounts counter?',
      answer: 'The Administrative Complex (ADM-1) is located near Gate 1. The Accounts & Fee Counter is in Room AD-08 on the Ground Floor, and the Registrar Office is in Room AD-105 on the 1st Floor.',
      category: 'Administration',
      keywords: 'admin, accounts, fees, registrar, admission, bonafide, adm-1',
      viewCount: 85,
    },
    {
      question: 'Where is the college cafeteria and what are the options available?',
      answer: 'The Cafeteria & Food Court is located on the Ground Floor of the Student Amenity Center (SAC-1), next to the amphitheatre. It serves hot South Indian and North Indian meals, fresh fruit juices, sandwiches, and coffee from 7:30 AM to 9:30 PM daily.',
      category: 'Facilities',
      keywords: 'canteen, cafeteria, food, lunch, breakfast, snacks, sac, eat',
      viewCount: 110,
    },
    {
      question: 'What is the procedure for campus Wi-Fi access?',
      answer: 'Students can register their laptop or mobile MAC address at the IT Helpdesk (Block B, Room 102). You will receive your WPA2 Enterprise credentials linked to your student roll number within 24 hours.',
      category: 'Facilities',
      keywords: 'wifi, internet, login, network, mac address, it cell',
      viewCount: 76,
    },
    {
      question: 'What medical facilities are available on campus?',
      answer: 'The Campus Health Clinic (HC-1) is located near the North Gate. It is staffed 24/7 with trained nurses, has a qualified physician on duty from 8:00 AM to 8:00 PM, and provides emergency first aid, basic medicines, and on-call ambulance service.',
      category: 'Facilities',
      keywords: 'medical, clinic, doctor, health, hospital, emergency, medicine, pharmacy, hc-1',
      viewCount: 54,
    },
    {
      question: 'How do I contact the hostel warden or apply for hostel accommodation?',
      answer: 'Hostel admissions and inquiries are managed at the Evergreen Hostels Office (HST-1, Room H-02, Ground Floor). You can also contact hostelwarden@smartcampus.edu or call +91 (080) 4123-4570.',
      category: 'Hostel',
      keywords: 'hostel, room, warden, stay, boys hostel, girls hostel, mess, hst-1',
      viewCount: 63,
    },
    {
      question: 'Where is the college auditorium located?',
      answer: 'The Grand University Auditorium is located on the Ground Floor of the Main Academic Block (Block A), accessible from the central quadrangle.',
      category: 'Facilities',
      keywords: 'auditorium, hall, seminar, cultural, stage, block a, ab-1',
      viewCount: 47,
    },
    {
      question: 'Where can I find parking for two-wheelers and four-wheelers?',
      answer: 'Student two-wheeler and visitor four-wheeler parking is located at the Main North Parking Lot near Gate 1. Faculty parking is reserved behind Main Academic Block A.',
      category: 'Transport',
      keywords: 'parking, vehicle, bike, car, cycle, gate 1',
      viewCount: 39,
    },
    {
      question: 'Are there ATM and banking facilities inside the campus?',
      answer: 'Yes, 24/7 State Bank of India (SBI) and HDFC Bank ATMs are located right next to the Gate 1 entrance at the external wall of the Administrative Complex.',
      category: 'Facilities',
      keywords: 'atm, bank, money, cash, sbi, hdfc',
      viewCount: 42,
    },
  ];

  for (const f of faqsData) {
    await prisma.faq.create({ data: f });
  }
  console.log(`❓ Created ${faqsData.length} FAQs.`);

  // 9. Synonyms for intelligent semantic matching
  const synonymsData = [
    { primaryWord: 'library', synonyms: 'books,reading room,reading hall,study hall,journals,central library,lib' },
    { primaryWord: 'canteen', synonyms: 'cafeteria,food court,mess,food,lunch,dinner,breakfast,snacks,coffee shop,cafe' },
    { primaryWord: 'administration', synonyms: 'admin,principal,registrar,accounts,fees,admission,office,director,clerk' },
    { primaryWord: 'computer science', synonyms: 'cse,computer,cs,software,coding,programming,ai,ml,it department' },
    { primaryWord: 'washroom', synonyms: 'restroom,toilet,latrine,bathroom,water closet,loo' },
    { primaryWord: 'laboratory', synonyms: 'lab,practical room,research lab,computing lab,hardware lab' },
    { primaryWord: 'hostel', synonyms: 'dorm,dormitory,residence,hall of residence,boys hostel,girls hostel,mess' },
    { primaryWord: 'medical', synonyms: 'doctor,clinic,health center,first aid,hospital,nurse,medicine,pharmacy' },
    { primaryWord: 'sports', synonyms: 'gym,gymnasium,playground,football ground,badminton,basketball,fitness' },
    { primaryWord: 'parking', synonyms: 'vehicle,bike parking,car parking,scooter parking,parking lot' },
    { primaryWord: 'exam', synonyms: 'examination,test,mid term,end term,timetable,datesheet,results,hall ticket' },
    { primaryWord: 'auditorium', synonyms: 'hall,seminar hall,convocation hall,stage,amphitheatre,events hall' },
    { primaryWord: 'contact', synonyms: 'phone,email,number,helpline,reach,call,directory' },
    { primaryWord: 'admission', synonyms: 'enrollment,apply,intake,eligibility,counseling,fees structure' },
  ];

  for (const syn of synonymsData) {
    await prisma.synonym.create({ data: syn });
  }
  console.log(`📖 Created ${synonymsData.length} synonym dictionaries for smart NLP.`);

  console.log('✅ Database seeding finished successfully!');
}

main()
  .catch((e) => {
    console.error('❌ Error during seeding:', e);
    process.exit(1);
  })
  .finally(async () => {
    await prisma.$disconnect();
  });
