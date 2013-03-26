set(SRC_FILES )
set(HDR_FILES )

add_library(oncilla-hw MODULE ${SRC_FILES} ${HDR_FILES} )

target_link_libraries(oncilla-hw ${DEPS_LIBRARIES})

install(TARGETS oncilla-hw DESTINATION lib)
