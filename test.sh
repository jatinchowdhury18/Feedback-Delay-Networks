echo "Testing create scipt..."
$run create.sh TravisFDN

for dir in $(find * -maxdepth 0 -type d); do
    if [[ ! -e $dir/CMakeLists.txt ]]; then
        continue
    fi
    echo "Testing ${dir}..."
    (
        cd $dir
        if [[ ! -d build ]]; then
            mkdir build
        fi
        cd build/
        cmake ..
        cmake --build . --config Release
    )
done
