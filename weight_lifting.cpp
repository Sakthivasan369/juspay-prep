#include<bits/stdc++.h> 
using namespace std;

int solve( vector<int> & nums){
    sort(nums.begin(),nums.end());
    nums.erase(unique(nums.begin(),nums.end()),nums.end());
    int ans=0,n=nums.size();
    int l=0;
    for(int r=0;r<n;r++){
        while((nums[r]-nums[l])>=n) l++;
        ans=max(ans,r-l+1);
    }
    return ans;
}
int main(){
    int n;
    cout<<"Enter the number of weights : ";
    cin>>n;
    vector<int> ans(n);
    for(int i=0;i<n;i++){
        cin>>ans[i];
    }
    cout<<"ans "<<solve(ans)<<"\n";
    return 0;
}