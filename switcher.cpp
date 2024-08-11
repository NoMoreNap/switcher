#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

string execCommand(const string& cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

unordered_map<string, string> loadConfig(const string& filename) {
    unordered_map<string, string> config;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening config file: " << filename << endl;
        return config;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, email;
        if (getline(ss, name, ';') && getline(ss, email)) {
            config[name] = email;
        }
    }
    return config;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "usage: " << argv[0] << " --to=<TargetName>" << endl;
        return 1;
    }

    string argument = argv[1];
    string targetValue;

    if (argument.find("--to=") == 0) {
        targetValue = argument.substr(5);
    } else {
        cerr << "argument does not start with --to=" << endl;
        return 1;
    }

    auto config = loadConfig("config.txt");

    if (config.empty()) {
        cerr << "configuration file is empty or could not be read." << endl;
        return 1;
    }

    auto it = config.find(targetValue);
    if (it != config.end()) {
        string userEmail = it->second;
        try {
            execCommand("git config --global user.name \"" + targetValue + "\"");
            execCommand("git config --global user.email \"" + userEmail + "\"");
            cout << "switched to " << targetValue << endl;
        } catch (const runtime_error& e) {
            cerr << "error executing command: " << e.what() << endl;
            return 1;
        }
    } else {
        cout << "unknown user" << endl;
    }

    return 0;
}
