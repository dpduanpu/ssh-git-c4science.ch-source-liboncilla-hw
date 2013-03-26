set(SRC_FILES HWOncillaBackend.cpp)
set(HDR_FILES HWOncillaBackend.h
              Common.h
			  nodes/Nodes.h)

add_library(oncilla-hw MODULE ${SRC_FILES} ${HDR_FILES} )

target_link_libraries(oncilla-hw ${DEPS_LIBRARIES})

install(TARGETS oncilla-hw DESTINATION lib)
