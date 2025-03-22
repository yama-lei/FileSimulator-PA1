#include "ClientInterface.h"
#include "VFS.h"
#include "Directory.h"
#include "File.h"
#include "FileObj.h"

void testClientInterface();

int main() {
    VFS vfs;
    vfs.run();
    return 0;
}

void testFileAndDirectory() {
        File file("file","File","Yama",1,nullptr);
    file.write("HELLO \\n");
    file.read();
    cout << "----\n";
    FileObj* objs[10];
    Directory dir("root", "Yama", 1, nullptr);
    objs[0] = new File("README.md", "File", "Yama", 2, &dir);
    objs[1] = new Directory("subDirectory", "Yama", 3, &dir);
    dir.add(objs[0]);
    dir.add(objs[1]);
    dir.display();
}


void testClientInterface() {
    ClientInterface ci("yama", new FileSystem("yama", InodeFactory::generateInode()));
    ci.showHelp();
    ci.listCurrentDir();
    ci.processCommand("create test.md");
    ci.listCurrentDir();
    ci.processCommand("mkdir dir");
    ci.listCurrentDir();
    ci.processCommand("ls");
    ci.processCommand("ls");
    ci.processCommand("create file.md");
    ci.processCommand("ls");
    ci.processCommand("cd C:");
    ci.processCommand("pwd");
    ci.processCommand("cd C:\\dir");
    ci.processCommand("pwd");
}