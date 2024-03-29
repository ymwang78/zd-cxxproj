#!/bin/sh
export HOME=~/zd-cxxproj
$CUR=$PWD
cd $HOME
#cd libsrc/libzce/cmake
#cmake ..
#cmake --build .
#mv libzce.a $HOME/bin/release
#cd $HOME
cd libsrc/libzce
scons; scons install
cd $HOME

cd slot
scons; scons install
cd $HOME

cd $CUR
