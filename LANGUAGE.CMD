@Echo Off
If *%1==* Goto Help
If Exist %1.dll Goto Install
If Exist %1.hlp Goto Install
Goto Help

:Install
If Exist %1.dll Copy %1.dll Clock.dll 1>NUL
If NOT Exist %1.dll Echo Language DLL not found!
If Exist %1.hlp Copy %1.hlp Clock.hlp 1>NUL
If NOT Exist %1.hlp Echo Language help file not found!
Echo Language %1 activated.
Goto Exit

:Help
Echo You forgot to specify a valid language!

:Exit

