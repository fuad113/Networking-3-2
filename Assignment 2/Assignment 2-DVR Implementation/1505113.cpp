#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define INF 100000000

using namespace std;


struct router
{

	string nxthop;
	int cost;

	router()
	{

	}

	router(string h,int  c)
	{
		nxthop=h;
		cost=c;		

	}
	
};

string IntToString(int a)
{
 
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;

}


map <string,router> routing_table; 


void PrintRoutingTable()
{

	cout<< "Destination			" << "Next Hop			" << " Cost\n" ;
	cout<< "-----------			" << "---------			" << " -------\n" ;

    auto itr = routing_table.begin();

    while(itr != routing_table.end())
    {
        cout << itr->first <<"			"<< itr->second.nxthop << "			" << itr->second.cost << endl;
        itr++;
    }


}




int main(int argc, char *argv[]){



	//getting myrouter ip
  	
  	string myrouterip;

    char * argument1=argv[1];
    int size=strlen(argument1);

    for(int i=0;i<size;i++)
    {
      myrouterip.push_back(argument1[i]);
    }
    
    
    //making the routing table
    
    ifstream file;
    file.open(argv[2]);
     
    string ip1 ,ip2;
    int c;
    vector<string> adjacency_vector; 
    map <string,int> NeighbourCostList;
    
    while(file >> ip1 >> ip2 >> c)
    {
    	
       	if(ip1==myrouterip)
       	{
       		routing_table[ip2]=router(ip2,c);
       		NeighbourCostList[ip2]=c;
       		adjacency_vector.push_back(ip2);
       	}

       	else if(ip2==myrouterip)
       	{
       		routing_table[ip1]=router(ip1,c);
       		NeighbourCostList[ip1]=c;
       		adjacency_vector.push_back(ip1);
       	}


       	else
       	{
       		 bool flag=false;

       		for(int i=0;i<adjacency_vector.size();i++)
       		{
       			if(adjacency_vector[i] == ip1)
       				{
       					flag=true;
       					break;
       				}

       		}

       		if(flag==false)
       			routing_table[ip1]=router ("  -  ",INF);

 			
 			flag=false;

       		for(int i=0;i<adjacency_vector.size();i++)
       		{
       			if(adjacency_vector[i] == ip2)
       				{
       					flag=true;
 						break;
       				}
       		}

       		if(flag==false)
       			routing_table[ip2]=router ("  -  ",INF);


       	}

    }
   

    PrintRoutingTable();


	int bytes_received;
	int bind_flag;
	int sockfd;
	
    struct sockaddr_in client_address;
	struct sockaddr_in router_address;
    socklen_t addrlen;
	

	if(argc != 3){
		printf("%s <ip address>\n", argv[1]);
		exit(1);
	}
	
	
//	printf("Router's IP address: %s",IPAddress);
	
    

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(4747);
    inet_pton(AF_INET, argv[1], &client_address.sin_addr);


    struct sockaddr_in send_address; 
	send_address.sin_family = AF_INET;
	send_address.sin_port = htons(4747);


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind_flag = bind(sockfd, (struct sockaddr*) &client_address, sizeof(sockaddr_in));


     cout<<"--------------------------------------"<<endl;
	if(!bind_flag) 
		cout<<"Connection successful!!"<<endl;
	else 
		cout<<"Connection failed!!!"<<endl;

    cout<<"--------------------------------------"<<endl;
  

   

	while(true)
	{
		char buffer[1024];
		bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &router_address, &addrlen);
		cout<<bytes_received<<endl;
		if(bytes_received!=-1)
		{
			string command;
            for(int i=0;i<4;i++)
            {
            	command.push_back(buffer[i]);
            }


            //showing the routing table
            if(command == "show")
            	PrintRoutingTable(); 



            //updating the cost command
            else if(command == "cost")
            {
            	string ip1,ip2;
            	int temp,c;
            	int arr[10];
            	int k=0;

            	for(int i=4;i<bytes_received;i++)
            	{
            		temp=buffer[i];
            		if(temp < 0)
            			temp=temp+256;

   					arr[k]=temp;
   					k++;
            		
            	}

                
                ip1= IntToString(arr[0])+ "."+IntToString(arr[1])+ "." + IntToString(arr[2])+"."+IntToString(arr[3]);
                ip2= IntToString(arr[4])+ "."+IntToString(arr[5])+ "." + IntToString(arr[6])+"."+IntToString(arr[7]);
                c=(int) arr[8];

                
                if(ip1== myrouterip)
                	NeighbourCostList[ip2]=c;
                else
                	NeighbourCostList[ip1]=c;



                auto it = NeighbourCostList.begin();
                
                while(it != NeighbourCostList.end())
                {
                	int temp1,temp2;

                	string temp_ip=it->first;

                	temp1=routing_table[it->first].cost;
                	temp2=it->second;

                    if(temp1>temp2)
                     {
                       routing_table[temp_ip].nxthop = temp_ip;
                       routing_table[temp_ip].cost = temp2;

                     }
                        
                    it++;
                }

                
            }



            //works of sending message
            else if(command == "send")
            {

                string ip1,ip2,msg;
            	int temp,msglen;
            	int arr[10];
            	int k=0;

            	for(int i=4;i<bytes_received;i++)
            	{
            		temp=buffer[i];
            		if(temp < 0)
            			temp=temp+256;

   					arr[k]=temp;
   					k++;
            		
            	}

                
                ip1= IntToString(arr[0])+ "."+IntToString(arr[1])+ "." + IntToString(arr[2])+"."+IntToString(arr[3]);
                ip2= IntToString(arr[4])+ "."+IntToString(arr[5])+ "." + IntToString(arr[6])+"."+IntToString(arr[7]);
                msglen=(int) arr[8];
             
                temp=msglen+10;

                for(int i=10;i<temp;i++)
                {
                	msg.push_back(arr[i]);
                }	


                if(ip2 == myrouterip)
                {
                	cout<< "Message From " + ip1 + " : " + msg + "\n";
                }

                else 

                {   

                	if(routing_table[ip2].nxthop != "  -  ")
                	{
                		cout << "Message Sent to Router-" << routing_table[ip2].nxthop << "\n" ;
                		send_address.sin_addr.s_addr = inet_addr(routing_table[ip2].nxthop.c_str());
					    sendto(sockfd, buffer, 1024, 0, (struct sockaddr*) &send_address, sizeof(sockaddr_in));

                	}

                	else
                		cout<< "Cannot Send Message to Router " << ip2;

    

                }
 				

            }

            else if(command[0]=='c' && command[1]=='l' && command[2]=='k')
            {

            	string str = "Table: ";
 
    			auto it = routing_table.begin();

    			while(it != routing_table.end())
    		    {
        			if(it->second.nxthop != "  -  ")
           				str += myrouterip + " -> " + it->first + " -> " + IntToString(it->second.cost) + "\n";
 
        		    it++;
    			}

    			char tempbuffer[1024];

    			strcpy(tempbuffer,str.c_str());
                 
                string temp=""; 
    			for(int i=0;i<adjacency_vector.size();i++)
    			{
    				temp= adjacency_vector[i];
    				send_address.sin_addr.s_addr = inet_addr(temp.c_str());
					sendto(sockfd, tempbuffer, 1024, 0, (struct sockaddr*) &send_address, sizeof(sockaddr_in));
    			}

            }	          


		}
	}

	return 0;

}