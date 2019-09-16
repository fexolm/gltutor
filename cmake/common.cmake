macro(COPY_SHADERS target)
    file(GLOB Shaders ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag)
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

macro(COPY_MESHES target)
    file(GLOB Meshes ${CMAKE_CURRENT_SOURCE_DIR}/meshes/*.xml)
    add_custom_target(CopyMeshes-${target} DEPENDS ${Meshes})
    foreach(Mesh ${Meshes})
        get_filename_component(MeshName ${Mesh} NAME)
        set(Mesh_OUT ${CMAKE_CURRENT_BINARY_DIR}/meshes/${MeshName})
        set(Mesh_IN ${Mesh})
        add_custom_command(TARGET CopyMeshes-${target} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E
                copy ${Mesh_IN} ${Mesh_OUT})
    endforeach()
    add_dependencies(${target} CopyMeshes-${target})
endmacro()