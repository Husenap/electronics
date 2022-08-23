set(target_name bcm2835)

set(src_files
  bcm2835/bcm2835.c
  bcm2835/bcm2835.h)

add_library(bcm2835 SHARED ${src_files})
target_include_directories(bcm2835 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS bcm2835 DESTINATION lib)
install(FILES bcm2835.h DESTINATION include/bcm2835)