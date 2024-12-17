#!/usr/bin/env bash

cmake -Bbuild/debug -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cp build/debug/compile_commands.json .`

cmake -Bbuild/release -GNinja -DCMAKE_BUILD_TYPE=Release
