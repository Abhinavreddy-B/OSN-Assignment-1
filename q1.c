#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define FILE_NAME_UPPER_LIMIT 1000000
#define MAX_CHUNK_SIZE 1000000

//function to copy the reverse of str1 to str2
void strrev(char *str1, char *str2, int length)
{
    for (int i = 0; i < length; i++)
    {
        str2[length - i - 1] = str1[i];
    }
    str2[length] = '\0';
}

int main(int argc, char *argv[])
{

    //if the valid number of arguments are not passed
    if (argc != 2)
    {
        write(STDOUT_FILENO, "Invalid Number of arguments\n", 28);
        return -1;
    }

    //open the input file
    int input_file = open(argv[1], O_RDONLY);
    if (input_file < 0)
    {
        //handle errors in opening the input file
        perror("Unable to Open The File\nError: ");
        return 0;
    }

    //get the name of the input file
    char input_filename[FILE_NAME_UPPER_LIMIT];
    int curser = strlen(argv[1]) - 1;
    while (argv[1][curser] != '/' && curser >= 0)
    {
        curser--;
    }
    strcpy(input_filename, &argv[1][curser + 1]);

    //to hold the path of the output file
    char output_file_dir[FILE_NAME_UPPER_LIMIT + 20];
    sprintf(output_file_dir, "Assignment/1_%s", input_filename);

    //Make assignments directory with R,W,X permission for user
    int made_directory = mkdir("Assignment", S_IRWXU);
    if(made_directory < 0){
        perror("Unable to create Assignment directory\nError");
        return -1;
    }

    // write(STDOUT_FILENO,"\n",1);
    int output_file = open(output_file_dir, O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR );
    if(output_file < 0 ){
        perror("Unable to Create such file\n");
        return -1;
    }

    //to hold the file size
    long long int filesize = lseek(input_file, 0, SEEK_END);

    //to hold the offset
    long long int offset = filesize;
    
    //while we are not done with the file.....
    while (offset >= 0)
    {
        
        //if the left over chunk is lesser the max chunk size
        if (offset < MAX_CHUNK_SIZE)
        {
            //No o f chars to be read
            int chars_left = offset;

            //go to offset 0
            offset = lseek(input_file, 0, SEEK_SET);
            char ch[chars_left + 1];
            ch[chars_left] = '\0';

            //read the chunk , reverse , and write
            read(input_file,ch,chars_left);
            char reversed_chunk[chars_left + 1];
            strrev(ch, reversed_chunk, chars_left);
            write(output_file, reversed_chunk, chars_left);

        }
        else
        {

            offset = lseek(input_file,-MAX_CHUNK_SIZE,SEEK_CUR);
            char ch[MAX_CHUNK_SIZE + 1];
            ch[MAX_CHUNK_SIZE] = '\0';

            //read , reverse , and write
            int no_of_chars_read = read(input_file, ch, MAX_CHUNK_SIZE);
            ch[no_of_chars_read] = '\0';
            char reversed_chunk[MAX_CHUNK_SIZE + 1];
            strrev(ch, reversed_chunk, no_of_chars_read);
            write(output_file, reversed_chunk, MAX_CHUNK_SIZE);
        }

        //after each chunk is read...

        //find the percentage of file that is done
        double percent_done = ( (double) (filesize - offset) * 100) / filesize;
        char progress[50];

        //print the progress bar
        sprintf(progress, "\rProgress: %.3lf%c done",percent_done, '%');
        write(STDOUT_FILENO, progress, strlen(progress));

        //seek to the next chunk ( the left side one )
        offset = lseek(input_file, -MAX_CHUNK_SIZE, SEEK_CUR);
    }
    write(STDOUT_FILENO, "\n", 1);

    //close the files
    close(input_file);
    close(output_file);
}