# Autogenerated file, run tools/build/setup_cmake.py to regenerate

add_definitions(-DIMP_SWIG_WRAPPER)

include_directories(%(includepath)s)
link_directories(%(libpath)s)

GET_DIRECTORY_PROPERTY(includes INCLUDE_DIRECTORIES)

# SWIG doesn't need Python.h in its own include path
include_directories(${PYTHON_INCLUDE_DIRS})

# this is needed for some reason that I don't understand
set(swig_path %(swigpath)s ${IMP_SWIG_PATH})

file(STRINGS "${PROJECT_BINARY_DIR}/src/%(name)s_swig.deps" swigdeps)

set(wrap_py "${PROJECT_BINARY_DIR}/lib/IMP/%(name)s/__init__.py")
set(wrap_py_orig "${PROJECT_BINARY_DIR}/src/%(name)s_swig/IMP.%(name)s.py")
set(source ${PROJECT_BINARY_DIR}/src/%(name)s_swig/wrap.cpp
                          ${PROJECT_BINARY_DIR}/src/%(name)s_swig/wrap.h)

set(PATH_ARGS )
foreach(path ${includes})
  list(APPEND PATH_ARGS "--includepath=${path}")
endforeach(path)
foreach(path ${swig_path})
  list(APPEND PATH_ARGS "--swigpath=${path}")
endforeach(path)
add_custom_command(OUTPUT ${source} ${wrap_py} ${wrap_py_orig}
   COMMAND  python
            "${PROJECT_SOURCE_DIR}/tools/build/make_swig_wrapper.py"
            "--swig=${SWIG_EXECUTABLE}"
            "--module=%(name)s"
            ${PATH_ARGS}
   DEPENDS ${swigdeps} ${IMP_%(NAME)s_PYTHON_EXTRA_DEPENDENCIES}
   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
   COMMENT "Running swig on %(name)s")

add_library(_IMP_%(name)s MODULE ${source})

set_target_properties(_IMP_%(name)s PROPERTIES PREFIX "")

if(WIN32 AND NOT CYGWIN)
  set_target_properties(_IMP_%(name)s PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(_IMP_%(name)s
    imp_%(name)s
    "%(modules)s"
    "%(dependencies)s"
    ${IMP_SWIG_LIBRARIES}
  )

add_custom_target("imp_%(name)s_python" ALL DEPENDS ${source} _IMP_%(name)s
                  ${wrap_py}
                  %(other_pythons)s
)

set(IMP_%(NAME)s_PYTHON imp_%(name)s_python CACHE INTERNAL "" FORCE)

INSTALL(TARGETS _IMP_%(name)s DESTINATION ${CMAKE_INSTALL_PYTHONDIR})
# Install the original wrapper, since wrap_py is a symlink and install does
# not dereference symlinks
INSTALL(FILES ${wrap_py_orig}
        DESTINATION ${CMAKE_INSTALL_PYTHONDIR}/IMP/%(name)s
        RENAME __init__.py)
