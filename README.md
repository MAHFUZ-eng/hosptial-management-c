# Hospital Patient Management System

A C program built for Data Structures Lab Final, simulating a real-life
hospital patient management workflow.

## Data Structures Used
- **Singly Linked List** – master record of all registered patients
- **Queue** – OPD waiting line (FIFO)
- **Stack** – Emergency ward admissions (LIFO)
- **Bubble Sort** – sorting patients by age

## How to Compile & Run
```bash
gcc hospital.c -o hospital
./hospital
```

## Features
- Register new patient
- Add patient to OPD queue / serve next patient
- Admit patient to Emergency / discharge last admitted patient
- Display all patient records
- Sort patients by age
- Search patient by ID
