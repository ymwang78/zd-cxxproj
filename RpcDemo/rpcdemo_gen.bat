rem ===========================================================================================

cd src
zgen -t header_zdp -f * -I rpcdemo_inc.h -i rpcdemo_config.ptl -o rpcdemo_config_proto
zgen -t zxml -f * -I rpcdemo_inc.h -I rpcdemo_config_proto.h -I rpcdemo_config_pack.h -i rpcdemo_config.ptl -o rpcdemo_config
cd ..

cd demo

zgen -t zpy -f * -i rpcdemo.ptl -o rpcdemo
zgen -t header_zds -f * -I rpcdemo_inc.h -i rpcdemo.ptl -o rpcdemo_proto
zgen -t cpp_zds -f * -I rpcdemo_inc.h -I rpcdemo_proto.h -I rpcdemo_pack.h -i rpcdemo.ptl -o rpcdemo
move /y rpcdemo*.cpp ..\src
move /y rpcdemo*.h ..\src
cd ..

pause