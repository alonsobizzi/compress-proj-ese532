#include <stdint.h>

//struct controlStruct // Structure containing all parameters that are being fed into the code

//Suspect there will eventually be a command queue lumped into the chunkStructure but unwise to do now






typedef struct {
    void *chunkStart;      // Where current chunk starts
    uint8_t *cdcHash;      // Current CDC hash for last Evalpoint
    unsigned int lastEvalPt;   // Last evaluation point (i.e. where last invocation of cdc stopped evaluating
    unsigned int fileOffset;        // Where output chunk needs to be written to
} ChunkInfo;

//Probably move file Offset or do in some wonky threadsafe way (keep it NULL until prior chunk done with a check or similar) since requires prior chunks. Sequential so idk for now -C
