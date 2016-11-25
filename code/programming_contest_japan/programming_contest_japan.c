/* 
 * 	Created on: 2016/11/18
 *	Author: zuodw
 */

#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>


#define PRODUCT_CODE_MAX        24
#define TYPE_CODE_MAX           10
#define MASTER_PATH             "/data/master.csv"
#define BRANCH_PATH_1		"/data/shiten001.csv"
#define BRANCH_PATH_2           "/data/shiten002.csv"
#define BRANCH_PATH_3           "/data/shiten003.csv"
#define BRANCH_PATH_4           "/data/shiten004.csv"
#define BRANCH_PATH_5           "/data/shiten005.csv"
#define BRANCH_PATH_6           "/data/shiten006.csv"
#define BRANCH_PATH_7           "/data/shiten007.csv"
#define BRANCH_PATH_8           "/data/shiten008.csv"
#define BRANCH_PATH_9           "/data/shiten009.csv"
#define BRANCH_PATH_10          "/data/shiten010.csv"
#define OUT_FILE_PATH		"/data/outfile.csv"
#define OUT_FILE_TITLE		"日付,売上合計数"
#define NAME_SIZE_MAX           256
#define HASH_TABLE_LEN  	32
#define HASH_TABLE_POS		20161000
#define BRANCH_NUM_MAX		10


static const char* branch_path[] = 
{
	BRANCH_PATH_1,
	BRANCH_PATH_2,
        BRANCH_PATH_3,
        BRANCH_PATH_4,
        BRANCH_PATH_5,
        BRANCH_PATH_6,
        BRANCH_PATH_7,
        BRANCH_PATH_8,
        BRANCH_PATH_9,
        BRANCH_PATH_10,
};

typedef struct 
{
	uint32_t 	date;
	uint64_t	count;
} Out_Data;

static Out_Data m_out_data[HASH_TABLE_LEN];

/* master */
typedef struct
{
	char				product_code[NAME_SIZE_MAX];
	char				product_name[NAME_SIZE_MAX];
	char				type_code[NAME_SIZE_MAX];
	char				type_name[NAME_SIZE_MAX];
	char				price[NAME_SIZE_MAX];
} Product_Master_Title;

typedef struct
{
        uint32_t        product_code;
        char            product_name[NAME_SIZE_MAX];
        uint32_t        type_code;
        char            type_name[NAME_SIZE_MAX];
        uint64_t        price;
} Product_Master_Data;

typedef union 
{
	Product_Master_Title	title;
	Product_Master_Data	data;
} Product_Master;
Product_Master m_productMaster[PRODUCT_CODE_MAX];

/* shiten */
typedef struct
{
        uint32_t        branch_code;
        char            branch_name[NAME_SIZE_MAX];
        uint32_t        date;
        uint32_t        type_code;
        uint32_t        product_code;
        uint64_t        sold_number;
} Branch_Sales_Data;

//hash table data
typedef struct _Node
{
	Branch_Sales_Data* data;
	struct _Node* next;
} Hash_Node, *pHash_Node;

pHash_Node hash_table[HASH_TABLE_LEN];


/* Function */

/* IO */
void readProductMasterData(void);
void readBranchSalesData(void);
void writeFile(void);

/* Hash */
uint8_t hash_id_func(uint64_t date);
pHash_Node init_hash_node(void);
void init_hash_table(void);
void add_hash_node(pHash_Node newNode);

/* array */
void init_array(void);
void sort_array(void);

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
uint8_t hash_id_func(uint64_t date)
{
	uint8_t id;
	id = date % HASH_TABLE_POS;
	return id;
}

pHash_Node init_hash_node(void)
{
	pHash_Node node;
	node = (pHash_Node)malloc(sizeof(Hash_Node));
	node->data = (Branch_Sales_Data*)malloc(sizeof(Branch_Sales_Data));
	node->next = NULL;
	return node;
}

void init_hash_table(void)
{
	uint8_t i = 0;
	for (i = 0; i < HASH_TABLE_LEN; i++)
	{
		hash_table[i] = (pHash_Node)malloc(sizeof(Hash_Node));
		hash_table[i]->next = NULL;
	}
	return;
}

void add_hash_node(pHash_Node newNode)
{
	pHash_Node 	node;
	uint8_t 	id;

	id = hash_id_func(newNode->data->date);
	if (hash_table[id]->next == NULL)
	{
		hash_table[id]->next = newNode;
	}
	else
	{
		node = hash_table[id]->next;
		while(node->next != NULL)
			node = node->next;
		node->next = newNode;	
	}
	return;	
}




void init_array(void)
{
	uint8_t 	i;
	uint64_t 	count = 0;
	pHash_Node 	node;	

	for (i = 0; i < HASH_TABLE_LEN; i++)
	{
		if (hash_table[i]->next == NULL)
		{
			m_out_data[i].date = 0;
			m_out_data[i].count = 0;
		}
		else
		{
			m_out_data[i].date = hash_table[i]->next->data->date;
			node = hash_table[i]->next;
			count = node->data->sold_number;
			while (node->next != NULL)
			{
				count = count + node->next->data->sold_number;
				node = node->next;
			}
			m_out_data[i].count = count;
		}
	}
}


void readProductMasterData(void)
{
    FILE*           fp = NULL;
    uint8_t         i = 0;

    fp = fopen(MASTER_PATH, "r");
    if (fp == NULL)
    {
            printf("open file failed!!!");
            return;
    }

    for (i = 0; !feof(fp) && i < PRODUCT_CODE_MAX; i++)
    {
		if (i == 0)
		{
			fscanf(fp, "%[^','],%[^','],%[^','],%[^','],%[^',']", m_productMaster[i].title.product_code, m_productMaster[i].title.product_name, m_productMaster[i].title.type_code, m_productMaster[i].title.type_name, m_productMaster[i].title.price);
		}
		else
		{
			fscanf(fp, "%d,%[^','],%d,%[^','],%ld", &m_productMaster[i].data.product_code, m_productMaster[i].data.product_name, &m_productMaster[i].data.type_code, m_productMaster[i].data.type_name, &m_productMaster[i].data.price);
		}
    }
    return;
}

void readBranchSalesData(void)
{
	uint8_t 	i = 0;
	uint64_t	j = 0;
	pHash_Node 	node;
	FILE* 		fp;
	char title[NAME_SIZE_MAX];
	
	for (i = 0; i < BRANCH_NUM_MAX; i++)
	{
		if (access(branch_path[i], F_OK) == 0)
		{
			fp = fopen(branch_path[i], "r");
        	if (fp == NULL)
        	{
               	 	printf("open file failed!!!");
                	return;
        	}

        	for (j = 0; !feof(fp); j++)
        	{
				if (j == 0)
				{
					fscanf(fp, "%s", title);	/* 跳过第一行字符串 */
				}
				else
				{
					node = init_hash_node();
                	fscanf(fp, "%d,%[^','],%d,%d,%d,%ld", &node->data->branch_code, node->data->branch_name, &node->data->date, &node->data->type_code, &node->data->product_code, &node->data->sold_number);
					add_hash_node(node);
				}
        	}
		}
		else
		{
			continue;
		}
	}
    	return;
}

void writeFile(void)
{
	uint8_t i = 0;
	FILE* fp;

	fp = fopen(OUT_FILE_PATH, "a+");
        if (fp == NULL)
        {
                printf("open out file failed!!!");
                return;
        }
	
	fprintf(fp, "%s\n", OUT_FILE_TITLE);
	for (i = 0; i < HASH_TABLE_LEN; i++)
	{
		if (m_out_data[i].date != 0)
			fprintf(fp, "%d,%ld\n", m_out_data[i].date, m_out_data[i].count);
	}
	return;
}

void main ()
{
	uint8_t			i;
	struct  timeval		start;
	struct  timeval		end;
	uint64_t		timer;

	gettimeofday(&start,NULL);

	readProductMasterData();
	
	init_hash_table();

	readBranchSalesData();
	
	init_array();

	writeFile();	

	gettimeofday(&end,NULL);
	timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	printf("timer = %ld us\n",timer);

	for(i = 0; i < HASH_TABLE_LEN; i++)
	{
		if (m_out_data[i].date != 0)
			printf("date = %d, count = %ld\n", m_out_data[i].date, m_out_data[i].count);
	}
    return;
}

