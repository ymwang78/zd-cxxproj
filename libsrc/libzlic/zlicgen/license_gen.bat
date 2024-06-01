rem ===========================================================================================
zgen -t header_zds -f * -I license.h -i license.ptl -o license_proto
zgen -t cpp_zds -f * -I license.h -I license_proto.h -I license_pack.h -i license.ptl -o license


