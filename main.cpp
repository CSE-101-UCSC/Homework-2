// PROVIDED FILE: DO NOT EDIT.
#include "flights.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

static bool parseTime(const std::string& text, int& time) {
    if (text.size() != 4)
        return false;

    time = 0;
    for (char ch : text) {
        if (ch < '0' || ch > '9')
            return false;
        time = time * 10 + ch - '0';
    }

    return time / 100 < 24 && time % 100 < 60;
}

int main(int argc, char* argv[]) {
    const char* usage = "Usage: ./FlightManager <input_file> (--printall | --find HHMM | --earliest HHMM | --range HHMM HHMM)\n";

    std::string flag;
    int findOrEarliestTime = 0;
    int rangeStart = 0;
    int rangeEnd = 0;

    bool validArgs = false;

    if (argc == 3) {
        flag = argv[2];
        validArgs = flag == "--printall";
    }

    if (argc == 4) {
        flag = argv[2];
        validArgs = (flag == "--find" || flag == "--earliest") &&
                    parseTime(argv[3], findOrEarliestTime);
    }

    if (argc == 5) {
        flag = argv[2];
        validArgs = flag == "--range" &&
                    parseTime(argv[3], rangeStart) &&
                    parseTime(argv[4], rangeEnd);
    }

    if (!validArgs) {
        std::cerr << usage;
        return 1;
    }

    std::ifstream input(argv[1]);

    if (!input) {
        std::cerr << "Error: cannot open input file.\n";
        return 1;
    }

    FlightTree tree;
    std::string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;

        std::istringstream fields(line);
        std::string cmd, id, time, destination, newTime, extra;

        if (!(fields >> cmd))
            continue;
        if (cmd[0] == '#')
            continue;

        int departure = 0;
        int newDeparture = 0;
        bool valid = (cmd == "ADD" || cmd == "CANCEL" || cmd == "DELAY") &&
                     bool(fields >> id >> time >> destination) && parseTime(time, departure);
        if (valid && cmd == "DELAY")
            valid = bool(fields >> newTime) && parseTime(newTime, newDeparture);
        if (fields >> extra)
            valid = false;

        if (!valid) {
            std::cerr << "Error: invalid command on line " << lineNumber << ".\n";
            clearTree(tree);
            return 1;
        }

        setInputContext(lineNumber, line);

        const Flight* existing = findFlight(tree, departure);

        bool legal = cmd == "ADD" ? !existing :
            existing && existing->id == id && existing->destination == destination;
        if (cmd == "DELAY")
            legal = legal && newDeparture > departure && !findFlight(tree, newDeparture);

        if (!legal) {
            std::cerr << "Error: conflicting update on line " << lineNumber << ".\n";
            clearTree(tree);
            return 1;
        }

        if (cmd == "ADD")
            addFlight(tree, Flight{departure, id, destination});
        else if (cmd == "CANCEL")
            cancelFlight(tree, departure);
        else
            delayFlight(tree, departure, newDeparture);
    }

    if (flag == "--printall")
        printSchedule(tree);
    else if (flag == "--find")
        printFlight(findFlight(tree, findOrEarliestTime));
    else if (flag == "--earliest")
        printFlight(earliestFlight(tree, findOrEarliestTime));
    else
        printRange(tree, rangeStart, rangeEnd);

    clearTree(tree);
    return 0;
}
