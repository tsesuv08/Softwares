/* kadai10-2.c it0723 2026/10/05 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char chr;
typedef unsigned int uint;

typedef struct
{	int id;
	chr name[20];
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

	chr flag; // new, changed, not saved
} fstr;

chr mncpy(chr *d, chr *s, uint n);
chr range(uint udr, uint ovr, uint chk);

uint fsyz(FILE **file);
uint flcnt(FILE **file);

fstr fsnew(FILE **file);
chr fsfree(fstr *fs);

chr menu(void);
chr show(fstr fs);
chr add(fstr *fs);
chr renew(fstr *fs);
chr del(fstr *fs);
chr flush(FILE **file, fstr *fs);

int main(void)
{	chr pwr = 1;
	chr id = 0;
	fstr fs;

	FILE *file;

	fs = fsnew(&file);

	while(pwr)
	{	id = menu();

		switch(id)
		{	case 1:
				show(fs);

				break;

			case 2:
				add(&fs);

				break;

			case 3:
				renew(&fs);

				break;

			case 4:
				del(&fs);

				break;

			case 8:
				if(!flush(&file, &fs))
					printf("Saved\n");

				else
					printf("Save error\n");

				break;

			case 9:
				if(fs.flag & 1 && !(fs.flag & 6))
				{	if(remove("seiseki.txt"))
						printf("File delete error.  Please delete file manually\n");
				} else if(fs.flag & 4)
				{	chr forced = 'N';

					printf("File changed but not saved, Is it okay to terminate (y/n) ? ");
					scanf(" %c", &forced);

					if(forced == 'Y' || forced == 'y')
						pwr = 0;
				} else
					pwr = 0;

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

chr mncpy(chr *d, chr *s, uint n)
{	while(n--)
		*d++ = *s++;

	return 0;
}

chr range(uint udr, uint ovr, uint chk)
{	if(--udr < chk && chk < ovr)
		return 1;

	return 0;
}
		

fstr fsnew(FILE **file)
{	fstr fs;
	chr flag = 0;

	*file = fopen("seiseki.txt", "r");
	if(!*file)
	{	*file = fopen("seiseki.txt", "w");
		fclose(*file);

		flag |= 1;
		printf("New file\n");
	} else
		fclose(*file);

	fs.fsize = fsyz(file);
	fs.fline = flcnt(file);
	fs.cap = 1;
	fs.idx = fs.fline;

	fs.dp.p = malloc(sizeof(uint));
	fs.dp.cap = 1;
	fs.dp.idx = 0;

	fs.rec = malloc(fs.fsize * sizeof(frec));

	fs.flag = flag;

	*file = fopen("seiseki.txt", "r");

	for(uint i = 0; i < fs.fline; i++)
	{	fseek(*file, 28L * i, SEEK_SET);
		fscanf(*file, "%3d%20s%4d", &fs.rec[i].id, fs.rec[i].name, &fs.rec[i].p);
	} fclose(*file);

	return fs;
}

chr fsfree(fstr *fs)
{	free(fs->rec);
	free(fs->dp.p);

	return 0;
}

uint fsyz(FILE **file)
{	uint t = 0;

	*file = fopen("seiseki.txt", "r");

	fseek(*file, 0, SEEK_END);
	t = ftell(*file);
	fseek(*file, 0, SEEK_SET);
	t -= ftell(*file);

	fclose(*file);

	return t;
}

uint flcnt(FILE **file)
{	uint line = 0;
	uint s = fsyz(file);
	chr c = 0;

	*file = fopen("seiseki.txt", "r");

	while(c != 255)
	{	c = fgetc(*file);
		if(c == '\n')
			line++;
	} fclose(*file);

	return line;
}

chr menu(void)
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

chr show(fstr fs)
{	printf("___ Record list ___\n");

	for(uint i = 0; i < fs.idx; i++)
	{	if(strcmp(fs.rec[i].name, " DELETED"))
			printf("%d %s %d\n", fs.rec[i].id, fs.rec[i].name, fs.rec[i].p);
	}

	printf("-------------------\n");

	return 0;
}

chr add(fstr *fs)
{	uint newP;
	chr newName[20];

	printf("Name and point? ");
	scanf("%20s %4d", newName, &newP);

	if(fs->dp.idx)
	{	fs->dp.idx--;
		mncpy(fs->rec[fs->dp.p[fs->dp.idx]].name, newName, 20);
		fs->rec[fs->dp.p[fs->dp.idx]].p = newP;
		fs->dp.p[fs->dp.idx] = 0;
	} else
	{	if(!(1 + fs->idx < fs->cap))
		{	fs->cap *= 2;
			fs->rec = realloc(fs->rec, fs->cap * sizeof(frec));
		} fs->rec[fs->idx].id = 1 + fs->idx;
		mncpy(fs->rec[fs->idx].name, newName, 20);
		fs->rec[fs->idx++].p = newP;
	} fs->flag |= 4;

	return 0;
}

chr renew(fstr *fs)
{	int id = 0;
	uint newP = 0;
	chr newName[20];

	printf("Update record id? ");
	scanf("%d", &id);

	id--;

	if(!range(0, fs->idx, id))
	{	printf("Not exist record: %d\n", 1 + id);

		return 1;
	} printf("New name and point? ");
	scanf("%20s %d", newName, &newP);

	mncpy(fs->rec[id].name, newName, 20);
	fs->rec[id].p = newP;

	fs->flag |= 4;

	return 0;
}

chr del(fstr *fs)
{	int dId = 0;

	printf("Record id you wish to delete? ");
	scanf("%d", &dId);

	dId--;

	if(!range(0, fs->idx, dId))
	{	printf("Not exist record: %d\n", 1 + dId);

		return 1;
	} if(!(1 + fs->dp.idx < fs->dp.cap))
	{	fs->dp.cap *= 2;
		fs->dp.p = realloc(fs->dp.p, fs->dp.cap * sizeof(uint));
	} fs->dp.p[fs->dp.idx++] = dId;
	mncpy(fs->rec[dId].name, " DELETED\0\0\0\0\0\0\0\0\0\0\0\0", 20);

	fs->flag |= 4;

	return 0;
}

chr flush(FILE **file, fstr *fs)
{	fs->flag |= 2;

	*file = fopen("seiseki.txt", "w");

	for(uint i = 0; i < fs->idx; i++)
	{	fseek(*file, 28L * i, SEEK_SET);
		fprintf(*file, "%3d%20s%4d\n", fs->rec[i].id, fs->rec[i].name, fs->rec[i].p);
	} fclose(*file);

	fs->fsize = fsyz(file);
	fs->fline = flcnt(file);

	fs->flag &= (255 - 4);

	return 0;
}
