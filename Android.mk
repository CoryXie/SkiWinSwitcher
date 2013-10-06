######################################
# Build the app.
######################################

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CERTIFICATE := platform

LOCAL_MODULE_TAGS := eng

LOCAL_SRC_FILES := \
        $(call all-java-files-under, src)

LOCAL_PACKAGE_NAME := SkiWinSwitcher

LOCAL_JNI_SHARED_LIBRARIES := libSkiWinSwitcher

include $(BUILD_PACKAGE)

######################################
# Build the shared library.
######################################

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := eng

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/jni

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libandroid_runtime 

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libSkiWinSwitcher

LOCAL_SRC_FILES := \
    jni/SkiWinSwitcher.c

include $(BUILD_SHARED_LIBRARY)
