#include <string>
#include <cstdio>
#include "outgoing.h"

/* This function creates the outgoing_message if there is an alert. Once the
 * alertMessage is created, the calling function should:
 * 1. Declare a string...for example string fileName
 * 2. Call Base_Unit getFilePath (fileName, 1);  --the 1 is the type of message
 *                        -- alert messages are type number = 1
 * 3. Declare a bool that will represent if a new log file will need to be opened.
 * 4. Call bool = Base_Unit logFile(fileName, &message, 0);
 * 5. This function call will check the message count and write the alert to
 *    to the current open file. If the message count was at the limit, then a
 *    new file name will be created and the string fileName that was sent in 
 *    will be updated.  
 */
void outgoing_message(string & alertMessage, int unit1, int unit2, int messageTime, double velocity, double distance, double bearing)
{
    velocity = roundToOneDecimal(velocity);
    distance = roundToOneDecimal(distance);
    string unit11 = std::to_string(unit1);
    string unit22 = std::to_string(unit2);
    string messageTime1 = std::to_string(messageTime);
    string velocity1 = doubleToString(velocity);
    string distance1 = doubleToString(distance);
    string bearing1 = doubleToString(bearing);
	alertMessage = "$" + unit11 + "->" + unit22 + ',' + messageTime1 + ',' + velocity1 + ',' +
            distance1 + ',' + bearing1 + "*" + "\n";
}

/*
===============
createAlert

This function creates the alert messages that will be sent out through the
serial port. It also creates the string that will be used for the log file that
will be created in association with the 
*/
void createAlert(string & alertMessage, string & alertLogMessage, char* fileName, 
    double velocity, Vehicle *v1, Vehicle * v2, double distance, int time)
{
    //declare a Calculations class member
    Calculations calc;

    // first calculate the speed which is in meters per second
    double speed = calc.knots_to_mps(velocity);

    // now we get the bearing
    int bearing = calc.getBearing(v1, v2);

    // we create the outgoing message;
    outgoing_message(alertMessage, v1->getUnit(), v2->getUnit(), time, speed,
        distance, bearing);

    //copy the message to use to create the log file for the alert
    alertLogMessage = alertMessage;

    //convert the outgoing message to a char * so that it can be transmitted
    stringToCharPointer(alertMessage, fileName);
}

/*This is a placeholder function that will be used to create network failure messages.*/
void network_failure_message(string& networkFailureMsg)
{
}

/* This is a placeholder function that will be used to create misc error messages*/
void misc_error_message(string& miscErrorMsg)
{

}