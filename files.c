/* National Autonomus University of Mexico */
/* School of Engineering */
/* Computer Enginnering */
/* Operative Systems */
/* Developed by Daniel Zarco*/

/* Libraries to use along development */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* Adding library for use strcpy()  for assignment string value from file to add value at variable */
#include <math.h> /* LIbrary to use round up for numbers of block to use */
/* Define struct for storage data of reading files */
const int size_block = 127;
struct files {
  char name[12]; /* parameter to limit of sotorage for name extention */
  int size, fragment; /* parameter to storage size of every file */
};
struct disk {
  int blocks[59];
};
/* functions to use */
struct disk set_data_diks(struct disk hdd);
struct disk disk_management(struct files arrf[], struct disk hdd);
void show_up(struct files arrf[], struct disk hdd);
struct disk add_file(struct files arrf[], struct disk hdd);
struct disk delete_file(struct files arrf[], struct disk hdd);
/* Beginning of main functions */
int main(){
  char buffer[12];
  int tam = 0;
  size_t line ;
  static struct files array_files[19]; /* Array of file struct to manage our information, 'cause is more easy manage that information from a file, and data from file storage in runtime. Size is 20 'cause in last zoom session Miss give us the limit of 20 files to read into source file'*/
  static struct disk hard_disk;
  FILE *source; /* Declaring pointer to track source file through runtime */
  source = fopen("./source.txt","r");
  /* This for use for reading information about our file and will can manage the information to allocation disk requeriments */
  for(int index = 0; index < 20; index++){
    if((line = fgetc(source)) != EOF){ /* Validate when file is different of last line and null */
      fscanf(source, "%s", buffer); /* gets name from first line of file */
      fscanf(source, "%d", &tam); /*  gets size of first line of file corresponding to first file's name (back instructions) */
      strcpy(array_files[index].name,buffer); /* Because we use an array, we don't pass the buffer information to struct's array name and we copy the content at variable */
      array_files[index].size = tam;
    }
    else{ /* Setting empty values for spaces to storage more files 'cause if we don assing value, we have trash in runtime */
      strcpy(array_files[index].name,"");
      array_files[index].size = 0;
    }
  }
  fclose(source);
  /* Beggnning of second architecture phase: Manage data */
  hard_disk = set_data_diks(hard_disk);
  hard_disk = disk_management(array_files, hard_disk);
  /* Starting loop for interface */
  int chose = 0;
  do{
    printf("***** Helium File Manager v1.0 ******\n");
    printf("Welcome, you can do next options: \n\n");
    printf("---> 1: Show disk directory and files storage.\n");
    printf("---> 2: Add file to disk.\n");
    printf("---> 3: Delete file to disk.\n");
    printf("---> 4: Exit.\n");
    printf("Choose: \n");
    scanf("%d", &chose);
    switch(chose){
      case 1:
        show_up(array_files, hard_disk);
        break;
      case 2:
        hard_disk = add_file(array_files, hard_disk);
        show_up(array_files, hard_disk);
        break;
      case 3:
        hard_disk = delete_file(array_files, hard_disk);
        show_up(array_files, hard_disk);
        break;
      case 4:
        printf("Thanks for use Helium File Manager :)\n");
        exit(0);
      default:
        printf("Your chose is not valid at show up options\n");
    }
  } while(chose != 4); /* We need a do while 'cause the part of do executing for first time before to entrance at while loop, for this case, the interface needs this loop */
}

/* end main */
/* Definition of header funtions  */
struct disk set_data_diks(struct disk hdd){
  /* We need two for cicles 'cause track struct array have another array inside that behaivor is like an matriz, and this anidade cicles we use to set 127 bytes size of every block; */
  for(int i = 1; i < 60; i++){
    hdd.blocks[i] = 127; /* this part we setting all values of our dik at 127, so this value is the size of the block */
  }
  return hdd; /* returning the hdd struct with sets value block */
}

struct disk disk_management(struct files arrf[], struct disk hdd){ /* whit this function we manage the information from file and gets first manage to the file at disk */
  int size_file = 0, aux = 0;
  int num_blocks = 0;
  int stari = 1, endi = 0;
  for(int i = 0; i < 20; i++){ /* this for accesing the array of files and next acces to hdd struct to assign the information of file in memory */ 
    size_file = arrf[i].size;
    num_blocks = (size_file/size_block)+1;
    endi = endi + num_blocks;
    aux = size_file;
    for(stari; stari < endi + 1; stari++){ /* we need sum one to the endi to get next block and keep the continuity */
      hdd.blocks[stari] = size_file;
      aux = aux - size_block;
      if(stari == endi){
        arrf[i].fragment = -aux; /* Because the fragment is an negative number, we pre multiplied by (-) to gets positive fragment */
      }
    }
    stari = endi + 1; /* gets the indez star with one unit more to get the nex file and do the requeriment of block at show up operation */
  }

  return hdd; /* returning hdd struct whit information modified */
}

void show_up(struct files arrf[], struct disk hdd){ /* with this function we show up every information that was modfied in the first time to manage data from file */
  int size_file;
  printf("****** Operative Systems ******\n");
  printf("Showing content of Disk: Helium - File manager\n");
  for(int i = 0; i < 20; i++){
    size_file = arrf[i].size;
    if(arrf[i].size != 0){
      printf("| File: %s, Size: %d,| Block: ",arrf[i].name, arrf[i].size);
      for(int j = 0; j < 60; j++){
        if(size_file == hdd.blocks[j]){
          printf("%d ", j);
        }
      }
    }
    else{
      continue;
    }
    printf("| Fragment of last block: %d bytes\n", arrf[i].fragment); /* in this line we show fragment that this value is a variable of file struct */
  }
  printf("****** Showig all files on disk[ok] ******\n\n");
}

struct disk add_file(struct files arrf[], struct disk hdd){
  char name_file[12];
  int size_file = 0, num_blocks = 0, blocks_emptyi = 0, remain_block = 0;
  printf("****** Adding file to disk ******\n");
  printf("What's name of file to add? Note: 12 characters maximum! \n");
  scanf("%s", name_file); /* gets name of file from user's input */
  printf("What's size (in bytes) of the file: %s:\n", name_file);
  scanf("%d", &size_file); /* gets size of file from user's input */
  num_blocks = (size_file/size_block)+1;
  remain_block = num_blocks; /* this is an index to gets the block remaining to assign into hdd blocks */
  printf("File: %s, Size: %d |Block: ", name_file, size_file);
  for(int i = 0; i < 20; i++){ /* this case is when to stop 'cause the cicle for continues with their iterations and need stop at first iteration when we have an empty space */
    if(strcmp(arrf[i-1].name, name_file) == 0){
      break;
    }
    else if(arrf[i].size == 0){ /* when we gets an empty space in memory files, we proceed to assign file information */
      arrf[i].size = size_file;
      strcpy(arrf[i].name, name_file);
      for(int j = 0; j < 60; j++){
        if(hdd.blocks[j] == 0){ /* this for counts how many block are empty to gets one index to can compare free block with numer of block to be of file */
          blocks_emptyi = blocks_emptyi + 1;
        }
      }
      if(blocks_emptyi > num_blocks){
        for(int m = 0; m < 60; m++){
          if(hdd.blocks[m] == 0 && remain_block > 0){ /* this condition if when we have fisrt index of block empty and gets remaining block to assign in hdd */
            hdd.blocks[m] = size_file;
            remain_block = remain_block -1;
            if(m != 0)
              printf("%d ", m);
          }
        }
      } 
      else{ /* in this case we assign the file in memory but we can't assign file on disk 'cause we don't have sufficent empty blocks */
        printf("WARNING: File was added but don't have free block on disk. File: %s, size: %d\n", name_file, size_file);
        break;
      }
    }
  }
  printf(" | was added on Disk.\n");
  return hdd; /* returning hdd struct */
}
struct disk delete_file(struct files arrf[], struct disk hdd){ /* this function will delete to specief dile by name */
  char name_file[12];
  int file_ref_size = 0;
  printf("****** Delete file to disk ******\n");
  printf("What's name of file to delete? Note: 12 characters maximum! \n");
  scanf("%s", name_file); /* gets file name to will delete */
  printf("File: %s | Blocks: ", name_file);
  for(int i = 0; i < 20; i++){ /* This functions have the same form to access than add_file, delete_file and show up, acces is like a waterfall, first array of files and next is hdd. */
    if(strcmp(arrf[i].name,name_file) == 0){
      file_ref_size = arrf[i].size;
      strcpy(arrf[i].name, "");
      for(int j = 0; j < 60; j++){
        if(hdd.blocks[j] == file_ref_size){
          printf("%d ",j);
          hdd.blocks[j] = 0;
        }
        else
          continue;
      }
      arrf[i].size = 0;
    }
  } 
  printf("was free\n");
  
  return hdd;
}
