# network-assignment
네트워크 수업 - Packet Parser 만들기, 소켓 프로그래밍

## HW1 - Stripping IP Packets
## Packet Trace
- Wireshark

## Format of Trace(PCAP formatted)
<img width="573" src="https://user-images.githubusercontent.com/26567880/91697340-ec066b00-ebab-11ea-93d0-f55efa586fd7.png">

- struct timeval sec : timestamp that packet was recorded into the trace in second
- struct timeval usec : timestamp that packet was recorded into the trace in micro-second
- caplen : the length of the packet presented in the trace 
- len : the actual length of the packet 

## Goal
- Parse the following information
<img width="799" src="https://user-images.githubusercontent.com/26567880/91697367-f32d7900-ebab-11ea-89ce-c38e6df9a91a.png">

## How to run in linux
$sudo apt-get install libpcap-dev //for pcap library

$g++ packetParser.cpp -o packetParser -lpcap

$./packetParser

## Experience
- pcap library를 사용하여 pcap_next_ex() 함수를 사용하여 pcap 파일을 읽었음
- DF(Don't fragments)와 MF(More fragments)를 구할 때 어려움이 있었음. Wireshark로 직접 확인해보았더니 DF/MF를 알 수 있는 Flags 가 분리되어 있지 않고 Fragment offset 안에 포함되어 있었기에, 비트 연산자 & 를 사용하여 Flags 부분을 분리해냄

## Result
- Screenshot of output for a sample packet
<img width="425" alt="packet" src="https://user-images.githubusercontent.com/26567880/91695880-c4ae9e80-eba9-11ea-9c51-661e2ff14d87.png">

- Verification with Wireshark
<img width="465" alt="wireshark" src="https://user-images.githubusercontent.com/26567880/91696097-10f9de80-ebaa-11ea-8366-c92e23b15a3a.png">

## HW2 - Socket Programming
### First Stage
- After connecting to the server the designated server will ask your ID and your server’s IP address and port number. 
- My server should be running at this time and have a valid IPv4 address and TCP port number.

### Second Stage
- The designated starts the client on its machine and opens the second connection to my server on the given the IP address and the port number. 
- The new client will deliver the 20-byte string (token) on the second connection. 
- My server must pass the token to my client and my client must return this token to the designated server on the first connection.

### Concurrent Programming
- Lab's client sends multiple requests from several process at the same time.
- Multi Process로 구현함

### Architecture
<img width="705" src="https://user-images.githubusercontent.com/26567880/91699629-861be280-ebaf-11ea-891a-f0f9fe2e7b38.png">

### Experience
#### 포트포워딩 설정하기
- virtualBox에서 돌리고 있는 나의 서버를 외부에서도 접속할 수 있도록 허용하기 위해서는 먼저, virtualBox의 네트워크 모드를 확인해보아야한다. 
- 나의 virtualBox는 Bridge 모드로 설정돼있었다. 그래서 iptime 공유기 설정에 들어가서 포트포워딩 설정에 규칙을 추가해주었다.
- Bridge : 공유기가 실제 PC에게 IP를 줄 뿐만 아니라, 공유기가 VM에게도 IP를 준다. 공유기는 VM도 별개의 실제 PC로 보기 때문에 개별적으로 IP를 할당 한다. 즉, VM은 실제 PC가 물리적 네트워크에서 동작하는 것처럼 동작한다.

#### Multi Process 구현하기 중 좀비 프로세스 처리하기
- 프로세스에서 return하게 되면, 자동으로 자원 정리를 해줄 수 있는 함수를 등록하였다. 시그널 핸들링이라고도 하는데, 이 시그널 핸들링에 remove_zombie 함수를 저장해두었다.
- waitpid 함수는 인수로 주어진 pid 번호의 자식프로세스가 종료되거나, 시그널 함수를 호출하는 신호가 전달될 때까지 waitpid 호출한 영역에서 일시 중지 된다.





