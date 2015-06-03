#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <set>
#include <unordered_set>
#include <iterator>
#include <cstring>

/*------------------------------
STRUTTURE UTILI
------------------------------*/
#define WHITE 0
#define BLACK 1

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

struct eComp  {
	bool operator() (Edge* e1, Edge* e2) const {
		return e1->getTotalDegree() > e2->getTotalDegree();
	}
};

/*------------------------------
END STRUTTURE UTILI
------------------------------*/

void log(std::string,int V,Vertex** vertices,int E,Edge** edges,bool log_edges);
void reload_weight(int V,Vertex** vertices,int E,bool use_degree);
void merge_sort(Edge** edges,int start,int end);

int main(int argc,char* argv[]){

	int V,E;
	Vertex** vertices;
	Edge** edges;
	std::set<Edge*, eComp> classifica;

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
		classifica.insert(edge);
	}

	fclose(input);

	//log("Lettura del Grafo",V,vertices,E,edges,false);
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

	

	reload_weight(V,vertices,E,true);

	//BAR-YEHUDA & EVEN con pesi by Kele
	for(int i = 0; i < E; i++){
		
		Vertex* vA = edges[i]->getVertexA();
		Vertex* vB = edges[i]->getVertexB();
		
		int epsilon = vA->getWeight() < vB->getWeight() ? vA->getWeight() : vB->getWeight();
		
		vA->setWeight(vA->getWeight()-epsilon);
		vB->setWeight(vB->getWeight()-epsilon);

	}

	//log("Uso dell'algoritmo BAR-YEHUDA & EVEN con pesi by Kele",V,vertices,E,edges,false);


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

void log(std::string title,int V,Vertex** vertices,int E,Edge** edges,bool log_edges){

	static int log_number = 1;

	std::cout << "-------------------------" << std::endl;
	std::cout << "Log N° " << log_number++ << std::endl;
	std::cout << title << std::endl;
	std::cout << "-------------------------" << std::endl;

	std::cout << "######### VERTICES ###########" << std::endl;

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

	if(log_edges){
		std::cout << "######### EDGES ###########" << std::endl;
		for(int i = 0; i < E; i++){
			std::cout << "E: " << i << "	W: "<< edges[i]->getTotalDegree() << std::endl;
		}
	}

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

void merge(Edge** edges,int start,int k,int end){

	Edge** temp = new Edge*[end-start+1];

	int pLeft = start;
	int pRight = k+1;

	int i = 0;
	while(pLeft <= k && pRight <= end){
		if(edges[pLeft]->getTotalDegree() < edges[pRight]->getTotalDegree()){
			temp[i++] = edges[pLeft++];
		}else{
			temp[i++] = edges[pRight++];
		}
	}

	//Rimangono elementi sul lato sinistro
	while(pLeft <= k){
		temp[i++] = edges[pLeft++];
	}

	//Rimangono elementi sul lato destro
	while(pRight <= end){
		temp[i++] = edges[pRight++];
	}

	std::memcpy(&(edges[start]),temp,(end-start+1)*sizeof(Edge*));

}

void merge_sort(Edge** edges, int start, int end){

	if(start >= end)
		return;

	int k = start+( (end-start)/2 );

	merge_sort(edges,start,k);
	merge_sort(edges,k+1,end);

	merge(edges,start,k,end);

}