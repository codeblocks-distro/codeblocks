rem @echo off

set CB_DEVEL_DIR=devel%1
set CB_OUTPUT_DIR=output%1
set CB_DEVEL_RESDIR=..\..\..\%CB_DEVEL_DIR%\share\CodeBlocks\images
set CB_OUTPUT_RESDIR=..\..\..\%CB_OUTPUT_DIR%\share\CodeBlocks\images

md %CB_DEVEL_RESDIR%\DoxyBlocks > nul 2>&1
md %CB_OUTPUT_RESDIR%\DoxyBlocks > nul 2>&1

for %%g in (16x16,20x20,24x24,28x28,32x32,40x40,48x48,56x56,64x64) do (
    md %CB_DEVEL_RESDIR%\DoxyBlocks\%%g > nul 2>&1
    copy images\%%g\*.png %CB_DEVEL_RESDIR%\DoxyBlocks\%%g > nul 2>&1
    md %CB_OUTPUT_RESDIR%\DoxyBlocks\%%g > nul 2>&1
    copy images\%%g\*.png %CB_OUTPUT_RESDIR%\DoxyBlocks\%%g > nul 2>&1
)

md %CB_DEVEL_RESDIR%\settings > nul 2>&1
md %CB_OUTPUT_RESDIR%\settings > nul 2>&1
copy *.png %CB_DEVEL_RESDIR%\settings\ > nul 2>&1
copy *.png %CB_OUTPUT_RESDIR%\settings\ > nul 2>&1

rem exit 0
