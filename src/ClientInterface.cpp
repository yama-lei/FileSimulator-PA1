#include "ClientInterface.h"
using namespace std;
ClientInterface::ClientInterface(const string& username, FileSystem* filesystem)
    : filesystem(filesystem), username(username) {
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
    cout << "NOW execue the commands:  ";
    for (string c : cmd) {
        cout << c << " ";
    }
    cout << "\n";
    string first = cmd[0];
    uint64_t len = cmd.size();
    if (first == "create") {
        if (len != 2) {
            cout << "ERROR: create a file by command: create <filename> /n";
            return false;
        }
        else {
            createFile(cmd[1]);
            return true;
        }
    }
    else if(first=="delete"){
        if (len != 2) {
            cout << "ERROR: delete a file by command: delete <filename> /n";
            return false;
        }
        else {
            deleteFile(cmd[1]);
            return true;
        }
    }
    else if(first=="read") {
        if (len != 2) {
                cout << "ERROR: read a file by command: read <filename> /n";
                return false;
            }
            Directory* dir = filesystem->getCurrentDir();
            uint64_t inode = filesystem->search(cmd[1], "File");
            dynamic_cast<File*>(dir->getChild(inode))->read();
            return true;
    }
    else if (first == "write") {
        if (len != 2) {
                cout << "ERROR: write a file by command: write <filename> /n";
                return false;
           }
           Directory* dir = filesystem->getCurrentDir();
           string data;
           getline(cin, data);
           uint64_t inode = filesystem->search(cmd[1], "File");
           return dynamic_cast<File*>(dir->getChild(inode))->write(data);
     }
    else if(first=="mkdir") {
        if (len != 2) {
            cout << "ERROR: make a new dir by command: mkdir <dirname> /n";
            return false;
        }
        return createDir(cmd[1]);
    }
    else if (first == "rmdir") {
        if (len != 1) {
            if (cmd[1] == "-r") {
                if (len != 3) {
                    cout << "ERROR: remove the dir by command:  rmdir [-r] <dirname> \n";
                }

                deleteDir(cmd[2], true);
            }
            else {
                deleteDir(cmd[1], false);
            }
        }
        else {
            cout << "ERROR: remove the dir by command:  rmdir [-r] <dirname> \n";
        }
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
         << "  quit                     - Exit program\n" << std::endl;
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
        cout << "ERROR: the name of the file shouldn't contain '\\'\n";
        return false;
    }
}

bool ClientInterface::deleteFile(const string& name) {
    // TODO: Delete file with given name, returns true if file deleted successfully
    // note 1: use filesystem to delete file
    return filesystem->deleteFile(name);
}

string ClientInterface::readFile(const string& name) {
    // TODO: Read content from file with given name, returns file content as string
    // note 1: search file by name
    // note 2: cast to File type and read content

    fprintf(stderr, "Error: ClientInterface::readFile() is not implemented yet!\n");
    assert(0);
    return "";
}

bool ClientInterface::writeFile(const string& name, const string& data) {
    // TODO: Write data to file with given name, returns true if write successful
    // note 1: search file by name
    // note 2: process quoted strings
    // note 3: cast to File type and write content

    fprintf(stderr, "Error: ClientInterface::writeFile() is not implemented yet!\n");
    assert(0);
    return false;
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

bool ClientInterface::changeDir(const string& path) {
    // TODO: Change current directory to given path, returns true if directory changed successfully
    // note 1: resolve path to get target directory
    // note 2: validate target is directory type to set

    fprintf(stderr, "Error: ClientInterface::changeDir() is not implemented yet!\n");
    assert(0);
    return false;
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
