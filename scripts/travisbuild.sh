#!/bin/sh

# a build script for travis ci

if [ "${TRAVIS_BUILD_TYPE}" = "test" ]; then
    echo "let us TEST !!"
    cd src/test
    qmake -qt=qt4 ./test.pro
#    make clean
#    make mocclean
    make
    mkdir debug
    cp test debug/test
    ./debug/test
    exitcode=$?
    if [ ${exitcode} = 0 ]; then
        echo "ALL tests finished successfully!! (${?})"
    else
        echo "some tests failed ... (${?})"
    fi
    exit ${exitcode}
elif [ "${TRAVIS_BUILD_TYPE}" = "testclang" ]; then
    echo "let us TEST with clang!!"
    cd src/test
    qmake -qt=qt4 ./test.pro -spec unsupported/linux-clang
#    qmake -qt=qt4 ./test.pro -spec linux-llvm
    make
    mkdir debug
    cp test debug/test
    ./debug/test
    exitcode=$?
    if [ ${exitcode} = 0 ]; then
        echo "ALL tests finished successfully!! (${?})"
    else
        echo "some tests failed ... (${?})"
    fi
    exit ${exitcode}
elif [ "${TRAVIS_BUILD_TYPE}" = "clang" ]; then
    echo "clang build"
    qmake -qt=qt4 -v ./src/kouets.pro -spec unsupported/linux-clang
    ./src/gen_git_hash.sh H kouetshash
    ./src/gen_git_hash.sh CPP kouetshash
    qmake -qt=qt4 ./src/kouets.pro -spec unsupported/linux-clang
    make
else
    echo "gcc build"
    qmake -qt=qt4 -v ./src/kouets.pro
    ./src/gen_git_hash.sh H kouetshash
    ./src/gen_git_hash.sh CPP kouetshash
    qmake -qt=qt4 ./src/kouets.pro
    make
fi

