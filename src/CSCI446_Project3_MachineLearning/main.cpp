/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: byrdie
 *
 * Created on October 26, 2016, 9:31 PM
 */



#include "main.h"

record out("test.txt");

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

//    QApplication app(argc, argv);

    // create and show your widgets here
    out << "This has been a test\n";

    CancerDataset cn;
    cn.print_dataset(true);
    
//    return app.exec();
}
