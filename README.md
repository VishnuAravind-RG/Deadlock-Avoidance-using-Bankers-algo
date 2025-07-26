Banker's Algorithm with Priority Scheduling and Advanced Features

📌 Overview
This project implements the Banker's Algorithm enhanced with priority scheduling, deadlock detection, blockchain-inspired transaction logging, and real-time simulation. It ensures safe resource allocation while prioritizing high-priority processes, preventing deadlocks, and logging system events for security and analysis. The system is designed for educational and demonstration purposes, showcasing advanced operating system concepts in C++.
📁 Project Structure
📂 Bankers-Algorithm-Priority
├── 📄 CMakeLists.txt    # Build configuration
├── 📂 src              # Source code
│   ├── main.cpp       # Complete implementation (Banker's algorithm, priority scheduling, etc.)
├── 📂 logs             # Output log files
│   ├── system.log     # System event logs
│   ├── blockchain.log # Blockchain transaction logs
├── 📂 config           # Configuration files
│   ├── system_state.txt # Sample state export/import file
├── 📄 README.md        # Project documentation
├── 📄 LICENSE          # MIT License details

Note: The current implementation is contained in a single main.cpp file for simplicity. To modularize, you can split into banker.cpp (algorithm logic), blockchain.cpp (hashing and logging), and simulation.cpp (simulation features), with corresponding headers.
⚙️ Installation & Compilation
Prerequisites

C++ Compiler: GCC, Clang, or MSVC (C++11 or later)
CMake: For cross-platform building (optional, as the code can be compiled directly)
No External Libraries: The implementation uses only the C++ standard library, eliminating dependencies on OpenSSL or libcurl.

Steps to Compile & Run
# Clone the repository (replace with your actual repo URL)
git clone https://github.com/yourusername/Bankers-Algorithm-Priority.git
cd Bankers-Algorithm-Priority

# Compile directly with g++
g++ src/main.cpp -o banker

# Alternatively, build using CMake
mkdir build && cd build
cmake ..
make

# Run the executable
./banker

Compilation Notes

The code is self-contained and requires no external libraries, making it portable across platforms.
Use -std=c++11 or later if your compiler defaults to an older standard:g++ -std=c++11 src/main.cpp -o banker



📝 Input Configuration
The program initializes with a default configuration (5 processes, 4 resources) but supports dynamic configuration via text file import or interactive menu. Below is a sample configuration format for system_state.txt:
nprocesses: 5
nresources: 4
available: 10 10 10 10
total_resources: 10 10 10 10
processes:
id: 0
Max: 5 5 5 5
Allocation: 0 0 0 0
Need: 5 5 5 5
status: false
priority: 3
cpu_usage: 0.25
wait_time: 0
id: 1
Max: 5 5 5 5
Allocation: 0 0 0 0
Need: 5 5 5 5
status: false
priority: 5
cpu_usage: 0.40
wait_time: 0
...
history:
pid: 0
resources: 1 0 0 0
timestamp: 1697051234
action: allocate

Interactive Input:

Use the menu (option 2) to request resources, e.g., P0 requesting 1 0 0 0.
Add processes (option 4) with max resources and priority.
Load custom configurations (option 16) from a text file.

📌 Sample Output
Upon running, the program displays a menu-driven interface. Example output for checking the safe state (option 1):
[SYSTEM] Current State:
Process  Allocation    Need       Priority  Status
P0       0 0 0 0      5 5 5 5    3         Active
P1       0 0 0 0      5 5 5 5    5         Active
P2       0 0 0 0      5 5 5 5    1         Active
P3       0 0 0 0      5 5 5 5    4         Active
P4       0 0 0 0      5 5 5 5    2         Active

Available: 10 10 10 10

[LOG] System is in safe state. Sequence: P2 → P0 → P4 → P3 → P1

Example simulation output (option 12):
Simulation: P3 requests [1 0 0 0]
Request granted. System safe.

🎯 Features
✅ Banker's Algorithm: Prevents deadlocks by ensuring safe resource allocation.✅ Priority Scheduling: Allocates resources based on process priority (lower number = higher priority).✅ Deadlock Detection: Identifies cycles in the resource allocation graph (option 21).✅ Blockchain Logging: Logs transactions (e.g., allocations, releases) using a simple FNV-1a hash for integrity (option 14).✅ Real-Time Simulation: Simulates resource requests/releases with random processes (options 12/13).✅ Text-Based Export/Import: Saves and loads system state for persistence (options 15/16).✅ Performance Metrics: Tracks request processing time, deadlock probability, and resource utilization (option 10).✅ Security Audit: Detects resource leaks and blockchain tampering (option 11).✅ Resource Utilization Trends: Analyzes historical allocation patterns (option 17).✅ Process Status Monitoring: Displays CPU usage, wait time, and priority (option 18).✅ Thread-Safe Operations: Uses mutexes for concurrent simulation.✅ Colorized Output: Enhances readability with ANSI color codes.  
🔍 How It Works

Initialization: Sets up processes, resources, and blockchain with default or user-provided configurations.
Banker's Algorithm:
Calculates process needs (Need = Max - Allocation).
Checks for a safe sequence using priority-based scheduling.
Allocates resources only if the system remains safe.


Priority Scheduling: Processes with higher priority (lower priority number) are favored in safe sequence generation.
Deadlock Detection: Uses depth-first search to detect cycles in the resource allocation graph.
Blockchain Logging: Each allocation/release is logged as a block with a hash, ensuring auditability.
Simulation: Runs a background thread to simulate random resource requests/releases, testing system robustness.
Export/Import: Saves system state (processes, resources, history) to a text file for later restoration.
Metrics & Monitoring: Tracks performance, utilization, and system health for analysis.

📜 License
This project is licensed under the MIT License - see the LICENSE file for details.
📢 Contributing

Fork the repository 🍴
Create a new branch 🚀 (git checkout -b feature-name)
Commit changes 🎯 (git commit -m 'Add feature')
Push to branch ⬆️ (git push origin feature-name)
Submit a Pull Request 🤝

Suggested Contributions

Modularize code into separate files (e.g., banker.cpp, blockchain.cpp).
Add a graphical interface using SFML or Qt.
Implement network synchronization for distributed systems.
Integrate a database (e.g., SQLite) for persistent storage.
Enhance deadlock prediction with machine learning.

🛠️ Troubleshooting

Compilation Errors: Ensure a C++11-compliant compiler. Use g++ -std=c++11 if needed.
File I/O Issues: Verify write permissions for system.log, blockchain.log, and system_state.txt.
Simulation Hangs: Stop simulation (option 13) before exiting to avoid detached thread issues.
Invalid Input: The program validates process IDs and resource vectors, displaying errors for invalid inputs.

📚 References

Silberschatz, Galvin, Gagne, Operating System Concepts (Banker's Algorithm)
Tanenbaum, Modern Operating Systems (Deadlock Prevention)
FNV-1a Hash Algorithm (used for blockchain)


💡 "Safe resource allocation with priority scheduling keeps systems deadlock-free!" 🚀
