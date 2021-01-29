if (NOT DEFINED DANG_ONCE)
	set(DANG_ONCE TRUE)

	function (dang_asset_magic ASSET_FILE PATH_TO_SEARCH)

		dang_generate_assetlist(${ASSET_FILE} ${PATH_TO_SEARCH})

		# file(TOUCH_NOCREATE ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_FILE})
		blit_assets_yaml (${PROJECT_NAME} ${ASSET_FILE})

	endfunction()

	function (dang_generate_assetlist ASSET_FILE PATH_TO_SEARCH)

		# make sure that xmllint is installed and available or the bash script to generate the asset.yml will fail
		find_program(XMLLINT NAMES xmllint DOC "make sure to have xmllint installed")
		if (DEFINED XMLLINT-NOTFOUND)
			message(FATAL_ERROR "xmllint not found!\nInstall with: apt get install libxml2-utils\n")
		endif()

		# get the inputs/outputs for the asset tool (at configure time)
		execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libs/DANG/tools/asset_generator.sh ${ASSET_FILE} ${PATH_TO_SEARCH})

	endfunction()

endif ()
