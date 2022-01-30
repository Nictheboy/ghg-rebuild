cc = g++
prom = ghg
header = $(shell find ./src -maxdepth 1 -name "*.h")
src = $(shell find ./src -maxdepth 1 -name "*.cpp")
object_code = code.cpp
$(prom): FORCE
	cat $(header) $(src) > $(object_code)
	$(cc) -o $(prom) $(object_code)
	rm $(object_code)
FORCE:;
