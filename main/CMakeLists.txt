file(GLOB_RECURSE SRC_FILES_CPP "src/*.cpp")
file(GLOB_RECURSE SRC_FILES_C "src/*.c")

idf_component_register(SRCS 
                    "weatherstation.cpp" 
                    "weather24.c"
                    "weather32.c"
                    "weather48.c"
                    ${SRC_FILES_CPP}
                    ${SRC_FILES_C}
                    INCLUDE_DIRS "./include")

spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)