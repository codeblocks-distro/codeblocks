@echo off
cls
md ..\..\..\devel\share\CodeBlocks\images\codesnippets > nul 2>&1
md ..\..\..\output\share\CodeBlocks\images\codesnippets > nul 2>&1
copy resources\*.png ..\..\..\devel\share\CodeBlocks\images\codesnippets\ > nul 2>&1
copy resources\*.png ..\..\..\output\share\CodeBlocks\images\codesnippets\ > nul 2>&1