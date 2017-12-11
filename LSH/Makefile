# In  order  to  execute  this  "Makefile " just  type  "make "
OBJS     = main.o Hamming.o Euclidean.o Cosine.o Hashtable.o Heuristic.o List.o Node.o Random.o Validation.o ValidationList.o 				ValidationNode.o Space.o
SOURCE   = main.cpp Hamming.cpp Euclidean.cpp Cosine.cpp Hashtable.cpp Heuristic.cpp List.cpp Node.cpp Random.cpp Validation.cpp 				ValidationList.cpp ValidationNode.cpp Space.cpp
HEADER   = Hamming.h Euclidean.h Cosine.h Hashtable.h Heuristic.h List.h Node.h Random.h Validation.h ValidationList.h 					ValidationNode.h Space.h
OUT      = recommendation
CC       = g++
FLAGS    = -g -c
# -g  option  enables  debugging  mode
# -c flag  generates  object  code  for  separate  files
all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)
#  create/ compile  the  individual  files  >> separately <<

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Space.o: Space.cpp Space.h
	$(CC) $(FLAGS) Space.cpp

Hamming.o: Hamming.cpp Hamming.h
	$(CC) $(FLAGS) Hamming.cpp

Euclidean.o: Euclidean.cpp Euclidean.h
	$(CC) $(FLAGS) Euclidean.cpp

Cosine.o: Cosine.cpp Cosine.h
	$(CC) $(FLAGS) Cosine.cpp

Hashtable.o: Hashtable.cpp Hashtable.h
	$(CC) $(FLAGS) Hashtable.cpp

Heuristic.o: Heuristic.cpp Heuristic.h
	$(CC) $(FLAGS) Heuristic.cpp

List.o: List.cpp List.h
	$(CC) $(FLAGS) List.cpp

Node.o: Node.cpp Node.h
	$(CC) $(FLAGS) Node.cpp

Random.o: Random.cpp Random.h
	$(CC) $(FLAGS) Random.cpp

Validation.o: Validation.cpp Validation.h
	$(CC) $(FLAGS) Validation.cpp

ValidationList.o: ValidationList.cpp ValidationList.h
	$(CC) $(FLAGS) ValidationList.cpp

ValidationNode.o: ValidationNode.cpp ValidationNode.h
	$(CC) $(FLAGS) ValidationNode.cpp



#  clean  house
clean :
	rm -f $(OBJS) $(OUT)
