#include "App.h"


int appIter(unsigned char* inputBuf, unsigned char* outputBuf, ChunkInfo *currentChunk,
          shaSoc){
          
  
          
 size_t newChunkLen, bool chunkfound = CDC(someinputoftheabove); // We need to know if we have a new chunk or not. If so, let's go / we need init point and length of new chunk
  
  if(chunkfound == 1){
    unsigned char ShaTarg[SHA384_DIGEST_SZ]; // SHA output, probably should init only once but penalty not bad // May need to move SHA384_DIGEST_SZ to other header
    compute_sha3_hash(shaSoc, currentChunk.chunkStart, newChunkLen,  ShaTarg); // Hash whole chunk. Really it is better if we did this incrementally
    int matchID = checkMatch_PH(ShaTarg, matchLib)
    if (matchID==1) { //If we match
    //sendDupChunk(matchID, &file[currentChunk.fileOffset]) // Never hits comment for now
    }
    else{
    //Theoretically run these concurrent
    // Needs a pre output here for chunk header
    LZW_PH(currentChunk.chunkStart, &file[currentChunk.fileOffset], newChunkLen); //Compress (really send) current chunk from chunkstart for length Chunklen to file at fileoffset for the given chunk
    }
    currentChunk.fileOffset=currentChunk.fileOffset+newChunkLen
    currentChunk.chunkStart=currentChunk.chunkStart+newChunkLen
  }
}
