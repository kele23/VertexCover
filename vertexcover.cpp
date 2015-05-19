#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <unordered_set>
#include <iterator>

/*------------------------------
STRUTTURE UTILI
------------------------------*/
#define WHITE 0
#define BLACK 1

class Edge{

public:

	Edge(Vertex* vertexA,Vertex* vertexB){
		this->vertexA = vertexA;
		this->vertexB = vertexB;
	}

	void setVertexA(Vertex* vertexA){
		this->vertexA = vertexA;
	}

	void setVertexB(Vertex* vertexB){
		this->vertexB = vertexB;
	}

	void setColor(int color){
		this->color = color;
	}

	Vertex* getVertexA(){
		return this->vertexA;
	}

	Vertex* getVertexB(){
		return this->vertexB;
	}

	int getColor(){
		return this->color;
	}

	int getTotalDegree(){
		return vertexA->getDegree() + vertexB->getDegree();
	}

private:
	Vertex* vertexA;
	Vertex* vertexB;
	int color = WHITE;
};


class Vertex{

public:
	
	Vertex(int weight,std::unordered_set<int> *edges = NULL){
		this->weight = weight;
		this->edges = edges;
	}

	void addEdge(int edge){
		if(this->edges == NULL)
			this->edges = new std::unordered_set<int>();
		this->edges->insert(edge);
	}

	void removeEdge(int edge){
		if(this->edges != NULL)
			this->edges->erase(edge);
	}

	std::unordered_set<int>* getEdges(){
		return this->edges;
	}

	int getWeight(){
		return this->weight;
	}

	void setWeight(int weight){
		this->weight = weight;
	}

	int getDegree() {
		if(this->edges == NULL)
			return 0;
		return this->edges->size();
	}

private:
	int weight;
	std::unordered_set<int> *edges;
};

/*------------------------------
END STRUTTURE UTILI
------------------------------*/

void log(std::string,int V,Vertex** vertices,int E,Edge** edges);
void reload_weight(int V,Vertex** vertices,int E,bool use_degree);

int main(int argc,char* argv[]){

	int V,E;
	Vertex** vertices;
	Edge** edges;


	/*------------------------------
	LETTURA DA FILE
	------------------------------*/
	FILE *input = fopen("input.txt","r");

	fscanf(input,"%d %d\n",&V,&E);
	vertices = new Vertex*[V];
	edges = new Edge*[E];

	for(int i = 0; i < E; i ++){
		int vA,vB;
		fscanf(input,"%d %d\n",&vA,&vB);
		

		if(vertices[vA] == NULL)
			vertices[vA] = new Vertex(E);
		vertices[vA]->setWeight(vertices[vA]->getWeight()-1);
		vertices[vA]->addEdge(i);

		//MODIFY VERTEX B
		if(vertices[vB] == NULL)
			vertices[vB] = new Vertex(E);
		vertices[vB]->setWeight(vertices[vB]->getWeight()-1);
		vertices[vB]->addEdge(i);

		Edge* edge = new Edge(vertices[vA],vertices[vB]);
		edges[i] = edge;
	}

	fclose(input);

	log("Lettura del Grafo",V,vertices,E,edges);
	/*------------------------------
	END LETTURA DA FILE
	------------------------------*/

	/*

	for each edge e in E                        
        Let epsilon = Min{W(v): v endpoint of E} 
        W(v) -= epsilon for all v endpoint of E  
    return {v: W(v)=0}

	*/

	/*------------------------------
	COMPUTAZIONE RISULTATO
	------------------------------*/

	reload_weight(V,vertices,1,false);

	//BAR-YEHUDA & EVEN con pesi ad 1
	for(int i = 0; i < E; i++){
		
		Vertex* vA = vertices[edges[i]->getVertexA()];
		Vertex* vB = vertices[edges[i]->getVertexB()];
		
		int epsilon = vA->getWeight() < vB->getWeight() ? vA->getWeight() : vB->getWeight();
		
		vA->setWeight(vA->getWeight()-epsilon);
		vB->setWeight(vB->getWeight()-epsilon);

	}

	log("Uso dell'algoritmo BAR-YEHUDA & EVEN con pesi ad 1",V,vertices,E,edges);

	



	reload_weight(V,vertices,E,true);

	//BAR-YEHUDA & EVEN con pesi by Kele
	for(int i = 0; i < E; i++){
		
		Vertex* vA = vertices[edges[i]->getVertexA()];
		Vertex* vB = vertices[edges[i]->getVertexB()];
		
		int epsilon = vA->getWeight() < vB->getWeight() ? vA->getWeight() : vB->getWeight();
		
		vA->setWeight(vA->getWeight()-epsilon);
		vB->setWeight(vB->getWeight()-epsilon);

	}

	log("Uso dell'algoritmo BAR-YEHUDA & EVEN con pesi by Kele",V,vertices,E,edges);


	




	reload_weight(V,vertices,1,false);

	//Algoritmo KELE ( pesi a 1 )
	for(int i = 0; i < E; i++){
		
		Vertex* vA = vertices[edges[i]->getVertexA()];
		Vertex* vB = vertices[edges[i]->getVertexB()];
		
		//int epsilon = vA->getWeight() < vB->getWeight() ? vA->getWeight() : vB->getWeight();
		
		Vertex* min = vA->getWeight() < vB->getWeight() ? vA : vB;
		Vertex* max = min == vA ? vB : vA;

		min->setWeight(0);
		if(max->getWeight() != 0)
			max->setWeight(max->getWeight()+1);

	}

	log("Uso dell'algoritmo KELE, modifica grado a rimozione di un arco ( Basato su BAR-YEHUDA & EVEN ) con pesi ad 1",V,vertices,E,edges);







	reload_weight(V,vertices,E,true);

	//ALgoritmo KELE ( pesi a kele )
	for(int i = 0; i < E; i++){
		
		Vertex* vA = vertices[edges[i]->getVertexA()];
		Vertex* vB = vertices[edges[i]->getVertexB()];
		
		//int epsilon = vA->getWeight() < vB->getWeight() ? vA->getWeight() : vB->getWeight();
		
		Vertex* min = vA->getWeight() < vB->getWeight() ? vA : vB;
		Vertex* max = min == vA ? vB : vA;

		min->setWeight(0);
		if(max->getWeight() != 0)
			max->setWeight(max->getWeight()+1);

	}

	log("Uso dell'algoritmo KELE, modifica grado a rimozione di un arco ( Basato su BAR-YEHUDA & EVEN ) con pesi by Kele",V,vertices,E,edges);



	reload_weight(V,vertices,1,false);

	//Due approssimazione
	for(int i = 0; i < E; i++){

		if(edges[i]->getColor() == BLACK)
			continue;
		
		edges[i]->setColor(BLACK);

		Vertex* vA = vertices[edges[i]->getVertexA()];
		Vertex* vB = vertices[edges[i]->getVertexB()];

		vA->setWeight(0);
		vB->setWeight(0);

		std::unordered_set<int>* vAEdges = vA->getEdges();
		if(vAEdges != NULL){
			for(int e : *vAEdges){
				edges[e]->setColor(BLACK);
			}
		}

		std::unordered_set<int>* vBEdges = vB->getEdges();
		if(vBEdges != NULL){
			for(int e : *vBEdges){
				edges[e]->setColor(BLACK);
			}
		}

	}

	log("Due approssimazione",V,vertices,E,edges);




	reload_weight(V,vertices,1,false);

	//Due approssimazione con euristica per scelta arco di partenza

	for(int i = 0; i < E; i++){

		if(edges[i]->getColor() == BLACK)
			continue;

		edges[i]->setColor(BLACK);

		Vertex* vA = vertices[edges[i]->getVertexA()];
		Vertex* vB = vertices[edges[i]->getVertexB()];


		vA->setWeight(0);
		vB->setWeight(0);

		std::unordered_set<int>* vAEdges = vA->getEdges();
		if(vAEdges != NULL){
			for(int e : *vAEdges){
				edges[e]->setColor(BLACK);
			}
		}

		std::unordered_set<int>* vBEdges = vB->getEdges();
		if(vBEdges != NULL){
			for(int e : *vBEdges){
				edges[e]->setColor(BLACK);
			}
		}

	}

	log("Due approssimazione con euristica",V,vertices,E,edges);



	/*------------------------------
	END COMPUTAZIONE RISULTATO
	------------------------------*/




	/*------------------------------
	SCRITTURA RISULTATO
	------------------------------*/
	FILE *output = fopen("output.txt","w");

	std::stringstream soluzione;

	int cardinality = 0;
	for(int i = 0; i < V; i++){
		if(vertices[i]->getWeight() == 0){
			cardinality++;
			soluzione << i << " ";
		}	
	}

	fprintf(output,"%d %s\n",cardinality,soluzione.str().c_str());

	fclose(output);
	/*------------------------------
	END SCRITTURA RISULTATO
	------------------------------*/


}

void log(std::string title,int V,Vertex** vertices,int E,Edge** edges){

	static int log_number = 1;

	std::cout << "-------------------------" << std::endl;
	std::cout << "Log N° " << log_number++ << std::endl;
	std::cout << title << std::endl;
	std::cout << "-------------------------" << std::endl;

	std::stringstream soluzione;

	int solCount = 0;

	for(int i = 0; i < V; i++){
		std::cout << "V: " << i << "	W: " << vertices[i]->getWeight() << "	edges-> ";
		std::unordered_set<int>* es = vertices[i]->getEdges();
		if(es == NULL){
			std::cout << "NONE";
		}else{
			for(int e : *es){
				std::cout << e << " ";
			}
		}
		std::cout << std::endl;

		if(vertices[i]->getWeight() == 0){
			solCount++;
			soluzione << i << " ";
		}
	}

	if(!soluzione.str().empty())
		std::cout << "Soluzione ( " << solCount << " ) : " << soluzione.str() << std::endl;

}

void reload_weight(int V,Vertex** vertices,int E,bool use_degree){

	for(int i = 0; i < V; i++){

		int degree = 0;
		if(vertices[i]->getEdges() != NULL){
			degree = vertices[i]->getEdges()->size();
		}

		if(use_degree)
			vertices[i]->setWeight(E-degree);
		else
			vertices[i]->setWeight(E);

	}
}
