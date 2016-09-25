# Yet another FlappyBird
## Build
### Android
#### Requires
 - Android SDK
 - Android NDK

To build an unsigned .apk:
```
./gradlew assembleRelease
```
To build a signed .apk:
```
./gradlew assembleRelease \
	-Pandroid.injected.signing.store.file=$KEYSTORE \
	-Pandroid.injected.signing.store.password=$STORE_PASSWORD \
	-Pandroid.injected.signing.key.alias=$KEY_ALIAS \
	-Pandroid.injected.signing.key.password=$KEY_PASSWORD
```
.apk file will be inside the project app/build/outputs/apk/ directory.

### Desktop
#### Requires
 - CMake 
 - GLEW 
 - GLFW3

```
mkdir build
cd build
cmake -DTARGET_PLATFORM=Desktop ../app/src/main/cpp
make
./flappybird
```

## License
MIT
