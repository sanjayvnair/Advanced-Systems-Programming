1.	Place the input.txt (or any other name of file that contains the input) file in the directory that 				contains all the programs.
2.	While providing any new input, provide the input in a file mentioned above and make sure that following points 	   are kept in mind while creating a new input text file:
		(a)	each word in the input is in a new line.
		(b)	make sure while providing the input each word does not have a leading space in every single line.
3.	The words in the input are expected to be in a sorted order. if not, the mapper would still sort the input 		before showing the results on to the console. 
4.	If mapper program is being tested separately and if the results are to be shown in a non-sorted fashion then   	   comment lines 46-54 of mapper.c and 'make' the file. However, for combiner please ensure these lines are not 	commented because the reducer expects a sorted output from mapper for its algorithm to work properly.


COMPILING

make  						----------------------- compile all the files
make combiner               ----------------------- compile combiner code
make reducer                ----------------------- compile reducer code
make mapper                 ----------------------- compile mapper code
make clean					----------------------- for a fresh compilation

Running the code:

COMBINER PROGRAM           ./combiner <filename>      (within the same directory)
MAPPER PROGRAM             ./mapper <filename> 	      (within the same directory)
REDUCER PROGRAM            ./reducer

