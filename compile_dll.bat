"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl" /I"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include" /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt" /I"C:\Development\OpenSSL\include" /LD ./work/work_lib.c ./crypto/elasticpl_crypto.lib libeay32.lib /link /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x86" /LIBPATH:"C:\Development\OpenSSL\lib" /LTCG /DLL /OUT:%1