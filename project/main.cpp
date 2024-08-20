#include <iostream>
#include <fstream>
#include "message.pb.h"
 
using namespace std;
 
int main(int argc, char* argv[])
{
    Person john;
    john.setId(1234);
    john.setName("John Doe");
    john.setEmail("jdoe@example.com");
    
    std::ofstream output(args[0]);
    john.SerializeToOstream(output);
    return 0;
}