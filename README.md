# JPG-Recovery
Recover a deleted JPG

This code takes what would have been a memory card with deleted files, searches for a JPG header and then creates an image using the data after the header.
It's current limitation is it only stops writing when it gets to the end of the source file or finds another JPG header.
