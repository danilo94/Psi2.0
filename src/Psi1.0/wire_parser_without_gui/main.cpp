#include "mainwindow.h"
#include <opening_window.h>

#include <QApplication>

#include <unistd.h>
void processArguments(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    processArguments(argc,argv);
    /*MainWindow mainWindow;
   // mainWindow.showMaximized();
    mainWindow.show();*/
 //   opening_window window_open;
    //return app.exec();


}


void processArguments(int argc, char *argv[]){
    char c;
    string topLevelPath = "";
    string topLevelFolder = "";
    string listOfWires = "";
     while((c = getopt(argc, argv,"t:f:l:")) != -1){
        //printf("Entrou!!!\n");
        switch(c) {
           case 't':
                topLevelPath = string (optarg);
              break;
          case 'f':
                topLevelFolder = string (optarg);
              break;
          case 'l':
                listOfWires = string(optarg);
              break;
           default:
              std::cout << ("-t Full path of top level .\n -f Full path of folder\n -l List of wires to extern\n");
              exit(1);
        }
     }
     if (topLevelFolder.size()==0 || topLevelPath.size()==0 || listOfWires.size()==0){
         std::cout<< ("One or more arguments are incorrect.\n");
     }
     else{
         std::cout << (topLevelFolder) << std::endl;
         std::cout << (topLevelPath) << std::endl;
         std::cout << (listOfWires) << std::endl;
         opening_window window_open(0,topLevelFolder,topLevelPath,listOfWires);

         std::cout << ("Starting Wire Parsing");
     }
}
