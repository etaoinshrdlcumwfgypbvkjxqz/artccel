include(FetchContent)

# helpers
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE "OPT_IN" CACHE STRING "https://cmake.org/cmake/help/latest/module/FetchContent.html?highlight=fetchcontent_try_find_package_mode#variable:FETCHCONTENT_TRY_FIND_PACKAGE_MODE")

function(find_package_or_fetch_content PACKAGE_NAME TARGET_NAME TARGET_OPTIONS PACKAGE_OPTIONS)
	if(FETCHCONTENT_TRY_FIND_PACKAGE_MODE STREQUAL "OPT_IN")
		message(WARNING "'FETCHCONTENT_TRY_FIND_PACKAGE_MODE' is treated as 'ALWAYS'")
		set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE "ALWAYS")
	endif()

	if(FETCHCONTENT_TRY_FIND_PACKAGE_MODE STREQUAL "ALWAYS")
		list(REMOVE_ITEM PACKAGE_OPTIONS "REQUIRED")
		find_package("${PACKAGE_NAME}" ${PACKAGE_OPTIONS})

		if(NOT "${PACKAGE_NAME}_FOUND")
			set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE "NEVER")
		endif()
	endif()

	if(FETCHCONTENT_TRY_FIND_PACKAGE_MODE STREQUAL "NEVER")
		list(LENGTH TARGET_OPTIONS TARGET_OPTIONS_LENGTH)
		math(EXPR TARGET_OPTIONS_LENGTH "${TARGET_OPTIONS_LENGTH} - 1")

		foreach(TARGET_OPTION_INDEX RANGE 0 "${TARGET_OPTIONS_LENGTH}" 2)
			math(EXPR TARGET_OPTION_VALUE_INDEX "${TARGET_OPTION_INDEX} + 1")
			list(GET TARGET_OPTIONS "${TARGET_OPTION_INDEX}" TARGET_OPTION)
			list(GET TARGET_OPTIONS "${TARGET_OPTION_VALUE_INDEX}" TARGET_OPTION_VALUE)
			set("${TARGET_OPTION}" "${TARGET_OPTION_VALUE}")
		endforeach()

		FetchContent_MakeAvailable("${PACKAGE_NAME}")
		add_library("${PACKAGE_NAME}::${TARGET_NAME}" ALIAS "${TARGET_NAME}")
		export(TARGETS "${TARGET_NAME}" NAMESPACE "${PACKAGE_NAME}::" FILE "${TARGET_NAME}-targets.cmake")
	else()
		message(FATAL_ERROR "'FETCHCONTENT_TRY_FIND_PACKAGE_MODE' has invalid value: ${FETCHCONTENT_TRY_FIND_PACKAGE_MODE}")
	endif()
endfunction()

# dependencies
FetchContent_Declare(Microsoft.GSL
	GIT_REPOSITORY "https://github.com/microsoft/GSL.git"
	GIT_TAG "a3534567187d2edc428efd3f13466ff75fe5805c" # tag v4.0.0
	GIT_SHALLOW true
	GIT_PROGRESS true
	FIND_PACKAGE_ARGS
)
FetchContent_Declare(tl-expected
	GIT_REPOSITORY "https://github.com/TartanLlama/expected.git"
	GIT_TAG "3d741708b967b83ca1e2888239196c4a67f9f9b0" # tag v1.0.0
	GIT_SHALLOW true
	GIT_PROGRESS true
	FIND_PACKAGE_ARGS
)
find_package_or_fetch_content(Microsoft.GSL GSL "GSL_INSTALL;${ARTCCEL_INSTALL};GSL_TEST;${ARTCCEL_TEST}" "4.0.0;EXACT;REQUIRED;CONFIG")
find_package_or_fetch_content(tl-expected expected "EXPECTED_BUILD_TESTS;${ARTCCEL_TEST}" "1.0.0;EXACT;REQUIRED;CONFIG")
