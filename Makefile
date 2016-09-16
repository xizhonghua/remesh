all: remesh
	

remesh: main.cpp
	clang++ main.cpp -O2 -o remesh \
		-I/opt/local/include \
		-L/opt/local/lib \
		-lboost_system-mt \
		-lboost_program_options-mt \
		-lgmp -lcgal 