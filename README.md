# Time-Travelling File Management System

A Git-inspired **in-memory version control system** built in **C++**, designed to efficiently track, manage, and navigate historical file states using custom data structures.

This system allows users to create, update, snapshot, and roll back file versions while maintaining immutability and supporting branching histories.

---

## 🚀 Features

- Version-controlled in-memory file system inspired by **Git**
- Supports core commands:
  - `CREATE` – Add new files/directories  
  - `UPDATE` – Modify existing content  
  - `SNAPSHOT` – Capture current system state  
  - `ROLLBACK` – Revert to previous versions  
- Analytics commands:
  - `RECENT_FILES` – View most recently modified files  
  - `BIGGEST_TREES` – Identify highest storage trees
- **O(1)** version lookups using HashMaps
- **O(log n)** operations using balanced Trees
- Immutable snapshots using persistent data structures
- Branching history & conflict-free merges
- Real-time performance on **1000+ simulated operations**

---

## 🧠 Tech Stack

- Language: **C++**
- Libraries: **STL**
- Data Structures:  
  - Version Trees  
  - HashMaps  
  - Heaps  
  - Persistent Nodes

---

## 📂 Project Structure

