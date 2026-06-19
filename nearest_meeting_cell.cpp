#include<bits/stdc++.h>
using namespace std;

int nearest_meeting_cell(int node1,int node2,vector<int> &edges){
    int n=edges.size();

    vector<int> dist1(n,-1),dist2(n,-1);

    int curr=node1,d=0;

    while(curr!=-1 && dist1[curr]==-1){
        dist1[curr]=d++;
        curr=edges[curr];
    }

    curr=node2;
    d=0;

    while(curr!=-1 && dist2[curr]==-1){
        dist2[curr]=d++;
        curr=edges[curr];
    }

    int ans=-1;
    int mx=INT_MAX;

    for(int i=0;i<n;i++){
        if(dist1[i]!=-1 && dist2[i]!=-1){

            int td=max(dist1[i],dist2[i]);

            if(td<mx){
                mx=td;
                ans=i;
            }
        }
    }

    return ans;
}

int main(){
    int n;

    cout<<"Enter the number of edges";
    cin>>n;

    cout<<"\nEnter the edges";

    vector<int> edges(n);

    for(int i=0;i<n;i++){
        cin>>edges[i];
    }

    cout<<"\n";

    cout<<"Enter_the_nodes1 and nodes2";

    int nd1,nd2;
    cin>>nd1>>nd2;

    cout<<"\n";

    cout<<"ANS "<<nearest_meeting_cell(nd1,nd2,edges);

    return 0;
}