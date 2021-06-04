#include "decode.h"
#include "ui_decode.h"
#include <QMessageBox>
#include <QPixmap>
#include <iostream>
#include <queue>
#include<string.h>
#include <fstream>
#include <string>
#include <sstream>
#include <QPixmap>
#include <bitset>
typedef unsigned char unsigned_char_size;

decode::decode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::decode)
{
    ui->setupUi(this);

}

decode::~decode()
{
    delete ui;
}

// for the nodes that will form the tree
class node
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

//this arranges the nodes ascending
struct comp{
   bool operator()(node * s1, node * s2)
   {
      return s1->count > s2->count;
   }
};


//function makes the tree
node* priorityq(std::priority_queue<node*,std::vector<node*>,comp> priority_huffman)
{
    while (priority_huffman.size()!=1)
    {
    node* right = priority_huffman.top();
    priority_huffman.pop();
    node* left=priority_huffman.top();
    priority_huffman.pop();
    node* nnode = new node(' ',left->count + right->count);   //node of merged 2 smaller nodes
    nnode->l=left;
    nnode->r=right;
    priority_huffman.push(nnode);
    }
    return priority_huffman.top();
}


//function that decode the binary and store them in a file
void decode(node*root,std::string s,QString dece)
{
   char *opdec=new char[1000];
    QByteArray odec=dece.toLatin1();
    strcpy(opdec,odec.data());
    std::ofstream cleardec (odec);
    cleardec <<"";
            cleardec.close();

            std::fstream outdecode(odec,std::ios::in | std::ios::out |std::ios::app);
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
}


// named first as it's the first part which is common between decode or encode // int out[]
void firstdec(unsigned long ascicpy[],unsigned long countercpy[],int size_dec,std::string y,QString dec)
{
    std::priority_queue<node*,std::vector<node*>,comp>priority_huffman;
    for (int i = 0; i < size_dec; i++)
    {
            node* newNode = new node(ascicpy[i], countercpy[i]);
            priority_huffman.push(newNode);
        }
    node *root=priorityq(priority_huffman);

    decode(root,y,dec);
    }

//function that writes the picture
void print(QString dec,QString source,QString pic)
{
    char *decode=new char[1000];
    QByteArray d=dec.toLatin1();
    strcpy(decode,d.data());

    char *source_dec=new char[1000];
    QByteArray s=source.toLatin1();
    strcpy(source_dec,s.data());

    char *picture=new char[1000];
    QByteArray p=pic.toLatin1();
    strcpy(picture,p.data());

// opens the source file to get the paramenters
        std::fstream sourc(s, std::ios_base::in);
        unsigned short width   =  0;
        unsigned short height  =  0;
        unsigned short max_val  =  0;
        sourc>>width;
        sourc>>height;
        sourc>>max_val;
        sourc.close();
        unsigned long pix_no=width*height;

// opens the decode file to get the data
        std::fstream file(d, std::ios_base::in);
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
      delete [] decode;
      delete [] source_dec;
      delete [] picture;



}


void decode::on_pushButton_clicked()
{  //store inputs
    QString f=ui->folder->text();
    QString fd=ui->asci->text();
    QString oc=ui->output->text();
    QString d=ui->decode_2->text();
    QString s=ui->source->text();
    QString p=ui->pic->text();

    // store toatal locations
    QString freq_dec=f+fd;
    QString op_dec=f+oc;
    QString decode_location=f+d;
    QString source=f+s;
    QString pic=f+p;


    char *freqdec=new char[1000];
    QByteArray fredec=freq_dec.toLatin1();
    strcpy(freqdec,fredec.data());
//open asci file to get no. of used asciis
    std::fstream ascfil(fredec, std::ios_base::in);
    unsigned long total_table_no=0;
    int tem;
    while ( ascfil>>tem )
     {
       total_table_no++;
     }
     ascfil.close();
//open asci file to get frequency table
     std::fstream asc(fredec, std::ios_base::in);
     int used_asci_no_count=total_table_no/2;

     unsigned long* counter=new unsigned long[used_asci_no_count];
     unsigned long* asci=new unsigned long[used_asci_no_count];

     for(int i=0;i<used_asci_no_count;i++)
     {
         asc >> asci[i];
         asc >> counter[i];
     }

    char *opdec=new char[1000];
    QByteArray odec=op_dec.toLatin1();
    strcpy(opdec,odec.data());

//open binary encoded file
    std::fstream mfile(odec, std::ios::in |std::ios::binary);
    std::string dec="";
    int8_t enc_data;
    while (!mfile.eof()) {
        enc_data =mfile.get();
        int uncomp = static_cast<int>(enc_data);
        dec += std::bitset<8>(uncomp).to_string();
    }
    mfile.close();
   /* std::string line_by_line;
    std::string total_binary_no="";
          while ( getline (mfile,line_by_line) )
        {
          total_binary_no+=line_by_line;
        }
          mfile.close();
*/
// call the first function for decoding
     firstdec(asci,counter,used_asci_no_count,dec,decode_location);

// calling printing function to write the image
     print(decode_location,source,pic);

// view message to tell you that the decoded is finished
     QMessageBox::information(this, "Login", "decode is done, please check your folder");

     delete [] freqdec;
     delete [] opdec;
     delete [] counter;
     delete [] asci;
}
