add_executable(obelisk_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StringFuncTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerMakerTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerUtilTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ArrayParamCalcTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StackVectorTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MemoizedValueTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OptionalTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/HeapArrayTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MultiVectorTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OctreeTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImageFilterTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MutexedObjectTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LockUtilsTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ColourStructsTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConcurrentQueueTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SimpleAsyncTaskTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ObserverPatternCallbackTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BackgroundContainerProducerTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BackgroundThreadDeleterTests.cpp
)

include(${CMAKE_SOURCE_DIR}/cmake/obelisk-config-internal.cmake)

obelisk_config_internal(obelisk_tests)