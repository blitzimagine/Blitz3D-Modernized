# Blitz3D Modernized

Blitz3D Visual Studio 2010 compatible release.

## Build Instructions

```
1) Install directx7 sdk into same dir as blitzplus using dx7_sdk as folder name

2) Install freeimage241 into same dir as blitz3d: http://monkeycoder.co.nz/downloads/freeimage241.zip

3) Install fmodapi375win into same dir as blitz3d: http://www.fmod.org/files/public/fmodapi375win.zip

4) Install dplayx.lib, dplay.h and dplobby.h into a folder 'dplay' created into same dir as blitzplus, files may have to be downloaded

5) Open blitz3d.sln with MSVC2010 or MSVC2013.

6) Build project 'bblaunch' using config 'Win32 Blitz3D Release'.

7) Output files should end up in _release subdir.

8) Also copy fmodapi375win/api/fmod.dll to _release/bin.

9) Done?
```

## Known problems
* Almost done, all works from what I can see apart from an interesting (and annoying) bug when loading alpha'ed textures, causes it to crash.  Creating a texture with alpha does not crash.  Still yet to solve - see samples/agore/grassdemo/grass.bb for example.  Can anyone help eradicate this bug?
