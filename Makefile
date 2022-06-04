page: slot_header.o page.o main_slotted_page.o
	g++ -c slot_header.cpp
	g++ -c page.cpp
	g++ -c main_slotted_page.cpp
	g++ -o ./page slot_header.o page.o main_slotted_page.o

btree: slot_header.o page.o btree.o main_btree.o
	g++ -c slot_header.cpp
	g++ -c page.cpp
	g++ -c main_btree.cpp
	g++ -c btree.cpp
	g++ -o ./btree slot_header.o page.o btree.o main_btree.o

clean:
	rm -f page
	rm -f btree
	rm -f *.o
