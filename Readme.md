# Blitz3D Modernized

Blitz3D Visual Studio 2017 compatible release.

## Build Instructions

1. Open blitz3d.sln with Visual Studio 2017 community.
2. Build project 'bblaunch' using config 'Win32 Blitz3D Release'.
3. Output files should end up in _release subdir.
4. Also copy lib/fmodapi375win/api/fmod.dll to _release/bin.
5. Done?

## Known problems
* Almost done, all works from what I can see apart from an interesting (and annoying) bug when loading alpha'ed textures, causes it to crash.  Creating a texture with alpha does not crash.  Still yet to solve - see samples/agore/grassdemo/grass.bb for example.  Can anyone help eradicate this bug?
