
#include <iostream>
#include <string>
#include <array>
#include <memory>

using namespace std;


string execCommand(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char* argv[]) {
    string targetValue;


    if (argc > 1) {
        string argument = argv[1];

        if (argument.find("--to=") == 0) {
            targetValue = argument.substr(5);

        } else {
            cout << "argument does not start with --to=" << endl;
        }
    } else {
        cout << "no arguments provided!" << endl;
    }

    if (targetValue == "NoMoreNap") {
        execCommand("git config --global user.name \"NoMoreNap\"");
        execCommand("git config --global user.email \"vv2trash@gmail.com\"");
        cout << "switched to nomorenap" << endl;

    }

    else {
        cout << "unknown user" << endl;
    }


    return 0;
}