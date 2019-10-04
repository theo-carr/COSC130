//lab5
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct Pixel{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};
#pragma pack(push, 2)
struct BitmapFileHeader {
    short int type;
    int size;
    short int r1; //reserve
    short int r2; //reserve
    int offset;

};
struct BitmapInfoHeader {
    int size;
    int  w;
    int  h;
    short int planes;
    short int bitcount;
    int compression;
    int x_ppm; //x pixels per meter
    int y_ppm;
    int color_used;
    int color_important;
};
#pragma pack(pop)

int main(int argc, char** argv){
  char * infile = argv[1];
  char * outfile = argv[2];
  FILE * ifile = fopen(infile,"rb");
  if (ifile != nullptr){
      //declare header structs
      BitmapFileHeader fhead;
      BitmapFileHeader * fpoint;
      BitmapInfoHeader ihead;
      BitmapInfoHeader * ipoint;
      fpoint = &fhead;
      ipoint = &ihead;
      //read in file header
      fread(fpoint,(sizeof(char)*14),1,ifile);
      //read in info header
      fread(ipoint,(sizeof(char)*40),1,ifile);
      //calculate padding
      unsigned int pad_count =  4 - ((ihead.w * 3) % 4);
      //create pixel array
      Pixel pixel_array[ihead.h * ihead.w];
      Pixel * pp;
      pp = pixel_array;
      //move file pointer to offset
      fseek(ifile,fhead.offset,SEEK_SET);
      //copy pixels into array from ifile
      for (int i = 0; i < ihead.h; i++){
          fread(pp,(sizeof(char)*ihead.w),1,ifile);
          //skip padding and move array pointer
          fseek(ifile,pad_count,SEEK_CUR);
          pp += ihead.w;
      }
      //open outfile
      FILE * ofile = fopen(outfile,"wb");
      //reset file pointers to start
      rewind(ifile);
      fseek(ofile,0,SEEK_SET);
      //copy headers to output file
      fwrite(fpoint,(sizeof(char)*14),1,ofile);
      fwrite(ipoint,(sizeof(char)*40),1,ofile);
      //write pixel array to output file
      pp = pixel_array; //reset pp position in pixel_array
      //create padding to write into outfile
      if (pad_count == 3){
          char temp[3];
          char *  padding;
          padding = temp;
          for (int i = 0; i < ihead.h; i++){
              fwrite(pp,(sizeof(char)*ihead.w),1,ofile);
              fwrite(padding,1,3,ofile);
              pp += ihead.w;
          }
        }
      else if (pad_count == 2){
          char temp[2];
          char * padding;
          padding = temp;
          for (int i = 0; i < ihead.h; i++){
              fwrite(pp,(sizeof(char)*ihead.w),1,ofile);
              fwrite(padding,1,2,ofile);
              pp += ihead.w;
          }
      }
      else{
          unsigned char temp = 0;
          unsigned char * padding;
          padding = &temp;
          for (int i = 0; i < ihead.h; i++){
              fwrite(pp,(sizeof(char)*ihead.w),1,ofile);
              fwrite(padding,1,1,ofile);
              pp += ihead.w;
          }
      }
      
  }
    return 0;
}


