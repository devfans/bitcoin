
#include "skr.h"
#include <iostream>
#include <utilstrencodings.h>

using namespace std;

void print(string str);

int main(int argc, char* argv[])
{
    cout << "________________________________________________________________________________" << endl;
    string cmd = "0";
    int32_t cmd_int;
    if (argc == 1) 
        goto finish;
   
    cmd = argv[1];
    if (!ParseInt32(cmd, &cmd_int)) goto finish;

    switch (cmd_int) {
    case 0:
        print("testing");
        break;
    case 1:
        print("1:");
        break;
    case 2:
        print("2:");
        break;
    }
finish:
    print("Finished!");
    return 0;
}

void print(string str){
    cout << str << endl;
    return;
}


