LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := 						\
	$(LOCAL_PATH)/$(SDL_PATH)/include 		\
	$(LOCAL_PATH)/../SDL_image 				\
	$(LOCAL_PATH)/../SDL_mixer

LOCAL_CFLAGS := \
	-DSDL_NO_COMPAT

# currently not supporting Android: source goes here
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
  main.cpp


LOCAL_SHARED_LIBRARIES := SDL SDL_image SDL_mixer

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
