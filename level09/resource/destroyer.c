#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main (int argc, char **argv)
{
    char    buffer[BUFFER_SIZE];
    char    c;
    int     i;
    int     fd;
    ssize_t bytes;

    if (argc != 2)
    {
        printf("Error: ( %s ): Necesita argumento\n", argv[0]);
        return (1);
    }

    printf("Intentando abrir: %s\n", argv[1]);
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        return (1);

    bytes = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytes == -1)
    {
        printf("Error al leer el archivo\n");
        close(fd);
        return (1);
    }

    buffer[bytes] = '\0';

    i = 0;
    while (buffer[i] != '\0')
    {
        c = buffer[i] - i;
        printf("%c", c);
        i++;
    }
    close(fd);
    printf("\n");
    return (0);
}