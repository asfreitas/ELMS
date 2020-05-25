#ifndef OUTGOING_H
#define OUTGOING_H

#include "utilities.h"
#include "calculations.h"
#include <string>
using std::string;

void outgoing_message(string& alertMessage, int unit1, int unit2, int messageTime, double velocity, double distance, double bearing);
void createAlert(string& alertMessage, string& alertLogMessage, char* fileName,
    double velocity, Vehicle* v1, Vehicle* v2, double distance, int time);
void network_failure_message(string& networkFailureMsg);
void misc_error_message(string& miscErrorMsg);
#endif