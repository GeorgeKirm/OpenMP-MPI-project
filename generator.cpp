#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
//    cout<<argv[1]<<" "<<argv[2];
    if(argc<2){
        cout<<"Need more arguments. Exiting program"<<endl;
        return 0;
    }
    int coordinate_index = stoi(argv[2]);
    int utime;
    long int ltime;
    int i;
    float cords[coordinate_index][3];
    ltime = time(NULL);
    utime = (unsigned int) ltime/2;
    ofstream out(argv[1]);
//    srand(utime);for(i=0;i<coordinate_index;i++){
    srand(utime);for(i=0;i<100;i++){
        //vv1vv
        out<<(float)34*rand()/(RAND_MAX-1)<<" "<<(float)34*rand()/(RAND_MAX-1)<<" "<<(float)34*rand()/(RAND_MAX-1)<<endl;
    }//^^1vv
//        cords[i][0]=(float)34*rand()/(RAND_MAX-1);
//        cords[i][1]=(float)34*rand()/(RAND_MAX-1);
//        cords[i][2]=(float)34*rand()/(RAND_MAX-1);
//    }
//    for(i=0;i<100;i++){
//        out<<cords[i][0]<<" "<<cords[i][1]<<" "<<cords[i][2]<<endl;
//    }//^^1^^
    
    return 0;
}
