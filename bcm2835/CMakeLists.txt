add_library(bcm2835 SHARED bcm2835.c)
target_include_directories(bcm2835 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS bcm2835 DESTINATION lib)
install(FILES bcm2835.h DESTINATION include/bcm2835)