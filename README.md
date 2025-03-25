# Banker's Algorithm with Priority Scheduling

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

## 📌 Overview
This project implements the **Banker's Algorithm** with an added **priority scheduling** feature. It ensures safe resource allocation by considering both process priority and available system resources.

## 📁 Project Structure
```
📂 Bankers-Algorithm-Priority
├── 📄 CMakeLists.txt    # Build configuration
├── 📂 src              # Source code
│   ├── main.cpp       # Core implementation
│   ├── banker.cpp     # Banker's algorithm logic
│   ├── banker.h       # Header file
├── 📂 examples         # Sample input/output
├── 📄 README.md        # Project documentation
├── 📄 LICENSE          # MIT License details
```

## ⚙️ Installation & Compilation
### Prerequisites
- C++ Compiler (GCC, Clang, or MSVC)
- CMake (for cross-platform building)

### Steps to Compile & Run
```bash
# Clone the repository
git clone https://github.com/yourusername/Bankers-Algorithm-Priority.git
cd Bankers-Algorithm-Priority

# Build using CMake
mkdir build && cd build
cmake ..
make

# Run the executable
./banker
```

## 📝 Input Configuration
```plaintext
Number of resources: 3
Number of processes: 5

Process P0:
- Max: 7 5 3
- Allocation: 0 1 0
- Priority: 5

Process P1:
- Max: 3 2 2
- Allocation: 2 0 0
- Priority: 3

Available: 3 3 2
```

## 📌 Sample Output
```plaintext
[SYSTEM] Current State:
Process  Allocation    Need       Priority
P0       0 1 0        7 4 3      5
P1       2 0 0        1 2 2      3
P2       3 0 2        6 0 0      7

Available: 3 3 2

[LOG] Process P2 (Priority 7) allocated resources
[LOG] Safe sequence: P2 → P0 → P1 → P3 → P4
```

## 🎯 Features
✅ Implements Banker's Algorithm for deadlock prevention  
✅ Incorporates Priority Scheduling for resource allocation  
✅ Dynamic safety sequence generation  
✅ Logging for debugging and analysis  
✅ Optimized for C++ with efficient data structures  

## 🔍 How It Works
1. **Input Processing**: Reads number of processes, resources, allocation, and priorities.
2. **Need Calculation**: Computes remaining resource needs for each process.
3. **Safe Sequence Generation**: Uses priority scheduling to determine execution order.
4. **Resource Allocation**: Allocates resources based on availability and priority.
5. **Execution Log**: Outputs safe execution sequence if possible.

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📢 Contributing
1. Fork the repository 🍴
2. Create a new branch 🚀 (`git checkout -b feature-name`)
3. Commit changes 🎯 (`git commit -m 'Add feature'`)
4. Push to branch ⬆️ (`git push origin feature-name`)
5. Submit a Pull Request 🤝


---
💡 _"Efficient resource allocation ensures smooth system performance!"_ 🚀

