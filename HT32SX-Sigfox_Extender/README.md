# iMCP HT32SX Sigfox Network Extender

iMCP HT32SX Sigfox Network Extender are two **EXAMPLE** application developed to repeat (or extend) the Sigfox payload through a P2P network until arrives to a Sigfox Gateway. 

There are two options:

1. [HT32SX P2P+Sigfox](HT32SX_P2P_Sigfox): Uses a static gateway. The only way to know which node had sent the last message is adding this information to the Sigfox Payload, using at least 1 of the 12 bytes. 

2. [HT32SX P2P+Sigfox Generic Gateway](HT32SX-P2P-Sigfox_Generic_Gateway): Uses a totally generic Sigfox gateway. Users do not need to waste 1 byte to add the node information (node address). 