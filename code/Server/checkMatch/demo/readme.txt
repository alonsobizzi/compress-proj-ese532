I am currently just skipping CDC functionality and setting a bool to 1 

My intuition is that we need concurrent memory for everything else and the above will (in a more final version) just memcpy into some element of ChunkInfo struct (would need to mod the struct a bit). For now, I just manually update ChunkInfo outside the thing.

Once we have this, we compute the hash of (for now) the packet. We then check the match (always get 0 for placeholder though I wrote a demo showing this function actually works). We then run the lzw placeholder which is simply a placeholder for memcpy. 
