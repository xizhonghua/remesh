all:
	clang++ main.cpp -I/opt/local/include -L/opt/local/lib -lboost_system-mt -lgmp -lcgal -o remesh -O2