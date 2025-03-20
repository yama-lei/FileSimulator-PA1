#pragma once
#include<iostream>
#include <unordered_map>
#include "FileObj.h"
using namespace std;
class Directory: public FileObj{
private:
    std::unordered_map<uint64_t, FileObj*> children;

public:
    Directory(const string& name, const string& owner, const uint64_t& inode, FileObj* parent);
    ~Directory() {
        for (auto& pair : children) {
            delete pair.second;
        }
        children.clear();
        /*if (getParent() != nullptr) {
            //Waring !!! I thik we should tell his parents if the son node is deleted!
            auto parent = dynamic_cast<Directory*>(getParent());
            parent->removeDir(getInode());
        }
    }*/
    }
    // add, remove
    bool add(FileObj* child);
    bool remove(uint64_t inode);
    bool removeDir(uint64_t inode);
    
    // Getters
    FileObj* getChild(uint64_t inode);
    std::vector<FileObj*> getAll() const;
    size_t getCount() const;

    // helper function
    bool isEmpty() const;
    void display(size_t indext=0) const override;
    friend void showFileTree(Directory* dir,uint64_t indent=0) {
        for (uint64_t i = 0; i < indent; i++) {
            cout << " ";
        }
        cout << "|--- " << dir->getName()<<"\t[dir]" << endl;
        for (auto pair:dir->children) {
            if (pair.second->getType() == "Directory") {
                showFileTree(dynamic_cast<Directory*>(pair.second), indent + 4);
            }
            else {
                for (uint64_t i = 0; i < indent+4; i++) {
                    cout << " ";
                }
                cout << "|--- " << pair.second->getName()<<endl;
            }
        }
    }
};

