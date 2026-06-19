#include<bits/stdc++.h>
using namespace std;


int solve(string s){

    int e=0,n=s.length();
    vector<int> ans;

    for(int i=0;i<n;i++){
         if(s[i]=='D'){
            int j=i;
            while( j<n&& s[j]=='D' ) j++;
            ans.push_back(j-i);
            i=j-1;
         }
         else{

            e++;
         }
    }
    sort(ans.rbegin(),ans.rend());
    int take=min(e,int(ans.size()));
    int final_sum=accumulate(ans.begin(),ans.begin()+take,0);
    return final_sum;
    
}
int main(){
    cout<<"Enter the string : ";
    string s;
    cin>>s;
    cout<<"MAX_ENERGY_GAIN : "<<solve(s);
}