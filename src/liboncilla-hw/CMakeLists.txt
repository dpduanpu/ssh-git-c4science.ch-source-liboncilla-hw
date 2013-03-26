set(SRC_FILES HWOncillaBackend.cpp
              nodes/L0.cpp
              nodes/L1.cpp
              nodes/L2.cpp
              nodes/L3.cpp)
set(HDR_FILES HWOncillaBackend.h
              Common.h
              nodes/Nodes.h
              nodes/L0.h
              nodes/L1.h
              nodes/L2.h
              nodes/L3.h)

add_library(oncilla-hw MODULE ${SRC_FILES} ${HDR_FILES})

target_link_libraries(oncilla-hw ${DEPS_LIBRARIES})

install(TARGETS oncilla-hw DESTINATION lib)

# We don't install headers, because no-one should implement against the
# hw backend, but against liboncilla instead.
