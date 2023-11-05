#!/bin/sh
export SLICE=/usr/share/ice/slice
export HOME=~/cxxproj
export RVSLICE=$HOME/vsvr/libsrc

git pull

cd vsvr
git pull
cd $HOME

cd libsrc/libzce
git pull
cd $HOME

cd libsrc/libzce/zua
slice2cpp -I$SLICE -I.. --include-dir zua lua_rpc.ice
cd $HOME

cd vsvr/libsrc/ice
slice2cpp -I$SLICE -I.. rv_base.ice
slice2cpp -I$SLICE -I.. rv_coin.ice
slice2cpp -I$SLICE -I.. rv_contact.ice
slice2cpp -I$SLICE -I.. rv_dataservice.ice
slice2cpp -I$SLICE -I.. rv_dbs.ice
slice2cpp -I$SLICE -I.. rv_feeds.ice
slice2cpp -I$SLICE -I.. rv_game.ice
slice2cpp -I$SLICE -I.. rv_gateserver.ice
slice2cpp -I$SLICE -I.. rv_gdb.ice
slice2cpp -I$SLICE -I.. rv_gdb2.ice
slice2cpp -I$SLICE -I.. rv_mss.ice
slice2cpp -I$SLICE -I.. rv_songlist.ice
slice2cpp -I$SLICE -I.. rv_storm_ktv.ice
slice2cpp -I$SLICE -I.. rv_zhuan.ice
slice2cpp -I$SLICE -I.. vx_dbs.ice
cd $HOME

cd vsvr/vxice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. alarmbase.ice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. mssbase.ice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. nmsbase.ice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. nmsserver.ice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. ssumss.ice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. vx_cms.ice
slice2cpp -I$SLICE -I$RVSLICE -I. -I.. vx_mds.ice
cd $HOME

cd libsrc/libzce/cmake
cmake ..
cmake --build .
mv libzce.a $HOME/bin/release
cd $HOME

cd vsvr/rvdbs
scons
mv rvdbs $HOME/bin/release
scons -c
cd $HOME

cd vsvr/vxcoin
scons
mv vxcoin $HOME/bin/release
scons -c
cd $HOME

cd vsvr/rvgam
scons
mv rvgam $HOME/bin/release
scons -c
cd $HOME

cd vsvr/vxdbs
scons
mv vxdbs $HOME/bin/release
scons -c
cd $HOME

cd vsvr/vxmds
scons
mv vxmds $HOME/bin/release
scons -c
cd $HOME

cd vsvr/vxmss
scons
mv vxmss $HOME/bin/release
scons -c
cd $HOME

cd vsvr/vxrom
scons
mv vxrom $HOME/bin/release
scons -c
cd $HOME

cd vsvr/vxuas
scons
mv vxuas $HOME/bin/release
scons -c
cd $HOME

cd vsvr/zdcard
scons
mv zdcard $HOME/bin/release
scons -c
cd $HOME

cd vsvr/zdgame
scons
mv zdgame $HOME/bin/release
scons -c
cd $HOME

cd vsvr/zdgate
scons
mv zdgate $HOME/bin/release
scons -c
cd $HOME

cd vsvr/zdims
scons
mv zdims $HOME/bin/release
scons -c
cd $HOME
