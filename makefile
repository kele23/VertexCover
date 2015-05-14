vertexcover.x: vertexcover.cpp
	g++ -o $@ $< -std=c++11

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~