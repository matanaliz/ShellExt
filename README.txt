#How to use
0. Change log file path in Logger.h
1. Compile x64 build
2. Register .dll file with command:
	Regsvr32 CppShellExtProp.dll
3. Right-click on some file and push "Log it!"
4. File information will be added to log file.


#Known issues:
1. Not work if drag folders too
2. Thread pool stragne behaviour { logging n-1 files to log }
3. Not working on special symbols like: trademark, copyright 
4. File format alignment
5. Unstable on big amount of files


#TODO
1. Log file sort
2. Unittests