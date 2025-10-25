#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H//Creating a header file

#include <string>
#include <vector>
#include <ctime>//The ctime library is included to handle time-related functions
using namespace std;

// A node in the version history tree.
class TreeNode {
public:
    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(int id, const string& msg);//Constructor for creating a snapshot node with a message
    TreeNode(int id, TreeNode* p);//Constructor overloading for creating a new version node with a parent
    ~TreeNode();

    bool isSnapshot();//function to check if the node is a snapshot
};

// A HashMap specificaly for version IDs to TreeNodes
class VersionMap {
public:
    struct Entry {
        int key;
        TreeNode* value;
        bool is_empty;
        bool is_deleted;
    };
private:
    vector<Entry> entries;
    unsigned int capacity;
    unsigned int hashFunction(int key);
public:
    VersionMap(unsigned int capacity = 101);
    ~VersionMap();
    void insert(int key, TreeNode* value);
    TreeNode* get(int key);
};

//File class
class File {
private:
    void createNewVersion();
    void updateTime();
public:
    TreeNode* root;
    TreeNode* active_version;
    VersionMap version_map;
    int total_versions;
    File(string initial_message);
    ~File();
    void read();
    void insert(const string& content);
    void update(const string& content);
    void snapshot(const string& msg);
    void rollback(int version_id);
    void rollbackToParent();
    void history();
};

// A struct to hold the file properties for heap operations.
struct Fileppt {
    string filename;
    time_t last_modified;
    int total_versions;
};

// Custom hash map for string keys (filenames) to File*
class FileMap{
public:
    struct Entry {
        string key;
        File* value;
        bool is_empty;
        bool is_deleted;
    };
private:
    vector<Entry> entries;
    int capacity;
    int hashFunction(const string& key);
public:
    FileMap(unsigned int capacity = 101);
    ~FileMap();
    void insert(const string& key, File* value);
    File* get(const string& key);
    bool contains(const string& key);
    void erase(const string& key);
    vector<string> keys(); //for ordered listing
};

// The Heap data structure, specifically for fileproperties.
class FileHeap {
public:
    FileHeap(bool sort_by_recent);
    ~FileHeap();

    void push(const Fileppt& value);
    Fileppt pop();
    bool empty();
    vector<Fileppt> heap;
    bool sort_by_recent;

    void heapifyUp(unsigned int index);//unsigned int handles negative values if given by mistake. Heapify up to maintain heap property
    void heapifyDown(unsigned int index);//heapify down to maintain heap property
    bool compare(Fileppt& a, Fileppt& b);//Comparison function to determine heap order
};
#endif