#include "ClientInterface.h"
#include "VFS.h"
#include "Directory.h"
#include "File.h"
#include "FileObj.h"

void testClientInterface();
void testFileSystem();

int main() {
    //VFS vfs;
    //vfs.run();
    testClientInterface();
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

void testFileSystem() {
    FileSystem fs("root", InodeFactory::generateInode());
    fs.registerUser("yama");
    fs.setUser("yama");
    Directory* dir = fs.createDir("Dir1");
    fs.registerUser("Jam");
    File* fr = fs.createFile("File1");
    dir->add(fr);
    fs.setUser("Tam");
    fs.setUser("Jam");
    fs.createDir("dir2");
    fs.createFile("file3");
    fs.createFile("file4");
    fs.display();
    fs.changeDir(fs.search("dir2", "Directory"));
    fs.createFile("test");
    fs.display();
    fs.deleteFile("test");
    fs.display();
}

void testClientInterface() {
    ClientInterface ci("yama", new FileSystem("yama", InodeFactory::generateInode()));
    ci.showHelp();
    ci.listCurrentDir();
    ci.processCommand("create test.md");
    ci.listCurrentDir();
    ci.processCommand("write test.md");
    ci.processCommand("read test.md");
    ci.processCommand("mkdir dir");
    ci.listCurrentDir();
    ci.processCommand("rmdir dir");
    ci.listCurrentDir();
}