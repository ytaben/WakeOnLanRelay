#ifndef _H_WOLRELAY
#define _H_WOLRELAY
/*
    Verify that given message is a valid WOL packet
    @arg payload - the content of the packet to be verified
    @return - 1 if payload is a valid magic packet, 0 otherwise
*/
int isValidMagicPacket(char* payload);

/*
    Print the mac address from the given macgic packet payload
    @arg payload - the content of the packet to be verified
*/
void printMac(char* payload);
#endif