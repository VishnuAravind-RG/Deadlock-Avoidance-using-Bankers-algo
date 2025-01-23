# Banker's Algorithm Implementation

## Overview
This project implements the Banker's Algorithm, a resource allocation and deadlock avoidance algorithm used in operating systems. The algorithm checks whether a system is in a safe state and helps prevent deadlocks by ensuring that resource requests can be satisfied without leading to an unsafe condition.

## Features
- **Dynamic Resource Management**: Supports varying numbers of resources and processes.
- **Process Synchronization**: Utilizes mutexes and condition variables to manage concurrent access to shared resources.
- **Interactive User Interface**: Prompts users for input and displays the current state of resources and allocations in a user-friendly format.
- **Color-Coded Output**: Enhances visibility of messages using ANSI escape codes.
- **Logging**: Records significant events in a log file for later review.
- **Resource Request Simulation**: Allows users to simulate dynamic resource requests during execution.
- **Process Completion Simulation**: Simulates process completion and resource release based on user input.

## Requirements
- C++ Compiler (e.g., g++, clang++)
- C++ Standard Library

## Installation
1. Clone the repository:

2. Compile the program:

## Usage
1. Run the program:

2. Follow the prompts to enter the number of resources, processes, allocation matrix, and maximum demand matrix.

3. After entering the required information, the program will simulate resource allocation and display the current state of resources.

4. You can choose to test multiple scenarios by following the on-screen instructions.

## Example Input

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue if you have suggestions or improvements.

## Acknowledgments
- Inspired by concepts from operating systems courses and literature.
