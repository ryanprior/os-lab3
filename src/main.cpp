#include "Process.h"
#include <iostream>

int main() {
  using namespace std;
  proc_t pid, bst, arr, pri;
  while(cin >> pid >> bst >> arr >> pri) {
    ProcessMFQS p(pid,bst,arr,pri);
    cout << p << endl;
  }
  return 0;
}
