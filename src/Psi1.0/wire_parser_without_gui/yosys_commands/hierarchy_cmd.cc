#include "kernel/yosys.h"
#include "kernel/sigtools.h"
#include </usr/include/boost/algorithm/string/predicate.hpp>
#include </usr/include/boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>

using namespace std;

int main () {
    ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
    return 0;
}
USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN

struct MyPass : public Pass {
    MyPass() : Pass("my_cmd", "just a simple test") { }
    virtual void hierarchy(RTLIL::Module *module,RTLIL::Design *design, int i, int k, ofstream &myfile) {
        
        //if(k==1){}else{
            for(int j = 0; j< i;j++){
                log("\t");
            }
            log("%s (%zd wires, %zd cells)\n", log_id(module),
                GetSize(module->wires()), GetSize(module->cells()));
            
            for(auto wire : module->wires()){
                for(int j = 0; j< i;j++){
                    log("\t");
                }
                if (!boost::starts_with(log_id(wire), "$")){
                    log("   name=%s width=%d\n", log_id(wire), wire->width);
                    for(int j = 0; j< i;j++){
                        myfile << "    ";
                    }
                    myfile << "    wire|";
                    myfile << log_id(wire);
                    myfile << "|";
                    myfile << wire->width;
                    myfile << "|";
                    myfile << wire->attributes.at("\\src").decode_string().c_str();
                    myfile << "\n";
                    if(wire->width > 0){
                        for(int l=0;l<wire->width;l++){
                            for(int j = 0; j< i;j++){
                                myfile << "    ";
                            }
                            myfile << "        wiresub|";
                            myfile << log_id(wire);
                            myfile << "[";
                            myfile << l;
                            myfile << "]|";
                            myfile << "1";
                            myfile << "|";
                            myfile << wire->attributes.at("\\src").decode_string().c_str();
                            myfile << "\n";
                        }
                    }
                }
                
            }
        //}
        i++;
        if(GetSize(module->cells())>0){
            for(auto cell : module->cells()){
                
                std::string typecell = cell->type.c_str();
                std::string namecell = cell->name.c_str();
                log("RTL source for cell %s is at %s.\n", namecell.c_str(), cell->attributes.at("\\src").decode_string().c_str());
                if (boost::starts_with(typecell, "\\")){	
                    typecell.erase(0,1);
                    namecell.erase(0,1);
                    for (auto dmod : design->modules()){ 
                        
                        std::string ID = log_id(dmod);				
                        if(ID.compare(typecell)==0){
                           // if(k==1){}else{
                                for(int j = 0; j< i;j++){
                                    log("    ");
                                    myfile << "    ";
                                }
                            //}
                            
                           // if(k==1){}else{
                                log("Cell: name=%s,type=%s \n",cell->name.c_str(), cell->type.c_str());
                                myfile << typecell;
                                myfile << "|";
                                myfile << namecell;
                                myfile << "|";
                                myfile << "0";	
                                myfile << "|";
                                myfile << cell->attributes.at("\\src").decode_string().c_str();
                                myfile << "\n";
                           // }
                            k++;
                            hierarchy(dmod,design,i,k, myfile);
                        }			
                    }
                }
            }
        }
        
        
    }
    virtual void execute(std::vector<std::string> args, RTLIL::Design *design)
    {
        log("Arguments to my_cmd:\n");
        for (auto &arg : args)
            log("  %s\n", arg.c_str());
        
        log("Modules in current design:\n");
        RTLIL::Module *mod;
        RTLIL::Module *module = mod;
        for (auto mod1 : design->modules()){ 
            RTLIL::Module *module = mod1;
            ofstream myfile;
            myfile.open ("example.txt");
            myfile << "topmodule|";
            myfile << log_id(module);
            myfile << "|";		
            myfile << "0";
            myfile << "|";
            myfile << module->attributes.at("\\src").decode_string().c_str();
            myfile << "\n";
            hierarchy(module,design,0,0,myfile);
            myfile.close();            
            log("\n\n\n");
        }
        
        
    }
} MyPass;
PRIVATE_NAMESPACE_END
