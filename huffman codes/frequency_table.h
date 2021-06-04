#ifndef FREQUENCY_TABLE_H
#define FREQUENCY_TABLE_H
#include <iostream>
#include<fstream>
#include <map>

struct freq_table
{
  int asci_no;
  int counter;
};

  //define a map for frequency table
 std::map<int, freq_table>freq_map;

void freqtable(int*& counter,int*& picdata,int number,int& j)
{
  
char fr[1000]; //storing asci file and counter file
         std::cout<<"please enter asci file distenation"<<std::endl;
           std::cin>>fr;
for(int y=0;y<256;y++) // inisialize the values for asci and counters
 {
     counter[y]=0;
 }

 for(int x=0;x<number;x++)     // counting the frequency
         {
             for(int y=0;y<256;y++)
             {
              if(picdata[x]==y)
                  counter[y]++;
             }
         }
 for(int i=0;i<256;i++)   //saving non zero counters for asci in the map
 {
     if(counter[i]!=0)
     {
         freq_map[j].counter=counter[i];
         freq_map[j].asci_no=i;
         j++;
     }
 }


 
// make the folder empty to write new values
 std::ofstream clearfr (fr);
   clearfr <<"";
   clearfr.close();
// writing the frequency table
 std::fstream ascfile(fr,std::ios::in | std::ios::out |std::ios::app);

 for(int i=0;i<j;i++)
 {
     ascfile<<freq_map[i].asci_no<<" "<<freq_map[i].counter<<" ";
 }


}


#endif // FREQUENCY_TABLE_H