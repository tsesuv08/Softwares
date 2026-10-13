/* Record manager */
/* Start date: 2026/10/05 */
/* Last date: 2026/10/13 */
/* Created by TSesuv Xanuc Notsel */

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
} dp_t; // Stack方式だからデータの保存箇所の再利用は疎らな場所を使う事になる、速度・実装難度とのトレードオフ
// 只まぁ無駄に容量を食うと言う事はし無いから ...
// (先生の実装意図 ... DELETEDは再利用し無い ... よりかは ... )

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
chr redeploy(chr *s, uint ps);

uint lb(uint n);

chr fstate(chr *name);
uint fsize(chr *name);
uint fline(chr *name);

fstr fsnew(chr *name);
chr fsfree(fstr *fs);

chr menu(void);
chr show(fstr fs);
chr add(fstr *fs);
chr renew(fstr *fs);
chr del(fstr *fs);
chr flush(chr *name, fstr *fs);

int main(uint ac, chr **av)
{	chr pwr = 1;
	chr id = 0;
	chr *name = "seiseki.txt";
	fstr fs;

	for(uint i = 0; i < ac; i++)
	{	if(!strcmp(av[i], "/f") && i < 1 + ac)
		{	name = av[++i];

			break;
		}
	} fs = fsnew(name);
	if(fs.flag & 128) // fsnew: error
	{	switch(fs.flag)
		{	case 128: // file status error
				printf("recmgr: main: fsnew: ERROR: Can't open file\n");

				break;

			case 129:
				printf("recmgr: main: fsnew: fsyz: ERROR: Can't open file\n");

				break;

			case 130:
				printf("recmgr: main: fsnew: fline: fsize: ERROR: Can't open file\n");

				break;

			case 131:
				printf("recmgr: main: fsnew: fline: ERROR: Can't open file\n");

				break;

			case 132:
				printf("recmgr: main: fsnew: ERROR: Allocate error\n");

				break;

			case 133: // can allocate fs.dp.p, but can't allocate fs.rec
				printf("recmgr: main: fsnew: ERROR: Allocate error\n");
				free(fs.dp.p);

				break;

			case 134: // can allocate fs.rec and fs.dp.p, but can't open file
				printf("recmgr: main: fsnew: ERROR: Can't open file\n");
				fsfree(&fs);

				break;

			case 135: // can allocate fs.rec and fs.dp.p, but can't realloc fs.dp.p
				printf("recmgr: main: fsnew: ERROR: Can't open file\n");
				free(fs.rec);

				break;

			default:
				printf("recmgr: main: fsnew: ERROR: Unknown error\n");

				break;
		} return fs.flag - 127; // return code using fsnew error code
	} if(!(fs.flag & 1))
		printf("New file\n");

	while(pwr)
	{	id = menu();

		switch(id)
		{	case 1:
				show(fs);

				break;

			case 2:
				switch(add(&fs))
				{	case 1:
						printf("recmgr: main: add: ERROR: Allocate error\n");

						break;

					default:
						break;
				} break;

			case 3:
				switch(renew(&fs))
				{	case 1:
						printf("recmgr: main: renew: ERROR: Record does not exist\n");

						break;

					case 2:
						printf("recmgr: main: renew: ERROR: Not exist record: %d\n", 1 + fs.fsize);
						fs.fsize = fsize(name);

						break;

					default:
						break;
				} break;

			case 4:
				switch(del(&fs))
				{	case 1:
						printf("recmgr: main: del: ERROR: Record does not exist\n");

						break;

					case 2:
						printf("recmgr: main: del: ERROR: Not exist record: %d\n", 1 + fs.fsize);
						fs.fsize = fsize(name);

						break;

					case 3:
						printf("recmgr: main: del: ERROR: Record was deleted: %d\n", 1 + fs.fsize);
						fs.fsize = fsize(name);

						break;

					case 4:
						printf("recmgr: main: del: ERROR: Allocate error\n");

						break;

					default:
						break;
				} break;

			case 8:
				if(!flush(name, &fs))
					printf("Saved\n");

				else
					printf("recmgr: main: flush: ERROR: Can't open file\nSave error\n"); // save error don't return main

				break;

			case 9:
				if(fs.flag & 4)
				{	chr forced = 'N';

					printf("Data changed but not saved to file, is it okay to terminate (y/n) ? ");
					scanf(" %c", &forced);

					if(forced == 'Y' || forced == 'y')
						pwr = 0;
				} else
					pwr = 0;

				break;

			default:
				printf("Invalid id: %d\n", id);

				break;
		}
	} fsfree(&fs);

	return 0;
}

chr mncpy(chr *d, chr *s, uint n)
{	while(n--)
		*d++ = *s++;

	return 0;
}

chr range(uint udr, uint ovr, uint chk)
{	return udr < ++chk && chk < ++ovr;
}

chr redeploy(chr *s, uint ps)
{	while(--ps)
		s++;

	*s = 0;

	return 0;
}

uint lb(uint n)
{	uint k = 0;

	while(n >> ++k);

	return --k;
}		

fstr fsnew(chr *name)
{	fstr fs;
	fstr err;
	chr fstatus = fstate(name);

	FILE *file;

	err.flag = 0;

	if(!fstatus)
	{	err.flag |= 128;

		return err;
	} if(fstatus & 1)
	{	fs.fsize = fsize(name);
		fs.fline = fline(name);

		if(fs.fsize == (uint)-1)
		{	err.flag |= 129;

			return err;
		} if(fs.fline == (uint)-1)
		{	err.flag |= 130;
		
			return err;
		} if(fs.fline == (uint)-2)
		{	err.flag |= 131;
		
			return err;
		} fs.cap = 1 << lb(fs.fsize);
		fs.idx = fs.fline;
	} else
	{	fs.fsize = fs.fline = fs.idx = 0;
		fs.cap = 1;
	}

	fs.dp.p = malloc(sizeof(uint));
	if(!fs.dp.p)
	{	err.flag |= 132;

		return err;
	} fs.dp.cap = 1;
	fs.dp.idx = 0;

	fs.rec = malloc(fs.cap * sizeof(frec));
	if(!fs.rec)
	{	err.flag |= 133;

		return err;
	} fs.flag = fstatus & 1;

	if(fstatus & 1)
	{	file = fopen(name, "r");
		if(!file)
		{	err.flag |= 134;

			return err;
		} for(uint i = 0; i < fs.fline; i++)
		{	fseek(file, 28L * i, SEEK_SET);
			fscanf(file, "%3d%20s%4d", &fs.rec[i].id, fs.rec[i].name, &fs.rec[i].p);

			if(!strncmp(fs.rec[i].name, " DELETED\0\0\0\0\0\0\0\0\0\0\0\0", 20))
			{	if(!(1 + fs.dp.idx < fs.dp.cap))
				{	fs.dp.cap *= 2;

					fstr tmp;
					tmp.dp.p = realloc(fs.dp.p, fs.dp.cap * sizeof(uint));
					if(!tmp.dp.p)
					{	err.flag |= 135;

						return err;
					} fs.dp.p = tmp.dp.p;
				} fs.dp.p[fs.dp.idx++] = i;
			}
		} fclose(file);
	}

	return fs;
}

chr fsfree(fstr *fs)
{	free(fs->rec);
	free(fs->dp.p);

	return 0;
}

chr fstate(chr *name)
{	chr status = 0; // exist, can create, <- and can remove

	FILE *file = fopen(name, "r");
	if(file)
	{	fclose(file);

		status |= 1;
	} else
	{	file = fopen(name, "w");
		if(file)
		{	fclose(file);

			status |= 2;

			if(!remove(name))
				status |= 4;

			else
				while(remove(name));
		}
	} return status;
}

uint fsize(chr *name)
{	uint t = 0;

	FILE *file = fopen(name, "r");
	if(!file)
		return -1;

	fseek(file, 0, SEEK_END);
	t = ftell(file);
	fseek(file, 0, SEEK_SET);
	t -= ftell(file);

	fclose(file);

	return t;
}

uint fline(chr *name)
{	uint line = 0;
	uint s = fsize(name);
	chr c = 0;

	if(s == (uint)-1)
		return -1;

	FILE *file = fopen(name, "r");
	if(!file)
		return -2;

	while(c != 255)
	{	c = fgetc(file);
		if(c == '\n')
			line++;
	} fclose(file);

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
{	if(!fs.idx)
	{	printf("Records does not exist\n");
	
		return 1;
	} printf("___ Record list ___\n");

	for(uint i = 0; i < fs.idx; i++)
	{	if(strncmp(fs.rec[i].name, "DELETED\0\0\0\0\0\0\0\0\0\0\0\0\0", 20))
			printf("%d %s %d\n", fs.rec[i].id, fs.rec[i].name, fs.rec[i].p);
	} printf("-------------------\n");

	return 0;
}

chr add(fstr *fs)
{	uint newP;
	chr newName[20];

	printf("Name and point? ");
	scanf("%20s %4d", newName, &newP);
	redeploy(newName, 20);

	if(fs->dp.idx)
	{	mncpy(fs->rec[fs->dp.p[--fs->dp.idx]].name, newName, 20);
		fs->rec[fs->dp.p[fs->dp.idx]].p = newP;
		fs->dp.p[fs->dp.idx] = 0;
	} else
	{	if(!(1 + fs->idx < fs->cap))
		{	fs->cap *= 2;
			fstr tmp;
			tmp.rec = realloc(fs->rec, fs->cap * sizeof(frec));
			if(!tmp.rec)
				return 1;

			fs->rec = tmp.rec;
		} fs->rec[fs->idx].id = 1 + fs->idx;
		mncpy(fs->rec[fs->idx].name, newName, 20);
		fs->rec[fs->idx++].p = newP;
	} fs->flag |= 4;

	return 0;
}

chr renew(fstr *fs)
{	if(!fs->idx)
		return 1;

	int id = 0;
	uint newP = 0;
	chr newName[20];

	printf("Update record id? ");
	scanf("%d", &id);

	id--;

	if(!range(0, fs->idx, id))
	{	fs->fsize = id;

		return 2;
	} printf("New name and point? ");
	scanf("%20s %d", newName, &newP);
	redeploy(newName, 20);

	mncpy(fs->rec[id].name, newName, 20);
	fs->rec[id].p = newP;

	fs->flag |= 4;

	return 0;
}

chr del(fstr *fs)
{	if(!fs->idx)
		return 1;

	int dId = 0;

	printf("Record id you wish to delete? ");
	scanf("%d", &dId);

	dId--;

	if(!range(0, fs->idx, dId))
	{	fs->fsize = dId;

		return 2;
	} else if(!strcmp(fs->rec[dId].name, " DELETED\0\0\0\0\0\0\0\0\0\0\0\0"))
	{	fs->fsize = dId;

		return 3;
	} if(!(1 + fs->dp.idx < fs->dp.cap))
	{	fs->dp.cap *= 2;
		fstr tmp;
		tmp.dp.p = realloc(fs->dp.p, fs->dp.cap * sizeof(uint));
		if(!tmp.dp.p)
			return 4;

		fs->dp.p = tmp.dp.p;
	} fs->dp.p[fs->dp.idx++] = dId;
	mncpy(fs->rec[dId].name, " DELETED\0\0\0\0\0\0\0\0\0\0\0\0", 20);

	fs->flag |= 4;

	return 0;
}

chr flush(chr *name, fstr *fs)
{	fs->flag |= 2;

	FILE *file = fopen(name, "w");
	if(!file)
		return 1;

	for(uint i = 0; i < fs->idx; i++)
	{	fseek(file, 28L * i, SEEK_SET);
		fprintf(file, "%3d%20s%4d\n", fs->rec[i].id, fs->rec[i].name, fs->rec[i].p);
	} fclose(file);

	fs->fsize = fsize(name);
	fs->fline = fline(name);

	if(fs->fsize == (uint)-1 || fs->fline == (uint)-2)
		return 2;

	if(fs->fline == (uint)-1)
		return 3;

	fs->flag &= (255 - 4);

	return 0;
}
