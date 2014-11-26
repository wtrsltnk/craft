
add_subdirectory(libzynth)

add_executable(whoosh
    whoosh.cpp
    )
    
target_link_libraries(whoosh
    libzynth
    )

