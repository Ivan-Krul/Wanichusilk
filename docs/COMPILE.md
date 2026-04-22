# COMPILE

## Required libraries

 - SDL3
 - SDL3_image
 - SDL3_ttf

## Using terminal

It's independanly from the distribution family or operating system, the process of compiling is similar using terminal.

You first need to have **git**, C++ compiler of your choice (MSVC++, GCC, CLang or something else) and generator of your choice (ninja by default) pre-installed.

### in Windows With VCPKG

First you should clone it from [github](https://github.com/microsoft/vcpkg.git). 
In case you have already installed it, be sure to set `$VCPKG_ROOT` to the location of the program.

In other case, after clonning you should run `bootstrap-vcpkg` in source directory in terminal
With that you should set enviroument variable `$VCPKG_ROOT` (assign as `C:/path/to/vcpkg`) (in `SystemPropertiesAdvanced`).

You may need to install Visual Studio, specifically `MSVC Build tools for x64/x86 (Latest?)` and `Windows <WINDOWS_VERSION> SDK`.
`Just-In-Time Debugging` and `MSVC AddressSanitizer` would be handy FOR DEBUGGING ONLY.
The reason of installation is the dependancies on build tooling for `vcpkg`.

> [!TIP]
> If you would manage to avoid tooling dependancy of `vcpkg` from Visual Studio, it would be fine as well.

### With Vendoring for rest of systems

For vendoring it was used `git submodule` commands. 

> [!IMPORTANT]  
> For Windows only, be sure that path doesn't include spaces.

After a cloning main repo, you should run at source directory `git submodule update --init --recursive --depth 1`.

### after all that...

Run `cmake` either with presets, or simply ...
```
mkdir build
cd build
cmake ..
make
```

To look which preset you want, type `cmake --list-presets`, and run `cmake --preset=<YOUR_CHOICE>`.
Same thing would be to building, using `cmake --build --list-presets` to look up which preset you may want, and run
`cmake --build --preset=<YOUR_CHOICE>`.

For other settings use inline caches when you type your cmake command (i.e. `cmake --preset=<YOUR_CHOICE> -G "Unix Makefiles" ...`)

Compiled program should be at directory `/bin` within source directory.

> [!TIP]
> If you would know better or easier tooling than that, lmk.

## in Windows using dedicated IDE

In case of compiling in dedicated Visual Studio Enviroument (in IDE), you need to be sure that vcpkg works and installs correctly.
After that you can now generate cmake configuration, and then you can normally compile.

