
include_directories(../libzynth/src ${LIBZYNTH_INCLUDE_DIRS})

add_executable(whoosh
    whoosh.cpp
    )
    
target_link_libraries(whoosh
    libzynth
    )

