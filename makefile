vertexcover.x: vertexcover.cpp
	g++ -g -o $@ $< -std=c++11

.PHONY: clean

clean:
	rm -f *.o *~ *.x