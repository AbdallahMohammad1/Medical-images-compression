#include <iostream>
#include <queue>
#include<string.h>
#include <fstream>
#include <string>
#include <sstream>
#include "pic_in.h"
#include"frequency_table.h"
#include"build_tree.h"
#include <map>




int main()
{ 
    //choose between encode and decode
    std::cout<<"for encode choose '0' and for decode choose '1'"<<std::endl;
    bool choose;
    std::cin>>choose;
    std::cout<<std::endl;


 // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  // for encoding
    if(!choose)
 {
int number_of_pixels;
int *picdata=new int[10];

//calling "picin" function that reads the given picture and make a source(containing width, height,max. value) file for it
picin(picdata,number_of_pixels);

// //////////////////////////////////////////////
int *asci=new int[256];
int *counter=new int[256];
int i=0;

// calling "freqtable function that make frequency table for the picture and write it into a file"
freqtable(counter,picdata,number_of_pixels,i);
int *ascicpy=new int[i];
int *countercpy=new int[i];
for(int x=0;x<i;x++)
{
    ascicpy[x]=freq_map[x].asci_no;
    countercpy[x]=freq_map[x].counter;
}
delete [] asci;
delete [] counter;

// ////////////////////////////////////////////

std::string binary;  //don't care

// calling "first" function which is the first stage and common between encode and decode
first(ascicpy,countercpy,i,binary,choose,picdata,number_of_pixels); 
delete [] ascicpy;
 delete []countercpy;

}
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  // for decoding

    else if(choose)
    {   
      //reads the saved frequeny table
      char locasci[1000];
        std::cout<<"please enter the saved asci file location "<<std::endl;
          std::cin>>locasci;

           std::fstream ascfil(locasci, std::ios_base::in);
    unsigned long total_table_no=0;
    int tem;
    while ( ascfil>>tem )
     {
       total_table_no++;
     }
     ascfil.close();
         std::fstream asc(locasci, std::ios_base::in);
     int used_asci_no_count=total_table_no/2;

     int* counter=new int[used_asci_no_count];
     int* asci=new int[used_asci_no_count];

     for(int i=0;i<used_asci_no_count;i++)
     {
         asc >> asci[i];
         asc >> counter[i];
     }

        // reads the encoded binary file
        char loco[1000];              //location of output txt file
                std::cout<<"please enter the saved output file"<<std::endl;
                  std::cin>>loco;
        std::fstream mfile(loco, std::ios_base::in);
    std::string line_by_line;
    std::string total_binary_no="";
          while ( getline (mfile,line_by_line) )
        {
          total_binary_no+=line_by_line;
        }
          mfile.close();
          int picdata[1];  //don't care
         unsigned long number_of_pixels=0;   //don't care
        
        // calling the same function for encode
    first(asci,counter,used_asci_no_count,total_binary_no,choose,picdata,number_of_pixels);    

    }
return 0;
}
