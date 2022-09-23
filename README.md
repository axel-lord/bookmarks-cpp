### Bookmark Browser

A c++ utility for keeping track of bookmarks.

## Build
1. Install 
[cmake](https://cmake.org/) (at least 3.19).
2. Install 
[vcpkg](https://github.com/microsoft/vcpkg).
3. Install 
[google benchmark](https://github.com/google/benchmark), 
[catch2](https://github.com/catchorg/Catch2), 
[fmt](https://github.com/fmtlib/fmt), 
[range-v3](https://github.com/ericniebler/range-v3) and 
[spdlog](https://github.com/gabime/spdlog) using your vcpkg instance.
4. On Windows (msvc) go to step 5 else go to step 8.
5. Install [Visual Studio 2022](https://visualstudio.microsoft.com/vs/).
6. Open project folder with visual studio.
7. Build all or demo. Build is now finished.
8. Run cmake according to the instructions given 
by vcpkg, or however you want if you feel like it. Build is now finished.
9. If my instructions where not enough wait for me to write better ones, or 
build however many other cmake+vcpkg projects are needed to follow them. The first 
option might take less time.

## Notes
GPL3 is used due to being the path of least resistance, the program has no need for non-free code but may have need for gpl licensed code.
