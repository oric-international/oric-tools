#include <stdio.h>
#include "randoslib.h"

void menu()
{
	char name[40];

	while (1) {
		print_dir();
		printf("\n\n");
		printf("1- Copy file from randos disk to host\n");
		printf("2- Copy file from host to randos disk\n");
		printf("3- Delete randos file\n");
		printf("4- Change directory\n");
		printf("0- Quit\n");
		switch (getch())
		{
			case '0': return;
			case '1':
				printf("File name: ");
				scanf("%s",name);
				if (load(name)) printf("Error loading %s\n",name);
				break;
			case '2':
				printf("File name: ");
				scanf("%s",name);
				if (save(name)) printf("Error saving %s\n",name);
				break;
			case '3':
				printf("File name: ");
				scanf("%s",name);
				if (delete(name)) printf("Error deleting %s\n",name);
				break;
			case '4':
			default:
		}
	}
}

int main()
{
	char name[40];
	printf("disk file :"); scanf("%s",name);
	if (open_randos_disk(name)) {
		printf("Cannot open randos disk %s\n",name);
		exit(1);
	}
	menu();
	close_randos_disk();
}
