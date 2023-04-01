calculator: calculator.o main.o calculator.hpp
	clang++ calculator.o main.o -o calculator

main.o: calculator.o calculator.hpp
	clang++ -c main.cpp

calculator.o: calculator.cpp calculator.hpp
	clang++ -c calculator.cpp

clean:
	rm *.o calculator
