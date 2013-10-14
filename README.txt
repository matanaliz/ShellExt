#How to use
0. Change log file path in Logger.h
1. Compile x64 build
2. Register .dll file with command:
	Regsvr32 CppShellExtProp.dll
3. Right-click on some file and push "Log it!"
4. File information will be added to log file.


#Known issues:
1. Error on unregister
2. Not work if drag folders too


#TODO
1. Add file information
2. Compute check sum
3. Threading
4. Log file sort
5. Unittests