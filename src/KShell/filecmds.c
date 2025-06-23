#include <fileio/fileio.h>
#include <io/kio.h>
void rm() {
    char filename[9];
    char filext[4];
    kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');
    kprintf("Enter File Extension: ");
    kgetstr(filext, 4);
    kflush();
    kputchar('\n');
    fat16_remove_file(filename, filext);
}

void mkf() {
    char filename[9];
    char filext[4];
    kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');
    kprintf("Enter File Extension: ");
    kgetstr(filext, 4);
    kflush();
    kputchar('\n');
    mkfile(filename, filext);
}

void writef() {
    char filename[9];
    char ext[4];
    char data[510];

    kprintf("\nEnter Filename (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');

    kprintf("Enter File Extension: ");
    kgetstr(ext, 4);
    kflush();
    kputchar('\n');

    kprintf("Start Writing Data");
    kputchar('\n');
    kgetstr(data, 500);
    kflush();

    int len = 0;
    while (len < 510 && data[len] != '\0') {
        len++;
    }
    if (len + 2 < 510) {
        data[len] = '\r';     // Carriage Return
        data[len + 1] = '\n'; // Line Feed
        data[len + 2] =
            '\0'; // Null terminate (optional, if your strings use it)
    }

    writefile(filename, ext, data, sizeof(data));
}

void readf() {
    char filename[9];
    char filext[4];
    kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');
    kprintf("Enter File Extension: ");
    kgetstr(filext, 4);
    kflush();
    kputchar('\n');
    fileconts(filename, filext);
}