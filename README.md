# Data Blocks Exchange
* First you need to start the server part

**Usage**: `./DataBlocksExchange <port>\n`

* To run the client part:

**Usage**: `./client <host> <service>\n`


### The logic of operation

The result of the operation will be a request from the client for a list of a random number of hash keys. In response, the server must return the data blocks corresponding to this key, taken from the imaginary block storage.

