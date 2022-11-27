#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#define FILE_NAME_UPPER_LIMIT 1000000
#define MAX_CHUNK_SIZE 1000000

typedef long long int ll;

// function to copy the reverse of str1 to str2
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

    // if the valid number of arguments are not passed
    if (argc != 4)
    {
        write(STDOUT_FILENO, "Invalid Number of arguments\n", 28);
        return -1;
    }

    // open the input file
    int input_file = open(argv[1], O_RDONLY);
    if (input_file < 0)
    {
        // handle errors in opening the input file
        perror("Unable to Open The File\nError");
        return 0;
    }

    //break points
    ll break_point_1 = atoll(argv[2]);
    ll break_point_2 = atoll(argv[3]);


    // get the name of the input file
    char input_filename[FILE_NAME_UPPER_LIMIT];
    int curser = strlen(argv[1]) - 1;
    while (argv[1][curser] != '/' && curser >= 0)
    {
        curser--;
    }
    strcpy(input_filename, &argv[1][curser + 1]);

    // to hold the path of the output file
    char output_file_dir[FILE_NAME_UPPER_LIMIT + 20];
    sprintf(output_file_dir, "Assignment/2_%s", input_filename);

    // to hold the file size
    long long int filesize = lseek(input_file, 0, SEEK_END);

    //if the provided breakpoints are invalid
    if(break_point_1>break_point_2||break_point_1<0||break_point_2>filesize){
        write(STDOUT_FILENO , "\e[0;31mInvalid breakpoints provided\nExiting...\e[0m\n",51);
        return -1;
    }

    // to hold the offset
    long long int offset = filesize;

    //seek to breakpoint 1
    offset = lseek(input_file, break_point_1, SEEK_SET);
    
    // Make assignments directory with R,W,X permission for user
    int made_directory = mkdir("Assignment", S_IRWXU);
    if(made_directory < 0){
        perror("Unable to create Assignment directory\nError");
        return -1;
    }

    //output file descriptor
    int output_file = open(output_file_dir, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_file < 0)
    {
        perror("Unable to Create such file\n");
        return 0;
    }
    
    // while we are not done with the first part of the file.....
    while (offset >= 0)
    {

        // if the left over chunk is lesser the max chunk size
        if (offset < MAX_CHUNK_SIZE)
        {
            // No of chars to be read
            int chars_left = offset;

            // go to offset 0
            offset = lseek(input_file, 0, SEEK_SET);
            char ch[chars_left + 1];
            ch[chars_left] = '\0';

            // read the chunk , reverse , and write
            read(input_file, ch, chars_left);
            char reversed_chunk[chars_left + 1];
            strrev(ch, reversed_chunk, chars_left);
            write(output_file, reversed_chunk, chars_left);
        }
        else
        {

            offset = lseek(input_file, -MAX_CHUNK_SIZE, SEEK_CUR);
            char ch[MAX_CHUNK_SIZE + 1];
            ch[MAX_CHUNK_SIZE] = '\0';

            // read , reverse , and write
            int no_of_chars_read = read(input_file, ch, MAX_CHUNK_SIZE);
            ch[no_of_chars_read] = '\0';
            char reversed_chunk[MAX_CHUNK_SIZE + 1];
            strrev(ch, reversed_chunk, no_of_chars_read);
            write(output_file, reversed_chunk, MAX_CHUNK_SIZE);
            // offset = lseek(input_file, -MAX_CHUNK_SIZE, SEEK_CUR);
        }

        // after each chunk is read...

        // find the percentage of file that is done
        double percent_done = ((double)( break_point_1 - offset)*100) / filesize;
        char progress_bar[50];

        // print the progress bar
        sprintf(progress_bar, "\rProgress: %.3lf%c done", percent_done, '%');
        write(STDOUT_FILENO, progress_bar, strlen(progress_bar));

        // seek to the next chunk ( the left side one )
        offset = lseek(input_file, -MAX_CHUNK_SIZE, SEEK_CUR);
    }

    //seek to the first break point
    offset = lseek(input_file, break_point_1, SEEK_SET);

    //while we dont reach 2nd break point....
    while (offset <= break_point_2)
    {
        //if the remaining chunk is larger than chuk size
        if (break_point_2 - offset + 1 > MAX_CHUNK_SIZE)
        {
            char ch[MAX_CHUNK_SIZE + 1];
            ch[MAX_CHUNK_SIZE] = '\0';

            // read , reverse , and write
            int no_of_chars_read = read(input_file, ch, MAX_CHUNK_SIZE);
            offset += MAX_CHUNK_SIZE;
            write(output_file, ch, MAX_CHUNK_SIZE);
        }
        else
        {
            // if the remaining chunk is smaller than the chunk size
            int chars_to_be_read = break_point_2 - offset + 1;

            char ch[chars_to_be_read + 1];
            ch[chars_to_be_read] = '\0';

            read(input_file, ch, chars_to_be_read);
            write(output_file, ch, chars_to_be_read);
            offset+=chars_to_be_read;
        }

        // calculate and print the progress bar
        double percent_done = ((double)(offset)*100) / filesize;
        char progress_bar[50];

        sprintf(progress_bar, "\rProgress: %.3lf%c done",percent_done, '%');
        write(STDOUT_FILENO, progress_bar, strlen(progress_bar));
    }

    // seek to the end ( to read the 3rd part in reverse order)
    offset = lseek(input_file,0,SEEK_END);

    //while we are not done with 3rd part....
    while (offset > break_point_2+1)
    {

        //if the left over chunk is lesser the max chunk size
        if (offset - break_point_2 - 1 < MAX_CHUNK_SIZE)
        {
            //No of chars to be read
            int chars_left = offset-break_point_2-1;

            //go to offset break_point_2+1
            offset = lseek(input_file, break_point_2+1, SEEK_SET);
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
        double percent_done = ( (double) break_point_2 + 1 +  ( filesize - offset) ) * 100 / filesize;
        char progress_bar[125];

        //print the progress bar
        sprintf(progress_bar, "\rProgress: %.3lf%c done", percent_done, '%');
        write(STDOUT_FILENO, progress_bar, strlen(progress_bar));

        //seek to the next chunk ( the left side one )
        offset = lseek(input_file, -MAX_CHUNK_SIZE, SEEK_CUR);
    }
    write(STDOUT_FILENO, "\n", 1);

    // close the files
    close(input_file);
    close(output_file);
}