# Time-Travelling File Management System

A Git-like in-memory version control system implemented in C++ with custom data structures. This system enables time-travel capabilities for file management, allowing you to create snapshots, rollback to previous versions, and track complete file history through a tree-based versioning system.

## 🌟 Features

- **Version Control**: Complete version history tracking with parent-child relationships
- **Snapshot System**: Create named snapshots at any point in time
- **Time Travel**: Rollback to any previous version or parent version
- **File Operations**: CREATE, READ, INSERT, UPDATE with automatic versioning
- **History Tracking**: View complete snapshot history with timestamps
- **File Analytics**: Query recently modified files and files with most versions
- **Custom Data Structures**: Implements hash maps, heaps, and version trees from scratch

## 🏗️ Architecture

### Core Data Structures

1. **TreeNode (Version Tree)**
   - Each node represents a file version with unique ID
   - Parent-child relationships form a tree structure
   - Snapshots marked with timestamps and messages
   - Stores content, creation time, and metadata

2. **VersionMap (Hash Map)**
   - O(1) lookup for version IDs to TreeNode mapping
   - Custom hash function with linear probing collision resolution
   - Efficient version retrieval

3. **FileMap (Hash Map)**
   - Maps filenames (strings) to File objects
   - O(1) file lookup by name
   - Handles deletion markers for removed entries

4. **FileHeap (Priority Queue)**
   - Max-heap implementation for file analytics
   - Supports sorting by modification time or version count
   - Used for RECENT_FILES and BIGGEST_TREES queries

5. **File Class**
   - Manages individual file version history
   - Maintains active version pointer
   - Handles INSERT, UPDATE, SNAPSHOT, and ROLLBACK operations

6. **FileSystem Class**
   - Central system managing multiple files
   - Command parser and dispatcher
   - Coordinates all file operations

## 📋 Prerequisites

- C++ compiler with C++11 support (g++, clang++)
- Standard C++ library

## 🚀 Installation & Compilation

```bash
# Clone the repository
git clone https://github.com/Parvm26/Time-Travelling-File-Management-System.git
cd Time-Travelling-File-Management-System

# Compile the project
g++ -std=c++11 -o file_system main.cpp DataStructures.cpp

# Run the application
./file_system
```

## 💻 Usage

### Available Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| **CREATE** | `CREATE <filename>` | Creates a new file |
| **READ** | `READ <filename>` | Displays current content of the file |
| **INSERT** | `INSERT <filename> <content>` | Appends content to the file |
| **UPDATE** | `UPDATE <filename> <content>` | Replaces file content |
| **SNAPSHOT** | `SNAPSHOT <filename> <message>` | Creates a named snapshot of current version |
| **ROLLBACK** | `ROLLBACK <filename> [versionID]` | Rolls back to specific version or parent |
| **HISTORY** | `HISTORY <filename>` | Shows snapshot history with timestamps |
| **RECENT_FILES** | `RECENT_FILES [num]` | Lists most recently modified files (default: 5) |
| **BIGGEST_TREES** | `BIGGEST_TREES [num]` | Lists files with most versions (default: 5) |
| **EXIT** | `EXIT` | Exits the application |

### Example Usage

```bash
# Create a new file
CREATE document.txt

# Add content to the file
INSERT document.txt Hello World!

# Read the current content
READ document.txt

# Create a snapshot
SNAPSHOT document.txt Initial version

# Update the content
UPDATE document.txt This is new content

# Create another snapshot
SNAPSHOT document.txt Updated version

# View history
HISTORY document.txt

# Rollback to previous version (parent)
ROLLBACK document.txt

# Rollback to specific version ID
ROLLBACK document.txt 0

# View recently modified files
RECENT_FILES 5

# View files with most versions
BIGGEST_TREES 3

# Exit the system
EXIT
```

### Interactive Example Session

```
Available commands:
  CREATE <filename>
  READ <filename>
  INSERT <filename> <content>
  UPDATE <filename> <content>
  SNAPSHOT <filename> <message>
  ROLLBACK <filename> [versionID]
  HISTORY <filename>
  RECENT_FILES [num]
  BIGGEST_TREES [num]
  EXIT

> CREATE notes.txt
File 'notes.txt' created successfully.

> INSERT notes.txt Meeting notes for project discussion
> READ notes.txt
Content of file (version 1):
Meeting notes for project discussion

> SNAPSHOT notes.txt First draft
Snapshot created for version ID 1

> UPDATE notes.txt Final meeting notes with action items
> READ notes.txt
Content of file (version 2):
Final meeting notes with action items

> HISTORY notes.txt
Snapshot history for file:
  - ID: 0, Time: 2025-10-30 14:30:00, Message: "File created."
  - ID: 1, Time: 2025-10-30 14:31:15, Message: "First draft"

> ROLLBACK notes.txt 1
Successfully rolled back to version ID 1.

> EXIT
```

## 🔧 Technical Implementation Details

### Version Tree Structure

- **Branching**: Each version can have multiple children, creating a tree structure
- **Snapshots**: Versions can be marked as snapshots with timestamps and messages
- **Active Version**: System maintains a pointer to the current working version
- **Auto-versioning**: INSERT and UPDATE on snapshots automatically create new versions

### Hash Map Implementation

- **Capacity**: Default capacity of 101 for both VersionMap and FileMap
- **Collision Resolution**: Linear probing for handling collisions
- **Hash Functions**:
  - VersionMap: Simple modulo operation on integer keys
  - FileMap: Polynomial rolling hash for string keys

### Time Complexity

| Operation | Time Complexity | Notes |
|-----------|----------------|-------|
| CREATE | O(1) average | Hash map insertion |
| READ | O(1) average | Direct version access |
| INSERT/UPDATE | O(1) average | Content modification |
| SNAPSHOT | O(1) | Mark version with timestamp |
| ROLLBACK | O(1) average | Hash map lookup |
| HISTORY | O(h) | h = height from active to root |
| RECENT_FILES | O(n log k) | n files, top k results |
| BIGGEST_TREES | O(n log k) | n files, top k results |

### Space Complexity

- **Per File**: O(v) where v = number of versions
- **Overall System**: O(f * v) where f = number of files

## 🎯 Use Cases

1. **Document Editing**: Track changes in documents with named snapshots
2. **Configuration Management**: Maintain configuration file history
3. **Code Versioning**: Simple version control for code files
4. **Collaborative Editing**: Each user's changes create new version branches
5. **Audit Trail**: Complete history with timestamps for compliance

## 🧪 Testing

To test the system, you can create a test script:

```bash
# Create a test input file
cat > test_input.txt << EOF
CREATE test1.txt
INSERT test1.txt Content 1
SNAPSHOT test1.txt Version 1
UPDATE test1.txt Content 2
SNAPSHOT test1.txt Version 2
CREATE test2.txt
INSERT test2.txt Another file
HISTORY test1.txt
RECENT_FILES 2
BIGGEST_TREES 2
EXIT
EOF

# Run the test
./file_system < test_input.txt
```

## 📚 Project Structure

```
.
├── main.cpp              # Main entry point, FileSystem class, command processing
├── DataStructures.hpp    # Header file with class declarations
├── DataStructures.cpp    # Implementation of all data structures
└── README.md            # This file
```

## 🔒 Design Decisions

1. **In-Memory Storage**: All data stored in RAM for fast access (no persistence)
2. **Custom Data Structures**: Built from scratch to demonstrate proficiency
3. **Tree-based Versioning**: Allows branching unlike linear version control
4. **Lazy Version Creation**: New versions created only on modification of snapshots
5. **Command-line Interface**: Simple text-based interface for ease of use

## ⚠️ Limitations

- **No Persistence**: Data lost when program exits (in-memory only)
- **No Merge Operations**: Cannot merge different version branches
- **Fixed Capacity**: Hash maps have fixed capacity (can be resized if needed)
- **Single User**: Not designed for concurrent access

## 🚀 Future Enhancements

- [ ] Persistent storage (save/load from disk)
- [ ] File deletion support
- [ ] Branch merging capabilities
- [ ] Export version history to standard formats
- [ ] GUI interface
- [ ] Multi-user support with conflict resolution
- [ ] Diff view between versions
- [ ] Search within file contents

## 👨‍💻 Author

**Parvm26**

## 📄 License

This project is available for educational purposes. Please check with the repository owner for specific license details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

---

**Note**: This is an educational project demonstrating implementation of version control concepts and custom data structures in C++. 
