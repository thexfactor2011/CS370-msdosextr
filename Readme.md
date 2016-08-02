# Linux Kernel Development Projects CS370
## MS DOS Fat Reader/Extractor Assignment
[Assignment URL](https://www.cs.drexel.edu/~wmm24/cs370_su16/project/fs.htm)

### General Information
We have been asked to recover data from floppies that use an old version of Microsoft's DOS file system called FAT12 (i.e. does not have support for long file names, so you can consider all files to have names of up to 8 characters with a 3 character extension). The entire contents of these floppies have been extracted and are stored as separate Unix files.
However the data files within each floppy volume, are still stored within the Unix file in the format used by the MSDOS file system.

You have to search for the specification of the MSDOS file system so that you can create a program that will be able to produce catalog listings of each floppy image and be able to extract the files from the image.

### How to run
* `make cc`
* `msdosextr sample.bin`

