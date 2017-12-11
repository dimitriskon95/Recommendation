# In  order  to  execute  this  "Makefile " just  type  "make "
OBJS     = main.o List.o Node.o Random.o Centroid.o Cluster.o ClusterCosine.o ClusterEuclidean.o ClusterHamming.o Space.o 				Validation.o ValidationList.o ValidationNode.o
SOURCE   = main.cpp List.cpp Node.cpp Random.cpp Centroid.cpp ClusterCosine.cpp ClusterEuclidean.cpp ClusterHamming.cpp Space.cpp 				Validation.cpp ValidationList.cpp ValidationNode.cpp
HEADER   = List.h Node.h Random.h Centroid.h Cluster.h ClusterCosine.h ClusterEuclidean.h ClusterHamming.h Space.h Validation.h 			ValidationList.h ValidationNode.h
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

List.o: List.cpp List.h
	$(CC) $(FLAGS) List.cpp

Node.o: Node.cpp Node.h
	$(CC) $(FLAGS) Node.cpp

Random.o: Random.cpp Random.h
	$(CC) $(FLAGS) Random.cpp

Centroid.o: Centroid.cpp Centroid.h
	$(CC) $(FLAGS) Centroid.cpp

Cluster.o: Cluster.cpp Cluster.h
	$(CC) $(FLAGS) Cluster.cpp

ClusterCosine.o: ClusterCosine.cpp ClusterCosine.h
	$(CC) $(FLAGS) ClusterCosine.cpp

ClusterEuclidean.o: ClusterEuclidean.cpp ClusterEuclidean.h
	$(CC) $(FLAGS) ClusterEuclidean.cpp

ClusterHamming.o: ClusterHamming.cpp ClusterHamming.h
	$(CC) $(FLAGS) ClusterHamming.cpp

Space.o: Space.cpp Space.h
	$(CC) $(FLAGS) Space.cpp 

Validation.o: Validation.cpp Validation.h
	$(CC) $(FLAGS) Validation.cpp

ValidationList.o: ValidationList.cpp ValidationList.h
	$(CC) $(FLAGS) ValidationList.cpp

ValidationNode.o: ValidationNode.cpp ValidationNode.h
	$(CC) $(FLAGS) ValidationNode.cpp

#  clean  house
clean :
	rm -f $(OBJS) $(OUT)
