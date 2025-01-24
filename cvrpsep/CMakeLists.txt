add_library(cvrpsep STATIC
    cvrpsep/basegrph.h cvrpsep/basegrph.cpp
    cvrpsep/binpack.h cvrpsep/BinPack.cpp
    cvrpsep/blocks.h cvrpsep/blocks.cpp
    cvrpsep/brnching.h cvrpsep/brnching.cpp
    cvrpsep/capsep.h cvrpsep/capsep.cpp
    cvrpsep/cnstrmgr.h cvrpsep/cnstrmgr.cpp
    cvrpsep/combsep.h cvrpsep/combsep.cpp
    cvrpsep/compcuts.h cvrpsep/compcuts.cpp
    cvrpsep/compress.h cvrpsep/compress.cpp
    cvrpsep/cutbase.h cvrpsep/cutbase.cpp
    cvrpsep/fcapfix.h cvrpsep/fcapfix.cpp
    cvrpsep/fcisep.h cvrpsep/fcisep.cpp
    cvrpsep/fcits.h cvrpsep/fcits.cpp
    cvrpsep/glmsep.h cvrpsep/glmsep.cpp
    cvrpsep/grsearch.h cvrpsep/grsearch.cpp
    cvrpsep/hpmstar.h cvrpsep/hpmstar.cpp
    cvrpsep/htoursep.h cvrpsep/htoursep.cpp
    cvrpsep/intap.h cvrpsep/intap.cpp
    cvrpsep/memmod.h cvrpsep/memmod.cpp
    cvrpsep/mstarsep.h cvrpsep/mstarsep.cpp
    cvrpsep/mxf.h cvrpsep/mxf.cpp
    cvrpsep/newhtour.h cvrpsep/newhtour.cpp
    cvrpsep/sort.h cvrpsep/sort.cpp
    cvrpsep/strcomb.h cvrpsep/strcomb.cpp
    cvrpsep/strngcmp.h cvrpsep/strngcmp.cpp
    cvrpsep/twomatch.h cvrpsep/twomatch.cpp
)

target_include_directories(cvrpsep PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cvrpsep)
