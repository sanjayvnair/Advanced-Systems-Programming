1.	Place the input.txt (or any other name of file that contains the input) file in the directory that contains the main code.
2.	While providing any new input, provide the input in a file mentioned above and make sure that following points are kept in mind while creating a new input text file:
		(a)	each word in the input is in a new line.
		(b)	make sure while providing the input each word does not have a leading space in every single line.
3.	The words in the input are expected to be in a sorted order. If not, the mapper pool updater would still sort the input before used by the mapper. This is to ensure that each entry in the buffer hold the words that start with the same letter 




Some observations :

1. The bounded buffer size is dynamicaly allocated based on the number of sets of words that start with the same letter as mentioned in the design for assignment2 document.
2. The arguements of threads can take any value but however as long as the number of slots is less that the number of threads each thread can work on a different slot simultaneously. If greater there would not be any performance enhancement
3. The word list is stored in the wordCount.txt file which gets generated in the same directory that has the program along with the letterCount.txt
4. As mentioned in the design file writing should be done using a single thread and since the main thread is capable of doing so , it has been implemented the same way.

NOTE:

Contents of the pool can be printed for verification by uncommenting certain sections in each function as mentioned below.

Lines 318-323 	- Mapper pool contents
Lines 235 		- Summarizer pool contents
Lines 112 		- Reducer pool contents

===================================================================================================

COMPILING

make  						----------------------- compile the wordStatistics.c program

make clean					----------------------- for a fresh compilation and deletes the 														already existing wordCount and letterCount 														files
==================================================================================================

RUNNING CODES:

./wordStatistics <input file> <mapper_threads> <reducer_threads> <summarizer_threads> 

SAMPLE COMMAND:

./wordStatistics input.txt 1 3 3

