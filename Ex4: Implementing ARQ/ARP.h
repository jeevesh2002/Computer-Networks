typedef char string[50];

#define REQ 1
#define ACK 2
#define DATA 3

typedef struct ARP_PACKET{
    int mode;
    string src_ip;
    string dest_ip;
    string src_mac;
    string dest_mac;
    string data;
}arp;

arp createARPPacket(int mode){
    arp packet;
    bzero(&packet, sizeof(packet));

    packet.mode = mode;
    printf("\nEnter the details of packet.\n");
    printf("Source IP\t: ");
    scanf(" %s", packet.src_ip);
    printf("Source MAC\t: ");
    scanf(" %s", packet.src_mac);
    printf("Destination IP\t: ");
    scanf(" %s", packet.dest_ip);
    printf("16 bit data\t: ");
    scanf(" %s", packet.data);

    return packet;
}

void printPacket(arp packet){
    if (packet.mode == REQ)
        printf("%d|%s|%s|%s|%s\n", packet.mode, packet.src_mac, packet.src_ip, "00:00:00:00:00:00", packet.dest_ip);
    else if (packet.mode == ACK)
        printf("%d|%s|%s|%s|%s\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_ip, packet.dest_mac);
    else
        printf("%d|%s|%s|%s|%s|%s\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_ip, packet.dest_mac, packet.data);

}

