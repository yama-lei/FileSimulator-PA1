#include "FileSystem.h"

FileSystem::FileSystem(const string& username, const uint64_t& inode)
    : root(new Directory("", username, inode, nullptr)), cur(root), username(username) {
    // no change


    users.insert(username);
}

// Navigation
bool FileSystem::changeDir(const uint64_t& inode) {
    // TODO: Change current directory using inode directly, return true if change successfully, otherwise false
    // note 1: check if the inode exists in children [or parent] and if target is directory
    // note 2: update current directory pointer
    
    fprintf(stderr, "Error: FileSystem::changeDir() is not implemented yet!\n");
    assert(0);  
    return false;
}

// File operation "local"
File* FileSystem::createFile(const string& name) {
    // TODO: Create a new file in current directory, return File* if create successfully, otherwise nullptr
    // note 1: check if name already exists
    // note 2: generate new inode via InodeFactory, and update config_table

    fprintf(stderr, "Error: FileSystem::createFile() is not implemented yet!\n");
    assert(0);  
    return nullptr;
}

bool FileSystem::deleteFile(const string& name, const string& user){
    // TODO: Delete file with given inode from current directory, return true if delete successfully, otherwise false
    // note 1: check if the inode exists and if target is a file, and check permission via user and file owner
    // note 2: update config_table

    fprintf(stderr, "Error: FileSystem::deleteFile() is not implemented yet!\n");
    assert(0);  
    return false;
}

// Dir operation "local"
Directory* FileSystem::createDir(const string& name) {
    // TODO: Create a new directory in current directory, return Directory* if create successfully, otherwise nullptr
    // note 1: check if name already exists
    // note 2: generate new inode via InodeFactory, and update config_table

    fprintf(stderr, "Error: FileSystem::createDir() is not implemented yet!\n");
    assert(0);  
    return nullptr;
}

bool FileSystem::deleteDir(const string& name,const string& user, bool recursive){
    // TODO: Delete directory with given name from current directory, return true if delete target successfully, otherwise false 
    // note 1: if recursive is true, delete all contents and their config_table entries, and check permission via user and directory owner
    // note 2: if recursive is false, only delete if empty

    fprintf(stderr, "Error: FileSystem::deleteDir() is not implemented yet!\n");
    assert(0);  
    return false;
}

// File and Dir operation "Global"
uint64_t FileSystem::search(const string& name, const string& type) {
    // TODO: Search name in config_table, return inode if found in config_table, 0 if not found
    // note 1: try to find relative path (current path + name) in config_table
    // note 2: try to find absolute path (from root) in config_table first
    
    fprintf(stderr, "Error: FileSystem::search() is not implemented yet!\n");
    assert(0);  
    return 0;
}

// Getters
string FileSystem::getCurrentPath() const{
    // TODO: Get the full path of current directory
    // note 1: combine path from root to current directory
    // note 2: handle special case when at root

    fprintf(stderr, "Error: FileSystem::getCurrentPath() is not implemented yet!\n");
    assert(0);  
    return "";
}

string FileSystem::getUserName() const {
    // TODO: Get current username

    fprintf(stderr, "Error: FileSystem::getUserName() is not implemented yet!\n");
    assert(0);  
    return "";
}

// Setters
bool FileSystem::setUser(const string& username) {
    // TODO: Set new User to use FileSystem
    // note 1: check if the user is in set of users
    // note 2: update current user

    fprintf(stderr, "Error: FileSystem::setUser() is not implemented yet!\n");
    assert(0);  
    return false;
}

bool FileSystem::setCurrentDir(Directory* newDir){
    // TODO: Set new current dir in FileSystem, return true if set successfully, otherwise false

    fprintf(stderr, "Error: FileSystem::setCurrentDir() is not implemented yet!\n");
    assert(0);  
    return false;
}

// User management methods
bool FileSystem::hasUser(const string& username) const {
    // TODO: Check if user exists in users set, returns true if username exists, otherwise false
    fprintf(stderr, "Error: FileSystem::hasUser() is not implemented yet!\n");
    assert(0);  
    return false;
}

bool FileSystem::registerUser(const string& username) {
    // TODO： Check if user exists in users set, return true if register successfully, otherwise false

    fprintf(stderr, "Error: FileSystem::registerUser() is not implemented yet!\n");
    assert(0);  
    return false;
}

// helper function
FileObj* FileSystem::resolvePath(const string& path) {
    // TODO: resolve path, you can use strtok() in c library or istringstream with getline() in c++
    // return target of FileObj* if resolve successfully, otherwise nullptr

    fprintf(stderr, "Error: FileSystem::resolvePath() is not implemented yet!\n");
    assert(0);  
    return nullptr;
}