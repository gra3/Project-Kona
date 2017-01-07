LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2

OPENAL_PATH := ../OpenAL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/../SDL2_image \
	$(LOCAL_PATH)/../SDL_ttf \
	$(LOCAL_PATH)/$(OPENAL_PATH)/include \
	$(LOCAL_PATH)/../libzip

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c
		
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -std=c++11

LOCAL_SRC_FILES += main.cpp \
				   SoundManager.cpp \
				   Control.cpp \
				   Button.cpp \
				   SoundBox.cpp \
				   Screen.cpp \
				   TestScreen.cpp \
				   TextRenderer.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf openal libzip

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
