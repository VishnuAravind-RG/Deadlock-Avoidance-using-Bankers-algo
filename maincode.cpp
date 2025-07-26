#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <map>
#include <queue>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>
#include <functional>
using namespace std;

// ANSI escape codes for color output
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

// Global configuration
int nprocesses = 5, nresources = 4;
vector<int> seq;
atomic<bool> simulation_running(false);
mutex mtx;
condition_variable cv;

// Resource allocation history tracking
struct AllocationHistory {
    int pid;
    vector<int> resources;
    time_t timestamp;
    string action; // e.g., "allocate", "release"
};

vector<AllocationHistory> history;
unordered_map<int, vector<vector<int>>> historical_need;

// Enhanced process structure
typedef struct {
    int id;
    vector<int> Max;
    vector<int> Allocation;
    vector<int> Need;
    bool status; // true if completed, false if active
    int priority;
    time_t start_time;
    time_t end_time;
    vector<int> request_history;
    double cpu_usage; // Simulated CPU usage
    int wait_time; // Time spent waiting for resources
} process;

vector<process> processes;
vector<int> available;
vector<int> total_resources;
vector<int> historical_available;

// Blockchain-like security structure
struct Block {
    int index;
    time_t timestamp;
    string transaction;
    string previous_hash;
    string hash;
};

vector<Block> blockchain;

// Simulation statistics
struct SimulationStats {
    int requests_processed;
    int deadlocks_detected;
    int deadlocks_resolved;
    double avg_response_time;
    int total_cycles;
};

SimulationStats sim_stats = {0, 0, 0, 0.0, 0};

// Logging function
void LogAction(const string& action, const string& details) {
    ofstream log("system.log", ios::app);
    time_t now = time(nullptr);
    log << ctime(&now) << action << ": " << details << endl;
    log.close();
}

// Function prototypes
void InitializeBlockchain();
string CalculateHash(const Block& block);
void AddBlock(const string& transaction);
void VisualizeResourceGraph();
void PredictiveAllocation();
void HandleDeadlock();
void SimulationWorker();
void StartSimulation();
void StopSimulation();
void SaveStateToFile(const string& filename);
void LoadStateFromFile(const string& filename);
void NetworkSync();
void PerformanceMetrics();
void GenerateSecurityReport();
void DisplayResourceUtilizationTrends();
void DisplayBlockchain();
void AddProcess(int max_resources, int priority);
void RemoveProcess(int pid);
void RequestResources(int pid, const vector<int>& request);
void ReleaseResources(int pid, const vector<int>& release);
void DetectDeadlockCycle();
void ExportToText();
void DisplayProcessStatus();
void LoadConfigFromText(const string& filename);
void DisplaySimulationStats();
void UpdatePriorityQueue();
void DisplayPriorityQueue();
void ValidateInput(int pid, const vector<int>& vec, const string& type);
void InitializeSystem();

// ======================== Core Banker's Algorithm Functions ========================

void DisplayAllocationTable(const vector<process>& processes) {
    cout << "\n\t" << BOLD << "Allocation Table" << RESET;
    cout << "\nProcess\t";
    for (int i = 0; i < nresources; i++) {
        cout << "R" << i << "\t";
    }
    cout << "Priority\tStatus";
    cout << endl;

    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << "\t";
        for (int j = 0; j < nresources; j++) {
            cout << processes[i].Allocation[j] << "\t";
        }
        cout << processes[i].priority << "\t" << (processes[i].status ? "Done" : "Active");
        cout << endl;
    }
    LogAction("Display", "Allocation table displayed");
}

bool IsSafe(vector<process> processes, vector<int> available) {
    vector<int> work = available;
    vector<bool> finish(nprocesses, false);
    seq.clear();

    while (true) {
        bool found = false;
        for (int i = 0; i < nprocesses; i++) {
            if (!finish[i] && !processes[i].status) {
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

                    // Record history
                    AllocationHistory h;
                    h.pid = i;
                    h.resources = processes[i].Allocation;
                    h.timestamp = time(nullptr);
                    h.action = "allocate";
                    history.push_back(h);
                }
            }
        }
        if (!found) break;
    }

    for (int i = 0; i < nprocesses; i++) {
        if (!finish[i] && !processes[i].status) return false;
    }
    return true;
}

// ======================== Enhanced Features ========================

void InitializeBlockchain() {
    Block genesis;
    genesis.index = 0;
    genesis.timestamp = time(nullptr);
    genesis.transaction = "Genesis Block";
    genesis.previous_hash = "0";
    genesis.hash = CalculateHash(genesis);
    blockchain.push_back(genesis);
    LogAction("Blockchain", "Initialized with genesis block");
}

string CalculateHash(const Block& block) {
    stringstream ss;
    ss << block.index << block.timestamp << block.transaction << block.previous_hash;
    string input = ss.str();

    // Simple hash function (FNV-1a)
    const uint64_t FNV_PRIME = 16777619;
    const uint64_t FNV_OFFSET = 2166136261;
    uint64_t hash = FNV_OFFSET;

    for (char c : input) {
        hash ^= static_cast<uint64_t>(c);
        hash *= FNV_PRIME;
    }

    stringstream hash_ss;
    hash_ss << hex << setw(16) << setfill('0') << hash;
    return hash_ss.str();
}

void AddBlock(const string& transaction) {
    Block newBlock;
    newBlock.index = blockchain.size();
    newBlock.timestamp = time(nullptr);
    newBlock.transaction = transaction;
    newBlock.previous_hash = blockchain.back().hash;
    newBlock.hash = CalculateHash(newBlock);
    blockchain.push_back(newBlock);

    // Log to file
    ofstream log("blockchain.log", ios::app);
    log << "Block #" << newBlock.index << " | " << ctime(&newBlock.timestamp);
    log << "Transaction: " << transaction << endl;
    log << "Hash: " << newBlock.hash << endl << endl;
    log.close();
    LogAction("Blockchain", "Added block with transaction: " + transaction);
}

void VisualizeResourceGraph() {
    cout << "\n" << BOLD << CYAN << "Resource Allocation Graph:" << RESET << endl;
    cout << "Processes: ";
    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << " ";
    }
    cout << "\nResources: ";
    for (int i = 0; i < nresources; i++) {
        cout << "R" << i << "(" << available[i] << ") ";
    }
    cout << "\n\nAllocations:\n";

    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << " -> ";
        for (int j = 0; j < nresources; j++) {
            if (processes[i].Allocation[j] > 0) {
                cout << "R" << j << "(" << processes[i].Allocation[j] << ") ";
            }
        }
        cout << endl;
    }

    cout << "\nRequests:\n";
    for (int i = 0; i < nprocesses; i++) {
        cout << "P" << i << " needs: ";
        for (int j = 0; j < nresources; j++) {
            if (processes[i].Need[j] > 0) {
                cout << "R" << j << "(" << processes[i].Need[j] << ") ";
            }
        }
        cout << endl;
    }

    // Detect potential deadlocks visually
    bool potential_deadlock = false;
    for (int i = 0; i < nprocesses; i++) {
        for (int j = 0; j < nresources; j++) {
            if (processes[i].Need[j] > available[j]) {
                cout << RED << "! P" << i << " is waiting for R" << j << RESET << endl;
                potential_deadlock = true;
            }
        }
    }

    if (potential_deadlock) {
        cout << YELLOW << "\nWarning: Potential deadlock detected!" << RESET << endl;
        sim_stats.deadlocks_detected++;
    } else {
        cout << GREEN << "\nNo immediate deadlock detected" << RESET << endl;
    }
    LogAction("Graph", "Resource allocation graph displayed");
}

void PredictiveAllocation() {
    cout << "\n" << BOLD << MAGENTA << "Predictive Resource Allocation:" << RESET << endl;

    if (history.size() < 10) {
        cout << "Insufficient data for prediction (need at least 10 history records)" << endl;
        return;
    }

    vector<double> avg_increase(nresources, 0.0);
    for (int j = 0; j < nresources; j++) {
        double sum = 0;
        int count = 0;
        for (size_t i = 1; i < history.size(); i++) {
            int diff = history[i].resources[j] - history[i-1].resources[j];
            if (diff > 0) {
                sum += diff;
                count++;
            }
        }
        avg_increase[j] = (count > 0) ? sum / count : 0;
    }

    cout << "Predicted resource requests in next cycle:\n";
    for (int j = 0; j < nresources; j++) {
        cout << "R" << j << ": " << fixed << setprecision(2) << avg_increase[j] << "\t";
    }
    cout << endl;

    cout << "\nAllocation strategy suggestion:\n";
    for (int j = 0; j < nresources; j++) {
        if (avg_increase[j] > available[j]) {
            cout << RED << "Warning: R" << j << " may be insufficient (Need: " 
                 << avg_increase[j] << ", Available: " << available[j] << ")" << RESET << endl;
        } else if (avg_increase[j] > available[j] * 0.7) {
            cout << YELLOW << "Monitor R" << j << " closely (Need: " 
                 << avg_increase[j] << ", Available: " << available[j] << ")" << RESET << endl;
        } else {
            cout << GREEN << "R" << j << " is sufficient (Need: " 
                 << avg_increase[j] << ", Available: " << available[j] << ")" << RESET << endl;
        }
    }
    LogAction("Prediction", "Predictive allocation analyzed");
}

void HandleDeadlock() {
    cout << "\n" << BOLD << RED << "Deadlock Handling Mechanism" << RESET << endl;

    int lowest_priority = INT_MAX;
    int victim = -1;

    for (int i = 0; i < nprocesses; i++) {
        if (!processes[i].status && processes[i].priority < lowest_priority) {
            lowest_priority = processes[i].priority;
            victim = i;
        }
    }

    if (victim == -1) {
        cout << "No suitable victim found" << endl;
        return;
    }

    cout << "Terminating process P" << victim << " (priority: " 
         << processes[victim].priority << ") to resolve deadlock" << endl;

    for (int j = 0; j < nresources; j++) {
        available[j] += processes[victim].Allocation[j];
        processes[victim].Allocation[j] = 0;
        processes[victim].Need[j] = 0;
    }
    processes[victim].status = true;

    string transaction = "Deadlock resolution: Terminated P" + to_string(victim);
    AddBlock(transaction);
    sim_stats.deadlocks_resolved++;
    cout << "Resources released. System should now be deadlock-free." << endl;
    LogAction("Deadlock", "Resolved by terminating P" + to_string(victim));
}

void SimulationWorker() {
    while (simulation_running) {
        {
            lock_guard<mutex> lock(mtx);
            auto start = chrono::high_resolution_clock::now();

            int p = rand() % nprocesses;
            vector<int> req(nresources, 0);

            for (int j = 0; j < nresources; j++) {
                if (processes[p].Need[j] > 0) {
                    req[j] = rand() % min(processes[p].Need[j] + 1, available[j] + 1);
                }
            }

            cout << CYAN << "\nSimulation: P" << p << " requests [";
            for (int r : req) cout << r << " ";
            cout << "]" << RESET << endl;

            bool granted = true;
            for (int j = 0; j < nresources; j++) {
                if (req[j] > available[j]) {
                    granted = false;
                    break;
                }
            }

            if (granted) {
                for (int j = 0; j < nresources; j++) {
                    available[j] -= req[j];
                    processes[p].Allocation[j] += req[j];
                    processes[p].Need[j] -= req[j];
                }

                if (IsSafe(processes, available)) {
                    cout << GREEN << "Request granted. System safe." << RESET << endl;
                    string transaction = "P" + to_string(p) + " allocated resources";
                    AddBlock(transaction);
                } else {
                    cout << RED << "Request granted but led to unsafe state. Rolling back." << RESET << endl;
                    for (int j = 0; j < nresources; j++) {
                        available[j] += req[j];
                        processes[p].Allocation[j] -= req[j];
                        processes[p].Need[j] += req[j];
                    }
                }
            } else {
                cout << YELLOW << "Request denied. Insufficient resources." << RESET << endl;
                processes[p].wait_time += 1;
            }

            historical_need[p].push_back(processes[p].Need);
            sim_stats.requests_processed++;
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
            sim_stats.avg_response_time = (sim_stats.avg_response_time * (sim_stats.requests_processed - 1) + duration) / sim_stats.requests_processed;
            sim_stats.total_cycles++;
        }

        this_thread::sleep_for(chrono::seconds(1 + rand() % 3));
    }
}

void StartSimulation() {
    if (simulation_running) {
        cout << "Simulation already running" << endl;
        return;
    }

    simulation_running = true;
    thread worker(SimulationWorker);
    worker.detach();
    cout << GREEN << "Simulation started" << RESET << endl;
    LogAction("Simulation", "Started");
}

void StopSimulation() {
    simulation_running = false;
    cout << RED << "Simulation stopped" << RESET << endl;
    LogAction("Simulation", "Stopped");
}

void DisplayResourceUtilizationTrends() {
    cout << "\n" << BOLD << BLUE << "Resource Utilization Trends:" << RESET << endl;

    if (history.size() < 5) {
        cout << "Insufficient history data for trend analysis (need at least 5 records)" << endl;
        return;
    }

    vector<double> avg_utilization(nresources, 0.0);
    int cycles = min(5, static_cast<int>(history.size()));

    for (int i = history.size() - cycles; i < history.size(); i++) {
        for (int j = 0; j < nresources; j++) {
            avg_utilization[j] += history[i].resources[j];
        }
    }

    for (int j = 0; j < nresources; j++) {
        avg_utilization[j] = (avg_utilization[j] / cycles) / total_resources[j] * 100;
        cout << "R" << j << " average utilization (last " << cycles << " cycles): " 
             << fixed << setprecision(2) << avg_utilization[j] << "%" << endl;
    }
    LogAction("Trends", "Displayed resource utilization trends");
}

// ======================== New Features ========================

void AddProcess(int max_resources, int priority) {
    lock_guard<mutex> lock(mtx);
    process p;
    p.id = nprocesses;
    p.Max.resize(nresources, max_resources);
    p.Allocation.resize(nresources, 0);
    p.Need = p.Max;
    p.status = false;
    p.priority = priority;
    p.start_time = time(nullptr);
    p.end_time = 0;
    p.cpu_usage = (rand() % 50 + 10) / 100.0; // Random CPU usage 0.1-0.6
    p.wait_time = 0;
    processes.push_back(p);
    historical_need[nprocesses] = vector<vector<int>>();
    nprocesses++;
    cout << GREEN << "Added process P" << p.id << " with max resources " << max_resources 
         << " and priority " << priority << RESET << endl;
    string transaction = "Added process P" + to_string(p.id);
    AddBlock(transaction);
    LogAction("AddProcess", "P" + to_string(p.id) + " added");
}

void RemoveProcess(int pid) {
    lock_guard<mutex> lock(mtx);
    if (pid < 0 || pid >= nprocesses || processes[pid].status) {
        cout << RED << "Invalid or already completed process P" << pid << RESET << endl;
        return;
    }

    for (int j = 0; j < nresources; j++) {
        available[j] += processes[pid].Allocation[j];
    }
    processes[pid].status = true;
    processes[pid].end_time = time(nullptr);
    cout << GREEN << "Removed process P" << pid << RESET << endl;
    string transaction = "Removed process P" + to_string(pid);
    AddBlock(transaction);
    LogAction("RemoveProcess", "P" + to_string(pid) + " removed");
}

void RequestResources(int pid, const vector<int>& request) {
    lock_guard<mutex> lock(mtx);
    ValidateInput(pid, request, "request");
    auto start = chrono::high_resolution_clock::now();

    bool can_request = true;
    for (int j = 0; j < nresources; j++) {
        if (request[j] > processes[pid].Need[j] || request[j] > available[j]) {
            can_request = false;
            break;
        }
    }

    if (can_request) {
        vector<process> temp_processes = processes;
        vector<int> temp_available = available;

        for (int j = 0; j < nresources; j++) {
            temp_available[j] -= request[j];
            temp_processes[pid].Allocation[j] += request[j];
            temp_processes[pid].Need[j] -= request[j];
        }

        if (IsSafe(temp_processes, temp_available)) {
            for (int j = 0; j < nresources; j++) {
                available[j] = temp_available[j];
                processes[pid].Allocation[j] = temp_processes[pid].Allocation[j];
                processes[pid].Need[j] = temp_processes[pid].Need[j];
            }
            processes[pid].request_history.insert(processes[pid].request_history.end(), request.begin(), request.end());
            cout << GREEN << "Request granted for P" << pid << RESET << endl;
            string transaction = "P" + to_string(pid) + " allocated resources";
            AddBlock(transaction);
        } else {
            cout << RED << "Request denied: Unsafe state" << RESET << endl;
            processes[pid].wait_time += 1;
        }
    } else {
        cout << YELLOW << "Request denied: Insufficient resources or exceeds need" << RESET << endl;
        processes[pid].wait_time += 1;
    }

    sim_stats.requests_processed++;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    sim_stats.avg_response_time = (sim_stats.avg_response_time * (sim_stats.requests_processed - 1) + duration) / sim_stats.requests_processed;
    LogAction("Request", "P" + to_string(pid) + " requested resources");
}

void ReleaseResources(int pid, const vector<int>& release) {
    lock_guard<mutex> lock(mtx);
    ValidateInput(pid, release, "release");

    bool can_release = true;
    for (int j = 0; j < nresources; j++) {
        if (release[j] > processes[pid].Allocation[j]) {
            can_release = false;
            break;
        }
    }

    if (can_release) {
        for (int j = 0; j < nresources; j++) {
            available[j] += release[j];
            processes[pid].Allocation[j] -= release[j];
            processes[pid].Need[j] += release[j];
        }
        cout << GREEN << "Resources released for P" << pid << RESET << endl;
        string transaction = "P" + to_string(pid) + " released resources";
        AddBlock(transaction);

        AllocationHistory h;
        h.pid = pid;
        h.resources = release;
        h.timestamp = time(nullptr);
        h.action = "release";
        history.push_back(h);
    } else {
        cout << RED << "Cannot release: Exceeds allocated resources" << RESET << endl;
    }
    LogAction("Release", "P" + to_string(pid) + " released resources");
}

void DetectDeadlockCycle() {
    cout << "\n" << BOLD << RED << "Deadlock Cycle Detection:" << RESET << endl;
    vector<vector<int>> graph(nprocesses, vector<int>(nresources, 0));
    vector<bool> visited(nprocesses, false);
    vector<bool> in_stack(nprocesses, false);

    // Build resource allocation graph
    for (int i = 0; i < nprocesses; i++) {
        if (!processes[i].status) {
            for (int j = 0; j < nresources; j++) {
                if (processes[i].Need[j] > 0 && processes[i].Need[j] > available[j]) {
                    graph[i][j] = 1; // Process i needs resource j
                }
            }
        }
    }

    // DFS to detect cycles
    bool cycle_found = false;
    vector<int> cycle;

    function<void(int, vector<int>&)> dfs = [&](int v, vector<int>& path) {
        if (cycle_found) return;
        visited[v] = true;
        in_stack[v] = true;
        path.push_back(v);

        for (int j = 0; j < nresources; j++) {
            if (graph[v][j]) {
                for (int u = 0; u < nprocesses; u++) {
                    if (!processes[u].status && processes[u].Allocation[j] > 0) {
                        if (!visited[u]) {
                            dfs(u, path);
                        } else if (in_stack[u]) {
                            cycle_found = true;
                            cycle = path;
                            cycle.push_back(u);
                            return;
                        }
                    }
                }
            }
        }

        in_stack[v] = false;
        path.pop_back();
    };

    for (int i = 0; i < nprocesses; i++) {
        if (!visited[i] && !processes[i].status) {
            vector<int> path;
            dfs(i, path);
            if (cycle_found) break;
        }
    }

    if (cycle_found) {
        cout << RED << "Deadlock cycle detected: ";
        for (size_t i = 0; i < cycle.size(); i++) {
            cout << "P" << cycle[i];
            if (i < cycle.size() - 1) cout << " -> ";
        }
        cout << RESET << endl;
        sim_stats.deadlocks_detected++;
    } else {
        cout << GREEN << "No deadlock cycle detected" << RESET << endl;
    }
    LogAction("DeadlockCycle", cycle_found ? "Cycle detected" : "No cycle detected");
}

void ExportToText() {
    ofstream file("system_state.txt");
    if (!file.is_open()) {
        cout << RED << "Failed to open file for export: system_state.txt" << RESET << endl;
        return;
    }

    file << "nprocesses: " << nprocesses << "\n";
    file << "nresources: " << nresources << "\n";
    file << "available: ";
    for (int a : available) file << a << " ";
    file << "\n";
    file << "total_resources: ";
    for (int t : total_resources) file << t << " ";
    file << "\n";

    file << "processes:\n";
    for (const auto& p : processes) {
        file << "id: " << p.id << "\n";
        file << "Max: ";
        for (int m : p.Max) file << m << " ";
        file << "\nAllocation: ";
        for (int a : p.Allocation) file << a << " ";
        file << "\nNeed: ";
        for (int n : p.Need) file << n << " ";
        file << "\nstatus: " << (p.status ? "true" : "false") << "\n";
        file << "priority: " << p.priority << "\n";
        file << "cpu_usage: " << fixed << setprecision(2) << p.cpu_usage << "\n";
        file << "wait_time: " << p.wait_time << "\n";
    }

    file << "history:\n";
    for (const auto& h : history) {
        file << "pid: " << h.pid << "\n";
        file << "resources: ";
        for (int r : h.resources) file << r << " ";
        file << "\ntimestamp: " << h.timestamp << "\n";
        file << "action: " << h.action << "\n";
    }

    file.close();
    cout << GREEN << "System state exported to system_state.txt" << RESET << endl;
    LogAction("Export", "System state exported to text");
}

void DisplayProcessStatus() {
    cout << "\n" << BOLD << CYAN << "Process Status Monitor:" << RESET << endl;
    cout << "PID\tStatus\tCPU Usage\tWait Time\tPriority\n";
    for (const auto& p : processes) {
        cout << "P" << p.id << "\t" << (p.status ? "Done" : "Active") << "\t"
             << fixed << setprecision(2) << p.cpu_usage * 100 << "%\t"
             << p.wait_time << "s\t" << p.priority << endl;
    }
    LogAction("Status", "Displayed process status");
}

void LoadConfigFromText(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << RED << "Failed to open config file: " << filename << RESET << endl;
        return;
    }

    string line;
    getline(file, line);
    nprocesses = stoi(line.substr(line.find(": ") + 2));
    getline(file, line);
    nresources = stoi(line.substr(line.find(": ") + 2));

    getline(file, line);
    stringstream ss(line.substr(line.find(": ") + 2));
    available.clear();
    int val;
    while (ss >> val) available.push_back(val);

    getline(file, line);
    ss.clear();
    ss.str(line.substr(line.find(": ") + 2));
    total_resources.clear();
    while (ss >> val) total_resources.push_back(val);

    processes.clear();
    getline(file, line); // "processes:"
    while (getline(file, line) && line != "history:") {
        process p;
        p.id = stoi(line.substr(line.find(": ") + 2));
        getline(file, line);
        ss.clear();
        ss.str(line.substr(line.find(": ") + 2));
        p.Max.clear();
        while (ss >> val) p.Max.push_back(val);
        getline(file, line);
        ss.clear();
        ss.str(line.substr(line.find(": ") + 2));
        p.Allocation.clear();
        while (ss >> val) p.Allocation.push_back(val);
        getline(file, line);
        ss.clear();
        ss.str(line.substr(line.find(": ") + 2));
        p.Need.clear();
        while (ss >> val) p.Need.push_back(val);
        getline(file, line);
        p.status = (line.substr(line.find(": ") + 2) == "true");
        getline(file, line);
        p.priority = stoi(line.substr(line.find(": ") + 2));
        getline(file, line);
        p.cpu_usage = stod(line.substr(line.find(": ") + 2));
        getline(file, line);
        p.wait_time = stoi(line.substr(line.find(": ") + 2));
        processes.push_back(p);
        historical_need[p.id] = vector<vector<int>>();
    }

    history.clear();
    while (getline(file, line)) {
        AllocationHistory h;
        h.pid = stoi(line.substr(line.find(": ") + 2));
        getline(file, line);
        ss.clear();
        ss.str(line.substr(line.find(": ") + 2));
        h.resources.clear();
        while (ss >> val) h.resources.push_back(val);
        getline(file, line);
        h.timestamp = stoll(line.substr(line.find(": ") + 2));
        getline(file, line);
        h.action = line.substr(line.find(": ") + 2);
        history.push_back(h);
    }

    file.close();
    cout << GREEN << "Configuration loaded from " << filename << RESET << endl;
    LogAction("Config", "Loaded from " + filename);
}

void DisplaySimulationStats() {
    cout << "\n" << BOLD << BLUE << "Simulation Statistics:" << RESET << endl;
    cout << "Requests Processed: " << sim_stats.requests_processed << endl;
    cout << "Deadlocks Detected: " << sim_stats.deadlocks_detected << endl;
    cout << "Deadlocks Resolved: " << sim_stats.deadlocks_resolved << endl;
    cout << "Average Response Time: " << fixed << setprecision(2) << sim_stats.avg_response_time << " μs" << endl;
    cout << "Total Simulation Cycles: " << sim_stats.total_cycles << endl;
    LogAction("Stats", "Displayed simulation statistics");
}

void UpdatePriorityQueue() {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (int i = 0; i < nprocesses; i++) {
        if (!processes[i].status) {
            pq.push({processes[i].priority, i});
        }
    }
    cout << "\n" << BOLD << MAGENTA << "Priority Queue Updated:" << RESET << endl;
    while (!pq.empty()) {
        cout << "P" << pq.top().second << " (Priority: " << pq.top().first << ")" << endl;
        pq.pop();
    }
    LogAction("PriorityQueue", "Updated and displayed");
}

void ValidateInput(int pid, const vector<int>& vec, const string& type) {
    if (pid < 0 || pid >= nprocesses) {
        cout << RED << "Invalid process ID: " << pid << RESET << endl;
        throw invalid_argument("Invalid process ID");
    }
    if (vec.size() != static_cast<size_t>(nresources)) {
        cout << RED << "Invalid " << type << " size: expected " << nresources << RESET << endl;
        throw invalid_argument("Invalid " + type + " size");
    }
    for (int v : vec) {
        if (v < 0) {
            cout << RED << "Negative values not allowed in " << type << RESET << endl;
            throw invalid_argument("Negative values in " + type);
        }
    }
}

void InitializeSystem() {
    processes.clear();
    available.assign(nresources, 10);
    total_resources = available;
    nprocesses = 5;
    processes.resize(nprocesses);
    for (int i = 0; i < nprocesses; i++) {
        processes[i].id = i;
        processes[i].Max.assign(nresources, 5);
        processes[i].Allocation.assign(nresources, 0);
        processes[i].Need = processes[i].Max;
        processes[i].status = false;
        processes[i].priority = rand() % 5 + 1;
        processes[i].cpu_usage = (rand() % 50 + 10) / 100.0;
        processes[i].wait_time = 0;
        historical_need[i] = vector<vector<int>>();
    }
    sim_stats = {0, 0, 0, 0.0, 0};
    history.clear();
    blockchain.clear();
    InitializeBlockchain();
    cout << GREEN << "System initialized with default configuration" << RESET << endl;
    LogAction("Initialize", "System reset to default state");
}

// ======================== Menu System ========================

void DisplayMainMenu() {
    cout << "\n" << BOLD << "=== DEADLOCK AVOIDANCE SYSTEM ===" << RESET;
    cout << "\n1. Check Safe State";
    cout << "\n2. Request Resources";
    cout << "\n3. Release Resources";
    cout << "\n4. Add Process";
    cout << "\n5. Remove Process";
    cout << "\n6. Display Allocation Table";
    cout << "\n7. Visualize Resource Graph";
    cout << "\n8. Predictive Allocation";
    cout << "\n9. Handle Deadlock";
    cout << "\n10. Performance Metrics";
    cout << "\n11. Security Audit";
    cout << "\n12. Start Simulation";
    cout << "\n13. Stop Simulation";
    cout << "\n14. View Blockchain";
    cout << "\n15. Export to Text";
    cout << "\n16. Load Configuration from Text";
    cout << "\n17. Resource Utilization Trends";
    cout << "\n18. Process Status Monitor";
    cout << "\n19. Simulation Statistics";
    cout << "\n20. Update Priority Queue";
    cout << "\n21. Detect Deadlock Cycle";
    cout << "\n22. Initialize System";
    cout << "\n23. Exit";
    cout << "\n\nEnter your choice: ";
}

int main() {
    srand(time(NULL));
    InitializeSystem();

    string choice;
    int option = 0;
    do {
        DisplayMainMenu();
        getline(cin, choice);

        try {
            option = stoi(choice);
            switch (option) {
                case 1: {
                    if (IsSafe(processes, available)) {
                        cout << GREEN << "System is in safe state. Sequence: ";
                        for (int i = 0; i < seq.size(); i++) {
                            cout << "P" << seq[i];
                            if (i < seq.size() - 1) cout << " → ";
                        }
                        cout << RESET << endl;
                    } else {
                        cout << RED << "System is in unsafe state!" << RESET << endl;
                    }
                    break;
                }
                case 2: {
                    int pid;
                    vector<int> req(nresources);
                    cout << "Enter process ID: ";
                    cin >> pid;
                    cout << "Enter resources to request (R0 R1 ...): ";
                    for (int i = 0; i < nresources; i++) cin >> req[i];
                    RequestResources(pid, req);
                    break;
                }
                case 3: {
                    int pid;
                    vector<int> rel(nresources);
                    cout << "Enter process ID: ";
                    cin >> pid;
                    cout << "Enter resources to release (R0 R1 ...): ";
                    for (int i = 0; i < nresources; i++) cin >> rel[i];
                    ReleaseResources(pid, rel);
                    break;
                }
                case 4: {
                    int max_res, priority;
                    cout << "Enter max resources per type: ";
                    cin >> max_res;
                    cout << "Enter priority (1-5): ";
                    cin >> priority;
                    AddProcess(max_res, priority);
                    break;
                }
                case 5: {
                    int pid;
                    cout << "Enter process ID to remove: ";
                    cin >> pid;
                    RemoveProcess(pid);
                    break;
                }
                case 6:
                    DisplayAllocationTable(processes);
                    break;
                case 7:
                    VisualizeResourceGraph();
                    break;
                case 8:
                    PredictiveAllocation();
                    break;
                case 9:
                    HandleDeadlock();
                    break;
                case 10:
                    PerformanceMetrics();
                    break;
                case 11:
                    GenerateSecurityReport();
                    break;
                case 12:
                    StartSimulation();
                    break;
                case 13:
                    StopSimulation();
                    break;
                case 14:
                    DisplayBlockchain();
                    break;
                case 15:
                    ExportToText();
                    break;
                case 16: {
                    string filename;
                    cout << "Enter config file name: ";
                    cin >> filename;
                    LoadConfigFromText(filename);
                    break;
                }
                case 17:
                    DisplayResourceUtilizationTrends();
                    break;
                case 18:
                    DisplayProcessStatus();
                    break;
                case 19:
                    DisplaySimulationStats();
                    break;
                case 20:
                    UpdatePriorityQueue();
                    break;
                case 21:
                    DetectDeadlockCycle();
                    break;
                case 22:
                    InitializeSystem();
                    break;
                case 23:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << RED << "Invalid choice. Try again." << RESET << endl;
            }
        } catch (const exception& e) {
            cout << RED << "Error: " << e.what() << RESET << endl;
            LogAction("Error", e.what());
        }

        if (option != 23) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != "23");

    return 0;
}

// Placeholder implementations for unimplemented functions
void SaveStateToFile(const string& filename) {
    cout << YELLOW << "SaveStateToFile not implemented" << RESET << endl;
    LogAction("SaveState", "Attempted to save state to " + filename);
}

void LoadStateFromFile(const string& filename) {
    cout << YELLOW << "LoadStateFromFile not implemented" << RESET << endl;
    LogAction("LoadState", "Attempted to load state from " + filename);
}

void NetworkSync() {
    cout << YELLOW << "NetworkSync not implemented" << RESET << endl;
    LogAction("NetworkSync", "Attempted network sync");
}

void PerformanceMetrics() {
    cout << "\n" << BOLD << BLUE << "Performance Metrics:" << RESET << endl;

    auto start = chrono::high_resolution_clock::now();
    bool safe = IsSafe(processes, available);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Safety check time: " << duration.count() << " μs" << endl;
    cout << "System state: " << (safe ? "Safe" : "Unsafe") << endl;

    vector<double> utilization(nresources, 0.0);
    for (int j = 0; j < nresources; j++) {
        int allocated = 0;
        for (int i = 0; i < nprocesses; i++) {
            allocated += processes[i].Allocation[j];
        }
        utilization[j] = static_cast<double>(allocated) / (allocated + available[j]) * 100;
        cout << "R" << j << " utilization: " << fixed << setprecision(2) << utilization[j] << "%" << endl;
    }

    int deadlock_conditions = 0;
    for (int i = 0; i < nprocesses; i++) {
        for (int j = 0; j < nresources; j++) {
            if (processes[i].Need[j] > available[j]) {
                deadlock_conditions++;
            }
        }
    }

    double deadlock_prob = min(1.0, static_cast<double>(deadlock_conditions) / (nprocesses * nresources) * 2);
    cout << "Deadlock probability: " << deadlock_prob * 100 << "%" << endl;
    LogAction("Metrics", "Displayed performance metrics");
}

void GenerateSecurityReport() {
    cout << "\n" << BOLD << MAGENTA << "Security Audit Report:" << RESET << endl;

    int warning_count = 0;

    for (int i = 0; i < nprocesses; i++) {
        for (int j = 0; j < nresources; j++) {
            if (processes[i].Allocation[j] > processes[i].Max[j]) {
                cout << RED << "SECURITY VIOLATION: P" << i << " allocated more than max for R" << j 
                     << " (" << processes[i].Allocation[j] << " > " << processes[i].Max[j] << ")" << RESET << endl;
                warning_count++;
            }
        }
    }

    int leaked_resources = 0;
    for (int j = 0; j < nresources; j++) {
        int total_alloc = 0;
        for (int i = 0; i < nprocesses; i++) {
            total_alloc += processes[i].Allocation[j];
        }

        if (total_alloc + available[j] != total_resources[j]) {
            cout << YELLOW << "RESOURCE LEAK: R" << j << " inconsistency ("
                 << total_alloc + available[j] << " vs " << total_resources[j] << ")" << RESET << endl;
            leaked_resources++;
            warning_count++;
        }
    }

    bool chain_valid = true;
    for (size_t i = 1; i < blockchain.size(); i++) {
        if (blockchain[i].previous_hash != blockchain[i-1].hash || 
            blockchain[i].hash != CalculateHash(blockchain[i])) {
            chain_valid = false;
            break;
        }
    }

    if (!chain_valid) {
        cout << RED << "BLOCKCHAIN TAMPERING DETECTED!" << RESET << endl;
        warning_count++;
    } else {
        cout << GREEN << "Blockchain integrity verified (" << blockchain.size() << " blocks)" << RESET << endl;
    }

    if (warning_count == 0) {
        cout << GREEN << "No security issues detected" << RESET << endl;
    } else {
        cout << RED << warning_count << " security warnings found" << RESET << endl;
    }
    LogAction("Security", "Generated security report");
}

void DisplayBlockchain() {
    cout << "\n" << BOLD << MAGENTA << "Blockchain Contents:" << RESET << endl;
    for (const auto& block : blockchain) {
        cout << "Block #" << block.index << " | " << ctime(&block.timestamp);
        cout << "Transaction: " << block.transaction << endl;
        cout << "Previous Hash: " << block.previous_hash << endl;
        cout << "Hash: " << block.hash << endl << endl;
    }
    LogAction("Blockchain", "Displayed blockchain contents");
}

void InteractiveTutorial() {
    cout << YELLOW << "InteractiveTutorial not implemented" << RESET << endl;
    LogAction("Tutorial", "Attempted to start interactive tutorial");
}

void ResetSystem() {
    cout << YELLOW << "ResetSystem not implemented" << RESET << endl;
    LogAction("Reset", "Attempted to reset system");
}

void DisplayPriorityQueue() {
    cout << YELLOW << "DisplayPriorityQueue not implemented" << RESET << endl;
    LogAction("PriorityQueue", "Attempted to display priority queue");
}

void ModifyResource() {
    cout << YELLOW << "ModifyResource not implemented" << RESET << endl;
    LogAction("ModifyResource", "Attempted to modify resource");
}

void CalculateDeadlockProbability() {
    cout << YELLOW << "CalculateDeadlockProbability not implemented" << RESET << endl;
    LogAction("DeadlockProb", "Attempted to calculate deadlock probability");
}
