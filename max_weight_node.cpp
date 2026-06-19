#include<bits/stdc++.h>
using namespace std;

int maximum_weight_node(vector<int> &edges){
    int n=edges.size();
    vector<int> temp(n,0);

    for(int i=0;i<n;i++){
        if(edges[i]!=-1){
            temp[edges[i]]+=i;
        }
    }

    int a=max_element(temp.begin(),temp.end())-temp.begin();
    return a;
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

    cout<<"ANS "<<maximum_weight_node(edges);

    return 0;
}