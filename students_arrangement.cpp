#include<bits/stdc++.h>
using namespace std;



int solve(vector<int> &pos,int n){
    sort(pos.begin(), pos.end());

    int j=0,ans=n;

    for(int i=0;i<n;i++){
        while(j<n && (long long )a[j]-a[i]<=n-1) j++;
        ans=min(ans,n-(j-i));
    }
    return ans;
}
int main(){
    int n;
    cout<<"enter the number of students"<<" :"<<"\n";

    cin>>n;
    cout<<"enter positions"<<"\n";
    vector<int> pos(n);
    for(int i=0;i<n;i++){
        cin>>pos[i];
    }
   cout<<"ANS : "<< solve(pos,n);
   return 0;
}