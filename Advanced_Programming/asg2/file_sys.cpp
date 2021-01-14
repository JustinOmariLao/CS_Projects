// $Id: file_sys.cpp,v 1.7 2019-07-09 14:05:44-07 - - $

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>
using namespace std;

#include "debug.h"
#include "file_sys.h"
#include "commands.h"
int inode::next_inode_nr{ 1 };

//************ Misc Functions ************//

struct file_type_hash {
    size_t operator() (file_type type) const {
        return static_cast<size_t> (type);
    }
};

ostream& operator<< (ostream& out, file_type type) {
    static unordered_map<file_type, string, file_type_hash> hash{
       {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
       {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
    };
    return out << hash[type];
}

ostream& operator<< (ostream& out, const inode_state& state) {
    out << "inode_state: root = " << state.root
        << ", cwd = " << state.cwd;
    return out;
}

file_error::file_error(const string& what) :
    runtime_error(what) {
}

void lsr(inode_ptr& dir) {
    map<string, inode_ptr> dirents = dir->contents->get_contents();
    print_lsr(dir);
    for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
        cout << setw(6) << i->second->get_inode_nr() << "  " 
            << setw(6) << i->second->contents->size() << "  ";
        if (i->first.compare(".") == 0 || i->first.compare("..") == 0) {
            cout << i->first << "/" << endl;
        }
        else {
            cout << i->first << endl;
        }
    }
    for (auto i = dirents.begin(); i != dirents.end(); ++i) {
        if (i->first.compare(".") == 0 ||
            i->first.compare("..") == 0) {}
        else {
            if (i->second->contents->is_dir()) {
                lsr(i->second);
            }
        }
    }
}

void print_lsr(const inode_ptr& curr_dir) {
    vector<string> path;
    path.push_back(curr_dir->get_name());
    map<string, inode_ptr> dirents =
        curr_dir->contents->get_contents();
    inode_ptr parent = dirents.at("..");
    while (parent->get_inode_nr() > 1) {
        path.push_back(parent->get_name());
        map<string, inode_ptr> dirents =
            parent->contents->get_contents();
        parent = dirents.at("..");
    }
    for (auto i = path.cend() - 1; i != path.cbegin() - 1; --i) {
        wordvec output_text = split(*i, "/");
        cout << "/" << output_text;
    }
    cout << ":" << endl;
}

//************ Inode State Functions ************//
inode_state::inode_state() {
    root = make_shared<inode>(file_type::DIRECTORY_TYPE);
    cwd = root;
    parent = root;
    root->contents->set_dir(cwd, parent);
    root->set_name("/");
    DEBUGF('i', "root = " << root << ", cwd = " << cwd
        << ", prompt = \"" << prompt() << "\"");
}

inode_state::~inode_state() {
    map<string, inode_ptr> base_dir = root->contents->get_contents();
    base_dir.clear();
    cwd = nullptr;
    parent = nullptr;
    root = nullptr;
}

// Shows the prompt character in console.
const string& inode_state::prompt() { return prompt_; }

void inode_state::print_path(const inode_ptr& curr_dir) const {
    vector<string> path;
    path.push_back(curr_dir->get_name());
    map<string, inode_ptr> dirents = 
        curr_dir->contents->get_contents();
    inode_ptr parent = dirents.at("..");
    while (parent->get_inode_nr() > 1) {
        path.push_back(parent->get_name());
        map<string, inode_ptr> dirents = 
            parent->contents->get_contents();
        parent = dirents.at("..");
    }
    for (auto i = path.cend() - 1; i != path.cbegin() - 1; --i) {
        wordvec output_text = split(*i, "/");
        cout << "/" << output_text;
    }
    cout << endl;
}

// Prints the directory after being called by ls and lsr.
void inode_state::print_dir
(const inode_ptr& curr_dir, const wordvec& args) const {
    map<string, inode_ptr> dirents = curr_dir->contents->get_contents();
    if (args.size() == 1) {
        wordvec output_name = split(curr_dir->get_name(), "/");
        cout << "/" << output_name << ":" << endl;
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            cout << setw(6) << i->second->get_inode_nr()
                << "  " << setw(6)
                << i->second->contents->size() << "  ";
            if (i->first.compare(".") == 0 || 
                    i->first.compare("..") == 0) {
                cout << i->first << "/" << endl;
            }
            else {
                cout << i->first << endl;
            }
        }
    }
    else {
        wordvec path_name = split(args.at(1), "/");
        inode_ptr ls_dir = curr_dir; bool dir_found = false;
        for (size_t i = 0; i < path_name.size(); ++i) {
            dir_found = false;
            for (auto j = dirents.cbegin(); j != dirents.cend(); ++j) {
                if (j->first == path_name.at(i) + "/") {
                    ls_dir = j->second;
                    dir_found = true;
                }
                else if (path_name.at(0) == j->first) {
                    ls_dir = j->second;
                    dir_found = true;
                    break;
                }
            }
            if (dir_found == false) {
                throw command_error("ls: invalid path");
            }
            dirents = ls_dir->contents->get_contents();
        }
        if (args.at(1) == "." || args.at(1) == "..") {
            cout << args.at(1) << ":" << endl;
        }
        else {
            string name_fix = ls_dir->get_name();
            name_fix.pop_back();
            name_fix = "/" + name_fix;
            cout << name_fix << ":" << endl;
        }
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            cout << setw(6) << i->second->get_inode_nr()
                << "  " << setw(6)
                << i->second->contents->size() << "  ";
            if (i->first.compare(".") == 0 || 
                    i->first.compare("..") == 0) {
                cout << i->first << "/" << endl;
            }
            else {
                cout << i->first << endl;
            }
        }
    }
}

void inode_state::list_recursively
(inode_state& curr_state, const wordvec& args) {
    inode_ptr lr;
    map<string, inode_ptr> dirents;
    if (args.size() == 1) {
        lr = curr_state.get_cwd();
        dirents = lr->contents->get_contents();
        wordvec outputName = split(lr->get_name(), "/");
        cout << "/" << outputName << ":" << endl;
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            cout << setw(6) << i->second->get_inode_nr()
                << "  " << setw(6)
                << i->second->contents->size() << "  ";
            if (i->first.compare(".") == 0 || 
                    i->first.compare("..") == 0) {
                cout << i->first << "/" << endl;
            }
            else {
                cout << i->first << endl;
            }
        }
        for (auto i = dirents.begin(); i != dirents.end(); ++i) {
            if (i->first.compare(".") == 0 ||
                i->first.compare("..") == 0) {}
            else {
                if (i->second->contents->is_dir()) {
                    lsr(i->second);
                }
            }
        }
    }
    else {
        wordvec path_name = split(args.at(1), "/");
        string path = args.at(1);
        path = path.at(0);
        if (path == "/")
            lr = root;
        else
            lr = curr_state.get_cwd();
        bool dir_found = false;
        dirents = lr->contents->get_contents();
        for (size_t i = 0; i < path_name.size(); ++i) {
            dir_found = false;
            for (auto j = dirents.cbegin(); j != dirents.cend(); ++j) {
                if (j->first == path_name.at(i) + "/") {
                    lr = j->second;
                    dir_found = true;
                }
            }
            if (dir_found == false) {
                throw command_error("lsr: invalid path");
            }
            dirents = lr->contents->get_contents();
        }
        wordvec outputName = split(lr->get_name(), "/");
        cout << "/" << outputName << ":" << endl;
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            cout << setw(6) << i->second->get_inode_nr()
                << "  " << setw(6)
                << i->second->contents->size() << "  ";
            if (i->first.compare(".") == 0 || 
                    i->first.compare("..") == 0) {
                cout << i->first << "/" << endl;
            }
            else {
                cout << i->first << endl;
            }
        }
        for (auto i = dirents.begin(); i != dirents.end(); ++i) {
            if (i->first.compare(".") == 0 ||
                i->first.compare("..") == 0) {}
            else {
                if (i->second->contents->is_dir()) {
                    lsr(i->second);
                }
            }
        }
    }
}

void inode_state::create_file
(const inode_ptr& curr_dir, const wordvec& words) const {
    wordvec path_name = split(words.at(1), "/");
    map<string, inode_ptr> dirents = curr_dir->
        contents->get_contents();
    inode_ptr mk_file = curr_dir;
    bool dir_found = false;
    inode_ptr same_file = nullptr;
    bool file_match = false;
    for (size_t i = 0; i < path_name.size() - 1; ++i) {
        dir_found = false;
        for (auto j = dirents.cbegin(); j != dirents.cend(); ++j) {
            if (j->first == path_name.at(i) + "/") {
                mk_file = j->second;
                dir_found = true;
            }
        }
        if (dir_found == false) {
            throw command_error("create_file: invalid path");
        }
        dirents = mk_file->contents->get_contents();
    }
    for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
        if (i->first == words.at(1) + "/") {
            throw command_error("create_file: "
                "directory has same name");
        }
        else if (i->first == words.at(1)) {
            file_match = true;
            same_file = i->second;
        }
    }
    if (file_match) {
        same_file->contents->writefile(words);
        dirents.insert(pair<string, inode_ptr>
            (same_file->get_name(), same_file));
        mk_file->contents->set_contents(dirents);
    }
    else {
        inode_ptr new_file = mk_file->contents->
            mkfile(path_name.at(path_name.size() - 1));
        new_file->contents->writefile(words);
        dirents.insert(pair<string, inode_ptr>
            (new_file->get_name(), new_file));
        mk_file->contents->set_contents(dirents);
    }
}

void inode_state::cat
(const inode_ptr& curr_dir, const wordvec& words) const {
    for (size_t i = 1; i != words.size(); i++) {
        bool file_found = false;
        map<string, inode_ptr> dirents =
            curr_dir->contents->get_contents();
        for (auto j = dirents.cbegin(); j != dirents.cend(); ++j) {
            if (j->first == words.at(i)) {
                if (j->second->contents->is_dir() == true) {
                    throw command_error("cat: cannot read directories");
                }
                else if (j->second->contents->is_dir() == false) {
                    file_found = true;
                    for (auto k = 
                        j->second->contents->readfile().begin();
                        k != j->second->contents->readfile().end();
                        ++k) {
                        cout << *k << " ";
                    }
                }
                cout << endl;
            }
        }
        if (file_found == false) {
            cout << "cat: " << words.at(1) 
                << ": No such file or directory" 
                << endl;
        }
    }
}

void inode_state::make_dir
(const inode_ptr& curr_dir, const wordvec& path) const {
    wordvec path_name = split(path.at(1), "/");
    map<string, inode_ptr> dirents = curr_dir->
        contents->get_contents();
    inode_ptr mk_dir = curr_dir;
    bool dir_found = false;
    for (size_t i = 0; i < path_name.size() - 1; ++i) {
        dir_found = false;
        for (auto j = dirents.cbegin(); j != dirents.cend(); ++j) {
            if (j->first == path_name.at(i) + "/") {
                mk_dir = j->second;
                dir_found = true;
            }
        }
        if (dir_found == false) {
            throw command_error("mkdir: invalid path");
        }
        dirents = mk_dir->contents->get_contents();
    }
    //Check to see if a dir with that name already exists
    for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
        if (i->first == path_name.at(path_name.size() - 1) || 
            i->first == path_name.at(path_name.size() - 1) + "/") {
            throw command_error
            ("mkdir: a directory already exists with that name");
        }
    }
    inode_ptr new_dir = mk_dir->contents->mkdir
    (path_name.at(path_name.size() - 1));
    new_dir->contents->set_dir(new_dir, mk_dir);
    dirents.insert(pair<string, inode_ptr>
        (new_dir->get_name(), new_dir));
    mk_dir->contents->set_contents(dirents);
}

void inode_state::change_dir
(inode_state& curr_state, const wordvec& args) {
    if (args.size() == 1) {
        cwd = curr_state.get_root();
    }
    else if (args.size() == 2) {
        wordvec path_name = split(args.at(1), "/");
        string path = args.at(1);
        inode_ptr cd;
        
        if (path == "..") {
            map<string, inode_ptr> dirents = 
                cwd->contents->get_contents();
            inode_ptr parent = dirents.at("..");
            cd = parent;
            dirents = cd->contents->get_contents();
            cwd = cd;
        }
        else {
            path = path.at(0);
            if (path == "/") {
                cd = curr_state.get_root();
            }
            else {
                cd = curr_state.get_cwd();
            }
            bool dir_found = false;
            map<string, inode_ptr> dirents = 
                cd->contents->get_contents();
            for (size_t i = 0; i < path_name.size(); ++i) {
                dir_found = false;
                for (auto j = dirents.cbegin(); 
                    j != dirents.cend(); ++j) {
                    if (j->first == path_name.at(i) + "/") {
                        cd = j->second;
                        dir_found = true;
                    }
                }
                if (dir_found == false) {
                    throw command_error("cd: invalid path");
                }
                dirents = cd->contents->get_contents();
            }
            cwd = cd;
        }
    }
    else {
        throw command_error("cd: too many args");
    }
}

// Removes the specified file or directory.
void inode_state::remove(const inode_ptr& curr_dir,
    const wordvec& args) const {
    for (size_t k = 1; k != args.size(); ++k) {
        bool arg_found = false;
        map<string, inode_ptr> dirents =
            curr_dir->contents->get_contents();
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            // Search
            if (i->first == args.at(k) || 
                    i->first == args.at(k) + "/") {
                arg_found = true;
                if (i->second->contents->is_dir() == false || 
                        i->second->contents->size() == 2) {
                    if (i->second->contents->is_dir() == true) {
                        map<string, inode_ptr> dir = 
                            i->second->contents->get_contents();
                        for (auto it = dir.cbegin(); 
                            it != dir.cend(); ++it) {
                            if ((it->first.compare(".") != 0) && 
                                    (it->first.compare(".."))) {
                                throw command_error
                                    ("rm: directory not empty");
                            }

                        }
                        dirents.clear();
                    }
                    curr_dir->contents->get_contents().erase(i->first);
                }
                else if (i->second->contents->is_dir() == true &&
                        i->second->contents->size() > 2) {
                    throw command_error("rm: directory not empty");
                }
            }
        }
        // If there are no matches in the directory's entities, error.
        if (!arg_found) {
            throw command_error("rm: file or directory not found");
        }
    }
}

void inode_state::remove_recursively(inode_state& curr_state,
        const wordvec& args) {
    inode_ptr curr_dir = curr_state.get_cwd();;
    map<string, inode_ptr> dirents = 
        curr_dir->contents->get_contents();;
    bool arg_found = false;
    for (size_t k = 1; k != args.size(); ++k) {
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            // Search
            if (i->first == args.at(k) || 
                    i->first == args.at(k) + "/") {
                arg_found = true;
                if (i->second->contents->is_dir() == false ||
                    i->second->contents->size() == 2) {
                    if (i->second->contents->is_dir() == true) {
                        map<string, inode_ptr> dir = 
                            i->second->contents->get_contents();
                        for (auto it = dir.cbegin(); 
                                it != dir.cend(); ++it) {
                            if ((it->first.compare(".") != 0) && 
                                    (it->first.compare(".."))) {
                                throw command_error
                                    ("rmr: directory not empty");
                            }

                        }
                        dirents.clear();
                    }
                    curr_dir->contents->get_contents().erase(i->first);
                    return;
                }
                else if (i->second->contents->is_dir() == true &&
                    i->second->contents->size() > 2) {
                    throw command_error("rmr: directory not empty");
                }
            }
        }
        // If there are no matches in the directory's entities, error.
        if (arg_found == false) {
            for (auto i = dirents.begin(); i != dirents.end(); ++i) {
                if (i->first.compare(".") == 0 ||
                    i->first.compare("..") == 0) {
                }
                else {
                    if (i->second->contents->is_dir()) {
                        rmr(i->second, args);
                    }
                }
            }
        }
    }
}

void rmr(inode_ptr& curr_dir, const wordvec& args) {
    map<string, inode_ptr> dirents = curr_dir->contents->get_contents();
    bool arg_found = false;
    for (size_t k = 1; k != args.size(); ++k) {
        for (auto i = dirents.cbegin(); i != dirents.cend(); ++i) {
            // Search
            if (i->first == args.at(k) || 
                    i->first == args.at(k) + "/") {
                arg_found = true;
                if (i->second->contents->is_dir() == false ||
                    i->second->contents->size() == 2) {
                    if (i->second->contents->is_dir() == true) {
                        map<string, inode_ptr> dir = 
                            i->second->contents->get_contents();
                        for (auto it = dir.cbegin(); 
                                it != dir.cend(); ++it) {
                            if ((it->first.compare(".") != 0) &&
                                    (it->first.compare(".."))) {
                                throw command_error
                                    ("rmr: directory not empty");
                            }

                        }
                        dirents.clear();
                    }
                    curr_dir->contents->get_contents().erase(i->first);
                }
                else if (i->second->contents->is_dir() == true &&
                    i->second->contents->size() > 2) {
                    throw command_error("rmr: directory not empty");
                }
            }
        }
        // If there are no matches in the directory's entities, error.
        if (arg_found == false) {
            for (auto i = dirents.begin(); i != dirents.end(); ++i) {
                if (i->first.compare(".") == 0 ||
                    i->first.compare("..") == 0) {
                }
                else {
                    if (i->second->contents->is_dir()) {
                        rmr(i->second, args);
                    }
                }
            }
        }
    }
}

//************ Inode Functions ************//
inode::inode(file_type type) : inode_nr(next_inode_nr++) {
    switch (type) {
    case file_type::PLAIN_TYPE:
        contents = make_shared<plain_file>();
        break;
    case file_type::DIRECTORY_TYPE:
        contents = make_shared<directory>();
        break;
    }
    DEBUGF('i', "inode " << inode_nr << ", type = " << type);
}

//************ Plain File Functions ************//
size_t plain_file::size() const {
    size_t size{ 0 };
    size = data.size();
    for (auto word = data.begin();
        word != data.end();
        word++) {
        size += word->size();
    }
    if (size > 1) size -= 1;
    DEBUGF('i', "size = " << size);
    return size;
}

const wordvec& plain_file::readfile() const {
    DEBUGF('i', data);
    return data;
}

void plain_file::writefile(const wordvec& words) {
    wordvec new_data;
    for (size_t i = 2; i < words.size(); ++i) {
        new_data.push_back(words.at(i));
    }
    data = new_data;
    DEBUGF('i', words);
}

void plain_file::remove(const string&) {
    throw file_error("is a plain file");
}

inode_ptr plain_file::mkdir(const string&) {
    throw file_error("is a plain file");
}

inode_ptr plain_file::mkfile(const string&) {
    throw file_error("is a plain file");
}

void plain_file::set_dir(inode_ptr cwd, inode_ptr parent) {
    throw file_error("is a plain file");
}

map<string, inode_ptr>& plain_file::get_contents() {
    throw file_error("is a plain file");
}

void plain_file::set_contents(const map<string, inode_ptr>&) {
    throw file_error("is a plain file");
}

//************ Directory Functions ************//
directory::directory() {
    dirents.insert(pair<string, inode_ptr>(".", nullptr));
    dirents.insert(pair<string, inode_ptr>("..", nullptr));
}

void directory::set_dir(inode_ptr cwd, inode_ptr parent) {
    map<string, inode_ptr>::iterator i = dirents.begin();
    i->second = cwd; ++i;
    i->second = parent;
}

size_t directory::size() const {
    size_t size{ 0 };
    size = dirents.size();
    DEBUGF('i', "size = " << size);
    return size;
}

const wordvec& directory::readfile() const {
    throw file_error("is a directory");
}

void directory::writefile(const wordvec&) {
    throw file_error("is a directory");
}

void directory::set_data(const wordvec& d) {
    throw file_error("is a directory");
}
void directory::remove(const string& filename) {
    DEBUGF('i', filename);
}

inode_ptr directory::mkdir(const string& dirname) {
    inode_ptr new_dir = make_shared<inode>(file_type::DIRECTORY_TYPE);
    new_dir->set_name(dirname + "/");
    DEBUGF('i', dirname);
    return new_dir;
}

inode_ptr directory::mkfile(const string& filename) {
    inode_ptr file = make_shared<inode>(file_type::PLAIN_TYPE);
    file->set_name(filename);
    DEBUGF('i', filename);
    return file;
}

