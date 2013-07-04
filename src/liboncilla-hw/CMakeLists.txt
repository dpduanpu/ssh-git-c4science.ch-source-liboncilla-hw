set(SRC_FILES xenomai-utils/NativeHolder.cpp
              xenomai-utils/NativeError.cpp
              config/MainSection.cpp
              config/MotorSection.cpp
              config/Config.cpp
              synchronization/Synchronizer.cpp
              synchronization/Queue.cpp
              synchronization/SBCPQueue.cpp
              synchronization/RBIOQueue.cpp
              HWOncillaBackend.cpp
              Common.cpp
              nodes/Trunk.cpp
              nodes/L0.cpp
              nodes/L1L2.cpp
              nodes/L3.cpp
              nodes/SupervisorL4.cpp
              nodes/SupervisorTrunk.cpp
              nodes/SupervisorWorld.cpp)

set(HDR_FILES xenomai-utils/NativeHolder.h
              xenomai-utils/NativeError.h
              config/MainSection.h
              config/MotorSection.h
              config/Config.h
              synchronization/Synchronizer.h
              synchronization/Queue.h
              synchronization/SBCPQueue.h
              synchronization/RBIOQueue.h
              HWOncillaBackend.h
              Common.h
              nodes/Nodes.h
              nodes/Trunk.h
              nodes/L0.h
              nodes/L1L2.h
              nodes/L3.h
              nodes/SupervisorL4.h
              nodes/SupervisorTrunk.h
              nodes/SupervisorWorld.h)

add_library(oncilla-hw MODULE ${SRC_FILES} ${HDR_FILES})
set_target_properties(oncilla-hw PROPERTIES LINK_FLAGS "-Wl,-unresolved-symbols=ignore-in-shared-libs")

#add_library(oncilla-hw-shared SHARED ${SRC_FILES} ${HDR_FILES})

target_link_libraries(oncilla-hw ${DEPS_LIBRARIES} ${XENOMAI_NATIVE_LIBRARIES})
#target_link_libraries(oncilla-hw-shared ${DEPS_LIBRARIES} ${XENOMAI_NATIVE_LIBRARIES})

install(TARGETS oncilla-hw DESTINATION lib)

#add_check_test(NAME liboncilla-hw-no-undef-sym
#               FILES liboncilla-hw-no-undef-sym-test.cpp
#			   LIBRARIES oncilla-hw-shared)


# We don't install headers, because no-one should implement against the
# hw backend, but against liboncilla instead.
