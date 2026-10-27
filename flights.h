// PROVIDED FILE: DO NOT EDIT. High-level flight operations are implemented for you.
#ifndef FLIGHTS_H
#define FLIGHTS_H

#include "types.h"

// Used by the fixed parser to identify the physical input line in diagnostics.
void setInputContext(int lineNumber, const std::string& command);

// Updates have valid inputs: ADD key absent; CANCEL key present;
// DELAY old key present and new key unoccupied, later, and in the same day.
void addFlight(FlightTree& tree, const Flight& flight);
void cancelFlight(FlightTree& tree, int departure);
void delayFlight(FlightTree& tree, int oldDeparture, int newDeparture);
const Flight* findFlight(const FlightTree& tree, int departure);
const Flight* earliestFlight(const FlightTree& tree, int departure); // earliest key >= departure
// Print one record, or NONE for nullptr. Formatting is centralized here.
void printFlight(const Flight* flight);
void printSchedule(const FlightTree& tree);
void printRange(const FlightTree& tree, int start, int end);
void clearTree(FlightTree& tree);
#endif
