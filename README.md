Some doc.

# Building

TODO: Is it `cdk.h` or `cdk/cdk.h`?  Use `--with-cdk-dir`.

## Linux

    sudo apt-get install libcdk5-dev

## FreeBSD

    autoreconf
    CXX=clang ./configure --with-cdk-dir=/usr/local
    make
    src/ocher --help

## Other

