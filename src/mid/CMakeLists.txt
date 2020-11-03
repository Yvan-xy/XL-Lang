add_library(mid
        visitor/visitor.cpp
        walker/dumper.cpp
        )

target_compile_features(mid PUBLIC cxx_std_17)
add_library(RJIT::mid ALIAS mid)