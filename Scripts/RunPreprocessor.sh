#!/bin/sh
echo Running preprocessor...
cd ../Tools/Preprocessor/bin/Release/net8.0/linux-x64/
./Preprocessor "../../../../../../" Sibox $1
