# CRPLib
Library for import/export Need for Speed 5 CRP models

[Here](CRPFormat.md) is file format structure

## Development and contributing
Feel free to send pull requests and raise issues.

When developing, you may want to use `-D CMAKE_BUILD_TYPE=Debug` flag when calling `cmake` in order to get debugging flags passed to compiler. This should also happen if you just run `cmake ..` with no flags, but I think I have noticed it does not always works as expected (probably has something to do with cmake cache). To check which flags is compiler using, run `make` with `VERBOSE=1`: `make VERBOSE=1`.

## Credits
The original CRPLib, and near completition of the CRP file format was done by Arushan . CRP file format description was released by Addict and Jesper-Juul Mortensen, and a special thanks to them as most of my work was based on theirs. 
