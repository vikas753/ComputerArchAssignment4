////////////////////////////////////////////
//  
//  Dinero Instrution trace generator with
//  a requird format
//
////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>

using namespace std;

#define OFFSET_SHFT 0
#define OFFSET_MASK 0x3f
#define INDEX_SHFT  6
#define INDEX_MASK  0xffc0
#define TAG_SHFT    16
#define TAG_MASK    0xffff0000

#define OFFSET_SHFT_NEW 0
#define OFFSET_MASK_NEW 0x1f

#define INDEX_SHFT_NEW 5
#define INDEX_MASK_NEW 0xffe0

#define TAG_SHFT_NEW   16
#define TAG_MASK_NEW   0xffff0000

#define WRITE_INDEX(v)        (v << INDEX_SHFT) & INDEX_MASK
#define WRITE_INDEX_NEW(v)    (v << INDEX_SHFT_NEW) & INDEX_MASK_NEW

#define WRITE_TAG_NEW(v)      (v << TAG_SHFT_NEW) & TAG_MASK_NEW

#define CACHE_SIZE        64*1024
#define CACHE_LINE_SIZE   64
#define NUM_CACHE_LINES   CACHE_SIZE / CACHE_LINE_SIZE

#define NUM_CACHE_LINES_NEW 2048

#define INSTRUCTION_CODE 2

void GenerateAddressTrace(ofstream& outData)
{
  for(int i=0;i<NUM_CACHE_LINES;i++)
  {
    int instructionCode = INSTRUCTION_CODE;
    int IndexWithAddress = WRITE_INDEX(i);
    outData << instructionCode << " " << IndexWithAddress << endl;
  } 
}

#define ADDRESS_1 1
#define ADDRESS_2 2
#define ADDRESS_3 3
#define ADDRESS_4 4
#define ADDRESS_5 5

#define WRITE_TAG(v) (v << TAG_SHFT) & TAG_MASK

// Generates 3 misses and 4 hits with only 3 unique addresses
// per index
void GenerateAddressTraceMissesHits(ofstream& outData)
{
  for(int index=0;index < NUM_CACHE_LINES;index++)
  {
    int addr1Val = WRITE_TAG(ADDRESS_1) + WRITE_INDEX(index);
    int addr2Val = WRITE_TAG(ADDRESS_2) + WRITE_INDEX(index);
    int addr3Val = WRITE_TAG(ADDRESS_3) + WRITE_INDEX(index);
    
    outData << INSTRUCTION_CODE << " " << addr1Val << endl;
    outData << INSTRUCTION_CODE << " " << addr2Val << endl;
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;
  }
}

// Generates 5 misses and 5 Hits with an interleaving of
// Miss-Hits-Miss-Hits ... 
void GenerateAddressTraceInterleaveMissHits(ofstream& outData)
{
  for(int index=0;index<NUM_CACHE_LINES;index++)
  {
    int addr1Val = WRITE_TAG(ADDRESS_1)| WRITE_INDEX(index);
    int addr2Val = WRITE_TAG(ADDRESS_2)| WRITE_INDEX(index);
    int addr3Val = WRITE_TAG(ADDRESS_3)| WRITE_INDEX(index);


    outData << INSTRUCTION_CODE << " " << addr1Val << endl;
    outData << INSTRUCTION_CODE << " " << addr1Val << endl;
 
    outData << INSTRUCTION_CODE << " " << addr2Val << endl;
    outData << INSTRUCTION_CODE << " " << addr2Val << endl;
 
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;   
    outData << INSTRUCTION_CODE << " " << addr3Val << endl;   
  
    outData << INSTRUCTION_CODE << " " << addr1Val << endl;   
    outData << INSTRUCTION_CODE << " " << addr1Val << endl;   
  
    outData << INSTRUCTION_CODE << " " << addr2Val << endl;
    outData << INSTRUCTION_CODE << " " << addr2Val << endl;
  }
}

// Generates 5 Misses and 3 hits for a data cache with a mix of
// loads and stores data reference stream

#define LOAD_CODE  0
#define STORE_CODE 1


void GenerateDataStreamMissesHitsPattern(ofstream& outData)
{ 
  for(int index=0;index<NUM_CACHE_LINES_NEW;index++)
  { 
    int addr1Val = WRITE_TAG(ADDRESS_1)| WRITE_INDEX(index);
    int addr2Val = WRITE_TAG(ADDRESS_2)| WRITE_INDEX(index);
    int addr3Val = WRITE_TAG(ADDRESS_3)| WRITE_INDEX(index);
    int addr4Val = WRITE_TAG(ADDRESS_4)| WRITE_INDEX(index);
    int addr5Val = WRITE_TAG(ADDRESS_5)| WRITE_INDEX(index);
 
    outData << LOAD_CODE << " " << addr1Val << endl;
    outData << LOAD_CODE << " " << addr2Val << endl;
    outData << LOAD_CODE << " " << addr3Val << endl;
    outData << LOAD_CODE << " " << addr4Val << endl;
    outData << LOAD_CODE << " " << addr5Val << endl;
    outData << STORE_CODE << " " << addr5Val << endl;
    outData << STORE_CODE << " " << addr5Val << endl;
    outData << STORE_CODE << " " << addr5Val << endl;
  }
}

#define OFFSET_GEN_SHFT                 5
#define WRITE_INDEX_GEN(v)        (v << OFFSET_GEN_SHFT) 
#define WRITE_TAG_GEN(v,i) (v << (i+OFFSET_GEN_SHFT)) 

void GenerateIDetectionStream(ofstream& outData)
{
  int num_cache_lines,indexCache;
  cout << " Enter Number of cacche lines : " << endl;
  cin >> num_cache_lines;

  int index = log(num_cache_lines) / log(2);
  int addr1Val= (WRITE_TAG_GEN(1,index))|WRITE_INDEX_GEN(0);
  outData << INSTRUCTION_CODE << " " << addr1Val << endl;
 
  for(int index=1;index<=num_cache_lines;index++)
  {
    int addr1Val = WRITE_INDEX_GEN(index);
    outData << INSTRUCTION_CODE << " " << addr1Val << endl;
  }
}

void GeneratePolicyDeterminationStream(ofstream& outData)
{
  int index = 0;
  for(int i=0;i<=100;i=i+10)
  {
    int addr1Val = WRITE_TAG(ADDRESS_1+i)| WRITE_INDEX(index);
    int addr2Val = WRITE_TAG(ADDRESS_2+i)| WRITE_INDEX(index);
    int addr3Val = WRITE_TAG(ADDRESS_3+i)| WRITE_INDEX(index);
    int addr4Val = WRITE_TAG(ADDRESS_4+i)| WRITE_INDEX(index);
    int addr5Val = WRITE_TAG(ADDRESS_5+i)| WRITE_INDEX(index);
  
    outData << LOAD_CODE << " " << addr1Val << endl;
    outData << LOAD_CODE << " " << addr2Val << endl;
    outData << LOAD_CODE << " " << addr3Val << endl;
    outData << LOAD_CODE << " " << addr4Val << endl;

    outData << LOAD_CODE << " " << addr1Val << endl;
    outData << LOAD_CODE << " " << addr5Val << endl;

    outData << LOAD_CODE << " " << addr1Val << endl;
    outData << LOAD_CODE << " " << addr3Val << endl;
    outData << LOAD_CODE << " " << addr4Val << endl;
    outData << LOAD_CODE << " " << addr5Val << endl;
  } 
}

int main()
{
  ofstream outData;
  outData.open("trace.txt");
  
  if( !outData ) { // file couldn't be opened
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }

  GeneratePolicyDeterminationStream(outData);

  return 0;
}
