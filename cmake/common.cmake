macro(COPY_SHADERS target)
    file(GLOB Shaders ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
    add_custom_target(CopyShaders-${target} DEPENDS ${Shaders})
    foreach(Shader ${Shaders})
        get_filename_component(ShaderName ${Shader} NAME)
        set(Shader_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/${ShaderName})
        set(Shader_IN ${Shader})
        add_custom_command(TARGET CopyShaders-${target} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E
                copy ${Shader_IN} ${Shader_OUT})
    endforeach()
    add_dependencies(${target} CopyShaders-${target})
endmacro()