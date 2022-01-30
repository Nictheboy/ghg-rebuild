cc = g++
prom = ghg
object_code = code.cpp
header = $(shell find ./src -maxdepth 1 -name "*.h")
#hpp = $(shell find ./src -maxdepth 1 -name "*.hpp")
src = $(shell find ./src -maxdepth 1 -name "*.cpp")
$(prom): FORCE
	cat $(header) $(src) > src/$(object_code)
	$(cc) -o $(prom) src/$(object_code)
	rm src/$(object_code)
	./$(prom)
FORCE:;
