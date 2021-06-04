#include "encode.h"
#include "ui_encode.h"
#include <QMessageBox>
#include <QPixmap>
#include <iostream>
#include <queue>
#include<string.h>
#include <fstream>
#include <string>
#include <sstream>
#include <QPixmap>
#include <map>
#include <bitset>
#include <vector>
typedef unsigned char uchar;

encode::encode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::encode)
{
    ui->setupUi(this);

}

encode::~encode()
{
    delete ui;
}

struct binary_encoded  // struct have each ascii number with its binary encoded and number of them
{
  int asci;
  int binary[256];
  int countb;     //count of binary number
};

int m=0;     //initialize counter for an array of struct binary_encode

class node     // for the nodes that will form the tree
{

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

struct freqtable
{
  int asci_no;
  int counter;
};

  //define a map for frequency table
 std::map<int, freqtable>freq_map;

 struct compare  //this arrange the nodes ascending
{
   bool operator()(node * s1, node * s2)
   {
      return s1->count > s2->count;
   }
};

void picin(int*& picdata,int& number,QString pic,QString source)
{
    delete []picdata;   //delete the initial dynamic array
    char *picture_location=new char[1000];
QByteArray p=pic.toLatin1();
strcpy(picture_location,p.data());

    int cols, rows, max_val;
        std::ifstream is (p, std::ifstream::binary);
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

            char *source_loc=new char[1000];
               QByteArray source_location=source.toLatin1();
               strcpy(source_loc,source_location.data());

            std::ofstream clearsource (source_location);   //for clearing previous data in the txt file
               clearsource <<"";
               clearsource.close();

            std::fstream source_file(source_location,std::ios::in | std::ios::out |std::ios::app);
               source_file<<cols<<" ";
               source_file<<rows<<" ";
               source_file<<max_val<<" ";

                delete [] first_read;
                delete[] source_loc;
                delete[] picture_location;

}

void freqtable(unsigned long*& counter,int*& picdata,int number,int& j,QString freq)
{
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


 char *fre=new char[1000];
 QByteArray fr=freq.toLatin1();
 strcpy(fre,fr.data());

 std::ofstream clearfr (fr);
   clearfr <<"";
   clearfr.close();

 std::fstream ascfile(fr,std::ios::in | std::ios::out |std::ios::app);

 for(int i=0;i<j;i++)
 {
     ascfile<<freq_map[i].asci_no<<" "<<freq_map[i].counter<<" ";
 }

 delete[] fre;

}

node* priority_q(std::priority_queue<node*,std::vector<node*>,compare> priority_huffman) //function makes the tree
{
    while (priority_huffman.size()!=1)
    {
    node* right = priority_huffman.top();
    priority_huffman.pop();
    node* left=priority_huffman.top();
    priority_huffman.pop();
    node* nnode = new node(' ',left->count + right->count);   //node of merged the 2 smallest nodes
    nnode->l=left;
    nnode->r=right;
    priority_huffman.push(nnode);
    }
    return priority_huffman.top();
}

void traverse(node* root, int arr_binary[], int count_binary,binary_encoded *& bin_enc)  //function to traverse the tree to get the encoded binary
{
    if(root->r)                               //making binary encoded using traverse property that follow the root path untill reaching the leaf
    {
        arr_binary[count_binary]=0;
        traverse(root->r,arr_binary,count_binary+1,bin_enc);           // if there is a path right it will add zero to the array of encoded binary
    }
    if(root->l)                                   // if there is a path left it will add one to the array of encoded binary
    {
        arr_binary[count_binary]=1;
        traverse(root->l,arr_binary,count_binary+1,bin_enc);
    }
    if(!root->l&& !root->r)                        // if there is no path more so it will stop searching
    {

        bin_enc[m].asci=root->ascii;
         std::cout<<bin_enc[m].asci<<" ";
        bin_enc[m].countb=count_binary;
                for (int i = 0; i < count_binary; i++) {
                    bin_enc[m].binary[i]=arr_binary[i];
                   std::cout<<bin_enc[m].binary[i];
                }
                m++;
                 std::cout<<std::endl;
    }

}
void encode(int picdata[],int used_ascii_no_count,int number_of_pixels,QString op,double& red_bits,binary_encoded *&bin_enc)   //function to make the whole pixels encoded and stored in a file
{
    char *opch=new char[1000];
    QByteArray oparr=op.toLatin1();
    strcpy(opch,oparr.data());

      int c=0;
      std::ofstream clearoutput (oparr);
      clearoutput <<"";
      clearoutput.close();

std::fstream output(oparr,std::ios::binary | std::ios::out );
std::string enc="";
for(int y=0;y<number_of_pixels-1;y++)
    {
        for(int x=0;x<used_ascii_no_count;x++)
            if(picdata[y]==bin_enc[x].asci)
            {
                for(int z=0;z<bin_enc[x].countb;z++)
                {
                    c++;
                    enc+= std::bitset<1>(bin_enc[x].binary[z]).to_string();
                }
            }

}

//for writing the encoded file in a suitable size of memory..
std::vector<uint8_t> dec_vec;
std::string encoder="";
for(auto enc1 : enc)
 {
    encoder.push_back(enc1);
    if(encoder.size() ==8)
    {
    dec_vec.push_back(std::bitset<8>(encoder).to_ulong());
    encoder="";
    }
  }
int num_of_zeros =enc.length() % 8;
if(num_of_zeros !=0)
  {
    for(int i=0;i<8-num_of_zeros;i++)
    {
        encoder.push_back('0');
    }
    if(encoder.size() ==8)
    {
        dec_vec.push_back(std::bitset<8>(encoder).to_ulong());
    }
}
for(auto enc2 : dec_vec)
{
    output<<enc2;
}

output.close();
std::cout<<"the new number of binaries is  ";
std::cout<<c<<std::endl;
 red_bits=number_of_pixels*8-c;
std::cout<<"which means that the number reduced binaries is  "<<red_bits<<std::endl;
delete [] opch;

}




void firstenc(int used_ascii_no_count,QString op,int picdata[],int number_of_pixels,double& red_bits)  // named first as it's the first part which is common between decode or encode
{
    std::priority_queue<node*,std::vector<node*>,compare>priority_huffman;
    for (int i = 0; i < used_ascii_no_count; i++)
    {
            node* newNode = new node(freq_map[i].asci_no, freq_map[i].counter);
            priority_huffman.push(newNode);
        }
    node *root=priority_q(priority_huffman);
    int arr_binary[256];
    int top=0;
         binary_encoded *bin_enc=new binary_encoded[256];
    traverse(root,arr_binary,top,bin_enc);
        encode(picdata,used_ascii_no_count,number_of_pixels,op,red_bits,bin_enc);
    delete [] bin_enc;
}


void encode::on_pushButton_clicked()
{   // save input as strings
    QString location_folder=ui->folder->text();
    QString pic_loc= ui->pic->text();
    QString source_loc= ui->source->text();
    QString asci_loc= ui->asci->text();
    QString output_loc=ui->output->text();

    // saving full location
    QString source=location_folder+source_loc;
    QString freq=location_folder+asci_loc;
    QString output_location=location_folder+output_loc;

    //define variables for picture reading function "picin"
    int number_of_pixels;
    int *picdata=new int[10];

    //calling picture reading function "picin"
    picin(picdata,number_of_pixels,pic_loc,source);

    //define variables for fequency table function "freqtable"
    unsigned long *counter=new unsigned long[256];
    int used_ascii_no_count=0; //determines the number of used ascii numbers
    freqtable(counter,picdata,number_of_pixels,used_ascii_no_count,freq);

    //define some variables for huffman fist stage function "firstenc"
    //std::string binary;
    double reduced_bits=0;

    //calling huffman fist stage function "firstenc"
    firstenc(used_ascii_no_count,output_location,picdata,number_of_pixels,reduced_bits);

    m=0; //re initialize counter for an array of struct binary_encode

    delete [] picdata;

    //commands to display reduced bits and percentage pf reduction
    float x=(reduced_bits/(number_of_pixels*8))*100;
    std::cout<<x<<std::endl;
    ui->lcdNumber->display(reduced_bits);
    ui->lcdNumber_2->display(x);

}
