# Сборка

```
git clone https://github.com/avoronkin/otus_c.git
cd otus_c/hw12-logger
mkdir build
cd build
cmake ..
cmake --build .
```

# Тест

```
cd otus_c/hw12-logger/build
./example -l error
./example -l debug
./example -l info
./example -l warning
./example -l error -f test.log
```
