set(SRC_FILES HWOncillaBackend.cpp
              Synchronizer.cpp
              nodes/Trunk.cpp
              nodes/L0.cpp
              nodes/L1L2.cpp
              nodes/L3.cpp
              nodes/SupervisorL4.cpp
              nodes/SupervisorTrunk.cpp
              nodes/SupervisorWorld.cpp
              nodes/DeepCopyable.cpp
              adapters/BrushlessMotorPosition.cpp
			  adapters/Servo.cpp
			  adapters/MagneticEncoder.cpp)

set(HDR_FILES HWOncillaBackend.h
              Synchronizer.h
              Common.h
              nodes/Nodes.h
              nodes/Trunk.h
              nodes/L0.h
              nodes/L1L2.h
              nodes/L3.h
              nodes/SupervisorL4.h
              nodes/SupervisorTrunk.h
              nodes/SupervisorWorld.h
              nodes/DeepCopyable.h
              adapters/Adapters.h
              adapters/BrushlessMotorPosition.h
			  adapters/Servo.h
			  adapters/MagneticEncoder.h)

add_library(oncilla-hw MODULE ${SRC_FILES} ${HDR_FILES})

target_link_libraries(oncilla-hw ${DEPS_LIBRARIES})

install(TARGETS oncilla-hw DESTINATION lib)

# We don't install headers, because no-one should implement against the
# hw backend, but against liboncilla instead.
