
#
# Sanitizers
#
find_package(Sanitizers)


#
# spdlog
#
set(LOG_NO_EXCEPTIONS ON)
set(LOG_NO_THREAD_ID ON)
# set(LOG_BUILD_PIC ON)
set(LOG_SANITIZE_ADDRESS ${SANITIZE_ADDRESS})
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)
