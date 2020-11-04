add_library(front
        lexer.cpp
        token.cpp
        parser.cpp
        logger.cpp
        "${RJIT_SOURCE_DIR}/define/AST.cpp"
        "${RJIT_SOURCE_DIR}/define/type.cpp"
        )

target_compile_features(front PUBLIC cxx_std_17)
add_library(RJIT::front ALIAS front)