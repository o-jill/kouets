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
    if [$? = 0]; then
        echo "ALL tests finished successfully!! (${?})"
    else
        echo "some tests failed ... (${?})"
    fi
else
    echo "normal  build"
    qmake -qt=qt4 -v ./src/kouets.pro
    ./src/gen_git_hash.sh H kouetshash
    ./src/gen_git_hash.sh CPP kouetshash
    qmake -qt=qt4 ./src/kouets.pro
    make
fi
