#ifndef _Graph_H_
#define _Graph_H_

#include <iostream>
#include <iterator>  // bidirectional_iterator_tag
#include <cassert>   // assert()
#include <algorithm> // copy
#include <cstddef>   // std::ptrdiff_t
#include <type_traits>
#include <vector>
#include <utility>
#include <set>

using std::set;
using std::copy;
using std::vector;
using std::cout;
using std::endl;

namespace sc
{

template < typename T >
class Graph
{

public:

  struct Edge
  {
    T data;
    bool isCovered;
    bool alreadySeen;
    Edge* next;

    Edge ( T d = T{}, Edge* n = nullptr, bool i = false, bool al = false )
      : data{ d },
        isCovered{ i },
        alreadySeen{ al },
        next{ n }
    { /* empty */ }
  };

  struct Vertex
  {
    T data;
    size_t degree;
    size_t loss;
    size_t gain;
    Edge* next;

    Vertex ( T d = T{}, Edge* edge = nullptr, size_t de = 0, size_t l = 0, size_t g = 0)
      : data{ d },
        degree{ de },
        loss{ l },
        gain{ g },
        next{ edge }
    { /* empty */ }
  };

private:

  vector<Vertex*> adjacencyList;

  using  iterator = typename vector<sc::Graph<T>::Vertex*>::iterator;

public:

  Graph()
  {
    /* empty */
  }

  ~Graph()
  {
    for ( auto vertex : adjacencyList ) {
      //cout << "VERTEX: " << vertex->data << endl;
      removeAllEdges(vertex->next);
      //cout << "DELETED VERTEX: " << vertex->data << endl;
      delete vertex;
    }
  }

 void removeAllEdges(Edge* edge)
 {
   if (edge != nullptr ) {
     if ( edge->next != nullptr )
     {
       removeAllEdges(edge->next);
     }
     //cout << "DELETED EDGE: " << edge->data << endl;
     delete edge;
   }
 }


  iterator begin()
  {
    return adjacencyList.begin();
  }

  iterator end()
  {
    return adjacencyList.end();
  }

  bool empty ( void ) const
  {
    return adjacencyList.size() == 0;
  }

  size_t size ( void ) const
  {
    return adjacencyList.size();
  }

  void addEdge ( std::pair<T,T> values )
  {
    addEdgeAux ( values );
    addEdgeAux ( std::make_pair ( values.second, values.first ) );
  }

  iterator findVertex ( T value )
  {
    auto it = begin();
    while ( it != end() )
    {
      if ( ( *it )->data == value )
      {
        return it;
      }
      it++;
    }
    return it;
  }

  size_t vertexDegree ( T value )
  {
    return ( *findVertex ( value ) )->degree;
  }

  size_t vertexLoss ( T value )
  {
    return ( *findVertex ( value ) )->loss;
  }

  void updateLoss ( T value )
  {
    ( *findVertex ( value ) )->loss++;
  }

  void updateGain ( T value )
  {
    ( *findVertex ( value ) )->gain++;
  }

  void updateLossNeighbors ( T value )
  {
    auto vertex = ( *findVertex ( value ) )->next;
    for (auto edge : *vertex) {
      updateLoss ( ( *edge ).data );
    }
  }

  void updateGainNeighbors ( T value )
  {
    auto vertex = ( *findVertex ( value ) )->next;
    for (auto edge : *vertex) {
      updateGain ( ( *edge ).data );
    }
  }

  void coverEdge ( std::pair<T,T> values )
  {
    coverEdgeAux ( values );
    coverEdgeAux ( std::make_pair ( values.second, values.first ) );
  }

  void seeEdge ( std::pair<T,T> values )
  {
    seeEdgeAux ( values );
    seeEdgeAux ( std::make_pair ( values.second, values.first ) );
  }

  // TODO: it is wrong
  // Look everybody in G and see if somebody has gain bigger than 0.
  bool isVertexCover(set<int> C) {
    iterator vertex;

    for (auto vertexValue : C) {
      vertex = findVertex(vertexValue);

      if (vertex != end()) {
        if((*vertex)->gain != 0) {
          return false;
        }
      }
      else {
        cout << "ERROR: Vertex not found in isVertexCover function" << endl;
        return false;
      }
    }
    return true;
  }

  iterator getMinimumLossVertex (set<int> C)
  {
    if (C.empty())
      return this->end();

    iterator minLossVertex = findVertex(*(C.begin()));
    iterator vertex = findVertex(*(C.begin()));
    size_t minLoss = (*vertex)->loss;

    for (auto vertexValue : C) {
      vertex = findVertex(vertexValue);

      if ((*vertex)->loss <= minLoss) {
        minLossVertex = vertex;
      }
    }

    return minLossVertex;
  }

private:

  void addEdgeAux ( std::pair<T,T> values )
  {
    Edge* n = new Edge ( values.second );

    if ( findVertex ( values.first ) != end() )
    {
      auto temp = findVertex ( values.first );
      ( *temp )->degree++;
      auto aux = ( *temp )->next;
      while ( aux->next != nullptr )
        aux = aux->next;
      aux->next = n;
    }
    else
      adjacencyList.push_back ( new Vertex ( values.first, n, 1 ) );
  }

  void coverEdgeAux ( std::pair<T,T> values )
  {
    auto aux = ( *findVertex ( values.first ) )->next;
    while ( aux != nullptr )
    {
      if ( aux->data == values.second )
      {
        aux->isCovered = true;
        return;
      }
      aux = aux->next;
    }
  }

  void seeEdgeAux ( std::pair<T,T> values )
  {
    auto aux = ( *findVertex ( values.first ) )->next;
    while ( aux != nullptr )
    {
      if ( aux->data == values.second )
      {
        aux->alreadySeen = true;
        return;
      }
      aux = aux->next;
    }
  }
};
}
#endif
