#include <iostream>
#include <message.pb.h>
 
using namespace std;
 
int main(int argc, char* argv[])
{
    Person john = Person.newBuilder()
    .setId(1234)
    .setName("John Doe")
    .setEmail("jdoe@example.com")
    .build();
    
    output = new FileOutputStream(args[0]);
    john.writeTo(output);
    return 0;
}