#include <string>
#include <cstdio>
#include "outgoing.h"

/* This function creates the outgoing_message if there is an alert. Once the message
 *  is built. Once the alertMessage is created, the calling function should:
 * 1. Declare a string...for example string fileName
 * 2. Call Base_Unit getFilePath (fileName, 1);  --the 1 is the type of message
 *                        -- alert messages are type number = 1
 * 3. Declare a bool that will represent if a new log file will need to be opened.
 * 4. Call bool = Base_Unit logFile(fileName, &message, 0);
 * 5. This function call will check the message count and write the alert to
 *    to the current open file. If the message count was at the limit, then a
 *    new file name will be created and the string fileName that was sent in 
 *    will be updated. 
 * 6. 

 */
void outgoing_message(string & alertMessage, int unit1, int unit2, int messageTime, int velocity, int distance, int bearing)
{
		alertMessage = '$' + unit1 + "->" + unit2 + ',' + messageTime + ',' + velocity + ',' +
			distance + ',' + bearing + '*' + '\\n';
}

/*This is a placeholder function that will be used to create network failure messages.*/
void network_failure_message(string& networkFailureMsg)
{
}

/* This is a placeholder function that will be used to create misc error messages*/
void misc_error_message(string& miscErrorMsg)
{

}