"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl" /I"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include" /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt" /LD ./lib/work_lib.c ./lib/elasticpl_crypto.lib /link /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x86" /DLL /OUT:%1