
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100

/* -------------------- Patient Record -------------------- */
typedef struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
    struct Patient *next;   /* used by Linked List */
} Patient;

/* -------------------- 1) LINKED LIST -------------------- *
 record of every patient
   ---------------------------------------------------------- */
Patient *listHead = NULL;

void addPatientToList(int id, char name[], int age, char disease[]) {
    Patient *newNode = (Patient *) malloc(sizeof(Patient));
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->age = age;
    strcpy(newNode->disease, disease);
    newNode->next = NULL;

    if (listHead == NULL) {
        listHead = newNode;
    } else {
        Patient *temp = listHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
    printf("\n[OK] Patient '%s' (ID: %d) added to hospital records.\n", name, id);
}

void displayPatientList() {
    if (listHead == NULL) {
        printf("\nNo patient records found.\n");
        return;
    }
    Patient *temp = listHead;
    printf("\n---------------- ALL PATIENT RECORDS ----------------\n");
    printf("%-6s %-15s %-5s %-15s\n", "ID", "Name", "Age", "Disease");
    while (temp != NULL) {
        printf("%-6d %-15s %-5d %-15s\n", temp->id, temp->name, temp->age, temp->disease);
        temp = temp->next;
    }
    printf("-------------------------------------------------------\n");
}

Patient* searchPatientById(int id) {
    Patient *temp = listHead;
    while (temp != NULL) {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/* -------------------- 2) QUEUE (OPD Waiting Line) -------------------- *
   In opd there should be first in first out like serial
   ------------------------------------------------------------------ */
typedef struct QNode {
    Patient *patient;
    struct QNode *next;
} QNode;

QNode *front = NULL, *rear = NULL;

void enqueuePatient(Patient *p) {
    QNode *newNode = (QNode *) malloc(sizeof(QNode));
    newNode->patient = p;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    printf("\n[OK] Patient '%s' added to OPD waiting queue.\n", p->name);
}

void serveNextPatient() {
    if (front == NULL) {
        printf("\nOPD Queue is empty. No patient to serve.\n");
        return;
    }
    QNode *temp = front;
    printf("\n[SERVING] Doctor is now seeing: %s (ID: %d, Disease: %s)\n",
           temp->patient->name, temp->patient->id, temp->patient->disease);
    front = front->next;
    if (front == NULL)
        rear = NULL;
    free(temp);
}

void displayQueue() {
    if (front == NULL) {
        printf("\nOPD Queue is empty.\n");
        return;
    }
    QNode *temp = front;
    printf("\n---------------- OPD WAITING QUEUE ----------------\n");
    int pos = 1;
    while (temp != NULL) {
        printf("%d. %s (ID: %d)\n", pos++, temp->patient->name, temp->patient->id);
        temp = temp->next;
    }
    printf("----------------------------------------------------\n");
}

/* -------------------- 3) STACK (Emergency Admissions) -------------------- *
 Stack id used for last in first out in emergency we have to add the patient to first 
   ------------------------------------------------------------------------ */
Patient *emergencyStack[MAX_PATIENTS];
int top = -1;

void pushEmergencyPatient(Patient *p) {
    if (top == MAX_PATIENTS - 1) {
        printf("\nEmergency ward is full!\n");
        return;
    }
    emergencyStack[++top] = p;
    printf("\n[OK] Patient '%s' admitted to EMERGENCY ward.\n", p->name);
}

void dischargeEmergencyPatient() {
    if (top == -1) {
        printf("\nNo patients currently in Emergency ward.\n");
        return;
    }
    Patient *p = emergencyStack[top--];
    printf("\n[DISCHARGED] Patient '%s' (ID: %d) discharged from Emergency ward.\n", p->name, p->id);
}

void displayEmergencyStack() {
    if (top == -1) {
        printf("\nEmergency ward is empty.\n");
        return;
    }
    printf("\n------------- EMERGENCY WARD (Top = Most Recent) -------------\n");
    for (int i = top; i >= 0; i--) {
        printf("%s (ID: %d, Disease: %s)\n",
               emergencyStack[i]->name, emergencyStack[i]->id, emergencyStack[i]->disease);
    }
    printf("----------------------------------------------------------------\n");
}

/* -------------------- 4) BUBBLE SORT -------------------- *
   sorts paitent so that child and older people can get treatment first
   ---------------------------------------------------------- */
void sortPatientsByAge() {
    int n = 0;
    Patient *arr[MAX_PATIENTS];
    Patient *temp = listHead;

    while (temp != NULL) {
        arr[n++] = temp;
        temp = temp->next;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j]->age > arr[j + 1]->age) {
                Patient *swap = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = swap;
            }
        }
    }

    printf("\n---------------- PATIENTS SORTED BY AGE ----------------\n");
    printf("%-6s %-15s %-5s %-15s\n", "ID", "Name", "Age", "Disease");
    for (int i = 0; i < n; i++) {
        printf("%-6d %-15s %-5d %-15s\n", arr[i]->id, arr[i]->name, arr[i]->age, arr[i]->disease);
    }
    printf("-----------------------------------------------------------\n");
}

/* -------------------- MAIN MENU -------------------- */
void showMenu() {
    printf("\n=====================================================\n");
    printf("       HOSPITAL PATIENT MANAGEMENT SYSTEM\n");
    printf("=====================================================\n");
    printf("1. Register New Patient (Linked List)\n");
    printf("2. Add Patient to OPD Queue\n");
    printf("3. Serve Next Patient in OPD (Dequeue)\n");
    printf("4. Display OPD Queue\n");
    printf("5. Admit Patient to Emergency (Push)\n");
    printf("6. Discharge Last Emergency Patient (Pop)\n");
    printf("7. Display Emergency Ward (Stack)\n");
    printf("8. Display All Patient Records\n");
    printf("9. Sort Patients by Age (Bubble Sort)\n");
    printf("10. Search Patient by ID\n");
    printf("0. Exit\n");
    printf("=====================================================\n");
    printf("Enter your choice: ");
}

int main() {
    int choice, id, age;
    char name[50], disease[50];

    while (1) {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                printf("Enter Name: ");
                scanf("%s", name);
                printf("Enter Age: ");
                scanf("%d", &age);
                printf("Enter Disease: ");
                scanf("%s", disease);
                addPatientToList(id, name, age, disease);
                break;

            case 2: {
                printf("Enter Patient ID to add to OPD Queue: ");
                scanf("%d", &id);
                Patient *p = searchPatientById(id);
                if (p != NULL)
                    enqueuePatient(p);
                else
                    printf("\nPatient not found. Register the patient first.\n");
                break;
            }

            case 3:
                serveNextPatient();
                break;

            case 4:
                displayQueue();
                break;

            case 5: {
                printf("Enter Patient ID to admit to Emergency: ");
                scanf("%d", &id);
                Patient *p = searchPatientById(id);
                if (p != NULL)
                    pushEmergencyPatient(p);
                else
                    printf("\nPatient not found. Register the patient first.\n");
                break;
            }

            case 6:
                dischargeEmergencyPatient();
                break;

            case 7:
                displayEmergencyStack();
                break;

            case 8:
                displayPatientList();
                break;

            case 9:
                sortPatientsByAge();
                break;

            case 10: {
                printf("Enter Patient ID to search: ");
                scanf("%d", &id);
                Patient *p = searchPatientById(id);
                if (p != NULL)
                    printf("\nFound -> ID: %d, Name: %s, Age: %d, Disease: %s\n",
                           p->id, p->name, p->age, p->disease);
                else
                    printf("\nPatient not found.\n");
                break;
            }

            case 0:
                printf("\nExiting system. Thank you!\n");
                exit(0);

            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }
    return 0;
}