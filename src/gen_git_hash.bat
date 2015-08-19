@echo off

if "%1" == "H" goto HEADER

if "%1" == "C" goto SOURCE_C

if "%1" == "CC" goto SOURCE_CC

if "%1" == "CPP" goto SOURCE_CPP

echo -- gen_git_hash.bat --
echo usage:
echo   $gen_git_hash.bat [TYPE] filename
echo TYPE:
echo   H   create header file.(it does not contain hash value. only declaration.)
echo   C   create c source file.
echo   CC  create cc source file.
echo   CPP create cpp source file.
echo
echo example:
echo   gen_git_hash H myhash
echo     myhash.h will be created.
echo   gen_git_hash C myhash
echo     myhash.c will be created.
echo   gen_git_hash CC myhash
echo     myhash.cc will be created.
echo   gen_git_hash CPP myhash
echo     myhash.cpp will be created.

goto END


:HEADER

echo /**^

 * this file was generated by gen_git_hash.bat.^

 */^

#ifndef __GIT_HASH_%2_H_^

#define __GIT_HASH_%2_H_^

^

#define GIT_COMMIT_HASH_TEXT(a) #a^

^

extern const char* branchname;^

^

extern const char* commithash;^

^

#endif  // __GIT_HASH_%2_H_ > %2.h

goto END

:SOURCE_C

echo #include "%2.h"^

const char* branchname = GIT_COMMIT_HASH_TEXT(> %2.c

git rev-parse --abbrev-ref HEAD >> %2.c

echo );^

^

const char* commithash = GIT_COMMIT_HASH_TEXT(>> %2.c

git rev-parse --short HEAD >> %2.c

echo ); >> %2.c

goto END

:SOURCE_CC

echo #include "%2.h"^

const char* branchname = GIT_COMMIT_HASH_TEXT(> %2.cc

git rev-parse --abbrev-ref HEAD >> %2.cc

echo );^

^

const char* commithash = GIT_COMMIT_HASH_TEXT(>> %2.cc

git rev-parse --short HEAD >> %2.cc

echo ); >> %2.cc

goto END

:SOURCE_CPP

echo #include "%2.h"^

const char* branchname = GIT_COMMIT_HASH_TEXT(> %2.cpp

git rev-parse --abbrev-ref HEAD >> %2.cpp

echo );^

^

const char* commithash = GIT_COMMIT_HASH_TEXT(>> %2.cpp

git rev-parse --short HEAD >> %2.cpp

echo ); >> %2.cpp

goto END

:END
@echo on