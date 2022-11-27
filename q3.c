#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_FILE_PATH_SIZE 1000000
#define MAX_CHUNK_SIZE 1000000

typedef long long int ll;

char *permission_holder[] = {"User", "Group", "Others"};
char *permission_type[] = {"read permissions", "write permission", "execute permission"};
int permission_masks[3][3] = {{S_IRUSR, S_IWUSR, S_IXUSR}, {S_IRGRP, S_IWGRP, S_IXGRP}, {S_IROTH, S_IWOTH, S_IXOTH}};

// function to print permissions
void Print_Perms(int perms, char *filename)
{
    // write(STDOUT_FILENO,"\n\n-------------------\n",22);
    for (int holder = 0; holder < 3; holder++){
        for (int type = 0; type < 3; type++){
            char sentence[100];
            if (perms & permission_masks[holder][type]){
                sprintf(sentence, "%s has %s on %s: Yes\n", permission_holder[holder], permission_type[type], filename);
            }else{
                sprintf(sentence, "%s has %s on %s: No\n", permission_holder[holder], permission_type[type], filename);
            }
            write(STDOUT_FILENO, sentence, strlen(sentence));
        }
    }
}

// check if str1 is reverse of str2
int checkrev(char *str1, char *str2)
{
    int length = strlen(str1);
    int flag = 1;
    for (int i = 0; str1[i] != '\0'; i++)
    {
        flag = flag && (str2[length - i - 1] == str1[i]);
    }
    return flag;
}

int main(int argc, char *argv[])
{
    // invalid number of arguments
    if (argc != 4)
    {
        write(STDOUT_FILENO, "Invalid Number of Arguments\nExiting....\n", 40);
        return 0;
    }

    // storing file and directory paths
    char new_file[MAX_FILE_PATH_SIZE];
    strcpy(new_file, argv[1]);
    char old_file[MAX_FILE_PATH_SIZE];
    strcpy(old_file, argv[2]);
    char directory[MAX_FILE_PATH_SIZE];
    strcpy(directory, argv[3]);

    // structures to store the meta data of the files
    struct stat director_perms, old_file_perms, new_file_perms;

    int is_directory_created = 1;

    // check if directory exists at that location
    if (stat(directory, &director_perms) == -1)
    {
        // perror("Some Problem occured\nError: ");
        // return 1;
        is_directory_created = 0;
    }
    if (is_directory_created)
    {
        if (!S_ISDIR(director_perms.st_mode))
        {
            is_directory_created = 0;
        }
    }

    if (is_directory_created)
    {
        write(STDOUT_FILENO, "Directory is created: Yes\n", 26);
        // Print_Perms(director_perms.st_mode, "directory");
    }
    else
    {
        write(STDOUT_FILENO, "Directory is created: No\n", 25);
        // write(STDOUT_FILENO,"!!!  Directory Does not exist , So permissions not printed\n",59);
    }

    // file descriptors
    int old_file_descrptr = open(old_file, O_RDONLY);
    int new_file_descrptr = open(new_file, O_RDONLY);

    // if files do not exist
    if (old_file_descrptr < 0)
    {
        perror("Error opening old file\nError");
        return -1;
    }

    if (new_file_descrptr < 0)
    {
        perror("Error opening new file\nError");
        return -1;
    }

    // file offsets
    ll old_file_offset = lseek(old_file_descrptr, 0, SEEK_END);
    ll new_file_offset = lseek(new_file_descrptr, 0, SEEK_END);

    int Is_reverse = 1;
    if (old_file_offset != new_file_offset)
    {
        Is_reverse = 0;
    }

    new_file_offset = lseek(new_file_descrptr, 0, SEEK_SET);

    // check if files are reversed
    while (old_file_offset >= 0 && Is_reverse)
    {
        if (old_file_offset >= MAX_CHUNK_SIZE)
        {
            old_file_offset = lseek(old_file_descrptr, -MAX_CHUNK_SIZE, SEEK_CUR);
            char old_file_chunk[MAX_CHUNK_SIZE + 1];
            old_file_chunk[MAX_CHUNK_SIZE] = '\0';
            char new_file_chunk[MAX_CHUNK_SIZE + 1];
            new_file_chunk[MAX_CHUNK_SIZE] = '\0';

            read(old_file_descrptr, old_file_chunk, MAX_CHUNK_SIZE);
            read(new_file_descrptr, new_file_chunk, MAX_CHUNK_SIZE);

            Is_reverse = Is_reverse && checkrev(old_file_chunk, new_file_chunk);
        }
        else
        {
            int chars_to_be_read = old_file_offset;
            old_file_offset = lseek(old_file_descrptr, -chars_to_be_read, SEEK_CUR);
            char old_file_chunk[chars_to_be_read + 1];
            old_file_chunk[chars_to_be_read] = '\0';
            char new_file_chunk[chars_to_be_read + 1];
            new_file_chunk[chars_to_be_read] = '\0';

            read(old_file_descrptr, old_file_chunk, chars_to_be_read);
            read(new_file_descrptr, new_file_chunk, chars_to_be_read);

            Is_reverse = Is_reverse && checkrev(old_file_chunk, new_file_chunk);
        }
        old_file_offset = lseek(old_file_descrptr, -MAX_CHUNK_SIZE, SEEK_CUR);
        new_file_offset += MAX_CHUNK_SIZE;
    }

    if (Is_reverse)
    {
        write(STDOUT_FILENO, "Whether file contents are reversed in newfile: Yes\n", 51);
    }
    else
    {
        write(STDOUT_FILENO, "Whether file contents are reversed in newfile: No\n", 50);
    }

    if (stat(new_file, &new_file_perms))
    {
        perror("Some Problem occured\nError: ");
    }
    else
    {

        Print_Perms(new_file_perms.st_mode, "newfile");
    }

    if (stat(old_file, &old_file_perms))
    {
        perror("Some Problem occured\nError: ");
    }
    else
    {

        Print_Perms(old_file_perms.st_mode, "oldfile");
    }

    if (is_directory_created)
    {
        Print_Perms(director_perms.st_mode, "directory");
    }
    else
    {
        write(STDOUT_FILENO, "!!!  Directory Does not exist , So permissions not printed\n", 59);
    }
}