# Note: if you get an error in the warnings.txt file that looks like this:
# Error: Failed to open temporary file /cygdrive/c/vis/sw/user-checkouts/bjackson/src/MinVR/build/docs/C:/vis/sw/user-checkouts/bjackson/src/MinVR/build/docs/doxygen_objdb_4320.tmp
# it is because you are using cygwin's /usr/bin/doxygen which doesn't play nicely with the non-cygwin version of cmake we use.
# Download a standalone version of doxygen and use that instead.

configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_BINARY_DIR}/docs/Doxyfile @ONLY)

add_custom_target(docs ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/docs/Doxyfile
                  COMMENT "Generating HTML Doxygen documentation" VERBATIM)
				  
install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/html/ DESTINATION "${MINVR_INSTALL_DIR}/docs/html")