#include <string>
#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>

using namespace std;

//hours, minutes and seconds
void ConvertReadableTime(int n, int usec)
{
    n = n % (24 * 3600);
    int hour = n / 3600;
    
    n %= 3600;
    int minutes = n / 60 ;
    
    n %= 60;
    int seconds = n;
    
    cout << hour << ":" << minutes << ":" << seconds << ":";
    printf("%06d\n", usec);
}

void parsing(pcap_t *pcap)
{
    struct pcap_pkthdr *header;
    const u_char *data;
    
    u_int packetCount = 0;
    while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
    {
        printf("Packet # %i\n", ++packetCount); //Show the packet number
        
        // loop through the packet and print it as hexidecimal representations of octets
        //for (int i=0; i < header->caplen; i++)
        //{
        //    if ( (i % 16) == 0)
        //        printf("\n");
        
        //    printf("%.2x ", data[i]); //Print each octet as hex (x), make sure there is always two characters (.2).
        //}
        //printf("\n");
        
        //pcap_pkthdr
        printf("Local Time: ");
        ConvertReadableTime((int)header->ts.tv_sec, (int)header->ts.tv_usec);
        
        printf("Captured Length: %d bytes\n", header->caplen); //Show the size in bytes of the packet
        printf("Actual Length: %d bytes\n", header->len);
        
        //IP header Length
        printf("IP header Length: %d bytes\n", (data[14]&15) * 4);
        
        //MAC Address
        printf("SRC MAC Address: ");
        for(int i=6; i<=11; i++){
            printf("%.2x", data[i]);
            
            if(i != 11) printf(":");
            else printf(" -> ");
        }
        
        printf("DST MAC Address: ");
        for(int i=0; i<=5; i++){
            printf("%.2x", data[i]);
            
            if(i != 5) printf(":");
            else printf("\n");
        }
        
        //IP Address
        printf("SRC IP Address: ");
        for(int i=26; i<=29; i++){
            printf("%d", data[i]);
            
            if(i != 29) printf(".");
            else printf(" -> ");
        }
        
        printf("DST IP Address: ");
        for(int i=30; i<=33; i++){
            printf("%d", data[i]);
            
            if(i != 33) printf(".");
            else printf("\n");
        }
        
        //IP Protocol
        printf("IP Protocol: ");
        int ipProtocol = data[23];
        
        switch (ipProtocol) {
            case 1: printf("ICMP\n"); break;
            case 6: printf("TCP\n"); break;
            case 17: printf("UDP\n"); break;
                
            default: printf("%d\n", ipProtocol); break;
        }
        
        //Identification
        printf("Identification: 0x");
        for(int i=18; i<=19; i++)
            printf("%.2x", data[i]);
        
        printf("\n");
        
        //Flags DF/MF
        printf("DF Flag: ");
        if((data[20]&64) != 0) printf("Set\n");
        else printf("Not Set\n");
        
        printf("MF Flag: ");
        if((data[20]&32) != 0) printf("Set\n");
        else printf("Not Set\n");
        
        //TTL
        printf("TTL: %d\n", data[22]);
        
        printf("\n\n");
    }
}

int main(int argc, char *argv[])
{
    string file;
    cout << "Input file name with path (example./home/sohyun/Network/packetCapture.pcap):\n";
    cin >> file;
    
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline(file.c_str(), errbuff);
    
    parsing(pcap);
    
    return 0;
}
