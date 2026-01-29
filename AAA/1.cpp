#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int main() {
    char msg[10] = "\n\00012345";
    string s(msg, 0, 4);
    cout << s << endl;
    return 0;
}