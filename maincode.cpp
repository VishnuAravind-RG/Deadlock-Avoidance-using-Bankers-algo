#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ANSI escape codes for color output
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

int nprocesses = 5, nresources = 4;
vector<int> seq;

typedef struct {
    int id;
    vector<int> Max; // Max resources needed
    vector<int> Allocation; // Allocated resources
    vector<int> Need; // Resources needed
    bool status;
    int priority; // Priority for scheduling
} process;

vector<process> processes;
vector<int> available;
vector<int> total_resources;

// Function to display the Allocation Table
void DisplayAllocationTable(vector<process> processes) {
    cout << "\n\tAllocation Table";
    cout << "\nProcess\t";
    for (int i = 0; i < nresources; i++) {
        cout << "R" << i << "\t";
    }
    cout << endl;

    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << "\t";
        for (int j = 0; j < nresources; j++) {
            cout << processes[i].Allocation[j] << "\t";
        }
        cout << endl;
    }
}

// Function to display the randomly generated input
void DisplayRandomInput(vector<process> processes, vector<int> available) {
    cout << "\n\tRandomly Generated Input";
    cout << "\n\nMax Matrix:\n";
    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << "\t";
        for (int j = 0; j < nresources; j++) {
            cout << processes[i].Max[j] << "\t";
        }
        cout << endl;
    }

    cout << "\nAllocation Matrix:\n";
    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << "\t";
        for (int j = 0; j < nresources; j++) {
            cout << processes[i].Allocation[j] << "\t";
        }
        cout << endl;
    }

    cout << "\nAvailable Vector:\n";
    for (int j = 0; j < nresources; j++) {
        cout << available[j] << "\t";
    }
    cout << endl;
}

// Function to check if the system is in a safe state
bool IsSafe(vector<process> processes, vector<int> available) {
    vector<int> work = available;
    vector<bool> finish(nprocesses, false);
    seq.clear();

    while (true) {
        bool found = false;
        for (int i = 0; i < nprocesses; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < nresources; j++) {
                    if (processes[i].Need[j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int j = 0; j < nresources; j++) {
                        work[j] += processes[i].Allocation[j];
                    }
                    finish[i] = true;
                    seq.push_back(i);
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    for (int i = 0; i < nprocesses; i++) {
        if (!finish[i]) return false;
    }
    return true;
}

// Function to simulate resource requests by a process
bool Request(vector<process> processes, vector<int> available, int p, vector<int> req) {
    for (int j = 0; j < nresources; j++) {
        if (req[j] > available[j] || req[j] > processes[p].Need[j]) {
            return false;
        }
    }

    for (int j = 0; j < nresources; j++) {
        available[j] -= req[j];
        processes[p].Allocation[j] += req[j];
        processes[p].Need[j] -= req[j];
    }

    return IsSafe(processes, available);
}

// Function to release resources
void ReleaseResources(int pid, vector<process> &processes, vector<int> &available) {
    for (int j = 0; j < nresources; j++) {
        available[j] += processes[pid].Allocation[j];
        processes[pid].Allocation[j] = 0;
        processes[pid].Need[j] = 0;
    }
    processes[pid].status = true;
    cout << GREEN << "Process P[" << pid << "] released resources." << RESET << endl;
}

// Function to generate random inputs
void GenerateRandomInput(vector<process> &processes, vector<int> &available) {
    srand(time(NULL));
    for (int i = 0; i < nprocesses; i++) {
        for (int j = 0; j < nresources; j++) {
            processes[i].Max[j] = rand() % 10 + 1;
            processes[i].Allocation[j] = rand() % (processes[i].Max[j] + 1);
            processes[i].Need[j] = processes[i].Max[j] - processes[i].Allocation[j];
            available[j] += processes[i].Allocation[j];
        }
    }
    for (int j = 0; j < nresources; j++) {
        available[j] = rand() % 10 + 1;
    }
}

// Function to display the safe sequence step-by-step
void DisplaySafeSequenceSteps(vector<process> processes, vector<int> available) {
    vector<int> work = available;
    vector<bool> finish(nprocesses, false);
    vector<int> safe_sequence;

    while (safe_sequence.size() < nprocesses) {
        bool found = false;
        for (int i = 0; i < nprocesses; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < nresources; j++) {
                    if (processes[i].Need[j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int j = 0; j < nresources; j++) {
                        work[j] += processes[i].Allocation[j];
                    }
                    finish[i] = true;
                    safe_sequence.push_back(i);
                    found = true;
                    cout << "Step " << safe_sequence.size() << ": P" << i << " added to safe sequence." << endl;
                }
            }
        }
        if (!found) {
            cout << "No safe sequence found." << endl;
            return;
        }
    }
    cout << "Safe sequence: ";
    for (int i = 0; i < safe_sequence.size(); i++) {
        cout << "P" << safe_sequence[i];
        if (i != safe_sequence.size() - 1) cout << " -> ";
    }
    cout << endl;
}

// Function to log events
void LogEvent(const string &message) {
    ofstream logFile("bankers_log.txt", ios::app);
    logFile << message << endl;
    logFile.close();
}

// Function to export results
void ExportResults(const vector<int> &safe_sequence) {
    ofstream outFile("results.txt");
    outFile << "Safe Sequence: ";
    for (int i = 0; i < safe_sequence.size(); i++) {
        outFile << "P" << safe_sequence[i];
        if (i != safe_sequence.size() - 1) outFile << " -> ";
    }
    outFile.close();
}

// Function to validate inputs
bool ValidateInput(vector<process> processes, vector<int> available) {
    for (int i = 0; i < nprocesses; i++) {
        for (int j = 0; j < nresources; j++) {
            if (processes[i].Allocation[j] > processes[i].Max[j]) {
                cout << RED << "Error: Allocation exceeds Max for P" << i << " R" << j << RESET << endl;
                return false;
            }
        }
    }
    return true;
}

// Function to display the menu
void DisplayMenu() {
    cout << "\n1. Check Safe State\n";
    cout << "2. Make Resource Request\n";
    cout << "3. Release Resources\n";
    cout << "4. Generate Random Input\n";
    cout << "5. Display Random Input\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(time(NULL));
    char choice;
    int p;
    vector<int> req(nresources, 0);

    do {
        // Clear vectors
        seq.clear();
        available.clear();
        processes.clear();
        available.resize(nresources);
        processes.resize(nprocesses);

        cout << "\nEnter num of resources : ";
        cin >> nresources;
        cout << "Enter num of processes : ";
        cin >> nprocesses;
        cout << endl;

        // Get Allocation Matrix
        for (int i = 0; i < nprocesses; i++) {
            processes[i].Max.resize(nresources);
            processes[i].Allocation.resize(nresources);
            processes[i].Need.resize(nresources);
            processes[i].id = i;
            processes[i].status = false;

            cout << "Enter Allocation array for P" << i << " : ";
            for (int j = 0; j < nresources; j++) {
                cin >> processes[i].Allocation[j];
            }
        }
        cout << endl;

        // Get Max Matrix
        for (int i = 0; i < nprocesses; i++) {
            cout << "Enter Max array for P" << i << " : ";
            for (int j = 0; j < nresources; j++) {
                cin >> processes[i].Max[j];
                processes[i].Need[j] = processes[i].Max[j] - processes[i].Allocation[j];
            }
        }

        // Get Available Matrix
        cout << "\nEnter Available array " << endl;
        for (int j = 0; j < nresources; j++)
            cin >> available[j];

        // Display Allocation Table
        DisplayAllocationTable(processes);

        // Display Menu
        char innerChoice;
        do {
            DisplayMenu();
            cin >> innerChoice;

            switch (innerChoice) {
                case '1':
                    if (IsSafe(processes, available)) {
                        cout << GREEN << "Yes, Safe state <";
                        for (int i = 0; i < seq.size(); i++) {
                            cout << "P" << seq[i];
                            if (i != seq.size() - 1) cout << ",";
                            else cout << ">" << RESET << endl;
                        }
                    } else {
                        cout << RED << "No, Unsafe state" << RESET << endl;
                    }
                    break;

                case '2':
                    cout << "Enter process index: ";
                    cin >> p;
                    cout << "Enter request for P" << p << " : ";
                    for (int j = 0; j < nresources; j++) {
                        cin >> req[j];
                    }
                    if (Request(processes, available, p, req)) {
                        cout << GREEN << "Request granted. Safe state maintained." << RESET << endl;
                    } else {
                        cout << RED << "Request denied. Unsafe state." << RESET << endl;
                    }
                    break;

                case '3':
                    cout << "Enter process index to release resources: ";
                    cin >> p;
                    ReleaseResources(p, processes, available);
                    break;

                case '4':
                    GenerateRandomInput(processes, available);
                    cout << GREEN << "Random input generated." << RESET << endl;
                    break;

                case '5':
                    DisplayRandomInput(processes, available);
                    break;

                case '6':
                    cout << "Exiting..." << endl;
                    break;

                default:
                    cout << RED << "Invalid choice. Try again." << RESET << endl;
            }

            if (innerChoice != '6') {
                cout << "\nDo you want to perform another operation? (y/n): ";
                cin >> choice;
            }
        } while (choice == 'y' || choice == 'Y');

        cout << "\nDo you want to test again with new inputs? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
