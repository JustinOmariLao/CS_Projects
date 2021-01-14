// $Id: main.cpp,v 1.12 2020-10-22 16:50:08-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <regex>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

class file_error: public runtime_error {
   public:
      explicit file_error (const string& what): runtime_error(what) {}
};

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv)
{
  fstream infile;

  regex empty_regex {R"(^\s*?$)"};
  regex comment_regex_ {R"(#.*)"};
  regex comment_regex {R"(^\s*(#.*)?$)"};
  regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
  regex key_regex {R"(^\s*(.*)\s*?$)"};
  regex key_equals_regex {R"(^\s*(.*)\s*=\s*$)"};
  regex equals_regex {R"(^\s*[=]\s*$)"};
  regex equals_value_regex {R"(^\s*[=]\s*(.*?)\s*$)"};


  smatch result;
  int linenumber = 0;
  std::string line;
  string filename;

  str_str_map test;
  str_str_pair pair;

  if (argc > 1)
  {

    for (int num = 1; num < argc; num++)
    {
      infile.open(argv[num]);

      if(infile.is_open()) 
      {

        filename = argv[num];   

        while (std::getline(infile, line))
        {
          ++linenumber;
          
          if(regex_search(line, result, empty_regex)) 
          {
            cout << filename << ": " << linenumber << ": " << endl;
            continue;
          }

          if(regex_search(line, result, comment_regex_)) 
          {
            cout << filename << ": " << linenumber 
              << ": " << line << endl;
            continue;
          }

          if(regex_search(line, result, equals_regex ))
          {
            cout << filename << ": " << linenumber 
              << ": " << "=" << endl;
            test.print_list();
            continue;
          }
          if(regex_search(line, result, equals_value_regex ))
          {
            cout << filename << ": " << linenumber 
              << ": " << "=" << result[1]  << endl;
            str_str_map out = test.search_for_keys(result[1]);
            out.print_list();
            continue;
          }
          if(regex_search(line, result, key_equals_regex ))
          {
            
            cout << filename << ": " << linenumber 
              << ": " << result[1] << "= "  << endl;
             if (test.find(result[1]) == false) {
              try {
                string key = result[1];
                throw file_error (key + ": key not found");
                } catch(file_error& e) {
                cerr << e.what() << endl;
                continue;
              }
            }
            else 
              test.erase(test.get_iterator());
              continue;
          }
          if(regex_search(line, result, key_value_regex))
          {
            cout << filename << ": " << linenumber 
              << ": " << result[1] << "=" << result [2] << endl;
            cout << result[1] << " = " << result [2] << endl;
            str_str_pair pair(result[1], result[2]);
            test.insert(pair);
            continue;
          }
          if(regex_search(line, result, key_regex)) 
          {
            cout << filename << ": " << linenumber 
              << ": " << result[1] << endl;

            if (test.find(result[1]) == false) {
              try {
              string key = result[1];
              throw file_error (key + ": key not found");
              } catch(file_error& e) {
                cerr << e.what() << endl;
                continue;
              }  
            }
            else {
              test.print_iterator();
              continue;
            }
          }
        }
          infile.close();
          linenumber = 0;
      }
      else
      {
          cerr << "keyvalue: " << filename 
            << ": No such file or directory" << endl;
      }
    }
  } 
  else if(argv[1] == NULL)
  {
    while (std::getline(cin, line))
    {
      ++linenumber;

      if(regex_search(line, result, empty_regex)) 
      {
        cout << "-" << ": " << linenumber << ": " << endl;
        continue;
      }
      if(regex_search(line, result, comment_regex)) 
      {
        cout << "-: " << linenumber << ": " << result[1] << endl;
        continue;
      }
      if(regex_search(line, result, equals_regex ))
      {
        cout << "-: "  <<linenumber << ": " << "=" << endl;
        test.print_list();
        continue;
      }
      if(regex_search(line, result, equals_value_regex ))
      {
        cout << "-: " << linenumber << ": =" << result[1] << endl;
        str_str_map out = test.search_for_keys(result[1]);
        out.print_list();
        continue;
      }
      if(regex_search(line, result, key_equals_regex ))
      {
        cout << "-: " << result[1] << "=: " 
          << linenumber << ": " << endl;
        if(test.find(result[1]) == false) {
          try {
         string key = result[1];
         throw file_error (key + ": key not found");
          } catch(file_error& e) {
            cerr << e.what() << endl;
            continue;
          }
        }
        else {
          test.erase(test.get_iterator());
          continue;
        }
      }
      if(regex_search(line, result, key_value_regex))
      {
        cout << "-" << ": " << linenumber << ": " 
          << result[1] << "=" << result [2] << endl;
        cout << result[1] << " = " << result [2] << endl;
        str_str_pair pair(result[1], result[2]);
        test.insert(pair);
        continue;
      }
      if(regex_search(line, result, key_regex)) 
      {
         cout << "-: " << linenumber  << ": " << result[1] << endl;
        if(test.find(result[1]) == false) {
          try {
         string key = result[1];
         throw file_error (key + ": key not found");
          } catch(file_error& e) {
            cerr << e.what() << endl;
            continue;
          }
        }
        else {
           test.print_iterator();
           continue;
        }
      }
    }
  }
}
