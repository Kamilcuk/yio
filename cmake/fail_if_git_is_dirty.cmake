

## @def fail_if_git_is_dirty
## Fails with FATAL_ERROR when current git working tree is dirty.
## Dirty meaning differst from index or from latest commit, anything.
## This is solely used in dashboard script, so that when it does 
## `git checkout` it will _not_ overwrite any precious files.
function(fail_if_git_is_dirty)
	execute_process(
		COMMAND git update-index --refresh
		WORKING_DIRECTORY ${CTEST_SOURCE_DIRECTORY}
		OUTPUT_VARIABLE output
		ERROR_VARIABLE error
		RESULT_VARIABLE ret
	)
	if(NOT ret EQUAL 0)
		message(FATAL_ERROR
			"fail_if_git_is_dirty: Current git tree is dirty. Bailing out from overwrite.\n"
			"Command git update-index --refresh run in ${CTEST_SOURCE_DIRECTORY}"
			"exited with nonzero exit status: ret=${ret} output=${output} error=${error}" 
		)
	endif()
	execute_process(
		COMMAND git diff-index --quiet HEAD --
		WORKING_DIRECTORY ${CTEST_SOURCE_DIRECTORY}
		OUTPUT_VARIABLE output
		ERROR_VARIABLE error
		RESULT_VARIABLE ret
	)
	if(NOT ret EQUAL 0)
		message(FATAL_ERROR
			"fail_if_git_is_dirty: Current git tree is dirty. Bailing out from overwrite.\n"
			"Command git diff-index HEAD -- failed in ${CTEST_SOURCE_DIRECTORY} "
			"Current git tree is dirty. Bailing out from overwrite. "
			"exited with nonzero exit status: ret=${ret} output=${output} error=${error}" 
		)
	endif()
endfunction()
