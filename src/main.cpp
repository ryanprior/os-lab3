#include "Process.h"
#include <iostream>
#include <algorithm>

int main(int argc, char **argv) {
  using namespace std;
  if(argc != 2) {
    cout << "wrong number of arguments" << endl;
    exit(1);
  } else {
    // Get lower case first argument as string
    std::string type(argv[1]);
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    // Read in records and print their representations to stdout
    if(type == "mfqs") {
      proc_t pid, bst, arr, pri;
      while(cin >> pid >> bst >> arr >> pri) {
        ProcessMFQS proc(pid,bst,arr,pri);
        cout << proc << endl;
      }
    } else if(type == "rts") {
      proc_t pid, bst, arr, pri, dln;
      while(cin >> pid >> bst >> arr >> pri >> dln) {
        ProcessRTS proc(pid, bst, arr, pri, dln);
        cout << proc << endl;
      }
    } else if(type == "whs") {
      proc_t pid, bst, arr, pri, io;
      while(cin >> pid >> bst >> arr >> pri >> io) {
        ProcessWHS proc(pid, bst, arr, pri, io);
        cout << proc << endl;
      }
    } else {
      cout << "unknown type: " << type << endl;
      exit(1);
    }
  }
  return 0;
}
