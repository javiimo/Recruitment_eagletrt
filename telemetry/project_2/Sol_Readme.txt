To execute it, it requires another argument with the relative path of the candump.log file. For example:
project_2.exe ../data/candump.log

To stop the reader from receiving any more data you can press any key and then press ENTER.

I have also commented out the line from the CMakeLists.txt that sets the .exe in the bin folder, so it is built in the build folder. Just because it kept giving me problems with finding the dll when executing it.

How are statistics done: Once the reader has finished receiving the messages of a Running session, it signals to the Main thread to compute the statistics of that Running session (it also signals if the session is interrupted by the user).
For the mean_time, it is calculated taking the difference between the first and last time a certain ID was received during that running session and divided by the number of times that ID was received.

Both the statistics csv and the logs are saved to a subfolder called Generated in the program's path.