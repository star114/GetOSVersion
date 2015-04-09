# GetOSVersion
* Getting Operating system version infomation on Windows.

## How to Build
* Using Visual studio 2013, open the project file, GetOSVersion.sln.
* Set the configuration (Release or Debug)
* Build solution.
* There are output files in $(SolutionDir)\Release

## How to Use
* GetOSVersion.exe must need just 1(one) argument.

Meaning of argument :
1 - Using GetVersionEx
2 - Using VerifyVersionInfo Known OSVersion
3 - Using VerifyVersionInfo UnKnown OSVersion

## Some problems
* Overcoming Windows 8.1's deprecation of GetVersionEx and GetVersion APIs, Use VerifyVersionIfo APIs
* But, It cannot overcome Windows 10's deprecation of VerifyVersionInfo.
* If you want to get correct OS version infomation, Use manifest files. 
* "<!-- Windows 10 --> <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>"

## What is your OS Version?
* Windows Version 	- Major - Minor
* Windows xp 		- 5 	- 3
* Windows Vista 	- 6 	- 0
* Windows 7 		- 6 	- 1
* Windows 8 		- 6 	- 2
* Windows 8.1 		- 6 	- 3
* Windows 10 		- 6 	- 4 	or 10 	- 0

## Developing
* C++
* Windows 8.1

### Tools
* VisualStudio 2013

#### License
MIT