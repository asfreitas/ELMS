#ifndef OUTGOING_H
#define OUTGOING_H
#include <string>
#include "utilities.h"
using std::string;

void outgoing_message(string& alertMessage, int unit1, int unit2, int messageTime, double velocity, double distance, double bearing);
void network_failure_message(string& networkFailureMsg);
void misc_error_message(string& miscErrorMsg);
#endif