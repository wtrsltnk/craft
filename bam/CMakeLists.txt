
include_directories(../libzynth/src ${LIBZYNTH_INCLUDE_DIRS})

add_executable(bam
    program.cpp
    )
    
target_link_libraries(bam
    libzynth
    )

