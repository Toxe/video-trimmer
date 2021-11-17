# enable testing with CTest
include(CTest)

# Catch2 v3
include(FetchContent)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG devel)
FetchContent_MakeAvailable(Catch2)
