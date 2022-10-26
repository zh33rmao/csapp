#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include "cachelab.h"

static int s;
static int E;
static int b;
static char* t;
int hit=0, miss=0, evicted=0;

void usage();
int init_cache();
int free_cache();

typedef struct _cache_line{
    int valid_bit;    
    unsigned tag;
    int time; // how many times 
    //Block* block;
} cache_line;

cache_line** cache = NULL;

int init_cache(){
    int S = 1 << s;
    cache = (cache_line**)malloc(sizeof(cache_line*)*S);// S = 2^s
    for (int i=0; i<S; i++){
        *(cache + i) = (cache_line*)malloc(sizeof(cache_line)*E);
        for(int j=0; j<E; j++){
            cache[i][j].valid_bit = 0; 
            cache[i][j].tag = 0; 
            cache[i][j].time = 0; 
        } 
    }
    return 1;
}

int free_cache(){
    int S = 1 << s;
    for(int i=0; i<S; i++){
        free(*(cache + i)); 
    }
    free(cache);
    return 1;
}

int update(int addr){
    unsigned mask = 0xffffffffffffffff; 
    int s_addr = (addr >> b) & (mask >> (64 - s));
    int t_addr = addr >> (s + b);

    // how to hit?
    for (int i=0; i<E; i++){
        if (cache[s_addr][i].tag == t_addr && cache[s_addr][i].valid_bit == 1){
            hit++;
            cache[s_addr][i].time++;
            return 1;
        }
    }
    // how to miss?
    for (int i=0; i<E; i++){
        if (cache[s_addr][i].tag != t_addr){
            cache[s_addr][i].valid_bit = 1;
            cache[s_addr][i].tag = t_addr;
            miss++;
            // ccache[s_addr][i].tache[s_addr][i].time++;
            return 1;
        }
    }
    //how to evited?
    int least_used = 99999999;
    int flag = 0;
    for (int i=0; i<E; i++){
        //cache_line ca = cache[s_addr][i]; 
        if ( cache[s_addr][i].time < least_used )
        {  
            least_used = cache[s_addr][i].time;
            flag = i; 
        }
    }
    evicted++;
    cache[s_addr][flag].tag = t_addr;
    cache[s_addr][flag].time = 0;
    cache[s_addr][flag].valid_bit = 1; 

}

int simulate(char* filePath){
    FILE* pFile;
    pFile = fopen(filePath, "r");
    char identifier;
    unsigned address;
    int size;

    while (fscanf(pFile, " %c %x,%d", &identifier, &address, &size)>0){
        switch (identifier)
        {
        case 'L':
            update(address);
            hit++;
            break;
        case 'S':
            update(address);
            break;
        case 'M':
            update(address);
            miss++;
            break;
         
        default:
            break;
        }
    }

}

void usage(){
    printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile> \n");
    printf("Options: \n");
    printf("  -h        Print this help message.\n");
    printf("  -v        Optional verbose flag.\n");
    printf("  -s <num>  Number of set index bits.\n");
    printf("  -E <num>  Number if lines per set.\n");
    printf("  -b <num>  Number of block offset bits.\n");
    printf("  -t <file> Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("linux> ./csim-ref -s4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("linux> ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");

}

int main(int argc, char** argv)
{
    int opt;
    while (-1 != (opt = getopt(argc, argv, "hvs:E:b:t:"))){
        switch (opt) {
            case 's': 
                s = atoi(optarg);
                printf("s: %d\n", s);
                break;
            case 'E':
                E = atoi(optarg);
                printf("E: %d\n", E);
                break;
            case 'b':
                b = atoi(optarg);
                printf("b: %d\n", b);
                break;
            case 't':
                t = optarg;
                printf("t: %s\n", t);
                break;
            case 'v':
                usage();
                break;
            case 'h':
                usage();
                break;
            case '?':
                usage(); 
                break;
        }
    }
   
//   for (int index = optind; index < argc; index++){
//       printf ("Non-option argument %s\n", argv[index]);
//       usage();
//   }
    init_cache();
    free_cache();
    printf("free cache successfully!!!\n");
    
    printSummary(hit, miss, evicted);
    return 0;
}
