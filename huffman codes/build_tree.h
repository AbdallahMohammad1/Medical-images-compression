#ifndef BUILD_TREE_H
#define BUILD_TREE_H

#include <iostream>
#include <queue>
#include<string.h>
#include <fstream>
#include <string>
#include <sstream>
#include<bitset>

typedef unsigned char unsigned_char_size;
struct binary_enc  // struct have each ascii number with its binary encoded and number of them
{
  int asci;
  int binary[20];
  int countb;     //count of binary number
};
binary_enc bin_enc[256];
int m=0;
class node { // for the nodes that will form the tree
public:
    int ascii;
   int count;
   node *l;
   node *r;
   node(int asci, int counter)
   {
       ascii=asci;
       count=counter;
     l = r = NULL;
   }
};

struct compare{ //this arrange the nodes ascending
   bool operator()(node * s1, node * s2)
   {
      return s1->count > s2->count;
   }
};

node* priority_q(std::priority_queue<node*,std::vector<node*>,compare> M) //function makes the tree
{
    while (M.size()!=1)
    {
    node* right = M.top();
    M.pop();
    node* left=M.top();
    M.pop();
    node* nnode = new node(' ',left->count + right->count);   //node of merged  the 2 smallest nodes
    nnode->l=left;
    nnode->r=right;
    M.push(nnode);
    }
    return M.top();
}

void traverse(node* root, int arr[], int counter)  //function to traverse the tree to get the encoded binary
{
    if(root->r)                               //making binary encoded using traverse property that follow the root path untill reaching the leaf
    {
        arr[counter]=0;
        traverse(root->r,arr,counter+1);           // if there is a path right it will add zero to the array of encoded binary
    }
    if(root->l)                                   // if there is a path left it will add one to the array of encoded binary
    {
        arr[counter]=1;
        traverse(root->l,arr,counter+1);
    }
    if(!root->l&& !root->r)                        // if there is no path more so it will stop searching
    {

        bin_enc[m].asci=root->ascii;
         std::cout<<bin_enc[m].asci<<" ";
        bin_enc[m].countb=counter;
                for (int i = 0; i < counter; i++) {
                    bin_enc[m].binary[i]=arr[i];
                   std::cout<<bin_enc[m].binary[i];
                }
                m++;
                 std::cout<<std::endl;
    }

}
void encode(int picdata[],int used_ascii_no_count,int number_of_pixels)   //function to make the whole pixels encoded and stored in a file
{char locout[1000];
    std::cout<<"please enter output encode destination"<<std::endl;
      std::cin>>locout;
      int c=0;
      std::ofstream clearoutput (locout);
      clearoutput <<"";
      clearoutput.close();

std::fstream output(locout,std::ios::in | std::ios::out |std::ios::app);

for(int y=0;y<number_of_pixels-1;y++)
    {
        for(int x=0;x<used_ascii_no_count;x++)
            if(picdata[y]==bin_enc[x].asci)
            {
                for(int z=0;z<bin_enc[x].countb;z++)
                {
                    c++;
                    output << bin_enc[x].binary[z];

                }
            }

}

output.close();
std::cout<<"the new number of binaries is  ";
std::cout<<c<<std::endl;
unsigned long red_bits=number_of_pixels*8-c;
std::cout<<"which means that the number reduced binaries is  "<<red_bits<<std::endl;
}
void decode(node*root,std::string s)         //function that decode the binary and store them in a file
{char locdecode[1000];
        std::cout<<"please enter output decode destination"<<std::endl;
          std::cin>>locdecode;
     std::ofstream cleardec (locdecode);
    cleardec <<"";
            cleardec.close();

            std::fstream outdecode(locdecode,std::ios::in | std::ios::out |std::ios::app);
                node *current=root;
                int n=s.size();
                std::string decode="";
                char *numbers=new char[n+1];
                strcpy(numbers,s.c_str());
                for(int dec=0;dec<n;dec++)
                {
                    if(numbers[dec]=='0')
                    {
                       current= current->r;
                    }
                    if(numbers[dec]=='1')
                    {
                       current= current->l;
                    }
                     if(current->l==NULL&&current->r==NULL)
                     {
                       outdecode <<current->ascii<<" ";
                         current=root;
                     }

                }
                outdecode.close();
    char source[1000];
        std::cout<<"please enter saved source location"<<std::endl;
          std::cin>>source;
          char p[1000];
        std::cout<<"please enter the decoded picture destination"<<std::endl;
          std::cin>>p;

  // opens the source file to get the paramenters
        std::fstream sourc(source, std::ios_base::in);
        unsigned short width   =  0;
        unsigned short height  =  0;
        unsigned short max_val  =  0;
        sourc>>width;
        sourc>>height;
        sourc>>max_val;
        sourc.close();
        unsigned long pix_no=width*height;

// opens the decode file to get the data
        std::fstream file(locdecode, std::ios_base::in);
        int * out=new int[pix_no];
        for(unsigned long i=0;i<pix_no;i++)
          {
            file >> out[i];
          }

       // allocate memory for your data
      unsigned char *buff = new unsigned char[pix_no*sizeof(unsigned_char_size)];

      for (unsigned long i = 0; i < pix_no; i++)
        buff[i] = out[i];


      // output file streams

      std::ofstream clearpic (p);
       clearpic <<"";
       clearpic.close();
      std::ofstream printout (p);

      // write the header
      printout << "P5\n" << width << " " << height << " "<<max_val<<std::endl;

      // write the data
      printout.write((char *)buff, pix_no*sizeof(unsigned_char_size));

      // close the stream
      printout.close();

      // free memory
      delete[] buff;

}



void first(int ascicpy[],int countercpy[],int no_of_used_asci,std::string s,bool choose,int picdata[],int number_of_pixels)  // named first as it's the first part which is common between decode or encode
{
    std::priority_queue<node*,std::vector<node*>,compare>M;
    for (int i = 0; i < no_of_used_asci; i++)
    {
            node* newNode = new node(ascicpy[i], countercpy[i]);
            M.push(newNode);
        }
    node *root=priority_q(M);
    int arr[256];
    int top=0;
    traverse(root,arr,top);
    if(choose)
    decode(root,s);
    else
        encode(picdata,no_of_used_asci,number_of_pixels);
    }
#endif // BUILD_TREE_H