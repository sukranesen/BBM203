# QuakeAssist Emergency Coordination System

## Description
This project is a simulation of an emergency coordination system developed for disaster response scenarios. It manages supply and rescue requests and assigns them to response teams based on urgency and capacity constraints.

The system processes commands from an input file and simulates real-time decision making in crisis situations.

## Features
- Manage supply and rescue requests using separate queues
- Assign requests to teams based on emergency priority
- Calculate emergency scores dynamically
- Handle team workload and capacity constraints
- Support rollback mechanism in case of overload
- Track missions using stack-based structure
- Process command-based simulation

## Data Structures
- Queue (implemented with dynamic circular arrays)
- Stack (implemented with dynamic arrays)

## Technologies
- C++
- Object-Oriented Programming
- Dynamic Memory Management
- File I/O

## How to Run
```bash
g++ -std=c++11 *.cpp *.h -o program
./program < input.txt
```
