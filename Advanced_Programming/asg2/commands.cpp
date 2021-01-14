// $Id: commands.cpp,v 1.18 2019-10-08 13:55:31-07 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash{
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"    , fn_rmr    },
   {"#"     , fn_comm  },
};

command_fn find_command_fn(const string& cmd) {
    // Note: value_type is pair<const key_type, mapped_type>
    // So: iterator->first is key_type (string)
    // So: iterator->second is mapped_type (command_fn)
    const auto result = cmd_hash.find(cmd);
    if (result == cmd_hash.end()) {
        throw command_error(cmd + ": no such function");
    }
    return result->second;
    DEBUGF('c', "[" << cmd << "]"); // moved to bottom
}

command_error::command_error(const string& what) :
    runtime_error(what) {
}

int exit_status_message() {
    int status = exec::status();
    cout << exec::execname() << ": exit(" << status << ")" << endl;
    return status;
}

// cat pathname . .
// Prints out contents of each file
void fn_cat(inode_state& state, const wordvec& words) {
    if (words.size() < 2)
        throw command_error("cat: no args specified");
    else {
        state.cat(state.get_cwd(), words);
    }
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// cd [pathname]
// Changes current working directory
void fn_cd(inode_state& state, const wordvec& words) {
    state.change_dir(state, words);
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// echo [words . . .]
// Echoes words to standard output
void fn_echo(inode_state& state, const wordvec& words) { // UNCHANGED
    DEBUGF('c', state);
    DEBUGF('c', words);
    cout << word_range(words.cbegin() + 1, words.cend()) << endl;

}

// exit[status]
// Exit the program with given status
void fn_exit(inode_state& state, const wordvec& words) {
    // Status already set to EXIT_SUCCESS
    // If exit is called with args
    if (words.size() >= 2) {
        exec exit;
        string s = "";
        bool alphaNum = false;
        for (size_t i = 1; i < words.size(); ++i)
            s += words.at(i);

        for (size_t j = 0; j != s.size(); ++j) {
            if (isalpha(s[j]) == true) {
                alphaNum = true;
            }
        }
        if (alphaNum == true) {
            // if alphabet then, set to 127
            exit.status(127);
        }
        else {
            // string to integer
            exit.status(stoi(s));
        }
    }
    DEBUGF('c', state);
    DEBUGF('c', words);
    throw ysh_exit();
}

// ls [pathname . . .]
// Displays a description of files and directories within
// the current working directory
void fn_ls(inode_state& state, const wordvec& words) {
    if (words.size() <= 2) {
        state.print_dir(state.get_cwd(), words);
    }
    else throw command_error("ls: invalid num of args");
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// lsr[pathname . . .]
// As ls, but prints out entirety of shell with a
// recursive depth-first preorder traversal
void fn_lsr(inode_state& state, const wordvec& words) {
    if (words.size() <= 2) {
        state.list_recursively(state, words);
    }
    else throw command_error("lsr: invalid num of args");
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// make pathname [words . . .]
// Makes file, includes contents within file if words are
// specified
void fn_make(inode_state& state, const wordvec& words) {
    if (words.size() == 1) {
        throw command_error("make: no arg");
    }
    else {
        state.create_file(state.get_cwd(), words);
    }
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// mkdir pathname
// Makes directory
void fn_mkdir(inode_state& state, const wordvec& words) {
    if (words.size() == 1) {
        throw command_error("mkdir: no arg");
    }
    else if (words.size() == 2) {
        state.make_dir(state.get_cwd(), words);
    }
    else throw command_error("mkdir: invalid arg");
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// prompt string
// Changes the character to be used as the prompt character.
void fn_prompt(inode_state& state, const wordvec& words) {
    string new_prompt = "";
    for (size_t i = 1; i < words.size(); ++i) {
        new_prompt += words.at(i);
        new_prompt += " ";
    }
    state.set_prompt(new_prompt);
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// pwd
// Prints current working directory
void fn_pwd(inode_state& state, const wordvec& words) {
    if (words.size() == 1) {
        state.print_path(state.get_cwd());
    }
    else throw command_error("pwd: invalid num of args");
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// rm pathname
// Removes specified file or directory, directory
// must be empty before removal
void fn_rm(inode_state& state, const wordvec& words) {
    if (words.size() == 2) {
        state.remove(state.get_cwd(), words);
    }
    else throw command_error("rm: invalid num of args");;
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// rmr pathname
// As rm, but using a depth-first postorder traversal.
void fn_rmr(inode_state& state, const wordvec& words) {
    if (words.size() == 2) {
        state.remove_recursively(state, words);
    }
    else throw command_error("rm: invalid num of args");;
    DEBUGF('c', state);
    DEBUGF('c', words);
}

// For comments, doesn't return or do anything.
void fn_comm(inode_state& state, const wordvec& words) {
    DEBUGF('c', state);
    DEBUGF('c', words);
}

