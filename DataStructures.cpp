#include "DataStructures.hpp"
#include <iostream>

using namespace std;

//TREENODE Implementation
TreeNode::TreeNode(int id, const string& msg){
    version_id = id;
    content = "";
    message = msg;
    created_timestamp = time(nullptr);//Sets the created timestamp to the current time
    snapshot_timestamp = time(nullptr);//Sets the snapshot timestamp to the current time
    parent = nullptr;
}

TreeNode::TreeNode(int id, TreeNode* p){//
    version_id = id;
    parent = nullptr;
    message = "";
    created_timestamp = time(nullptr);
    snapshot_timestamp = 0;//Snapshot time=0 indicates that it's not a snapshot
    parent = p;//set the parent to p
}

TreeNode::~TreeNode() {
    for (TreeNode* child : children) {
        delete child;
    }
}

bool TreeNode::isSnapshot() {
    return snapshot_timestamp != 0;
}

// VERSION MAP(HASH MAP) Implementation
VersionMap::VersionMap(unsigned int capacity) : capacity(capacity) {
    entries.resize(capacity, {0, nullptr, true});//Initialize all entries as empty and set Total no. of entries to capacity
}

VersionMap::~VersionMap(){}

unsigned int VersionMap::hashFunction(int key) {
    //Simple hashing function: used modulo to map
        return key % capacity;
}

void VersionMap::insert(int key, TreeNode* val) {
    unsigned int index=hashFunction(key);//Index is never negative
    while(!entries[index].is_empty && entries[index].key!=key) {
        index=(index+1)%capacity;
    }
    entries[index]={key, val, false};
}
//get function to retrieve a TreeNode* by its version ID key
TreeNode* VersionMap::get(int key) {
    unsigned int index = hashFunction(key);
    unsigned int start_index = index;
    while (!entries[index].is_empty) {
        if (entries[index].key == key) {
            return entries[index].value;
        }
        index = (index + 1) % capacity;
        if (index == start_index) break;
    }
    return nullptr;
}

// FILE
File::File(string initial_message){
    total_versions=1;
    root=new TreeNode(0, initial_message);
    active_version=root;
    version_map.insert(0, root);
}
File::~File() {
    delete root;
}
void File::createNewVersion(){
    TreeNode* new_node=new TreeNode(total_versions, active_version);
    active_version->children.push_back(new_node);
    active_version=new_node;
    version_map.insert(new_node->version_id, new_node);
    total_versions++;
}
void File::updateTime(){
    if(active_version){
        active_version->created_timestamp=time(nullptr);
    }
}
void File::read() {
    if(active_version) {
        cout<<"Content of file (version "<<active_version->version_id<<"):"<<endl;
        cout<<active_version->content<<endl;
    } else {
        cerr<<"Error: No active version to read."<< endl;
    }
}
void File::insert(const string& content) {
    if(!active_version){
        cerr<<"Error: No active version to modify."<<endl;
        return;
    }
    if(active_version->isSnapshot()){
        createNewVersion();
    }
    active_version->content+=content; // allow empty content
    updateTime();
}
void File::update(const string& content) {
    if(!active_version) {
        cerr<<"Error: No active version to modify."<<endl;
        return;
    }
    if(active_version->isSnapshot()) {
        createNewVersion();
    }
    active_version->content=content; // allow empty content
    updateTime();
}
void File::snapshot(const string& msg) {
    if(!active_version) {
        cerr<<"Error: No active version to snapshot."<<endl;
        return;
    }
    if(active_version->isSnapshot()) {
        cout<<"Version ID "<<active_version->version_id<<" is already a snapshot."<<endl;
    } else {
        active_version->snapshot_timestamp=time(nullptr);
        active_version->message=msg; // allow empty message
        cout<<"Snapshot created for version ID "<<active_version->version_id<<endl;
    }
    updateTime();
}
void File::rollback(int version_id) {
    TreeNode* target_version = version_map.get(version_id);
    if(target_version){
        active_version=target_version;
        cout<<"Successfully rolled back to version ID "<<version_id<<"."<<endl;
    } else {
        cerr<<"Error: Version ID "<<version_id<<" not found."<<endl;
    }
}
void File::rollbackToParent() {
    if (active_version && active_version->parent){
        active_version=active_version->parent;
        cout<<"Rolled back to parent version ID "<<active_version->version_id<<"."<<endl;
    } else {
        cerr<<"Error: Cannot roll back. No parent version found."<<endl;
    }
}
void File::history() {
    TreeNode* current=active_version;
    cout<<"Snapshot history for file:"<<endl;
    vector<TreeNode*> snapshots;
    while(current){
        if(current->isSnapshot()){
            snapshots.push_back(current);
        }
        current=current->parent;
    }
    for(int i = snapshots.size() - 1; i >= 0; --i){
        TreeNode* snapshot=snapshots[i];
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&snapshot->snapshot_timestamp));
        cout <<"  - ID: "<< snapshot->version_id 
             <<", Time: "<< time_str
             <<", Message: \""<< snapshot->message << "\"" << endl;
    }
}

// FILEMAP (HASH MAP FOR FILENAMES) Implementation
FileMap::FileMap(unsigned int capacity){
    this->capacity=capacity;
    entries.resize(capacity, {"", nullptr, true, false});
}
FileMap::~FileMap() {}

int FileMap::hashFunction(const string& key){
    int hash=0;
    for (int i=0; i<key.size(); i++) {
        hash=(hash*31+key[i])%capacity;
    }
    return hash;
}
void FileMap::insert(const string& key, File* value) {
    if(key.empty() || value==nullptr) return;
    int index= hashFunction(key);
    int first_deleted=-1;
    int start_index=index;
    while(!entries[index].is_empty || entries[index].is_deleted) {
        if(entries[index].key==key && !entries[index].is_empty && !entries[index].is_deleted) {
            entries[index].value=value;
            return;
        }
        if(entries[index].is_deleted && first_deleted==-1) {
            first_deleted=index;
        }
        index=(index+1)%capacity;
        if(index==start_index) break;
    }
    if(first_deleted!=-1) {
        entries[first_deleted]={key, value, false, false};
    } else {
        entries[index]={key, value, false, false};
    }
}
File* FileMap::get(const string& key) {//similar logic to versionMap get function
    if(key.empty()) return nullptr;
    int index=hashFunction(key);
    int start_index=index;
    while(!entries[index].is_empty || entries[index].is_deleted){
        if(!entries[index].is_empty && !entries[index].is_deleted && entries[index].key==key){
            return entries[index].value;
        }
        index=(index+1)%capacity;
        if (index==start_index) break;
    }
    return nullptr;
}
bool FileMap::contains(const string& key) {
    return get(key)!=nullptr;
}
void FileMap::erase(const string& key) {
    if(key.empty()) return;
    int index=hashFunction(key);
    int start_index=index;
    while(!entries[index].is_empty || entries[index].is_deleted){
        if(!entries[index].is_empty && !entries[index].is_deleted && entries[index].key==key) {
            entries[index].is_empty=false;
            entries[index].is_deleted=true;
            entries[index].value=nullptr;
            entries[index].key="";
            return;
        }
        index=(index+1)%capacity;
        if (index==start_index) break;
    }
}
vector<string> FileMap::keys() {
    vector<string> result;
    for (int i=0; i<(int)entries.size(); i++){
        if(!entries[i].is_empty && !entries[i].is_deleted){
            result.push_back(entries[i].key);
        }
    }
    return result;
}

//FILEHEAP
FileHeap::FileHeap(bool sort_by_recent){
    this->sort_by_recent=sort_by_recent;
}
FileHeap::~FileHeap(){};
bool FileHeap::compare(Fileppt& a, Fileppt& b){
    if(sort_by_recent){
        return a.last_modified>b.last_modified;
    } else {
        return a.total_versions>b.total_versions;
    }
}
void FileHeap::push(const Fileppt& value){
    heap.push_back(value);
    heapifyUp(heap.size()-1);
}

Fileppt FileHeap::pop(){
    if(empty()){
        cout<<"Heap is empty. Returning a default Fileppt.\n";
        return Fileppt{};
    }
    Fileppt root=heap.front();
    heap[0]=heap.back();
    heap.pop_back();
    heapifyDown(0);
    return root;
}

bool FileHeap::empty(){
    return heap.empty();
}

void FileHeap::heapifyUp(unsigned int idx){
    while(idx>0){
        unsigned int parent_index=(idx-1)/2;
        if(compare(heap[idx], heap[parent_index])){
            swap(heap[idx], heap[parent_index]);
            idx=parent_index;
        } else {
            break;
        }
    }
}

void FileHeap::heapifyDown(unsigned int index) {
    unsigned int l_child = 2 * index + 1;//index of left child
    unsigned int r_child = 2 * index + 2;//index of right child
    unsigned int best_index = index;

    if(l_child<heap.size() && compare(heap[l_child], heap[best_index])){
        best_index=l_child;
    }
    if(r_child<heap.size() && compare(heap[r_child], heap[best_index])){
        best_index=r_child;
    }

    if(best_index!=index){
        swap(heap[index], heap[best_index]);
        heapifyDown(best_index);
    }
}
