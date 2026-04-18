# Artifact Management System

## Description
This project is a command-based simulation system that manages artifacts and researchers. It supports assignment, tracking, and reporting operations while ensuring consistency between data structures.

## Features
- Manage artifacts and researchers
- Assign and return artifacts with validation rules
- Enforce researcher capacity constraints
- Process commands from an input file

## Data Structures
- AVL Tree (artifact management)
- Red-Black Tree (researcher management)

## Technologies
- C++
- Object-Oriented Programming

## How to Run
```bash
g++ -std=c++11 *.cpp *.h -o program
./program < input.txt
```
