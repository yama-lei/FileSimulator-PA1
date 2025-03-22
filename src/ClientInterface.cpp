#include "ClientInterface.h"
#define RESET   "\033[0m"    // 重置颜色
#define RED     "\033[31m"   // 红色前景
#define GREEN   "\033[32m"   // 绿色前景
using namespace std;
ClientInterface::ClientInterface(const string& username, FileSystem* filesystem)
    : filesystem(filesystem), username(username) {
    tempFile = nullptr;
    // no change
}

std::vector<string> ClientInterface::parseCommand(const string& cmdLine) {
    // TODO: Parse command line into vector of arguments, returns vector of parsed arguments
    // note 1: split by whitespace, you can use strtok() in c or istringstream in c++ to get tokens
    // note 2: handle quote string in two bounds for per token
    istringstream iss(cmdLine);
    string token = "";
    vector<string> commands;
    while (getline(iss, token, ' ')) {
        commands.push_back(token);
    }
    return commands;
}

bool ClientInterface::execueCommand(const std::vector<string>& cmd) {
    // TODO: Execute parsed command, returns true if command executed successfully
    // note 1: check first argument for command type
    // note 2: validate number of arguments
    // note 3: call corresponding operation method
    string first = cmd[0];
    uint64_t len = cmd.size();
    if (first == "create") {
        if (len == 1) {
            cout <<RED<<"ERROR: create a file by command: create <filename> \n"<<RESET;
            return false;
        }
        else {
            for (int i = 1; i < len; i++) {
                createFile(cmd[i]);
            }
            return true;
        }
    }
    else if(first=="delete"){
        if (len != 2) {
            cout <<RED<<"ERROR: delete a file by command: delete <filename> \n"<<RESET;
            return false;
        }
        else {
            deleteFile(cmd[1]);
            return true;
        }
    }
    else if(first=="read") {
        if (len == 1) {
                cout <<RED<<"ERROR: read a file by command: read <filename> \n"<<RESET;
                return false;
            }
            Directory* dir = filesystem->getCurrentDir();
            for (int i = 1; i < len; i++) {
                string content = readFile(cmd[i]);
                if(content!="ERROR"){
                    cout << "===== " << cmd[i] << " =====\n";
                    if (content != "") {
                        cout << content << endl;
                    }
                }
            }
            return true;
    }
    else if (first == "write") {
        //There are two modes of writing!
        if (len == 1) {
                cout <<RED<<"ERROR: write a file by command: write <filename>\n"<<RESET;
                return false;
           }

        if (len == 2) {
            //自己加的写作模式
            Directory* dir = filesystem->getCurrentDir();
            uint64_t inode = filesystem->search(cmd[1], "File");
            if (inode == 0) {
                cout << RED << "ERROR: no such file in the current dir!\n" << RESET;
                return false;
            }
            string data;
            cout << "---Type your text in the command line---\n";
            getline(cin, data);
            return dynamic_cast<File*>(dir->getChild(inode))->write(data);
        }
        else {
            if (cmd[2][0] != '\"' || cmd.back().back() != '\"') {
                cout << RED << "ERROR: The writing content should be quoted with a pair of \"\"!\n" << RESET;
            }
            string data = "";
            for (int i = 2; i < cmd.size(); i++) {
                data =data+ cmd[i]+" ";
            }
            return writeFile(cmd[1], data);
        }
     }
    else if(first=="mkdir") {
        if (len != 2) {
            cout <<RED<<"ERROR: make a new dir by command: mkdir <dirname> \n"<<RESET;
            return false;
        }
        return createDir(cmd[1]);
    }
    else if (first == "rmdir") {
        if (len != 1) {
            if (cmd[1] == "-r") {
                if (len != 3) {
                    cout <<RED<<"ERROR: remove the dir by command:  rmdir [-r] <dirname> \n"<<RESET;
                    return false;
                }

                deleteDir(cmd[2], true);
                return true;
            }
            else {
                deleteDir(cmd[1], false);
                return true;
            }
            
        }
        else {
            cout <<RED<<"ERROR: remove the dir by command:  rmdir [-r] <dirname> \n"<<RESET;
            return false;
        }
    }
    else if (first == "cd") {
        if (len == 2) {
            istringstream iss(cmd[1]);
            string token;
            vector<string> pathToken;
            while (getline(iss, token, '\\')) {
                pathToken.push_back(token);
            }
            return changeDir(pathToken);
            //统一走这个函数化简路径
        }
        else {
            cout <<RED<<"ERROR: use command cd <path> to change current dir!\n"<<RESET;
            return false;
        }
    }
    else if (first == "ls") {
        if (len ==1) {
            if (username == "root") {
                Directory* dir = filesystem->getCurrentDir();
                for (auto son : dir->getAll()) {
                    cout << son->getName() << " \towner:" << son->getOwner() << endl;
                }
            }  
            else {
                Directory* dir = filesystem->getCurrentDir();
                for (auto son : dir->getAll()) {
                    cout << son->getName() << endl;
                }
            }
            return true;
        }
        else {
            cout <<RED<<"ERROR: use command ls to list all the items in the current dir\n"<<RESET;
            return false;
        }
    }
    else if (first == "pwd") {
        cout << filesystem->getCurrentPath() << endl;
    }
    else if (first == "whoami") {
        cout << "The current user:  " << filesystem->getUserName() << endl;
    }
    else if (first=="clear") {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    } 
    else if (first == "help") {
        showHelp();
    }
    else if (first == "exit"||first=="quit") {
        //exit and quit has been done in the VFS.cpp, but it sholdn't be recognize as an Invalid Command.
    }
    else if (first=="ctrl") {
        if (len == 1 || len >= 4) {
            cout <<RED<<"ERROR: use ctrl c <name> or ctrl v to cv your files\n"<<RESET;
            return false;
        }
        if (cmd[1] == "c") {
            if (len != 3) {
                cout <<RED<<"ERROR: use ctrl c <name> to copy a file!\n"<<RESET;
                return false;
            }
            string name = cmd[2];
            //默认是文件
            uint64_t inode = filesystem->search(cmd[2], "File");
            if (inode != 0) {
                tempFile = filesystem->getCurrentDir()->getChild(inode);
                return true;
            }
            else {
                inode = filesystem->search(cmd[2], "Directory");
                if (inode != 0) {
                    tempFile = filesystem->getCurrentDir()->getChild(inode);
                    return true;
                    }
                return false;
                }
        }
        else if (cmd[1] == "v") {
            if (len != 2) {
                cout <<RED<<"ERROR: use ctrl c <name> to copy a file!\n"<<RESET;
                return false;
            }
            if (tempFile == nullptr) {
                cout << RED <<"ERROR: use command ctrl c <name> to copy a file or dir first!\n" << RESET;
            }  
            else {
                if (tempFile->getType() == "File") {
                    //复制file
                    filesystem->createFile(dynamic_cast<File*>(tempFile));
                }
                else {
                    //复制directory,注意这里是deepcopy
                    filesystem->createDir(dynamic_cast<Directory*>(tempFile));
                }
            }
        }
        else {
            cout <<RED<<"ERROR: use ctrl c  <name> or ctrl v to cv your files\n"<<RESET;
        }

    }
    else if (first == "tree") {
        if (len != 1) {
            cout << RED << "ERROR: use tree to show the structure of the filesystem\n" << RESET;
        }
        filesystem->showTree();
    }

    else {
        cout <<RED<<"ERROR: Unkonwn command!\n"<<RESET;
    }
}

void ClientInterface::processCommand(const string& cmdLine) {
    // TODO: Process user input command line, no return value
    // note 1: parse command line
    // note 2: execute command
    // note 3: handle exceptions  
    vector<string> cmds = parseCommand(cmdLine);
    execueCommand(cmds);

}

void ClientInterface::showHelp() const {
    // Show help message with all available commands and their usage, no return value
    std::cout << "Available commands:\n"
        << "  create <filename...>      - Create one or more new files\n"
        << "  delete <filename...>      - Delete one or more files\n"
        << "  read <filename...>        - Read content from one or more files\n"
        << "  write <filename> <text>   - Write text to file (supports '\\n' for newline)\n"
        << "  mkdir <dirname>           - Create a new directory\n"
        << "  rmdir [-r] <dirname>      - Remove directory (-r for recursive deletion)\n"
        << "  cd <path>                - Change directory (supports relative/absolute paths)\n"
        << "  ls                       - List current directory contents\n"
        << "  pwd                      - Show current working directory\n"
        << "  whoami                   - Show current user name\n"
        << "  clear                    - clear current command line\n"
        << "  help                     - Show this help message\n"
        << "  exit                     - Logout current user\n"
        << "  quit                     - Exit program\n" << std::endl
        << "-----------------------------new commands:-------------------------------\n"
        << "  ctrl c <name>            * To copy a file or directory\n"
        << "  ctrl v                   * To paste the copied file or directory in the current directory\n"
        << "  cd ..                    * To get to the parent dir\n"
        << "  tree                     * To get the tree structure of this file\n"
        << "  write <filename>         * Enter the writing mode which is more flexible and convient!\n";
}


bool ClientInterface::createFile(const string& name) {
    // TODO: Create new file with given name, returns true if file created successfully
    // note 1: validate file name
    // note 2: use filesystem to create file
    //remember to set the owner of the file!!!!!!! Because the FileSystem don't take this jobs!
    //note by 3/19: the sentence above is wrong!
    if (name.find('\\')==string::npos) {
        filesystem->createFile(name);
    }
    else {
        cout <<RED<<"ERROR: the name of the file shouldn't contain '\\'\n"<<RESET;
        return false;
    }
}

bool ClientInterface::deleteFile(const string& name) {
    // TODO: Delete file with given name, returns true if file deleted successfully
    // note 1: use filesystem to delete file
    return filesystem->deleteFile(name);
}
//Get the content of file with name as input
string ClientInterface::readFile(const string& name) {
    // TODO: Read content from file with given name, returns file content as string
    // note 1: search file by name
    // note 2: cast to File type and read content
    uint64_t inode = filesystem->search(name, "File");
    if (inode == 0) {
        cout << RED << "ERROR: the file " << name << " do not exists!\n" << RESET;
        return "ERROR";
    }
    File* file = dynamic_cast<File*>(filesystem->getCurrentDir()->getChild(inode));
    if(file==nullptr){
        return "ERROR";
    }
    string content = file->read();
    return content;
}

bool ClientInterface::writeFile(const string& name, const string& data) {
    // TODO: Write data to file with given name, returns true if write successful
    // note 1: search file by name
    // note 2: process quoted strings
    // note 3: cast to File type and write content
    string contentWithOutQuote = "";//去掉首尾的引号和最后多加的空格
    for (int i = 1; i < data.size() - 2; i++) {
        contentWithOutQuote += data[i];

    }
    Directory* dir = filesystem->getCurrentDir();
    uint64_t inode = filesystem->search(name, "File");
    if (inode == 0) {
        cout << RED << "ERROR: no such file in the current dir!\n" << RESET;
        return false;
    }
    return  dynamic_cast<File*>(dir->getChild(inode))->write(contentWithOutQuote);
}

bool ClientInterface::createDir(const string& name) {
    // TODO: Create new directory with given name, returns true if directory created successfully
    // note 1: use filesystem to create directory
    FileObj* ptr = filesystem->createDir(name);
    if (ptr != nullptr) {
        return true;
    }
    return false;
}

bool ClientInterface::deleteDir(const string& name, bool recursive) {
    // TODO: Delete directory with given name, returns true if directory deleted successfully
    // note 1: use filesystem to delete directory with recursive flag
    return   filesystem->deleteDir(name,filesystem->getUserName(), recursive);
}

bool ClientInterface::changeDir(const vector<string> pathToken) {
    if (pathToken.size() == 0) {
        return true;
    }
    vector<string> newPathToken;
    for (int i = 1; i < pathToken.size(); i++) {
        newPathToken.push_back(pathToken[i]);
    }
    if (pathToken[0] == "..") {
        auto parent = filesystem->getCurrentDir()->getParent();
        //如果在根路径下，cd ..应该不报错
        if (parent != nullptr) {
            filesystem->setCurrentDir(dynamic_cast<Directory*>(parent));
        }
        //目录往上面走一级
        return changeDir(newPathToken);
    }
    else if (pathToken[0] == ".") {
        return changeDir(newPathToken);
    }
    else {
        uint64_t inode=filesystem->search(pathToken[0],"Directory");
        if (inode == 0) {
            cout << RED << "ERROR: no such file in the current dir!\n" << RESET;
            }
        else {
            filesystem->changeDir(inode);
            return changeDir(newPathToken);
        }
    }
}

//警告，自从3.21重写了上面那个更加优雅的函数之后，这个函数就不在使用！！！但是这个函数仍能正确执行功能，即，根据path，更改对应的路径
    bool ClientInterface::changeDir(const string& path) {
    // TODO: Change current directory to given path, returns true if directory changed successfully
    // note 1: resolve path to get target directory
    // note 2: validate target is directory type to set
    // 注意，我cd ..命令走的不是这个函数
    istringstream iss(path);
    string token = "";
    getline(iss, token, '\\');
    if (token == "root") {
        //这个地方所谓root指的是我自己加的根路径
        filesystem->changeDir(filesystem->resolvePath(path)->getInode());
        return true;
    }
    else {
        FileObj* fb = filesystem->resolvePath(path, "Directory", true);
        if (fb == nullptr) {
            return false;
        }
        else {
            filesystem->changeDir(fb->getInode());
            return true;
        }
    }

    //还未实现cd ../../之类的指令
}

void ClientInterface::listCurrentDir() {
    // TODO: List all contents in current directory, no return value
    // note 1: print each child's name in current directory per line
    filesystem->display();
}

string ClientInterface::getCurrentPath() const {
    // TODO: Get current working directory path, returns current path as string
    // note 1: use filesystem to get current path

    return filesystem->getCurrentPath();
}

string ClientInterface::search(const string& name, const string& type) {
    // TODO: Search file or directory by name, returns formatted result string
    // note 1: use filesystem to search by name
    uint64_t inode=filesystem->search(name, type);
    FileObj* obj=filesystem->getCurrentDir()->getChild(inode);
    return obj->getName() + " " + obj->getType();
}
