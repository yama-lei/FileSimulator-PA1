#define RESET   "\033[0m"    // 重置颜色
#define RED     "\033[31m"   // 红色前景
#define GREEN   "\033[32m"   // 绿色前景
#include "VFS.h"
using namespace std;

//show new features
void showNewCommands() {
    cout << "In this project, I added some commands as follows:\n"
        << "-----------------------------new commands:-------------------------------\n"
        << "  ctrl c <name>            * To copy a file or directory\n"
        << "  ctrl v                   * To paste the copied file or directory in the current directory\n"
        << "  cd ..                    * To get to the parent dir\n"
        << "  tree                     * To get the tree structure of this file\n"
        << "  write  <filename>        * Enter the writing mode which is more flexible and convient!\n"
        << "  echo   <content>         * Repeat what the user input in the command                  \n"
        << "  users                    * List all the users of the FileSystem.Passwords will be shown if 'root' use this command\n"
        << "  reset password <name>    * ROOT ONLY! Reset the pw of any user.Error will occur if the user is not 'root'\n";
}
void loginAnimation() {
    cout << "Wait a second to login......\n";
    for (int i = 0; i < 20; i++) {
        cout << "[";
        for (int j = 0; j < i; j++) {
            cout << "=";
        }
        for (int j = 0; j < 20 - i; j++) {
            cout << " ";
        }
        cout << "]\r";
        _sleep(65);
    }
}
VFS::VFS() {
    filesystem = new FileSystem("root", InodeFactory::generateInode());
}

VFS::~VFS() {
    delete filesystem;
}

void VFS::printWelcome() const {
    std::cout << "File System Simulator Started" << std::endl;
    std::cout << "Please login with your username" << std::endl;
}

void VFS::printPrompt(const string& username, const string& path) const {
    std::cout << username << "@FileSimulator:" << path << "$ ";
}

void VFS::handleLogin(string& username) {
    std::cout << "===== User Login =====\n";
    cout << "Please input the name of user\n";
    std::getline(std::cin, username);
    if(!filesystem->hasUser(username)) {
        throw std::runtime_error("User does not exist. Please register first.");
    }
    if (username == "root"&& filesystem->getUserPassword("root") == "password") {
            //root初次登录
            cout << "The default password for root user is 'password', please reset your password\n";
            cout << "Set your new password: \n";
            string newPass = "";
            getline(cin, newPass);
            while (!filesystem->setUserPassword("root", newPass)) {
                cout << "Input your new password: \n";
                getline(cin, newPass);
            }
            cout << GREEN << "Success! your new password is " << newPass << endl << RESET;
            loginAnimation();
        }
    else {
        cout << "Please input your password: \n";
        string password;
        getline(cin, password);
        if (password == filesystem->getUserPassword(username)) {
            cout << GREEN << "Correct!\n" << RESET;
            loginAnimation();
        }
        else {
            throw std::runtime_error("Wrong Password. Please Try again or login in as root account to reset your password!.");
        }
    }
}

void VFS::handleRegister(string& username) {
    std::cout << "===== Register new user ===== \n";
    cout << "Please input desired username \n";
    std::getline(std::cin, username);
    cout << "Please input your password \n";
    string password;
    std::getline(std::cin, password);
    if(filesystem->hasUser(username)) {
        throw std::runtime_error("Username already exists. Please try another one.");
    }
    if(!filesystem->registerUser(username,password)) {
        throw std::runtime_error("Failed to register user.");
    }
    std::cout <<GREEN<< "User registered successfully!\n"<<RESET;
    loginAnimation();
}

void VFS::processUserCommands(const string& username) {
    ClientInterface client(username, filesystem);
    string command;
    
    while(true) {
        printPrompt(username, filesystem->getCurrentPath());
        std::getline(std::cin, command);

        if(command == "exit" || command == "quit") {
            break;
        }

        try {
            if (command == "") {
                continue;
            }

            client.processCommand(command);
        } catch(const std::exception& e) {
            std::cout << RED<<"Error: " << e.what() <<RESET<< std::endl;
        }
    }
}

void VFS::run() {
    string username, command;
    bool running = true;

    while(running) {
        // Clear screen first
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        std::cout << "============================\n";
        std::cout << "  File System Simulator\n";
        std::cout << "============================\n";
        std::cout << "1. Login\n2. Register\n3. Exit\n4. Show new commands\nPlease choose (1-3): ";
        std::string choice;
        std::getline(std::cin, choice);

        try {
            if(choice == "3" || choice == "exit" || choice == "quit" || choice == "q") {
                running = false;
                continue;
            }

            // Clear screen second
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif

            if(choice == "1" || choice == "login" || choice == "l" || choice == "L") {
                handleLogin(username);
            }
            else if(choice == "2" || choice == "register" || choice == "r" || choice == "R") {
                handleRegister(username);
            }
            else if (choice == "4" || choice == "check") {
                showNewCommands();
                std::cout << "Press Enter to continue...";
                std::cin.get();
                continue;
            }
            else {
                std::cout << RED<<"Invalid choice. Please try again.\n"<<RESET;
                std::cout << "Press Enter to continue...";
                std::cin.get();
                continue;
            }

            if(!filesystem->setUser(username)) {
                std::cout << RED<<"Failed to set user: " << username << RESET<<std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.get();
                continue;
            }

            // Clear screen third
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif

            processUserCommands(username);

        } catch(const std::exception& e) {
            std::cout << RED<<"Error: " << e.what() << RESET<<std::endl;
            std::cout <<"Press Enter to continue...";
            std::cin.get();
        }
    }

    std::cout << "\nThank you for using File System Simulator!\nBye!" << std::endl;
}
