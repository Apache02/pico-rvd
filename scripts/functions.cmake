function(add_deploy_target TARGET PATH_UF2)
    add_custom_target("${TARGET}---deploy"
            DEPENDS ${TARGET}
            DEPENDS ${PATH_UF2}
            COMMAND ${CMAKE_SOURCE_DIR}/scripts/deploy.sh ${PATH_UF2}
            )
endfunction()

