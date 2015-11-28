//
//  main.cpp
//  avl_binary_tree
//
//  Created by Alexander Arturo Baylon Ibanez on 17/11/15.
//  Copyright © 2015 aunesto17. All rights reserved.
//

#include "btree.hpp"

int main(int argc, const char * argv[]) {
    
    AVLTree<AVLTreeTrait<int>> prueba;
    
    int op,op1;
    int val;
    do {
        //cout << "1. Ingresar Dato" << endl;
        //cout << "2. Borrar Dato" << endl;
        //cout << "0. SALIR" << endl;
        op = ingresar_opcion();
        //clearScreen();
        switch (op) {
            case 1:
                val = ingresar_valor();
                prueba.insert(val);
                printPretty(prueba.m_root, 1, 0, cout);
                break;
            case 2:
                val = ingresar_valor();
                prueba.remove(val);
                printPretty(prueba.m_root, 1, 0, cout);
            default:
                break;
        }
    } while (op!=0);
    
    
    
    /*
    
    prueba.insert(2);
    prueba.insert(1);
    prueba.insert(4);
    prueba.insert(3);
    prueba.insert(5);
    //prueba.insert(21);
    //prueba.insert(30);
    //prueba.insert(2);
    //prueba.insert(7);
    //prueba.insert(11);
    //prueba.insert(8);
    //prueba.insert(4);
    
    printPretty(prueba.m_root, 1, 0, cout);
    prueba.remove(1);
    
    AVLTree<AVLTreeTrait<int>>::iterador i;
    
    for (i=prueba.begin(); i!=prueba.end(); i++)
        cout << *i << " ";
    cout << endl << endl;
    
    printPretty(prueba.m_root, 1, 0, cout);
    
    //printPretty(prueba.m_root, 5, 0, cout);
    */
    return 0;
}
