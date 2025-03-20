#define RESET   "\033[0m"    // 重置颜色
#define RED     "\033[31m"   // 红色前景
#define GREEN   "\033[32m"   // 绿色前景
#include "FileSystem.h"
using namespace std;

FileSystem::FileSystem(const string& username, const uint64_t& inode)
    : root(new Directory("root", username, inode, nullptr)), cur(root), username(username) {
    // no change
    users.insert(username);//注意，如果你要修改这里的默认根节点位置的话，请修改三处地方，这里下面和上面2处+ClientInterface的124行
    config_table.insert({ "root Directory",root->getInode()});
}

// Navigation
bool FileSystem::changeDir(const uint64_t& inode) {
    // TODO: Change current directory using inode directly, return true if change successfully, otherwise false
    // note 1: check if the inode exists in children [or parent] and if target is directory
    // note 2: update current directory pointer
    FileObj* dir = inodeToPointer(inode);
    if (dir == nullptr) {
        cout << RED<<"ERROR: check if the FileObj with inode " << inode << " is a Directory or if the inode exists!\n"<<RESET;
    }
    else {
        cur = dynamic_cast<Directory*>(dir);
        cout << GREEN<< "The current path is set to " << cur->getPath()<<endl<<RESET;
        return true;
    }
    return false;
}

// File operation "local"
File* FileSystem::createFile(const string& name) {
    // TODO: Create a new file in current directory, return File* if create successfully, otherwise nullptr
    // note 1: check if name already exists
    // note 2: generate new inode via InodeFactory, and update config_table
    if (config_table.count(cur->getPath() + '\\' + name + " File") == 0) {
        File* newFile = new File(name, "File", this->username, InodeFactory::generateInode(), cur);
        newFile->setPath(cur->getPath() +'\\'+name);
        config_table.insert({ cur->getPath() +'\\'+name + " File",newFile->getInode() });
        cur->add(newFile);
        return newFile;
    }
    else {
        cout<<RED << "ERROR: the file " << name << " already exists!\n"<<RESET;
    }
    
    return nullptr;
}

//create a copy of oldFile in the current dir of FileSystem
File* FileSystem::createFile(const File* oldFile) {
    File* newFile = createFile(oldFile->getName() + "(copy)");
    if (newFile == nullptr) {
        return nullptr;
    }
    newFile->write(oldFile->read());
    return newFile;
}
//delete the file in the current dir;
bool FileSystem::deleteFile(const string& name){
    
    // TODO: Delete file with given inode from current directory, return true if delete successfully, otherwise false
    // note 1: check if the inode exists and if target is a file, and check permission via user and file owner
    // note 2: update config_table
    if (config_table.count(cur->getPath() +'\\' + name + " File") != 0) {
        uint64_t fileInode = config_table[cur->getPath() +'\\'+name + " File"];
        if (cur->getChild(fileInode)->getType() == "File") {
            if (cur->getChild(fileInode)->getOwner()==username) {
                cur->remove(fileInode);
                //这里调用remove函数会自己把这个文件删除，并且使得cur目录下没有这个文件
                config_table.erase(cur->getPath() +'\\'+name + " File");
            }
            else {
                cout << RED<<"ERROR: the user " << username << " do not have the authority to delete this file!\n"<<RESET;
            }
        }
        else {
            cout << RED<<"ERROR: " << name << " is not a File! Please check and call deleteDir() instead!\n"<<RESET;
        }
    }
    else {
        cout << RED<<"ERROR: the file " << name << " do not exist!\n"<<RESET;
    }
    return false;
}

// Dir operation "local"
Directory* FileSystem::createDir(const string& name) {
    // TODO: Create a new directory in current directory, return Directory* if create successfully, otherwise nullptr
    // note 1: check if name already exists
    // note 2: generate new inode via InodeFactory, and update config_table
    if (config_table.count(cur->getPath() +'\\'+name + " Directory") == 0) {
        Directory* newDir = new Directory(name, username, InodeFactory::generateInode(), cur);
        newDir->setPath(cur->getPath() +'\\'+name);
        cur->add(newDir);
        config_table.insert({cur->getPath()+'\\'+name + " Directory" ,newDir->getInode()});
        return newDir;
    }
    else {
        cout << RED<<"ERROR: the directory already exists\n"<<RESET;
    }
    return nullptr;
}

//make a deep copy of the dir in the current dir of filesystem
Directory* FileSystem::createDir(const Directory* oldDir) {
    Directory* oldCur = cur;//保存当下的目录
    Directory* newDirectory = createDir(oldDir->getName() + "(copy)");
    vector<FileObj*> oldDirSons = oldDir->getAll();
    cur = newDirectory;//进入新目录
    for (FileObj* obj : oldDirSons) {
        string type = obj->getType();
        if (type == "Directory") {
            createDir(dynamic_cast<Directory*>(obj));
        }
        else {
            createFile(dynamic_cast<File*>(obj));
        }
        //recursively call the function
    }
    cur = oldCur;//最后记得回到最初的目录；
    return newDirectory;
}
bool FileSystem::deleteDir(const string& name,const string& user, bool recursive){
    // TODO: Delete directory with given name from current directory, return true if delete target successfully, otherwise false 
    // note 1: if recursive is true, delete all contents and their config_table entries, and check permission via user and directory owner
    // note 2: if recursive is false, only delete if empty
    
    if (config_table.count(cur->getPath() +'\\'+name + " Directory") == 0) {
        cout << RED<<"ERROR: the directory do not exist;\n"<<RESET;
        return false;
    }
    Directory* dir = dynamic_cast<Directory*>(cur->getChild(config_table[cur->getPath() +'\\'+name + " Directory"]));
    if (dir->getOwner() != user) {
        cout << RED << "ERROR: User " << user << " do not have the authority to delete this dir!\n" << RESET;
    }
    if (recursive) {
        cur->removeDir(search(name, "Directory"));
        config_table.erase(cur->getPath()+'\\'+name + " Directory");
        cout << GREEN<<"The directory " << name << " is deleted successfully!\n"<<RESET;
        return true;
    }
    else {
        if (dir->isEmpty()) {
            cur->removeDir(search(name, "Directory"));
            config_table.erase(cur->getPath() + '\\' + name + " Directory");
            return true;
        }
        else {
            cout << RED<<"ERROR: the directory is not empty, try to add parameter -r\n"<<RESET;
            return false;
        }
    }
    return false;
}

// File and Dir operation "Global"
uint64_t FileSystem::search(const string& name, const string& type) {
    // TODO: Search name in config_table, return inode if found in config_table, 0 if not found
    // note 1: try to find relative path (current path + name) in config_table
    // note 2: try to find absolute path (from root) in config_table first
    if (config_table.count(cur->getPath() + '\\' + name + " " + type) == 0) {
        return 0;
    }
    else {
        return config_table[cur->getPath() + '\\' + name + " " + type];
    }
}

// Getters
string FileSystem::getCurrentPath() const{
    // TODO: Get the full path of current directory
    // note 1: combine path from root to current directory
    // note 2: handle special case when at root

    return cur->getPath();
}

string FileSystem::getUserName() const {
    // TODO: Get current username
    return username;
}

// Setters
bool FileSystem::setUser(const string& username) {
    // TODO: Set new User to use FileSystem
    // note 1: check if the user is in set of users
    // note 2: update current user
    if (!hasUser(username)) {
        cout << RED<<"ERROR: The user do not exist,register first!\n"<<RESET;
    }
    else {
        this->username = username;
        return true;
    }
   
    return false;
}

bool FileSystem::setCurrentDir(Directory* newDir){
    // TODO: Set new current dir in FileSystem, return true if set successfully, otherwise false
    if (inodeToPointer(newDir->getInode()) == newDir) {
        cur = newDir;
        //看似傻逼的操作，实则检测了是否能从root根目录找到这个dir
        return true;
    }
    return false;
}

// User management methods
bool FileSystem::hasUser(const string& username) const {
    // TODO: Check if user exists in users set, returns true if username exists, otherwise false
    if (users.count(username) == 0) {
        return false;
    }
    else {
        return true;
    }
}

bool FileSystem::registerUser(const string& username) {
    // TODO： Check if user exists in users set, return true if register successfully, otherwise false
    try {
        if (hasUser(username)) {
            throw runtime_error("The user already exits!");
        }
        users.insert(username);
        return true;
    }
    catch (exception& e) {
        cout << e.what();
        return false;
    }
    
}

// helper function,get the dir or file with this path;
FileObj* FileSystem::resolvePath(const string& path,string type,bool relative) {
    // TODO: resolve path, you can use strtok() in c library or istringstream with getline() in c++
    // return target of FileObj* if resolve successfully, otherwise nullptr
    /*    istringstream ins(path);
    string token="";
    getline(ins, token, '\\');
    while (getline(ins, token, '\\')) {
          
    }
    这个地方存疑，我的实现可能比较奇怪；
    */

    if (type == "File") {
        string parentDir = "";
        istringstream iss(path);
        string token;
        vector<string> temp;
        while (getline(iss, token, '\\')) {
            temp.push_back(token);
        }
        temp.pop_back();//删除最后一个
        for (int i = 0; i < temp.size(); i++) {
            if (i == 0) {
                parentDir += temp[i];
            }
            else {
                parentDir += "\\" + temp[i];
            }
        }
        Directory* dir = dynamic_cast<Directory*>(resolvePath(parentDir));
        cout << "THE FUNCTION HAS Done Yet!!!\n";
        assert(0);
        
    }
    else {
        string newPath = "";
        if (relative) {
            newPath= cur->getPath() +"\\" + path;
        }
        else {
             newPath = path;
        }

            if (config_table.count(newPath + " " + type) == 0) {
                cout << RED<<"ERROR: the path do not exists\n"<<RESET;
                return nullptr;
            }
            else {
                uint64_t inode = config_table[newPath + " " + type];
                if (inode == 1) {
                    return root;
                }
                return inodeToPointer(inode);
            }
    }
    return nullptr;
}

void FileSystem::display() {
    cout << "----------FILE-SYSTEM--------------\n";
    cout << "Users:[ ";
    for (string user : users) {
        cout << user<<" ,";
    }
    cout << "]\n";
    cout << "Current User: " << username << endl;
    root->display();
    cout << "---------------------------------\n";
}

