#include<bits/stdc++.h>
using namespace std;

int largest_sum_cycle(vector<int> &edges){
    int n=edges.size();

    vector<int> vis(n,-1);
    vector<long long> pref(n,0);

    long long ans=-1;

    for(int start=0;start<n;start++){

        if(vis[start]!=-1) continue;

        int curr=start;
        long long sum=0;
        long long tsum=-1;

        while(curr!=-1){

            if(vis[curr]==start){
                tsum=sum-pref[curr]+curr;
                break;
            }

            if(vis[curr]!=-1) break;

            vis[curr]=start;

            sum+=curr;
            pref[curr]=sum;

            curr=edges[curr];
        }

        ans=max(ans,tsum);
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

    cout<<"ANS "<<largest_sum_cycle(edges);

    return 0;
}