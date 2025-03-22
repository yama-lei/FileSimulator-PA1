#define RESET   "\033[0m"    // 重置颜色
#define RED     "\033[31m"   // 红色前景
#define GREEN   "\033[32m"   // 绿色前景
#include "VFS.h"
using namespace std;
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
            cout << "Now login......\n";
            _sleep(1000);
        }
    else {
        cout << "Please input your password: \n";
        string password;
        getline(cin, password);
        if (password == filesystem->getUserPassword(username)) {
            cout << GREEN << "Correct!\n" << RESET;
            cout << "Now login......\n";
            _sleep(500);
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
    _sleep(500);
    cout << "Now login......\n";
    _sleep(1000);
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
                throw runtime_error("Empty command! Use  help to check valid commands!");
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
        std::cout << "1. Login\n2. Register\n3. Exit\nPlease choose (1-3): ";
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
