//
//  btree.hpp
//  AED_BinaryTree
//
//  Created by Alexander Arturo Baylon Ibanez on 6/10/15.
//  Copyright © 2015 Alexander Arturo Baylon Ibanez. All rights reserved.
//

#ifndef btree_hpp
#define btree_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <list>
#include <fstream>
#include <deque>
#include <iomanip>
#include <string>
#include <cmath>
#include <unistd.h>
#include <term.h>


using namespace std;

//Trait
template <class T>
struct AVLTreeTrait{
    typedef  T U;
    typedef  less<T> C;
    typedef  greater<T> D;
};


// Nodo
template<class T>
class cNode{
public:
    T m_data;
    T m_height;
    cNode<T> * m_child[2];
    cNode(){};
    cNode(T x){
        m_data = x;
        m_child[0] = m_child[1] = nullptr;
    }
    ~cNode(){};
};
//

template <class Tr>
class  CtreeIterator{
public:
    //el elemento principal es un stack
    //lleno de pares(nodo,estado)
    typedef pair<cNode<Tr>*,Tr> par;
    typedef stack<par>          pila;
    pila m_p;
    //constructor
    CtreeIterator(par p)
    {m_p = p;}
    CtreeIterator(){};
    
    //operaciones
    CtreeIterator<Tr> operator =(CtreeIterator<Tr> x)
    {
        m_p = x.m_p;
        return *this;}
    //operador !=
    bool operator != (CtreeIterator<Tr> x)
        {return m_p != x.m_p;}
    //Operador ++
    CtreeIterator<Tr> operator ++ (int){
        par * t = &m_p.top();
        //Mientras que no tengamos el siguiente elemento a mostrar
        while (t->second!=1) {
            switch (t->second) {
            //Caso 0: se va al hijo izquierdo
                case 0:
                    //ponemos el hijo izquierdo en la pila
                    //si es que hay uno
                    if (t->first->m_child[0]){
                        m_p.push(par(t->first->m_child[0],0));
                        t->second++; //cambiamos el estado
                        t = &m_p.top();} //actualizamos el temporal
                    else{//si no hay hijo izquierdo
                        t->second=1; //cambiamos el estado
                        t = &m_p.top(); //actualizamos el temporal
                    }
                    break;
            //Caso 2: se va al hijo derecho
                case 2:
                    if (t->first->m_child[1]) {
                        //ponemos el hijo derecho en la pila
                        m_p.push(par(t->first->m_child[1],0));
                        t->second=2; //cambiamos el estado
                        t = &m_p.top();} //actualizamos el temporal
                    else{//si no hay hijo derecho
                        t->second=3; //cambiamos el estado
                    }
                    break;
            //Caso 3: regresa al padre
                case 3:
                    //Si hay elementos en la pila
                    if (!m_p.empty()) {
                        m_p.pop(); //quitamos el elemento de la pila
                        if  (!m_p.empty()) {
                            t = &m_p.top();
                            if (t->second==0) m_p.top().second = 1;
                            if (t->second==2) m_p.top().second = 3;
                        }
                        break;
                    }
                    //Si la pila queda vacia
                    return *this;
                    break;
                default:
                    break;
            }
        }
        //Si tiene que irse al hijo derecho pero ya se imprimio
        if(t->first->m_child[1]) t->second=2;
        else t->second = 3;
        return *this;
    }
    Tr operator *(){
        //int temp = m_p.top().first->m_data;
        return m_p.top().first->m_data;
    }
    
    void print_stack()
    {
        cout << "m_p" << endl;
        pila temp = m_p;
        while (!temp.empty()) {
            cout << temp.top().first->m_data << " " << temp.top().second << endl;
            temp.pop();}
    }
};



//
template<class T>
cNode<T> ** rep(cNode<T> ** p)
{
    p = &((*p)->m_child[0]);
    while ((*p)->m_child[1])
        p = &((*p)->m_child[1]);
    cout << "contenido: " << (*p)->m_data << endl;
    return p;
}

//Arbol Binario
template <class Tr>
class AVLTree {
private:
    typedef typename Tr::U T;//tipo de dato
    //comparaciones
    typedef typename Tr::C C;
    typedef typename Tr::D D;
    
public:
    //iterador y stack
    typedef pair<cNode<T>*,T>   par;
    typedef stack<par>          pila;
    typedef CtreeIterator<T>    iterador;
    cNode<T> *  m_root;
    //instanciamos una funcion less
    less<T>     m_cmp;
    //instanciamos un iterador
    iterador i;
    iterador j;
    AVLTree(){m_root=nullptr;}
    bool find(T,cNode<T>**&,stack<cNode<T>*>&);
    bool insert(T);
    bool remove(T);
    //metodos para AVL
    
    void update_height(cNode<T> * p){//actualizar altura del nodo
        p->m_height = max(height(p->m_child[0]),height(p->m_child[1]))+1;}
    T height(cNode<T> * p){//obtener altura del nodo
        if(p) return p->m_height;
        return -1;}
    void right_rotate(cNode<T>*,stack<cNode<T>*>&);//rotar izquierda
    void left_rotate(cNode<T>*,stack<cNode<T>*>&);//rotar derecha
    //metodos de impresion
    void printTree(cNode<T>*);
    void printTreeAmplitud(cNode<T>*);
    //nos retorna un stack con el camino hacia el primer nodo
    iterador begin()
    {
        pila first;
        cNode<T> * p;
        for (p = m_root; p->m_child[0]!=nullptr; p=p->m_child[0]){
            first.push(par(p,0));}
        //como el ultimo elemento se va a imprimir sin condiciones, entonces
        //es estado 3
        first.push(par(p,3));
        i.m_p=first;
        return i;
    }
    //nos retorna un iterador vacio
    iterador end() {return j;}
    ~AVLTree(){};
};


//
template<class Tr>
bool AVLTree<Tr>::find(T x, cNode<T> **& p, stack<cNode<T>*> & s){
    for (p=&m_root; (*p) && (*p)->m_data!=x; p=&( (*p)->m_child[m_cmp((*p)->m_data,x)]))
        s.push(*p);
    return (*p);
}

//
template<class Tr>
bool AVLTree<Tr>::insert(T x){
    stack<cNode<T>*> path;
    cNode<T> ** p;
    if (find(x, p, path)) return 0;
    *p = new cNode<T>(x);
    update_height(*p);
    cout << "inserto: " << x << endl;
    cNode<T> * top;
    while (!path.empty()) {
        update_height(path.top());
        top = path.top();
        if (height(top->m_child[0]) >= 2+height(top->m_child[1])) {
            if (height(top->m_child[0]->m_child[0]) >= height(top->m_child[0]->m_child[1]) ){
                right_rotate(top,path);
                break;}
            else{
                path.push(top->m_child[0]);
                left_rotate(path.top(),path);
                right_rotate(path.top(),path);
                break;}}
        else if(height(top->m_child[1]) >= 2+height(top->m_child[0])){
            if (height(top->m_child[1]->m_child[1]) >= height(top->m_child[1]->m_child[0]) ){
                left_rotate(top,path);
                break;}
            else{
                path.push(top->m_child[1]);
                right_rotate(path.top(),path);
                left_rotate(path.top(),path);
                break;}}
        else path.pop();
    }
    
    return 1;
}

//
template<class Tr>
bool AVLTree<Tr>::remove(T x){
    stack<cNode<T>*> path;
    cNode<T> ** p;
    if (!find(x, p, path)) return 0;
    // Caso 2:tiene 2 hijos
    if ((*p)->m_child[0] && (*p)->m_child[1]) {
        cNode<T> ** q = rep<T>(p);
        (*p)->m_data = (*q)->m_data;
        p = q;
    }
    cNode<T> * t = *p;
    // Caso 1:tiene 1 hijo
    *p = (*p)->m_child[(*p)->m_child[1]!=0];
    delete t;
    cNode<T> * top;
    while (!path.empty()) {
        update_height(path.top());
        top = path.top();
        if (height(top->m_child[0]) >= 2+height(top->m_child[1])) {
            if (height(top->m_child[0]->m_child[0]) >= height(top->m_child[0]->m_child[1]) ){
                right_rotate(top,path);
                break;}
            else{
                path.push(top->m_child[0]);
                left_rotate(path.top(),path);
                right_rotate(path.top(),path);
                break;}}
        else if(height(top->m_child[1]) >= 2+height(top->m_child[0])){
            if (height(top->m_child[1]->m_child[1]) >= height(top->m_child[1]->m_child[0]) ){
                left_rotate(top,path);
                break;}
            else{
                path.push(top->m_child[1]);
                right_rotate(path.top(),path);
                left_rotate(path.top(),path);
                break;}}
        else path.pop();
    }
    return 1;
}


//rotaciones
template<class Tr>
void AVLTree<Tr>::right_rotate(cNode<T> * p,stack<cNode<T>*> & s)
{
    //cout << "top: " << s.top()->m_data << endl;
    if (!s.empty()) s.pop();//sacamos al nodo p del stack
    if (!s.empty()){
        cNode<T> * father = s.top();//guardamos al padre de p
        if (father->m_child[0]->m_data == p->m_data) father->m_child[0] = p->m_child[0];
        else father->m_child[1] = p->m_child[0];}//el padre ahora apunta al hijo de p
    else m_root = p->m_child[0];//si p es la raiz
    cNode<T> * i = p->m_child[0];//guardamos el hijo izq de p
    cNode<T> * t = p->m_child[0]->m_child[1];//guardamos el hijo der. del hijo izq. de p
    p->m_child[0]->m_child[1] = p;//el hijo der. de p, ahora apunta a p
    p->m_child[0] = t;//el hijo izq. de p, apunta a a t
    //update_height(i->m_child[0]);
    update_height(i->m_child[1]);
    update_height(i);
}


template<class Tr>
void AVLTree<Tr>::left_rotate(cNode<T> * p,stack<cNode<T>*> & s)
{
    if (!s.empty()) s.pop();//sacamos al nodo p del stack
    if (!s.empty()){
        cNode<T> * father = s.top();//guardamos al padre de p
        if (father->m_child[0]->m_data == p->m_data) father->m_child[0] = p->m_child[1];
        else father->m_child[1] = p->m_child[1];}//el padre ahora apunta al hijo der. de p
    else m_root = p->m_child[1];//si p es la raiz
    cNode<T> * d = p->m_child[1];//guardamos el hijo der de p
    cNode<T> * t = p->m_child[1]->m_child[0];//guardamos el hijo izq. del hijo der. de p
    p->m_child[1]->m_child[0] = p;//el hijo izq. de p, ahora apunta a p
    p->m_child[1] = t;//el hijo der. de p, apunta a a t
    //update_height(d->m_child[1]);
    update_height(d->m_child[0]);
    update_height(d);
}


//imprimir arbol en inorden
template<class Tr>
void AVLTree<Tr>::printTree(cNode<T> * p)
{
    if (p != nullptr) {
        printTree(p->m_child[0]);
        cout << p->m_data << " ";
        printTree(p->m_child[1]);
    }
}

//imprimir arbol por amplitud
template<class Tr>
void AVLTree<Tr>::printTreeAmplitud(cNode<T> * p)
{
    //vector se asemeja a una cola
    vector<cNode<T>*> l;
    l.push_back(p);
    while (!l.empty()) {
        cNode<T> * t = l.front();
        l.erase(l.begin());
        cout << t->m_data <<" ";
        if(t->m_child[0]) l.push_back(t->m_child[0]);
        if(t->m_child[1]) l.push_back(t->m_child[1]);
        cout << endl;
    }
    
}



string intToString(int val) {
    ostringstream ss;
    ss << val;
    return ss.str();
}

int maxHeight(cNode<int> *p) {
    if (!p) return 0;
    int leftHeight = maxHeight(p->m_child[0]);
    int rightHeight = maxHeight(p->m_child[1]);
    return (leftHeight > rightHeight) ? leftHeight + 1: rightHeight + 1;
}

// Print the arm branches (eg, /    \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<cNode<int>*>& nodesQueue, ostream& out) {
    deque<cNode<int>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel / 2; i++) {
        out << ((i == 0) ? setw(startLen-1) : setw(nodeSpaceLen-2)) << "" << ((*iter++) ? "/" : " ");
        out << setw(2*branchLen+2) << "" << ((*iter++) ? "\\" : " ");
    }
    out << endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<cNode<int>*>& nodesQueue, ostream& out) {
    deque<cNode<int>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->m_child[0]) ? setfill('_') : setfill(' '));
        out << setw(branchLen+2) << ((*iter) ? intToString((*iter)->m_data) : "");
        out << ((*iter && (*iter)->m_child[1]) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
    }
    out << endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<cNode<int>*>& nodesQueue, ostream& out) {
    deque<cNode<int>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        out << ((i == 0) ? setw(indentSpace+2) : setw(2*level+2)) << ((*iter) ? intToString((*iter)->m_data) : "");
    }
    out << endl;
}

void printPretty(cNode<int>*root, int level, int indentSpace, ostream& out) {
    int h = maxHeight(root);
    int nodesInThisLevel = 1;
    
    int branchLen = 2*((int)pow(2.0,h)-1) - (3-level)*(int)pow(2.0,h-1);
    int nodeSpaceLen = 2 + (level+1)*(int)pow(2.0,h);
    int startLen = branchLen + (3-level) + indentSpace;
    
    deque<cNode<int>*> nodesQueue;
    nodesQueue.push_back(root);
    for (int r = 1; r < h; r++) {
        printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
        branchLen = branchLen/2 - 1;
        nodeSpaceLen = nodeSpaceLen/2 + 1;
        startLen = branchLen + (3-level) + indentSpace;
        printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
        
        for (int i = 0; i < nodesInThisLevel; i++) {
            cNode<int> * currNode = nodesQueue.front();
            nodesQueue.pop_front();
            if (currNode) {
                nodesQueue.push_back(currNode->m_child[0]);
                nodesQueue.push_back(currNode->m_child[1]);
            } else {
                nodesQueue.push_back(NULL);
                nodesQueue.push_back(NULL);
            }
        }
        nodesInThisLevel *= 2;
    }
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
    printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
}

int ingresar_opcion()
{
    cout << "Ingrese una OPCION : ";
    int val;
    cin >> val;
    return val;
}

int ingresar_valor()
{
    cout << "dato: ";
    int val;
    cin >> val;
    return val;
}


void pausa()
{
    cout << "Presione una tecla para continuar.... ";
    cin.ignore().get();
}

void clearScreen(){
    for (int i=0; i<100; i++) {
        cout << endl;
    }
}

#endif /* btree_hpp */
