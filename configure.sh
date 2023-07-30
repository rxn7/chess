#!/usr/bin/env bash

cmake -Bbin/debug -GNinja -DCMAKE_BUILD_TYPE=Debug
cmake -Bbin/release -GNinja -DCMAKE_BUILD_TYPE=Release
