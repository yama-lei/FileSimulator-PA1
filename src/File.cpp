#include "File.h"
#include<string>
using namespace std;

File::File(const string& name, const string& type, const string& owner, const uint64_t& inode, FileObj* parent)
    :FileObj(name, (parent ? parent->getPath() + name + "/" : "/"), type, owner, inode, parent), content("") {
}

// read and write
string File::read() const {
    // TODO: return file content
    // note 1: you can choose to printf content into stdout in this function or not (Optional)
    cout << content;
    return content;
}

bool File::write(const string &data) { 
    //TODO: write data into file content, return true if write successfully, otherwise false
    // note 1: replace "\\n" into new line "\n"
    // for example, when you wrote "Hello PA-1\n" in command line, data will be "Hello PA-1\\n"
    content = data;
    int index = content.find("\\n");
    while (index != string::npos) {
        content.replace(index, 2, "\n");
        index = content.find("\\n", index);
    }
    return false;
}

string File::getContent() const {
    // TODO: retun file content
    return content;
}

// helper function
void File::display(size_t indent) const {
    for (size_t i = 0; i < indent; i++)cout << " ";
    std::cout << "[File: " << getName() << "] Size: " << content.length() << " bytes" << std::endl;
}
