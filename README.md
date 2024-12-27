### What did i do:

1. Prepare googletest & google benchmark
2. thread safe vector test, reference [三种std::vector并发安全的设计思路](https://segmentfault.com/a/1190000041334904)
3. Dictionary, queue + id_map impl


### Additional infos

1. vcpkg related config in CMakeLists.txt

```shell
-DVCPKG_TARGET_TRIPLET=arm64-osx
-DCMAKE_TOOLCHAIN_FILE=${vcpkg-install-position}/scripts/buildsystems/vcpkg.cmake
-G Ninja
-DCMAKE_BUILD_TYPE=Debug
-DCMAKE_MAKE_PROGRAM=${generate-by-clion-by-default}
```

2. cross-platform debug using docker container with CLion

> Debugging C++ in a Docker Container with CLion https://github.com/shuhaoliu/docker-clion-dev

```shell
docker build -t docker_clion_dev .
docker-compose up -d
docker-compose down
```