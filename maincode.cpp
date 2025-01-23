#include <iostream>
#include <vector>
#include <limits>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <iomanip> // For std::setw
#include <conio.h> // for getch()
#include <string>

using namespace std;

mutex mtx; // Mutex for synchronizing access to shared resources
condition_variable cv; // Condition variable for signaling between threads
ofstream logFile("bankers_log.txt"); // Log file for recording events

// ANSI escape codes for color output
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";

// Function to validate integer input
int getValidatedInput(int min, int max) {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << RED << "Invalid input. Please enter a value between " << min << " and " << max << ": " << RESET;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return value;
        }
    }
}

// Function to log messages to the log file
void logMessage(const string& message) {
    cout << message << endl; // Print to console
    logFile << message << endl; // Log to file
}

// Function to simulate resource request by a process
void requestResources(int processID, vector<vector<int>>& allocation, vector<vector<int>>& need, vector<int>& available) {
    unique_lock<mutex> lock(mtx); // Lock the mutex for this thread

    // Simulate resource request (for demonstration purposes)
    for (int j = 0; j < available.size(); j++) {
        if (need[processID][j] > available[j]) {
            logMessage(YELLOW + "Process P[" + to_string(processID) + "] waiting for resources..." + RESET);
            cv.wait(lock); // Wait until notified
        }
    }

    // Allocate resources if available
    for (int j = 0; j < available.size(); j++) {
        available[j] -= need[processID][j];
        allocation[processID][j] += need[processID][j];
    }

    logMessage(GREEN + "Process P[" + to_string(processID) + "] has been allocated resources." + RESET);
    lock.unlock();
    cv.notify_all(); // Notify other waiting processes
}

// Function to release resources after process completion
void releaseResources(int processID, vector<vector<int>>& allocation, vector<int>& available) {
    unique_lock<mutex> lock(mtx); // Lock the mutex for this thread

    for (int j = 0; j < available.size(); j++) {
        available[j] += allocation[processID][j];
        allocation[processID][j] = 0; // Release allocated resources
    }

    logMessage(GREEN + "Process P[" + to_string(processID) + "] has released resources." + RESET);
    lock.unlock();
    cv.notify_all(); // Notify other waiting processes
}

// Function to display current state of resources and allocations in a cool format
void displayState(const vector<vector<int>>& allocation, const vector<vector<int>>& need, const vector<int>& available) {
    cout << "\n============================== Current State ==============================\n";
    cout << setw(10) << "Process" << setw(15) << "Allocation" << setw(15) << "Need" << endl;

    cout << "--------------------------------------------------------------------------\n";

    for (size_t i = 0; i < allocation.size(); i++) {
        cout << setw(10) << "P[" + to_string(i) + "]";
        for (const auto& alloc : allocation[i]) {
            cout << setw(5) << alloc;
        }
        cout << setw(15);
        for (const auto& req : need[i]) {
            cout << setw(5) << req;
        }
        cout << endl;
    }

    cout << "--------------------------------------------------------------------------\n";

    cout << "\nAvailable Resources:\n";
    cout << "----------------------\n";

    for (size_t i = 0; i < available.size(); i++) {
        cout << "R[" + to_string(i) + "] = " << available[i] << endl;
    }

    cout << "--------------------------------------------------------------------------\n";
}

// Function to run the Banker's Algorithm simulation
void runBankersAlgorithm() {
    int countofr, countofp;

    cout << "\nEnter the number of resources (1 to 5): ";
    countofr = getValidatedInput(1, 5);

    vector<int> instance(countofr);
    vector<int> available(countofr);
    vector<vector<int>> allocation(10, vector<int>(countofr));
    vector<vector<int>> max(10, vector<int>(countofr));
    vector<vector<int>> need(10, vector<int>(countofr));

    for (int i = 0; i < countofr; i++) {
        cout << "\nEnter the max instances of Resource R[" << i << "] (non-negative integer): ";
        while (true) {
            cin >> instance[i];
            if (cin.fail() || instance[i] < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Invalid input. Please enter a non-negative integer: " << RESET;
            } else {
                available[i] = instance[i];
                break;
            }
        }
    }

    cout << "\nEnter the number of processes (1 to 10): ";
    countofp = getValidatedInput(1, 10);

    cout << "\nEnter the allocation matrix:\n";
    for (int i = 0; i < countofp; i++) {
        cout << "For the process P[" << i << "]\n";
        for (int j = 0; j < countofr; j++) {
            cout << "Allocation of resource R[" << j << "] (non-negative integer): ";
            while (true) {
                cin >> allocation[i][j];
                if (cin.fail() || allocation[i][j] < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter a non-negative integer: " << RESET;
                } else if (allocation[i][j] > instance[j]) {
                    cout << RED << "Allocation cannot exceed maximum instances. Please enter again: " << RESET;
                } else {
                    available[j] -= allocation[i][j];
                    break;
                }
            }
        }
    }

    cout << "\nEnter the MAX matrix:\n";
    for (int i = 0; i < countofp; i++) {
        cout << "For the process P[" << i << "]\n";
        for (int j = 0; j < countofr; j++) {
            cout << "Max demand of resource R[" << j << "] (non-negative integer): ";
            while (true) {
                cin >> max[i][j];
                if (cin.fail() || max[i][j] < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter a non-negative integer: " << RESET;
                } else if (max[i][j] < allocation[i][j]) {
                    cout << RED << "Max demand cannot be less than allocation. Please enter again: " << RESET;
                } else {
                    break;
                }
            }
        }
    }

   // Calculate NEED matrix
   for (int i = 0; i < countofp; i++) {
       for (int j = 0; j < countofr; j++) {
           need[i][j] = max[i][j] - allocation[i][j];
       }
   }

   // Create threads for each process requesting resources
   vector<thread> threads;
   for (int i = 0; i < countofp; i++) {
       threads.emplace_back(requestResources, i, ref(allocation), ref(need), ref(available));
   }

   // Join all threads
   for (auto& th : threads) {
       th.join();
       releaseResources(th.get_id().hash() % countofp, allocation, available); // Simulate releasing resources after completion
   }

   displayState(allocation, need, available); // Show current state after execution

   logFile.close(); // Close log file at the end of execution

   cout << GREEN + "\nSimulation complete. Check bankers_log.txt for details." + RESET<< endl;
}

// Main function to provide testing options
int main() {
    char choice;

    do {
        runBankersAlgorithm();

        cout<< YELLOW + "\nDo you want to test another scenario? (y/n): " + RESET;
        cin >> choice;

    } while(choice == 'y' || choice == 'Y');

   getch(); // Wait for key press
   return 0;
}
