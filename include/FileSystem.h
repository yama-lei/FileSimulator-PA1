#pragma once

#include <sstream>
#include "Directory.h"
#include "File.h"

class FileSystem{
private:
    Directory * root;
    Directory * cur;

    string username;
    std::set<string> users;
    std::unordered_map<string, uint64_t> config_table;
public:
    FileSystem(const string& username, const uint64_t& inode);
    ~FileSystem() = default;

    // Navigation
    bool changeDir(const uint64_t& inode);

    // File operation "local"
    File* createFile(const string& name);
    bool deleteFile(const string& name);

    // Dir operation "local"
    Directory* createDir(const string& name);
    bool deleteDir(const string& name, const string& user, bool recursive = false);

    // File and Dir operation "Global"
    uint64_t search(const string& name, const string& type);

    // Getters
    string getCurrentPath() const;
    string getUserName() const;

    // Setters
    bool setUser(const string& username);
    bool setCurrentDir(Directory* newDir);

    // User management methods
    bool hasUser(const string& username) const;
    bool registerUser(const string& username);

    // helper function
    Directory* getCurrentDir() const { return cur; }
    Directory* getRootDir() const { return root; }

    // helper function for change dir, no useful, use search directly
    FileObj* resolvePath(const string& path,string type="Directory",bool relative=false);
    void display();

    FileObj* inodeToPointer(uint64_t inode,Directory* rt=nullptr) {
        if (inode == 1) {
            return root;
        }
        if (rt == nullptr) {
            rt = root;
        }
        vector<FileObj*> sons=(rt->getAll());
        for (auto obj : sons) {
            if (obj->getInode() == inode) {
                return obj;
            }
            if(obj->getType()=="Directory") {
                FileObj* ptr = inodeToPointer(inode, dynamic_cast<Directory*>(obj));
                if (ptr !=nullptr ) {
                    return ptr;
                }
            }
        }
        return nullptr;
    }

};
