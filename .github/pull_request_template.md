## Summary

## Test Plan

## Checklist

- [ ] If this changes a header in `include/zce/` in a way modules built against the old header would break on (class layout, virtual functions, exported signatures, types packed across modules, state an inline function or template reads), `ZCE_ABI_VERSION` in `include/zce/zce_abi.h` is bumped and its history has a line for it.
- [ ] C/C++ sources and headers are UTF-8 with BOM.
