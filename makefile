vertexcover_0.x: vertexcover_0.cpp
	g++ -g -o $@ $< -std=c++11

vertexcover_1.x: vertexcover_1.cpp
	g++ -g -o $@ $< -std=c++11

vertexcover_2.x: vertexcover_2.cpp
	g++ -g -o $@ $< -std=c++11

vertexcover_3.x: vertexcover_3.cpp
	g++ -g -o $@ $< -std=c++11

vertexcover_4.x: vertexcover_4.cpp
	g++ -g -o $@ $< -std=c++11

vertexcover_5.x: vertexcover_5.cpp
	g++ -g -o $@ $< -std=c++11

.PHONY: clean

clean:
	rm -f *.o *~ *.x