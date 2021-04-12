# rle
rle-archivator
# DESCRIPTION
* Rle
reduces the size of the named files using run length encoding (RLE).
Whenever possible,
each file is replaced by one with the extension
"\&.rle" , while keeping the same ownership modes, access and modification times.
By default, *rle keeps the original file name and timestamp in the compressed file. 
* Unrle
unzip data in new file.
* Catrle
unzip in the standart output.
* Putrle
receives data from the standart input and appends in encoding (RLE)
form to the end of the file. 
* Inrle_crypt
crypt the RLE file by the key from the standart input.
KEY - in ASCII symbols.
* Unrle_crypt
unzip data in new file after receiving the key.
# Flags:
* rle [ " \-i " ]
* unrle [ " \-d " ]
* catrle [ " \-c " ]
* putrle [ " \-p " ]
*inrle_crypt [ " \-e " ]
* unrle_crypt [ " \-u " ]
