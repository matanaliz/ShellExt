#How to use
0. Change log file path in Logger.h. Default path is E:\log.txt
1. Compile x64 build
2. Register .dll file with command:
	Regsvr32 CppShellExtProp.dll
3. Right-click on some file and push "Log it!"
4. File information will be added to log file.


#Known issues:
1. Not work if drag folders too
2. Not working on special symbols like: trademark, copyright 
3. File format alignment
4. Need to restart explorer for rebuild dll
5. Hang on big files (slow checksum computing)


#TODO
1. Log file sort
2. Unittests
3. Fix it
4. Use ThreadPool for checksum computing!
5. Redesign FileImpl class (common checksum computing move to File)