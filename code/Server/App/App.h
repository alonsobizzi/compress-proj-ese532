#include <stdint.h>
#include <cstdlib>
#include "checkMatch.h"
#include "cdc.h"
#include "shaComp.h"
#include "lzw_SW.h"

//struct controlStruct // Structure containing all parameters that are being fed into the code

//Suspect there will eventually be a command queue lumped into the chunkStructure but unwise to do now

struct ChunkInfo {
    unsigned char *chunkStart;      // Where current chunk starts
    uint8_t *cdcHash;      // Current CDC hash for last Evalpoint
    unsigned int lastEvalPt;   // Last evaluation point (i.e. where last invocation of cdc stopped evaluating
    unsigned int fileOffset;        // Where output chunk needs to be written to
    unsigned char *chunkBuffer;
};

struct globalInfo {
    int shaSoc;
    void * matchLib; //Make sure pointer is ok with boost
    unsigned char* opFile;
    unsigned int chunkNum;
};

int appIter(unsigned char* inputBuf, unsigned char* outputBuf, ChunkInfo *currentChunk,
          globalInfo *worldData, size_t newPacketLength);

ChunkInfo* allocChunk(unsigned char *mem, uint8_t *hash, long int eval, int offset);
globalInfo* allocInit(int shaSoc, void * matchLib, unsigned char* file, unsigned int chunkNum);
//Probably move file Offset or do in some wonky threadsafe way (keep it NULL until prior chunk done with a check or similar) since requires prior chunks. Sequential so idk for now -C
