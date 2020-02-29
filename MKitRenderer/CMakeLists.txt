cmake_minimum_required(VERSION 3.4.1)

set(MAXSTAR_DIR ${PROJECT_SOURCE_DIR}/../3rdParty/MaxstAR)
set(GAME_PLAY3D_DIR ${PROJECT_SOURCE_DIR}/../3rdParty/gameplay3d)
set(GAME_PLAY3D_DEPS_DIR ${PROJECT_SOURCE_DIR}/../3rdParty/gameplay3d-deps)

set(OPENCV_DIR ${PROJECT_SOURCE_DIR}/../3rdParty/opencv-android-sdk)

set(MKIT_RENDERER_DIR ${PROJECT_SOURCE_DIR}/../Renderer)

set(MKIT_RENDERER_SRCS
		${MKIT_RENDERER_DIR}/AndroidHelper.cpp
		${MKIT_RENDERER_DIR}/AndroidVideoPlayer.cpp
		${MKIT_RENDERER_DIR}/BackgroundCameraQuad.cpp
		${MKIT_RENDERER_DIR}/BackgroundYuvCameraQuad.cpp
		${MKIT_RENDERER_DIR}/Layout1FormNode.cpp
		${MKIT_RENDERER_DIR}/Layout2FormNode.cpp
		${MKIT_RENDERER_DIR}/Layout3FormNode.cpp
		${MKIT_RENDERER_DIR}/ChildLayoutFormNode.cpp
		${MKIT_RENDERER_DIR}/IconLayoutFormNode.cpp
		${MKIT_RENDERER_DIR}/TagLayoutFormNode.cpp
		${MKIT_RENDERER_DIR}/TextLayoutFormNode.cpp
		${MKIT_RENDERER_DIR}/MKitHelper.cpp
		${MKIT_RENDERER_DIR}/MKitJni.cpp
		${MKIT_RENDERER_DIR}/MKitRendererManager.cpp
		${MKIT_RENDERER_DIR}/FormNode.cpp
		${MKIT_RENDERER_DIR}/VideoNode.cpp
		${MKIT_RENDERER_DIR}/ImageNode.cpp
		${MKIT_RENDERER_DIR}/QuadNode.cpp
		${MKIT_RENDERER_DIR}/SystemUtil.cpp
		${MKIT_RENDERER_DIR}/Utilities.cpp
		${MKIT_RENDERER_DIR}/VideoLayoutFormNode.cpp
		${MKIT_RENDERER_DIR}/MonoRenderer.cpp
		${MKIT_RENDERER_DIR}/StereoRenderer.cpp
		)

file(GLOB MKIT_RENDERER_SUB_SRCS
		${MKIT_RENDERER_DIR}/filter/*.cpp
		${MKIT_RENDERER_DIR}/utility/*.cpp
		${MKIT_RENDERER_DIR}/model/*.cpp
		${MKIT_RENDERER_DIR}/control/*.cpp
		${MKIT_RENDERER_DIR}/node/*.cpp
		)

add_library(MKitRenderer SHARED ${MKIT_RENDERER_SRCS} ${MKIT_RENDERER_SUB_SRCS})

find_library(android-lib android)
find_library(log-lib log)
find_library(z-lib z)
find_library(dl-lib dl)


include_directories(
		${MAXSTAR_DIR}/include
		${GAME_PLAY3D_DIR}/include
		${GAME_PLAY3D_DEPS_DIR}/include
		${OPENCV_DIR}/sdk/native/jni/include
		${MKIT_RENDERER_DIR}
)

#MAXST AR SDK SETTING
add_library(ar_sdk SHARED IMPORTED)
set_target_properties(ar_sdk PROPERTIES IMPORTED_LOCATION ${MAXSTAR_DIR}/lib/${ANDROID_ABI}/libMaxstAR.so)

#GAMEPLAY3D SETTING
add_library(gameplay3d SHARED IMPORTED)
set_target_properties(gameplay3d PROPERTIES IMPORTED_LOCATION ${GAME_PLAY3D_DIR}/lib/${ANDROID_ABI}/libgameplay.so)

#add_library(gameplay-deps STATIC IMPORTED)
#set_target_properties(gameplay-deps PROPERTIES IMPORTED_LOCATION ${GAME_PLAY3D_DEPS_DIR}/lib/${ANDROID_ABI}/libgameplay-deps.a)

#OPENCV SETTINGS
add_library(opencv SHARED IMPORTED)
set_target_properties(opencv PROPERTIES IMPORTED_LOCATION ${OPENCV_DIR}/sdk/native/libs/${ANDROID_ABI}/libopencv_java4.so)

#LINK_LIBRARIES
target_link_libraries( # Specifies the target library.
		MKitRenderer

		#gameplay-deps
		ar_sdk
		gameplay3d
        opencv
		${android-lib} ${log-lib} ${z-lib} ${dl-lib}
		)

add_custom_command(TARGET MKitRenderer POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${GAME_PLAY3D_DIR}/lib/${ANDROID_ABI}/libgameplay.so
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libgameplay.so

		COMMAND ${CMAKE_COMMAND} -E copy
		${MAXSTAR_DIR}/lib/${ANDROID_ABI}/libMaxstAR.so
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libMaxstAR.so

		COMMAND ${CMAKE_COMMAND} -E copy
		${MAXSTAR_DIR}/lib/${ANDROID_ABI}/libVideoPlayer.so
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libVideoPlayer.so

        COMMAND ${CMAKE_COMMAND} -E copy
        ${OPENCV_DIR}/sdk/native/libs/${ANDROID_ABI}/libopencv_java4.so
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libopencv_java4.so
		)