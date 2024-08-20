#include <iostream>
#include <fstream>
#include "message.pb.h"
 
using namespace std;
 
int main(int, char* argv[])
{
    Person john;
    john.set_id(1234);
    john.set_name("John Doe");
    john.set_email("jdoe@example.com");
    
    std::ofstream output(argv[0]);
    john.SerializeToOstream(&output);
    output.close();
    return 0;
}
