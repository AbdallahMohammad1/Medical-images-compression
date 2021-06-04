#include <iostream>
#include <queue>
#include<string.h>
#include <fstream>
#include <string>
#include <sstream>

void picin(int*& picdata,int& number)
{
    delete []picdata;   //delete the initial dynamic array
    char location[1000];
    std::cout<<"please enter location of picture"<<std::endl;
      std::cin>>location;
    
delete []picdata;   //delete the initial dynamic array
   
    int cols, rows, max_val;
        std::ifstream is (location, std::ifstream::binary);
        std::string line="";
        getline(is,line);
        std::stringstream pix;
        pix << is.rdbuf();
                pix >> cols >> rows;      // store number of columns and rows in variables
                std::cout << cols << " col , " << rows << " row" <<std:: endl;

                pix >> max_val;  //store the maximum ascii number can be present in the pixels
                std::cout<<max_val<<std::endl;

            is.seekg (0, is.end);
            int length = is.tellg();
            is.seekg (0, is.beg);

            unsigned char * first_read = new unsigned char [length];

            is.read ((char *)first_read,length);
            int no=length-cols*rows;   //correct the reading counter due due presence of elements before the pixel data
            number=cols*rows;
            picdata=new int[number];  // redefine the picdata array with new number of elements
              for(int i=no; i<length;i++)
              picdata[i-no] = first_read[i];

            char loc_source[1000];
    std::cout<<"please enter location of source file"<<std::endl;
      std::cin>>loc_source;

            std::ofstream clearsource (loc_source);   //for clearing previous data in the txt file
               clearsource <<"";
               clearsource.close();

            std::fstream source_file(loc_source,std::ios::in | std::ios::out |std::ios::app);
               source_file<<cols<<" ";
               source_file<<rows<<" ";
               source_file<<max_val<<" ";

                delete [] first_read;


}
