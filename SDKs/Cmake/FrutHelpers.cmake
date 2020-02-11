cmake_minimum_required(VERSION 3.4)

include(GitHelpers)

function(configure_frut frutDirectory)
    execute_process(COMMAND
            "cmake"
            "."
            "-DJUCE_ROOT=../JUCE"
            "-DCMAKE_BUILD_TYPE=Release"

            WORKING_DIRECTORY ${frutDirectory})
endfunction()

function(build_reprojucer frutDirectory)
    execute_process(COMMAND
            "cmake"
            "--build"
            "."
            "--target" "Jucer2Reprojucer"
            "--config" "Release"

            WORKING_DIRECTORY ${frutDirectory})
endfunction()

function(get_reprojucer_exe reprojucerFile)

    set(frutDir "${CMAKE_SOURCE_DIR}/SDKs/FRUT")

    if (APPLE)
        set(${reprojucerFile} "${frutDir}/Jucer2Reprojucer/Jucer2Reprojucer" PARENT_SCOPE)
    else ()
        message("Remember to set up the build for non-apple systems")
    endif ()

endfunction()

function(build_frut frutDir)
    configure_frut(${frutDir})
    build_reprojucer(${frutDir})
endfunction()

function(get_reprojuer_file result)

endfunction()

macro(update_frut)
    set(frutGit "https://github.com/McMartin/FRUT.git")
    set(frutDir "${PROJECT_SOURCE_DIR}/FRUT")

    update_git(${frutGit} "FRUT")
    build_frut(${frutDir})
endmacro()

function(create_cmake jucerFile jucerFilePath)

    set(frutCMake "${frutDir}/Reprojucer.cmake")
    set(frutDir "${PROJECT_SOURCE_DIR}/FRUT")
    get_reprojucer_exe(jucerEXE)

    execute_process(COMMAND
            ${jucerEXE}
            ${jucerFile}
            WORKING_DIRECTORY ${jucerFilePath} RESULT_VARIABLE rv)
    message("rv='${rv}'")
endfunction()

function(create_cmake_in_dir targetDir)
    file(GLOB files "${targetDir}/*.jucer")

    foreach (jucerFile ${files})
        create_cmake(${jucerFile} ${targetDir})
    endforeach ()
endfunction()

function(create_cmake_in_subdirs targetDir)
    sub_dir_list(subDirs ${targetDir})

    foreach (subDir ${subDirs})
        set(absoluteSubDir "${targetDir}/${subDir}")
        create_cmake_in_dir(${absoluteSubDir})
    endforeach ()
endfunction()
