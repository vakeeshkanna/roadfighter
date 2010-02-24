@rem *----------------------------------------------------------------------
@rem * ONCE YOU HAVE MODIFIED THE VARIABLES IN THIS FILE TO SUIT THE SETUP
@rem * OF YOUR MACHINE PLEASE RENAME THIS FILE TO "start-dev.cmd" TO PREVENT
@rem * THE ORIGINAL FILE FROM BEING CORRUPTED IN CVS ( IF YOU DONT DO THIS 
@rem * YOU MAY CAUSE THE AUTOMATIC WINDOWS NIGHTLY BUILD TO FAIL )
@rem *----------------------------------------------------------------------

@set DEVBASE=%1
@set DEBUG_RELEASE=%2
@set DYNAMIC_STATIC=%3
@set CPU=WIN32
@set ERM_MACHINE_TYPE=WIN32
@set BASE_PATH=%PATH%
@set DEVROOT=%DEVBASE%
@shift

@rem *----------------------------------------------------------------------
@rem *
@rem * Microsoft Development Studio root directory.
@rem *
@rem *   The C/C++ include files are assumed to be at:

@set MSDEVROOT=D:\apps\VS9.0

@rem *----------------------------------------------------------------------
@rem *
@rem * Directory in which Cygwin is installed.
@rem *

@set CYGWINDIR=C:\cygwin\bin

@rem *----------------------------------------------------------------------
@rem *
@rem * Directory in which installed utilities may be found.
@rem *

@set UTILDIR=D:\mercurial\source_cmake_2.6\win_dev_env\utils

@rem *----------------------------------------------------------------------
@rem *
@rem * MKS directory & root directory.
@rem *

@set MKSNTDIR=%UTILDIR%\mksnt

@rem *----------------------------------------------------------------------
@rem *
@rem * Perl directory
@rem *

@rem @set PERL=%UTILDIR%/mksnt

@rem *----------------------------------------------------------------------
@rem *
@rem * Programmer's Notepad directory.
@rem *

@set PN=%UTILDIR%\bin\pn

@rem @set THIRDPARTY=D:\dev\thirdparty


@rem *----------------------------------------------------------------------
@rem *
@rem * Java directory.
@rem *

@set JAVA_HOME=C:\Apps\jdk1.6.0_02

@set ANT_HOME=C:\Apps\apache-ant-1.7.0
@set JDK=%JAVA_HOME%

@rem @set PATH=%PATH%;%XERCESHOME%\bin



@rem @set CLASSPATH=%DFA_CLASSES%;%OTHER_CLASSES%;%BASE_CLASSPATH%


@echo MS Developer Studio...
@set MSDEV=%MSDEVROOT%\VC98
@set INSTALLSHIELD=C:\Program Files\InstallShield\Professional 6.3\Program
@set PATH=%PATH%;%MSDEV%\Bin;%MSDEVROOT%\Common\MSDev98\Bin;D:\dev\buildsupport\ant\bin;C:\Apps\CMake 2.4\bin;D:\dev\prebuild\cppunit\1.12.0\lib\WIN32;

@echo CVS...
@if "%CVSROOT%"=="" set CVSROOT=:pserver:waqqassharif@waqqas:/Users/waqqassharif/CVS
@rem if "%CVS_PASSFILE%"=="" set CVS_PASSFILE=%UTILDIR%/.cvspass

@echo Java...
@set CLASSPATH=%SUNJDK%\lib\tools.jar
@set CLASSPATH=%CLASSPATH%;%DEVROOT%\src

@set PATH=%JDK%\bin;C:\Apps\apache-ant-1.7.0\bin;%JDK%\jre\bin;%JDK%\jre\bin\server;%PATH%

:mks
@echo MKS...
@set SHELL=%MKSNTDIR%\sh.exe
@set PATH=%MKSNTDIR%;%PATH%

:done
@set PATH=%PATH%;%UTILDIR%\bin;%UTILDIR%\PFEDIT;%CYGWINDIR%
@set BASE_PATH=%PATH%
@set BASE_CLASSPATH=%CLASSPATH%

@set %USERNAME%=1

@echo Setting Java Path
@set PATH=%JAVA_HOME%/bin;%PATH%

@echo Setting Eclipse Path
@set PATH="C:\Apps\Eclipse-3.3M6-win32\";%PATH%

@rem Also include path to our bin folder 
@set PATH=%DEVROOT%\lib\%CPU%\%DEBUG_RELEASE%\%DYNAMIC_STATIC%;%DEVROOT%\bin\%CPU%\%DEBUG_RELEASE%\%DYNAMIC_STATIC%;%PATH%
@set PATH=%DEVROOT%\bin;%PATH%
@rem include path to GrandUI
@set PATH=C:\Apps\grand-ui-0.7.2;%PATH%
@set PATH=%UTILDIR%\mksnt;%PATH%
@set PATH="C:\Program Files\Microsoft DirectX 9.0 SDK (April 2005)\Lib\x86";%PATH%
@set PATH="D:\apps\CMake 2.6\bin";"D:\Program Files\Programmer's Notepad";"D:\apps\VS9.0\Common7\IDE";"D:\apps\VS9.0\VC\bin";"D:\apps\apache-maven-2.2.1\bin";%PATH%






@call %UTILDIR%\bin\root.cmd
@rem @call gd.cmd /
