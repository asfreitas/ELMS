#ifndef OUTGOING_H
#define OUTGOING_H
#include <string>
using std::string;

void outgoing_message(string &fileName, int unit1, int unit2, int messageTime, int velocity, int distance, int bearing);
void network_failure_message(string& networkFailureMsg);
void misc_error_message(string& miscErrorMsg);
#endif