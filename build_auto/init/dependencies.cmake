cmake_minimum_required(VERSION 3.16)

include(FetchContent)
include("${CMAKE_CURRENT_LIST_DIR}/../util.cmake")

# helpers
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE "OPT_IN" CACHE STRING "https://cmake.org/cmake/help/latest/module/FetchContent.html?highlight=fetchcontent_try_find_package_mode#variable:FETCHCONTENT_TRY_FIND_PACKAGE_MODE")

function(make_excludable_available tests_appended_out_var option_or_name)
	if(option_or_name STREQUAL "EXCLUDE_FROM_ALL")
		set(_exclude_from_all "EXCLUDE_FROM_ALL")
	else()
		list(PREPEND ARGN "${option_or_name}")
		set(_exclude_from_all "")
	endif()

	foreach(_name IN LISTS ARGN)
		FetchContent_GetProperties("${_name}")
		string(TOLOWER "${_name}" _lc_name)

		if(NOT "${_lc_name}_POPULATED")
			FetchContent_Populate("${_name}")
			add_subdirectory("${${_lc_name}_SOURCE_DIR}" "${${_lc_name}_BINARY_DIR}" ${_exclude_from_all})

			if(_exclude_from_all STREQUAL "EXCLUDE_FROM_ALL")
				get_subdirectories_recursively(_directories "${${_lc_name}_SOURCE_DIR}")
				list(APPEND _directories "${${_lc_name}_SOURCE_DIR}")

				foreach(_directory IN LISTS _directories)
					get_directory_property(_tests DIRECTORY "${_directory}" TESTS)

					if(_tests)
						list(APPEND "${tests_appended_out_var}" ${_tests})
					endif()
				endforeach()
			endif()
		endif()
	endforeach()

	set("${tests_appended_out_var}" "${${tests_appended_out_var}}" PARENT_SCOPE)
endfunction()

function(find_package_or_fetch_content tests_appended_out_var package target target_options package_options)
	if(FETCHCONTENT_TRY_FIND_PACKAGE_MODE STREQUAL "OPT_IN")
		set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE "ALWAYS")
		message(WARNING "'FETCHCONTENT_TRY_FIND_PACKAGE_MODE' is treated as 'ALWAYS'")
	endif()

	if(FETCHCONTENT_TRY_FIND_PACKAGE_MODE STREQUAL "ALWAYS")
		set(_find_package true)
	elseif(FETCHCONTENT_TRY_FIND_PACKAGE_MODE STREQUAL "NEVER")
		set(_find_package false)
	else()
		message(FATAL_ERROR "'FETCHCONTENT_TRY_FIND_PACKAGE_MODE' has invalid value: ${FETCHCONTENT_TRY_FIND_PACKAGE_MODE}")
	endif()

	if(_find_package)
		list(REMOVE_ITEM package_options "REQUIRED")
		find_package("${package}" ${package_options})

		if("${package}_FOUND")
			return()
		endif()
	endif()

	list(LENGTH target_options _target_options_len)
	math(EXPR _target_options_len "${_target_options_len} - 1")

	foreach(_target_option_idx RANGE 0 "${_target_options_len}" 2)
		math(EXPR _target_option_val_idx "${_target_option_idx} + 1")
		list(GET target_options "${_target_option_idx}" _target_option)
		list(GET target_options "${_target_option_val_idx}" _target_option_val)
		set("${_target_option}" "${_target_option_val}")
	endforeach()

	list(FIND ARGN "EXCLUDE_FROM_ALL" _exclude_from_all)

	if(_exclude_from_all EQUAL -1)
		set(_exclude_from_all "")
	else()
		set(_exclude_from_all "EXCLUDE_FROM_ALL")
	endif()

	make_excludable_available("${tests_appended_out_var}" ${_exclude_from_all} "${package}")
	add_library("${package}::${target}" ALIAS "${target}")
	export(TARGETS "${target}" NAMESPACE "${package}::" FILE "${target}-targets.cmake")

	set("${tests_appended_out_var}" "${${tests_appended_out_var}}" PARENT_SCOPE)
endfunction()

# dependencies
include("${CMAKE_CURRENT_LIST_DIR}/../data/dependencies.cmake")
