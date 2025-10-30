#include "App.h"


ChunkInfo* allocChunk(unsigned char* cS, uint8_t *hash, long int eval, int offset) {
    ChunkInfo* c = (ChunkInfo *)malloc(sizeof(ChunkInfo));
    if (!c) return NULL;  // Check allocation success

    c->chunkStart = cS;
    c->cdcHash = hash; // think this needs to have malloc
    c->lastEvalPt = eval;
    c->fileOffset = offset;
    return c;
}

int appIter(unsigned char* inputBuf, unsigned char* outputBuf, ChunkInfo *currentChunk,
          int shaSoc, void * matchLib, unsigned char* file, size_t newChunkLen){
          
  
   bool chunkfound=1;
   //size_t newChunkLen = 100; //junk
 //size_t newChunkLen, bool chunkfound = CDC(someinputoftheabove); // We need to know if we have a new chunk or not. If so, let's go / we need init point and length of new chunk
  //cdc_add_data(CDC *cdc, unsigned char *data, int len, chunkfound); // CDC goes into current chunk structure, data is packet data for current run, len is packet length, split the thing up a little and call cdc_send_chunk but break up cdc_send_chunk since it's packaged chunk needs to be accessible up here
  
  if(chunkfound == 1){
  
    //cdc_send_chunk(CDC *cdc, unsigned int start_pos, cdc->current_chunk_len) // Then send the chunk and clear / prep the CDC object for the next packet
    
    unsigned char ShaTarg[SHA384_DIGEST_SZ]; // SHA output, probably should init only once but penalty not bad // May need to move SHA384_DIGEST_SZ to other header
    compute_sha3_hash(shaSoc, inputBuf, newChunkLen,  ShaTarg); // Hash whole chunk. Really it is better if we did this incrementally
    int matchID = checkMatch_PH(ShaTarg, matchLib);
    if (matchID==1) { //If we match
    //sendDupChunk(matchID, &file[currentChunk.fileOffset]) // Never hits comment for now
    }
    else{
    //Theoretically run these concurrent
    // Needs a pre output here for chunk header
    lzw_PH(inputBuf, &file[currentChunk->fileOffset], newChunkLen); //Compress (really send) current chunk from chunkstart for length Chunklen to file at fileoffset for the given chunk
    }
    currentChunk->fileOffset=currentChunk->fileOffset+newChunkLen;
    currentChunk->chunkStart=currentChunk->chunkStart+newChunkLen;
  }
  return 0;
}
