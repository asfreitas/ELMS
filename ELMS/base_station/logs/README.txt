The folder contains the log files.

It is divided up into subfolders for:
1. alerts
   a. This subfolder stores all alert messages sent to 
      vehicles that they are too close to other vehicles.
2. incoming_messages
   a. All messages received from vehicles are stored in this
      subfolder. 
3. network_failure
   a. validated
      (1) This sub-folder will store messages if the warnings
          is validated.       
   b. not_validated
     (1) Not all network failure warnings will be validated.
         Some warnings may occur simply if the mine is not open. 
         This sub-folder is used when the failure is not validated
         as a means to sort out how to prevent false-alarms.
4. misc_errors
   a. This sub-folder will store any other errors not related to 
      network failure. 
