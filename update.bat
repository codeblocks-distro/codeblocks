@echo off
echo Creating output directory tree

if not exist output md output
if not exist output\share md output\share
if not exist output\share\CodeBlocks md output\share\CodeBlocks
if not exist output\share\CodeBlocks\images md output\share\CodeBlocks\images
if not exist output\share\CodeBlocks\images\codecompletion md output\share\CodeBlocks\images\codecompletion
if not exist output\share\CodeBlocks\plugins md output\share\CodeBlocks\plugins
if not exist output\share\CodeBlocks\templates md output\share\CodeBlocks\templates

set ZIPCMD=zip
set RESDIR=devel\share\CodeBlocks

echo Packing core UI resources
%ZIPCMD% -j9 %RESDIR%\resources.zip src\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\manager_resources.zip sdk\resources\*.xrc > nul
echo Packing plugins UI resources
%ZIPCMD% -j9 %RESDIR%\astyle.zip plugins\astyle\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\plugin_wizard.zip plugins\pluginwizard\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\class_wizard.zip plugins\classwizard\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\code_completion.zip plugins\codecompletion\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\compiler_gcc.zip plugins\compilergcc\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\debugger_gdb.zip plugins\debuggergdb\resources\*.xrc > nul
%ZIPCMD% -j9 %RESDIR%\todo.zip plugins\todo\resources\*.xrc > nul
echo Packing core UI bitmaps
cd src\resources
%ZIPCMD% -0 -q ..\..\%RESDIR%\resources.zip images\*.png images\16x16\*.png > nul
echo Packing plugins UI bitmaps
cd ..\..\plugins\compilergcc\resources
%ZIPCMD% -0 -q ..\..\..\%RESDIR%\compiler_gcc.zip images\*.png images\16x16\*.png > nul
cd ..\..\..\plugins\debuggergdb\resources
%ZIPCMD% -0 -q ..\..\..\%RESDIR%\debugger_gdb.zip images\*.png images\16x16\*.png > nul
cd ..\..\..

echo Copying external exception handler
copy /y setup\exchndl.dll output > nul
copy /y setup\exchndl.dll devel > nul
echo Copying files
copy /y %RESDIR%\*.zip output\share\codeblocks > nul
copy /y src\resources\images\*.png %RESDIR%\images > nul
copy /y src\resources\images\*.png output\share\codeblocks\images > nul
copy /y plugins\codecompletion\resources\images\*.png %RESDIR%\images\codecompletion > nul
copy /y plugins\codecompletion\resources\images\*.png output\share\codeblocks\images\codecompletion > nul
copy /y templates\*.c* output\share\codeblocks\templates > nul
copy /y templates\*.h* output\share\codeblocks\templates > nul
copy /y templates\*.template output\share\codeblocks\templates > nul
copy /y templates\*.png output\share\codeblocks\templates > nul
copy /y templates\*.c* %RESDIR%\templates > nul
copy /y templates\*.h* %RESDIR%\templates > nul
copy /y templates\*.template %RESDIR%\templates > nul
copy /y templates\*.png %RESDIR%\templates > nul
copy /y tips.txt devel > nul
copy /y tips.txt output > nul
copy /y tools\ConsoleRunner\console_runner*.exe output > nul
copy /y tools\ConsoleRunner\console_runner*.exe devel > nul
copy /y devel\*.exe output > nul
copy /y devel\*.dll output > nul
copy /y %RESDIR%\plugins\*.dll output\share\codeblocks\plugins > nul
echo Stripping debug info from output tree
strip output\*.exe
strip output\*.dll
strip output\share\CodeBlocks\plugins\*.dll

set ZIPCMD=
set RESDIR=
