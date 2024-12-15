#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "a:b:")) != -1) {
        switch (opt) {
            case 'a':
                printf("Option -a with value: %s\n", optarg);
                break;
            case 'b':
                printf("Option -b with value: %s\n", optarg);
                break;
            default:
                printf("Unknown option\n");
        }
    }
    return 0;
}
