#include "DataStructures.hpp"
#include <iostream>

using namespace std;

// FILESYSTEM - To manage multiple files with O(1) lookup using the hashmap FileMap
class FileSystem {
public:
    FileMap file_map;
    vector<string> file_order; // to preserve insertion order for listing

    FileSystem(){}
    ~FileSystem(){
        vector<string> keys = file_map.keys();
        for(int i=0; i<keys.size(); i++){
            File* f=file_map.get(keys[i]);
            delete f;
        }
    }

    void processCommand(const string& command){
        // Input Parsing: split only first two tokens, rest is content/message (can be empty)
        string cmd, filename, rest;
        size_t pos1 = command.find(' ');//find the space character
        if(pos1== string::npos){//No space, entire string is treated as a command
            cmd=command;        
        } else {
            cmd=command.substr(0,pos1);
            size_t pos2=command.find(' ', pos1 + 1);//sssearch another space after the first space
            if(pos2 == string::npos){//No second space, rest of the string is filename
                filename = command.substr(pos1 + 1);
            } else {
                filename = command.substr(pos1 + 1, pos2 - pos1 - 1);
                rest = command.substr(pos2 + 1); // content or message, can be empty
            }
        }

        if(cmd.empty()) return;//If empty command is given, ignore it

        if(cmd=="CREATE" && !filename.empty()){
            handleCreate(filename);
        } else if (cmd=="READ" && !filename.empty()) {
            handleRead(filename);
        } else if (cmd=="INSERT" && !filename.empty()) {
            handleInsert(filename, rest);
        } else if (cmd=="UPDATE" && !filename.empty()) {
            handleUpdate(filename, rest);
        } else if (cmd=="SNAPSHOT" && !filename.empty()) {
            handleSnapshot(filename, rest);
        } else if (cmd=="ROLLBACK" && !filename.empty()) {
            string version_str = rest;
            handleRollback(filename, version_str);
        } else if (cmd=="HISTORY" && !filename.empty()) {
            handleHistory(filename);
        } else if (cmd=="RECENT_FILES") {
            int num = 5;
            if (!filename.empty()) num = stoi(filename);
            handleRecentFiles(num);
        } else if (cmd=="BIGGEST_TREES") {
            int num = 5;
            if (!filename.empty()) num = stoi(filename);
            handleBiggestTrees(num);
        } else {
            cerr<<"Error: Unknown command."<<endl;
        }
    }

private:
    void handleCreate(const string& filename) {
        if(!file_map.contains(filename)){
            file_map.insert(filename, new File("File created."));
            file_order.push_back(filename);
            cout<<"File '"<<filename<<"' created successfully."<<endl;
        } else {
            cerr<<"Error: File '"<<filename<<"' already exists."<<endl;
        }
    }

    void handleRead(const string& filename){
        File* f=file_map.get(filename);
        if(f){
            f->read();
        } else {
            cerr<<"Error: File '"<<filename<<"' not found."<<endl;
        }
    }

    void handleInsert(const string& filename, const string& content) {
        File* f=file_map.get(filename);
        if(f){
            f->insert(content);
        } else {
            cerr<<"Error: File '"<<filename<<"' not found."<<endl;
        }
    }

    void handleUpdate(const string& filename, const string& content) {
        File* f=file_map.get(filename);
        if(f){
            f->update(content);
        } else {
            cerr<<"Error: File '"<<filename<<"' not found."<<endl;
        }
    }

    void handleSnapshot(const string& filename, const string& message) {
        File* f=file_map.get(filename);
        if(f){
            f->snapshot(message);
        } else {
            cerr<<"Error: File '"<<filename<<"' not found."<<endl;
        }
    }

    void handleRollback(const string& filename, const string& version_str) {
        File* f=file_map.get(filename);
        if(f){
            if(version_str.empty()){
                f->rollbackToParent();
            } else {
                int version_id=0;
                bool valid=true;
                for(int i=0; i<version_str.size(); i++){
                    if(version_str[i]<'0' || version_str[i]>'9') { valid=false; break; }
                    version_id=version_id*10 + (version_str[i]-'0');
                }
                if(valid){
                    f->rollback(version_id);
                } else {
                    cerr<<"Error: Invalid version ID provided."<<endl;
                }
            }
        } else {
            cerr<<"Error: File '"<<filename<<"' not found."<<endl;
        }
    }

    void handleHistory(const string& filename) {
        File* f=file_map.get(filename);
        if(f){
            f->history();
        } else {
            cerr<<"Error: File '"<<filename<<"' not found."<<endl;
        }
    }

    void handleRecentFiles(int num) {
        if(num<0) {
            cerr<<"Error: Number of files must be non-negative."<<endl;
            return;
        }
        FileHeap recent_files_heap(true);
        for(int i=0; i<file_order.size(); i++){
            File* file=file_map.get(file_order[i]);
            if(file && file->active_version){
                recent_files_heap.push({file_order[i], file->active_version->created_timestamp, 0});
            }
        }
        cout<<"Most recently modified files:"<<endl;
        for(int i=0; i<num && !recent_files_heap.empty(); i++){
            Fileppt file=recent_files_heap.pop();
            cout<<"  -> "<<file.filename<<endl;
        }
    }

    void handleBiggestTrees(int num) {
        if(num<0) {
            cerr<<"Error: Number of files must be non-negative."<<endl;
            return;
        }
        FileHeap biggest_trees_heap(false);
        for(int i=0; i<file_order.size(); i++){
            File* file=file_map.get(file_order[i]);
            if(file){
                biggest_trees_heap.push({file_order[i], 0, file->total_versions});
            }
        }
        cout<<"Files with the most versions:"<<endl;
        for(int i=0; i<num && !biggest_trees_heap.empty(); i++){
            Fileppt file=biggest_trees_heap.pop();
            cout<<"  - "<<file.filename<<" ("<<file.total_versions<<" versions)"<<endl;
        }
    }
};

void showUsage() {
    //List all the available commands and their input format
    cout << "Available commands:" << endl;
    cout << "  CREATE <filename>" << endl;
    cout << "  READ <filename>" << endl;
    cout << "  INSERT <filename> <content>" << endl;
    cout << "  UPDATE <filename> <content>" << endl;
    cout << "  SNAPSHOT <filename> <message>" << endl;
    cout << "  ROLLBACK <filename> [versionID]" << endl;
    cout << "  HISTORY <filename>" << endl;
    cout << "  RECENT_FILES [num]" << endl;
    cout << "  BIGGEST_TREES [num]" << endl;
    cout << "  EXIT" << endl;// Exit the program
}

int main() {
    FileSystem fs;
    string line;

    showUsage();
    cout << "\n> ";

    while(true){
        getline(cin, line);
        if(line.empty()){
            cout<<"> ";
            continue;
        }
        if(line.substr(0, 4)=="EXIT"){
            break;
        }
        fs.processCommand(line);
        cout<<endl<<"> ";//Prompt for next command
    }

    return 0;
}