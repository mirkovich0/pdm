Test.exe: test.cpp ../Drivers/API/Src/crc8.c  ../Drivers/API/Inc/crc8.h 
	g++ test.cpp ../Drivers/API/Src/crc8.c -I ../Drivers/API/Inc/ -o Test.exe

Test: Test.exe
	./Test.exe

clean:
	rm Test.exe
