#include "Boot.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void Step(const string& message) {
    cout << "[ \033[32m OK \033[0m ] " << message << endl;
    this_thread::sleep_for(chrono::milliseconds(300));
}

void Boot() {
    cout << "OS SIMULATOR 1.0 starting...\n\n";
    Step("Initializing process table");
    Step("Initializing memory manager");
    Step("Mounting root file system");
    Step("Loading scheduler (Round Robin)");
    Step("Starting shell");
    cout << "\n";
}