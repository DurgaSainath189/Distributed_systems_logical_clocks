#include<bits/stdc++.h>
using namespace std;

int main()
{
    int no_of_processes,no_of_processes_cs,timestamp,Process;
    cout<<"Enter number of Processors :";
    cin>>no_of_processes;
    cout<<"Enter number of Processors which want to enter critical section:";
    cin>>no_of_processes_cs;
    vector<int> timestamps(no_of_processes,0);
    vector<int> request;
    map <int,int> mp;
    for(int i=0;i<no_of_processes_cs;i++)
    {
        cout<<"\nEnter timestamp:";
        cin>>timestamp;
        cout<<"Enter Processes number:";
        cin>>Process;
        timestamps[Process-1]=timestamp;
        request.push_back(Process);
        mp[timestamp]=Process;
    }

    cout<<"\nProcessors and Timestamp:\n";
    for(int i=0;i<timestamps.size();i++)
    {
        cout<<i+1<<" "<<timestamps[i]<<endl;
    }

    for(int i=0;i<request.size();i++)
    {
        cout<<"\n Request from Process:"<<request[i]<<endl;
        for(int j=0;j<timestamps.size();j++)
        {
            if(request[i]!=(j+1))
            {
                if(timestamps[j]>timestamps[request[i]-1] || timestamps[j]==0)
                    cout<<j+1<<" Replied\n";
                else
                    cout<<j+1<<" Rejected\n";
            }
        }
    }

    cout<<endl;
    map<int,int>:: iterator it;
    it=mp.begin();
    int c=0;
    for(it=mp.begin();it!=mp.end();it++)
    {
        cout<<"Process "<<it->second<<" entered Critical Section \n";
        c++;
    }
    return 0;
}


