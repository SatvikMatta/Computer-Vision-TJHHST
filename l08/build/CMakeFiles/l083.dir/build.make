# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build"

# Include any dependencies generated for this target.
include CMakeFiles/l083.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/l083.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/l083.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/l083.dir/flags.make

CMakeFiles/l083.dir/l083.cpp.o: CMakeFiles/l083.dir/flags.make
CMakeFiles/l083.dir/l083.cpp.o: /Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My\ Drive/12th\ Grade/CV1/l08/l083.cpp
CMakeFiles/l083.dir/l083.cpp.o: CMakeFiles/l083.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/l083.dir/l083.cpp.o"
	/opt/homebrew/bin/aarch64-apple-darwin21-g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/l083.dir/l083.cpp.o -MF CMakeFiles/l083.dir/l083.cpp.o.d -o CMakeFiles/l083.dir/l083.cpp.o -c "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/l083.cpp"

CMakeFiles/l083.dir/l083.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/l083.dir/l083.cpp.i"
	/opt/homebrew/bin/aarch64-apple-darwin21-g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/l083.cpp" > CMakeFiles/l083.dir/l083.cpp.i

CMakeFiles/l083.dir/l083.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/l083.dir/l083.cpp.s"
	/opt/homebrew/bin/aarch64-apple-darwin21-g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/l083.cpp" -o CMakeFiles/l083.dir/l083.cpp.s

# Object files for target l083
l083_OBJECTS = \
"CMakeFiles/l083.dir/l083.cpp.o"

# External object files for target l083
l083_EXTERNAL_OBJECTS =

l083: CMakeFiles/l083.dir/l083.cpp.o
l083: CMakeFiles/l083.dir/build.make
l083: /opt/homebrew/lib/libopencv_gapi.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_stitching.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_alphamat.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_aruco.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_barcode.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_bgsegm.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_bioinspired.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_ccalib.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_dnn_objdetect.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_dnn_superres.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_dpm.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_face.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_freetype.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_fuzzy.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_hfs.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_img_hash.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_intensity_transform.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_line_descriptor.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_mcc.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_quality.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_rapid.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_reg.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_rgbd.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_saliency.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_sfm.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_stereo.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_structured_light.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_superres.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_surface_matching.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_tracking.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_videostab.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_viz.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_wechat_qrcode.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_xfeatures2d.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_xobjdetect.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_xphoto.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_shape.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_highgui.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_datasets.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_plot.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_text.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_ml.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_phase_unwrapping.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_optflow.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_ximgproc.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_video.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_videoio.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_imgcodecs.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_objdetect.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_calib3d.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_dnn.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_features2d.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_flann.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_photo.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_imgproc.4.6.0.dylib
l083: /opt/homebrew/lib/libopencv_core.4.6.0.dylib
l083: CMakeFiles/l083.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable l083"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/l083.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/l083.dir/build: l083
.PHONY : CMakeFiles/l083.dir/build

CMakeFiles/l083.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/l083.dir/cmake_clean.cmake
.PHONY : CMakeFiles/l083.dir/clean

CMakeFiles/l083.dir/depend:
	cd "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08" "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08" "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build" "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build" "/Users/satvik/Library/CloudStorage/GoogleDrive-1795710@fcpsschools.net/My Drive/12th Grade/CV1/l08/build/CMakeFiles/l083.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/l083.dir/depend

