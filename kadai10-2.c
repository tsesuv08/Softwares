/* kadai10-2.c it0723 2026/10/05 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

typedef struct
{	int id;
	char name[20];
	int p;
} frec;

typedef struct
{	uint *p;
	uint cap;
	uint idx;
} dp_t;

typedef struct
{	uint fsize;
	uint fline;
	uint cap;
	uint idx;
	dp_t dp;
	frec *rec;
} fstr;

char mncpy(char *d, char *s, uint n);

uint fsyz(FILE *file);
uint flcnt(FILE *file);

fstr fsnew(FILE *file);
char fsfree(fstr *fs);

char menu(void);
char show(fstr fs);
char add(fstr *fs);
char renew(fstr *fs);
char del(fstr *fs);
char flush(FILE *file, fstr fs);

char flag = 0;

int main(void)
{	char pwr = 1;
	char id = 0;
	fstr fs;

	FILE *file;

	fs = fsnew(file);

	while(pwr)
	{	id = menu();

		switch(id)
		{	case 1:
				show(fs);

				break;

			case 2:
				add(&fs);

				break;

			case 4:
				del(&fs);

				break;

			case 8:
				if(!flush(file, fs))
					printf("Saved\n");

				else
					printf("Save error\n");

				break;

			case 9:
				if(flag & 1 && !(flag & 2))
				{	if(remove("seiseki.txt"))
						printf("File delete error.  Please delete file manually\n");
				} pwr = 0;

				break;

			case 10:
				printf("[fs] fsize: %d, fline: %d, cap: %d, idx: %d\n[dp] cap: %d, idx: %d\n", fs.fsize, fs.fline, fs.cap, fs.idx, fs.dp.cap, fs.dp.idx);

				break;

			default:
				printf("Invalid id: %d\n", id);

				break;
		}
	}

	fsfree(&fs);

	return 0;
}

char mncpy(char *d, char *s, uint n)
{	while(n--)
		*d++ = *s++;

	return 0;
}

fstr fsnew(FILE *file)
{	fstr fs;

	file = fopen("seiseki.txt", "r");
	if(!file)
	{	file = fopen("seiseki.txt", "w");
		fclose(file);

		flag |= 1;
		printf("New file\n");
	} else
		fclose(file);

	fs.fsize = fsyz(file);
	fs.fline = flcnt(file);
	fs.cap = 1;
	fs.idx = fs.fline;

	fs.dp.p = malloc(sizeof(uint));
	fs.dp.cap = 1;
	fs.dp.idx = 0;

	fs.rec = malloc(fs.fsize * sizeof(frec));

	file = fopen("seiseki.txt", "r");

	for(uint i = 0; i < fs.fline; i++)
	{	fseek(file, 28L * i, SEEK_SET);
		fscanf(file, "%3d%20s%4d", &fs.rec[i].id, fs.rec[i].name, &fs.rec[i].p);
	} fclose(file);

	return fs;
}

char fsfree(fstr *fs)
{	free(fs->rec);
	free(fs->dp.p);

	return 0;
}

uint fsyz(FILE *file)
{	uint t = 0;

	file = fopen("seiseki.txt", "r");

	fseek(file, 0, SEEK_END);
	t = ftell(file);
	fseek(file, 0, SEEK_SET);
	t -= ftell(file);

	fclose(file);

	return t;
}

uint flcnt(FILE *file)
{	uint line = 0;
	uint s = fsyz(file);
	char c = 0;

	file = fopen("seiseki.txt", "r");

	while(c != EOF)
	{	c = fgetc(file);
		if(c == '\n')
			line++;
	} fclose(file);

	return line;
}

char menu(void)
{	int select = 0;

	printf("*** MENU ***\n");

	printf("1: List\n");
	printf("2: Add\n");
	printf("3: Update\n");
	printf("4: Delete\n");
	printf("8: Save manually\n");
	printf("9: Quit program\n");

	printf("Number? ");
	scanf("%d", &select);

	return select;
}

char show(fstr fs)
{	printf("___ Record list ___\n");

	for(uint i = 0; i < fs.idx; i++)
	{	if(strcmp(fs.rec[i].name, " DELETED"))
			printf("%d %s %d\n", fs.rec[i].id, fs.rec[i].name, fs.rec[i].p);
	}

	printf("-------------------\n");

	return 0;
}

char add(fstr *fs)
{	uint i = 0;
	uint newP;
	char newName[20];

	printf("Name and point? ");
	scanf("%20s %4d", newName, &newP);

	if(fs->dp.idx)
	{	fs->dp.idx--;
		mncpy(fs->rec[fs->dp.p[fs->dp.idx]].name, newName, 20);
		fs->rec[fs->dp.idx].p = newP;
		fs->dp.p[fs->dp.idx] = 0;
	} else
	{	if(!(1 + fs->idx < fs->cap))
		{	fs->cap *= 2;
			fs->rec = realloc(fs->rec, fs->cap * sizeof(frec));
		} fs->rec[fs->idx].id = 1 + fs->idx;
		mncpy(fs->rec[fs->idx].name, newName, 20);
		fs->rec[fs->idx++].p = newP;
	}

	return 0;
}

char renew(fstr *fs)
{	return 0; // TODO: Update struct by overwrite
}

char del(fstr *fs)
{	int dId = 0;

	printf("Record id you wish to delete? ");
	scanf("%d", &dId);

	dId--;

	if(dId < 0 || fs->idx < dId)
	{	printf("Not exist record: %d\n", 1 + dId);

		return 1;
	} if(!(1 + fs->dp.idx < fs->dp.cap))
	{	fs->dp.cap *= 2;
		fs->dp.p = realloc(fs->dp.p, fs->dp.cap * sizeof(uint));
	} fs->dp.p[fs->dp.idx++] = dId;
	mncpy(fs->rec[dId].name, " DELETED\0\0\0\0\0\0\0\0\0\0\0\0", 20);;

	return 0; // TODO: Update struct by overwrite
}

char flush(FILE *file, fstr fs)
{	flag |= 2;

	file = fopen("seiseki.txt", "w");

	for(uint i = 0; i < fs.idx; i++)
	{	fseek(file, 28L * i, SEEK_SET);
		fprintf(file, "%3d%20s%4d\n", fs.rec[i].id, fs.rec[i].name, fs.rec[i].p);
	} fclose(file);

	fs.fsize = fsyz(file);
	fs.fline = flcnt(file);

	return 0;
}

