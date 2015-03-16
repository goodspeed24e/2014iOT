SET(TRANSPORT_SRC_LIST rpc_session.cpp method.cpp)

ADD_LIBRARY(transport SHARED ${TRANSPORT_SRC_LIST})
TARGET_LINK_LIBRARIES(transport common boost_iostreams)
