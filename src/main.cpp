#include "ClientInterface.h"
#include "VFS.h"
#include "Directory.h"
#include "File.h"
#include "FileObj.h"
int main() {
    //VFS vfs;
    //vfs.run();
    Directory root("root", "yama", 1, nullptr);
    Directory dir("dir","yama",2,nullptr);
    root.add(&dir);
    root.display();
    return 0;
}