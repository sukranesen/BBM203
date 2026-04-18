# Rail Marshal System

## Description
This project is a simulation of a rail yard management system that organizes, assembles, and dispatches freight trains. It models how wagons are classified, grouped, and combined into trains based on destination and cargo type.

The system processes commands from an input file and simulates real-world railway logistics operations.

## Features
- Classify wagons based on destination and cargo type
- Maintain sorted wagon lists by weight
- Assemble trains from classified wagons
- Manage departure tracks and dispatch operations
- Enforce system constraints such as coupler load and hazardous cargo rules
- Process command-based simulation

## Data Structures
- Doubly Linked List (wagon management)
- Matrix structure (classification yard organization)

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
