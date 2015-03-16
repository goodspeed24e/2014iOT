SET(NOTIFICATION_CENTER_SRC_LIST NotificationCenter.cpp NotificationSingleton.cpp)

ADD_LIBRARY(notification_center SHARED ${NOTIFICATION_CENTER_SRC_LIST})
#TARGET_LINK_LIBRARIES(notification_center ???)
