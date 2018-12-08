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

int StringToInt(string str)
{
  int sum=0;
  int m=1;

  for(int i=str.length()-1;i>=0;i--)
  {
  	sum+=(str[i]-48)*m;
  	m*=10;
  }

  return sum;

}


map <string,router> routing_table; 





void PrintRoutingTable()
{

	cout<< "Destination		  " << "Next Hop		  " << "Cost\n" ;
	cout<< "-----------		  " << "---------		  " << "-----\n" ;

    auto itr = routing_table.begin();

    while(itr != routing_table.end())
    {
    	if(itr->second.cost ==0)
    		cout << itr->first <<"		  "<< itr->second.nxthop << "		  " << "        0" << endl;
    	else
        	cout << itr->first <<"		  "<< itr->second.nxthop << "		  " << itr->second.cost << endl;
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
		//cout<<bytes_received<<endl;

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
                
                int tempc1=(int) arr[8];
                int tempc2=(int) arr[9];

                c=(tempc2*256)+tempc1;


                
                if(ip1== myrouterip)
                {
                	NeighbourCostList[ip2]=c;
                	if(routing_table[ip2].nxthop == ip2)
                	{
                		routing_table[ip2].cost=c;
                	}

                	else if(routing_table[ip2].cost > c)
                	{
                		routing_table[ip2].cost=c;
                		routing_table[ip2].nxthop=ip2;
                	}
                
                }
                else
                {
                	NeighbourCostList[ip1]=c;
                	if(routing_table[ip1].nxthop == ip1)
                	{
                		routing_table[ip1].cost=c;
                	}
                	else if(routing_table[ip1].cost > c)
                	{
                		routing_table[ip1].cost=c;
                		routing_table[ip1].nxthop=ip1;
                	}
                }
                
            }



            //works of sending message
            else if(command == "send")
            {

                string ip1,ip2,msg;
            	int temp,msglen;
            	int k=0;
            	vector<int> arr;

            	for(int i=4;i<bytes_received;i++)
            	{
            		temp=buffer[i];
            		if(temp < 0)
            			temp=temp+256;

   					arr.push_back(temp);
   					
            		
            	}

                
                ip1= IntToString(arr[0])+ "."+IntToString(arr[1])+ "." + IntToString(arr[2])+"."+IntToString(arr[3]);
                ip2= IntToString(arr[4])+ "."+IntToString(arr[5])+ "." + IntToString(arr[6])+"."+IntToString(arr[7]);
                int tempc1=(int) arr[8];
                int tempc2=(int) arr[9];



                msglen=(tempc2*256) + tempc1;

                int temp2=msglen+10;

                for(int i=10;i<temp2;i++)
                {
                	msg.push_back(arr[i]);
                }	


                if(ip2 == myrouterip)
                {
                	cout<< "Message From " + ip1 + " : " + msg + "\n\n";
                }

                else 

                {   
                	

                	if(routing_table[ip2].cost != INF)
                	{
                		cout << "Message Sent to Router-" << routing_table[ip2].nxthop << "\n\n" ;
                		send_address.sin_addr.s_addr = inet_addr(routing_table[ip2].nxthop.c_str());
					    sendto(sockfd, buffer, 1024, 0, (struct sockaddr*) &send_address, sizeof(sockaddr_in));

                	}

                	else
                		cout<< "Cannot Send Message to Router " << ip2 << "\n";

    

                }
 				

            }



            else if(command[0]=='c' && command[1]=='l' && command[2]=='k')
            {

            	string str = "";
 
    			auto it = routing_table.begin();

    			while(it != routing_table.end())
    		    {
        			if(it->second.nxthop != "  -  ")
           				str += myrouterip + ">" + it->first + ">" + IntToString(it->second.cost) + "+";
 
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



            else
            {

            	stringstream ss1;
                ss1<< buffer;
                string temp1;
                vector<string> received_message;
                
                while(getline(ss1,temp1,'+'))
                {
 					received_message.push_back(temp1);
                }


                for(int i=0;i<received_message.size();i++)
                {

                	stringstream ss;
                	ss<< received_message[i];
                	string temp;
                	vector<string> received_vector;
                
                	while(getline(ss,temp,'>'))
                	{
 						received_vector.push_back(temp);
                	}



                	string sender=received_vector[0];
                	string sender_neighbour=received_vector[1];
                	int cost_received= StringToInt(received_vector[2]);

                	
                	//cout<< "value of i: " << i <<endl; 
                	//cout<<  sender << endl;
                	//cout<< sender_neighbour<<endl;
                	//cout<<  cost_received<< endl;
                    
                     
                	if(routing_table[sender_neighbour].nxthop == sender)
                		routing_table[sender_neighbour].cost= routing_table[sender].cost +cost_received;

                	if(sender_neighbour != myrouterip)
                	{

                		if(routing_table[sender_neighbour].cost > routing_table[sender].cost + cost_received)
                		{

                			routing_table[sender_neighbour].cost=routing_table[sender].cost + cost_received ;
                			routing_table[sender_neighbour].nxthop = routing_table[sender].nxthop;	

                		}

                	}
                   

                	received_vector.clear();
                	
                }

                    auto itr = NeighbourCostList.begin();
   				
   				    while(itr != NeighbourCostList.end())
    				
    				{
        				string temp=itr->first;
        				int  tempc=itr->second;

        				if(routing_table[temp].cost > tempc)
        					routing_table[temp].cost=tempc;

        				itr++;
        			}      

            }

		}
	}

	return 0;

}