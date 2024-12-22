:: This batch file runs a test example.
::
:: Execute the make command first to build the executable example files and
:: the example programs.
@echo off

:: Here you should replace this with your own installation of OpenSSL 1.0.2
:: 
:: In my case I prefer to set the binary directory in the PATH but
:: you also can copy and paste your OpenSSL binaries in the same directory
:: of your executable file.
::
set PATH=C:\Dev\openssl-1.0.2\u-win32

build\examples\SimpleHttpClient.exe
