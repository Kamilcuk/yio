
file(GLOB PYTHON_TESTS "*.py")

foreach(T_SOURCE IN LISTS PYTHON_TESTS)
    get_filename_component(T_NAME ${T_SOURCE} NAME_WLE)
    add_test(NAME python_${T_NAME}
        COMMAND "${PYTHON_EXE}" ${T_SOURCE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set_tests_properties(python_${T_NAME} PROPERTIES LABELS "nomemcheck;python")
endforeach()
