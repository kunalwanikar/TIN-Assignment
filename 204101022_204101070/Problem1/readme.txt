
==================================================Instruction to use Code of Problem 1 ====================================================

1.all the topology, connection ,request file should be available and in same directory as problem1.cpp

2.compile the code

3.after compilation execut the code 
	usage :
	$ ./routing -top topologyfile.txt -conn connectionfile.txt -rt routingtablefile.txt -ft forwardingtablefile.txt -path pathsfile.txt -flag 
	hop/dist -p 0/1

you can give file name as per your convinient.

Input Info :
-> for topology you can give top14.txt or top24.txt or any other file
-> for connection you can give NSFNET_100.txt or ARPANET_100.txt or any other file
-> flag (you can use dist/hop) 
-> p (use 0 for optimistic/use 1 for pessimistic). 

Output Info :
-> After Execution of the code output files are generated in the same directory as problem1.cpp
-> Output file names are
	- routingtablefile.txt 
	- forwardingtablefile.txt 
	- path.txt

->File formats :

	1. Routing table file

		**for each router to every other node there are two entries 
		one for shortest path and other for second shortest path

		----------------------------------------------------------
		destination node   |  path to destination  |   distance  |
		----------------------------------------------------------
		**path is denoted as src-->int.1-->int.2-->.....-->dest

	2. Forwarding table

		------------------------------------------------------------
		Interface in  |  Label in |   Interface out  |  Label out  |
		------------------------------------------------------------

		**Interface and Label is set to -1 when entry is not applicable.

	3. Path table 

		----------------------------------------------------------------------
		Connection ID |   Source |    Destination |  Label List  |  Path Cost|
		----------------------------------------------------------------------

		**Label List is denoted as Label1-->Label2-->Label3.....
