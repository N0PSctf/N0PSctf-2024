## ZIPZIP

### Description

zipzipzipzipzipzip

**Author: algorab**

### Solution

We are facing a zip file that looks normal.

When unzipping it, we get a documentation about the zip format, giving us the hint that we need to analyze the file, looking for any unusual data.

When looking at the file structure, we can see that between the file data and the Central Directory, there is some data that do not belong to the file, as we can see by looking at the `compressed_size` of the file.
Moreover, by analyzing the excess of data, we can come to the conclusion that it is a local file header, without its signature (`PK\3\4`). All the fields of a local file header are present.
Therefore, we need to reconstruct a ZIP file that will allow us to unzip data hidden data.
This can be done as follows:
```python
from zipfile import ZipFile, ZIP_DEFLATED
import os
import struct

def seek_signature(data, signature):
    i = 0
    while data[i:i+4] != signature:
        i += 1
    return i

def read_zip_data(data):
    header = data[:4]
    version = int.from_bytes(data[4:6], byteorder='little')
    purpose_bit = data[6:8]
    compression_method = int.from_bytes(data[8:10], byteorder='little')
    last_modification_time = int.from_bytes(data[10:12], byteorder='little')
    last_modification_date = int.from_bytes(data[12:14], byteorder='little')
    crc_32 = data[14:18]
    compressed_size = int.from_bytes(data[18:22], byteorder='little')
    uncompressed_size = int.from_bytes(data[22:26], byteorder='little')
    file_name_length = int.from_bytes(data[26:28], byteorder='little')
    extra_field_length = int.from_bytes(data[28:30], byteorder='little')
    file_name = data[30:30+file_name_length]
    raw_data = data[30+file_name_length+extra_field_length:30+file_name_length+extra_field_length+compressed_size]
    return file_name, data[18:22], data[22:26], crc_32, 30+file_name_length+extra_field_length+compressed_size

data = open("archive.zip", 'rb').read()

_, _, _, _, data_len = read_zip_data(data)
sig_loc = seek_signature(data, b"PK\x01\x02")

# Adding header to zlib data
data = b'PK\x03\x04' +  data[data_len:sig_loc]

file_name, compressed_size, uncompressed_size, crc_32, data_len = read_zip_data(data)

print(compressed_size)

# header
cd = b"PK\x01\x02"
# Version made by
cd += b"\x14\x03"
# Version needed to extract
cd += b"\x14\x00"
# General purpose bit flag
cd += b"\x00\x00"
# Compression method
cd += b"\x08\x00"
# File last modification time + file last modification date
cd += b"\x0B\x72\x68\x57"
# CRC-32, compressed size, uncompressed size
cd += crc_32 + compressed_size + uncompressed_size
# File name length
cd += b"\x20\x00"
# Extra field length
cd += b"\x00\x00"
# File comment length
cd += b"\x00\x00"
# Disk number start
cd += b"\x00\x00"
# Internal file attributes
cd += b"\x00\x00"
# External file attributes
cd += b"\x00\x00\xa4\x81"
# Relative offset of local header
cd += b"\x00\x00\x00\x00"
cd += file_name
# header
eocd = b"PK\x05\x06"
# Disk number
eocd += b"\x00\x00"
# Disk where central directory starts
eocd += b"\x00\x00"
# Number of entries in the central directory on this disk
eocd += b"\x01\x00"
# Total number of records
eocd += b"\x01\x00"
# Size of central directory
eocd += struct.pack('<L', len(cd))
# Offset of start of central directory with respect to the starting disk number
eocd += struct.pack('<L', data_len)
# Comments length
eocd += b"\x00\x00"

data += cd
data += eocd

open("out.zip", "wb").write(data)
```

We can then unzip it and read the flag.
### Flag

`N0PS{z1p_z1p_z1p_z1p}`