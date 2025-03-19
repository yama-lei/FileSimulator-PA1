#include "Directory.h"
using namespace std;
Directory::Directory(const string& name, const string& owner, const uint64_t& inode, FileObj* parent)
    :FileObj(name, "", "Directory", owner, inode, parent) { 
    // TODO: finish construction function like File class
    if (parent != nullptr) {
        this->setPath(parent->getPath() + '\\' + name);
    } else {
        this->setPath(name);  
    }
}


// add, remove
bool Directory::add(FileObj* child) {
    // TODO: add a FileObj to current Directory, return true if add successfully, otherwise false
    // note 1: you can't add a nullptr 
    // note 2: set child's parent pointer
    if (child == nullptr) {
        return false;
    }
    child->setParent(this);
    this->children[child->getInode()] = child;
    child->setPath(this->getPath() + '\\' + child->getName());
    return true;
}

bool Directory::remove(uint64_t inode) {
    // TODO: Remove child object by inode, return true if remove successfully, otherwise false
    // note 1: free memory if child object need to be removed
    // note 2: only remove files, not directories (use removeDir for directories)
    if (this->children.count(inode) == 0) {
        cout << "The file do not exist!\n";
        return false;
    }
    try {
        if (this->children[inode]->getType() == "Directory") {
            throw runtime_error("ERROR: use removeDir to remove directories!");
        }
        delete this->children[inode];
        this->children.erase(inode);
    }catch (const exception& e){
        cout<<e.what()<<endl;
        return false;
    }
    
    
    return true;
}

bool Directory::removeDir(uint64_t inode){
    // TODO: Remove directory object by inode, return true if remove successfully, otherwise false
    // note 1: free memory of the directory and all its children recursively
    // note 2: return false if inode not in current directory or target is not a directory
    // note 3: use dynamic_cast to safely convert FileObj* to Directory*
    // note 4: recursively remove all children before removing the directory itself
    if (children.count(inode) == 0) {
        cout << "No such a file or directory in current path!\n";
        return false;
   }
    auto& item = children[inode];
    try {
        if (item->getType() != "Directory") {
            throw runtime_error("ERROR: use remove to remove files!");
        }
        children.erase(inode);
        delete item;
    }catch(const exception& e){
        cout << e.what() << endl;
        return false;
    }
    return true;
}

// Getters
FileObj* Directory::getChild(uint64_t inode) {
    // TODO: Get child object by inode if found, otherwise nullptr
    try {
        if (children.count(inode) == 0) {
            throw runtime_error("File or Directory with inode" + to_string(inode) + " not found ");
        }

        return children[inode];
    }
    catch(exception e){
        cout << e.what() << endl;
        return nullptr;
    }
}


std::vector<FileObj*> Directory::getAll() const {
    // TODO: Get all child objects in vector, may be empty
    vector<FileObj*> all;
    for (auto pair : children) {
        all.push_back(pair.second);
    }
    return all;
}

size_t Directory::getCount() const {
    // TODO: Get number of children
    return children.size();
}

// helper function
bool Directory::isEmpty() const {
    // TODO: return true if directory is empty, otherwise flase
    return children.empty();
}

// helper function
void Directory::display( size_t indent) const {

    if (indent == 0) {
        for (int i = 0; i < indent; i++)
            cout << " ";
        std::cout << "  Path:" <<getPath()<<" [Directory: " << getName() << "]"<< std::endl;
    }
    for (const auto& pair : this->children) {
        for (int i = 0; i < indent; i++)
            cout << " ";
        std::cout << "- " <<"Path: "<<pair.second->getPath()<<" " << pair.second->getType() << ": "
                 << pair.second->getName() << " with owner: " << pair.second->getOwner() << std::endl;
     //This part is modified by me.
        if (pair.second->getType() == "Directory") {
            pair.second->display(indent+4);
        }
    }
}

