SET(COMMON_SRC_LIST service_engine.cpp logger_linux.cpp properties.cpp properties_value_general_limitation.cpp xmlrpc_datetime.cpp)
#SET(COMMON_SRC_LIST service_engine.cpp settings.cpp logger_windows.cpp)

ADD_LIBRARY(common SHARED ${COMMON_SRC_LIST})
TARGET_LINK_LIBRARIES(common ${Boost_LIBRARIES})
