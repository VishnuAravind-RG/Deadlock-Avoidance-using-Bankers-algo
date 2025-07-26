Banker's Algorithm with Priority Scheduling and Advanced Features
 
📌 Overview
This project implements the Banker's Algorithm enhanced with priority scheduling, deadlock detection, blockchain-inspired transaction logging, and real-time simulation. It ensures safe resource allocation while prioritizing high-priority processes, preventing deadlocks, and logging system events for security and analysis. Designed for educational and demonstration purposes, it showcases advanced operating system concepts in C++.
📁 Project Structure
📂 Bankers-Algorithm-Priority
├── 📄 CMakeLists.txt    # Build configuration (optional)
├── 📂 src              # Source code
│   ├── main.cpp       # Complete implementation (Banker's algorithm, priority scheduling, etc.)
├── 📂 logs             # Output log files
│   ├── system.log     # System event logs
│   ├── blockchain.log # Blockchain transaction logs
├── 📂 config           # Configuration files
│   ├── system_state.txt # Sample state export/import file
├── 📄 README.md        # Project documentation
├── 📄 LICENSE          # MIT License details

Note: The current implementation is in a single main.cpp file for simplicity. For scalability, consider splitting into banker.cpp (algorithm logic), blockchain.cpp (hashing/logging), and simulation.cpp, with corresponding headers.
⚙️ Installation & Compilation
Prerequisites

C++ Compiler: GCC, Clang, or MSVC (C++11 or later)
CMake: Optional, for cross-platform building
No External Libraries: Uses only the C++ standard library

Steps to Compile & Run
# Clone the repository
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

Use -std=c++11 if your compiler defaults to an older standard:g++ -std=c++11 src/main.cpp -o banker


Ensure write permissions for logs/ and config/ directories for log and state files.

📝 Input Configuration
The program initializes with a default configuration (5 processes, 4 resources) but supports dynamic configuration via text file import or interactive menu. Sample system_state.txt format:
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
Add processes (option 4) with max resources and priority (lower number = higher priority).
Load custom configurations (option 16) from a text file.

📌 Sample Output
Running the program displays a menu-driven interface. Example output for checking the safe state (option 1):
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
✅ Banker's Algorithm: Prevents deadlocks with safe resource allocation.✅ Priority Scheduling: Favors processes with lower priority numbers.✅ Deadlock Detection: Identifies cycles in the resource graph (option 21).✅ Blockchain Logging: Logs transactions with FNV-1a hash for auditability (option 14).✅ Real-Time Simulation: Simulates resource requests/releases (options 12/13).✅ Text-Based Export/Import: Saves/loads system state (options 15/16).✅ Performance Metrics: Tracks response time, deadlock probability (option 10).✅ Security Audit: Detects resource leaks and blockchain issues (option 11).✅ Resource Utilization Trends: Analyzes historical data (option 17).✅ Process Status Monitoring: Shows CPU usage, wait time (option 18).✅ Thread-Safe Operations: Uses mutexes for simulation.✅ Colorized Output: Improves readability with ANSI codes.  
🔍 How It Works

Initialization: Sets up processes, resources, and blockchain with defaults or user input.
Banker's Algorithm: Computes needs, checks safe sequences with priority consideration.
Priority Scheduling: Prioritizes lower-numbered priorities in safe sequence.
Deadlock Detection: Uses DFS to find cycles in the allocation graph.
Blockchain Logging: Records transactions as blocks with FNV-1a hashes.
Simulation: Runs a thread for random resource requests/releases.
Export/Import: Saves/restores state to/from text files.
Metrics & Monitoring: Provides performance and utilization insights.

📜 License
This project is licensed under the MIT License - see the LICENSE file for details.
📢 Contributing

Fork the repository 🍴
Create a new branch 🚀 (git checkout -b feature-name)
Commit changes 🎯 (git commit -m 'Add feature')
Push to branch ⬆️ (git push origin feature-name)
Submit a Pull Request 🤝

Suggested Contributions

Modularize code into banker.cpp, blockchain.cpp, simulation.cpp.
Add a graphical interface (e.g., SFML, Qt).
Implement network synchronization for distributed systems.
Integrate SQLite for persistent storage.
Enhance deadlock prediction with AI/ML.

🛠️ Troubleshooting

Compilation Errors: Ensure a C++11 compiler. Use g++ -std=c++11.
File I/O Issues: Verify write permissions for logs/ and config/.
Simulation Hangs: Stop simulation (option 13) before exiting.
Invalid Input: Program validates process IDs and resources, showing errors.

📚 References

Silberschatz, Galvin, Gagne, Operating System Concepts (Banker's Algorithm)
Tanenbaum, Modern Operating Systems (Deadlock Prevention)
FNV-1a Hash Algorithm (used for blockchain)

🚀 Acknowledgements

Developed with guidance from xAI's Grok 3, enhancing OS concepts.
Tested on [insert system, e.g., Ubuntu 22.04] with GCC 11.2.0.


💡 "Safe resource allocation with priority scheduling keeps systems deadlock-free!" 🚀
