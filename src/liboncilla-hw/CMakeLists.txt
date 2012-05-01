set(SRC_FILES OncillaHWSupervisor.cpp)
set(HDR_FILES OncillaHWSupervisor.h)

add_library(oncilla-hw SHARED ${SRC_FILES} ${HDR_FILES} )

target_link_libraries(oncilla-hw ${SBCPD_LIBRARIES} ${LIBONCILLA_LIBRARIES}
${BIOROB_RBIO_LIBRARIES})


install(TARGETS oncilla-hw DESTINATION lib)


install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/liboncilla-hw
        DESTINATION ${INCLUDE_DIR}
        FILES_MATCHING PATTERN "*.h"
                       PATTERN "CMakeFiles*" EXCLUDE)